/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by 
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#include <string.h>

#include "board.h"
#include "mxc_device.h"
#include "mxc_delay.h"
#include "mxc.h"
#include "utils.h"
#include "camera.h"
#include "facedetection.h"
#include "post_process.h"
#include "utils.h"
#include "MAXCAM_Debug.h"
#include "cnn_1.h"
#ifdef BOARD_FTHR_REVA
#include "tft_ili9341.h"
#endif
#include "led.h"
#include "lp.h"

#define S_MODULE_NAME "facedetection"
#define PRINT_TIME 1

/************************************ VARIABLES ******************************/
volatile uint32_t cnn_time; // Stopwatch

static void run_cnn_1(int x_offset, int y_offset);

#ifdef TFT_ENABLE
static int g_dma_channel_tft = 1;
static uint8_t *rx_data = NULL;
static void setup_dma_tft(uint32_t *src_ptr)
{
    printf("TFT DMA setup\n");
    // TFT DMA
    MXC_DMA->ch[g_dma_channel_tft].status = MXC_F_DMA_STATUS_CTZ_IF; // Clear CTZ status flag
    MXC_DMA->ch[g_dma_channel_tft].dst = (uint32_t)rx_data; // Cast Pointer
    MXC_DMA->ch[g_dma_channel_tft].src = (uint32_t)src_ptr;
    MXC_DMA->ch[g_dma_channel_tft].cnt = IMAGE_XRES * IMAGE_YRES;

    MXC_DMA->ch[g_dma_channel_tft].ctrl =
        ((0x1 << MXC_F_DMA_CTRL_CTZ_IE_POS) + (0x0 << MXC_F_DMA_CTRL_DIS_IE_POS) +
         (0x1 << MXC_F_DMA_CTRL_BURST_SIZE_POS) + (0x0 << MXC_F_DMA_CTRL_DSTINC_POS) +
         (0x1 << MXC_F_DMA_CTRL_DSTWD_POS) + (0x1 << MXC_F_DMA_CTRL_SRCINC_POS) +
         (0x1 << MXC_F_DMA_CTRL_SRCWD_POS) + (0x0 << MXC_F_DMA_CTRL_TO_CLKDIV_POS) +
         (0x0 << MXC_F_DMA_CTRL_TO_WAIT_POS) + (0x2F << MXC_F_DMA_CTRL_REQUEST_POS) + // SPI0 -> TFT
         (0x0 << MXC_F_DMA_CTRL_PRI_POS) + // High Priority
         (0x0 << MXC_F_DMA_CTRL_RLDEN_POS) // Disable Reload
        );

    MXC_SPI0->ctrl0 &= ~(MXC_F_SPI_CTRL0_EN);
    MXC_SETFIELD(MXC_SPI0->ctrl1, MXC_F_SPI_CTRL1_TX_NUM_CHAR,
                 (IMAGE_XRES * IMAGE_YRES) << MXC_F_SPI_CTRL1_TX_NUM_CHAR_POS);
    MXC_SPI0->dma |= (MXC_F_SPI_DMA_TX_FLUSH | MXC_F_SPI_DMA_RX_FLUSH);

    // Clear SPI master done flag
    MXC_SPI0->intfl = MXC_F_SPI_INTFL_MST_DONE;
    MXC_SETFIELD(MXC_SPI0->dma, MXC_F_SPI_DMA_TX_THD_VAL, 0x10 << MXC_F_SPI_DMA_TX_THD_VAL_POS);
    MXC_SPI0->dma |= (MXC_F_SPI_DMA_TX_FIFO_EN);
    MXC_SPI0->dma |= (MXC_F_SPI_DMA_DMA_TX_EN);
    MXC_SPI0->ctrl0 |= (MXC_F_SPI_CTRL0_EN);
}

static void start_tft_dma(uint32_t *src_ptr)
{
    while ((MXC_DMA->ch[g_dma_channel_tft].status & MXC_F_DMA_STATUS_STATUS)) {
        {
        }
    }

    if (MXC_DMA->ch[g_dma_channel_tft].status & MXC_F_DMA_STATUS_CTZ_IF) {
        MXC_DMA->ch[g_dma_channel_tft].status = MXC_F_DMA_STATUS_CTZ_IF;
    }

    MXC_DMA->ch[g_dma_channel_tft].cnt = IMAGE_XRES * IMAGE_YRES;
    MXC_DMA->ch[g_dma_channel_tft].src = (uint32_t)src_ptr;

    // Enable DMA channel
    MXC_DMA->ch[g_dma_channel_tft].ctrl += (0x1 << MXC_F_DMA_CTRL_EN_POS);
    // Start DMA
    MXC_SPI0->ctrl0 |= MXC_F_SPI_CTRL0_START;
}
#endif

int face_detection(void)
{
#if (PRINT_TIME == 1)
    uint32_t start_time = utils_get_time_ms();
    uint32_t capture_time, process_time, total_time;
#endif
    // Capture the image
    camera_start_capture_image();
    /* Sleep until camera interrupt */
    MXC_LP_EnterSleepMode();
#ifdef TFT_ENABLE
    MXC_TFT_Stream(X_START, Y_START, IMAGE_XRES, IMAGE_YRES);
#endif

    /* Check for received image */
    if (camera_is_image_rcv()) {
        PR_DEBUG("Image received\n");
#if (PRINT_TIME == 1)
        capture_time = utils_get_time_ms() - start_time;
        process_time =
            utils_get_time_ms(); // Mark the start of process_time.  Var will be re-used to re-calculate itself.
#endif

#ifdef IMAGE_TO_UART
        break;
#endif

        // Enable CNN peripheral, enable CNN interrupt, turn on CNN clock
        // CNN clock: 50 MHz div 1
        cnn_enable(MXC_S_GCR_PCLKDIV_CNNCLKSEL_PCLK, MXC_S_GCR_PCLKDIV_CNNCLKDIV_DIV1);
        /* Configure CNN 1 to detect a face */
        cnn_1_init(); // Bring CNN state machine into consistent state
        cnn_1_load_weights(); // Load CNN kernels
        cnn_1_load_bias(); // Load CNN bias
        cnn_1_configure(); // Configure CNN state machine

        /* Run CNN on time on original and shifted images */
        run_cnn_1(0, 0);

#if (PRINT_TIME == 1)
        process_time = utils_get_time_ms() - process_time;
        total_time = utils_get_time_ms() - start_time;
#endif

#if (PRINT_TIME == 1)
        PR_INFO("Image Capture Time : %dms", capture_time);
        PR_INFO("FaceDetect Process Time : %dms", process_time);
        PR_INFO("Total FaceDetect Time : %dms", total_time);
#endif
    }

    return 0;
}

static void run_cnn_1(int x_offset, int y_offset)
{
    uint32_t imgLen;
    uint32_t w, h;
    /* Get current time */
    uint32_t pass_time = 0;
    uint8_t *raw;
    // Get the details of the image from the camera driver.
    camera_get_image(&raw, &imgLen, &w, &h);

#ifdef TFT_ENABLE
#ifdef BOARD_FTHR_REVA
    int dma_time = utils_get_time_ms();
    setup_dma_tft((uint32_t *)raw);
    start_tft_dma((uint32_t *)raw);
    // Wait for DMA to finish
    while ((MXC_DMA->ch[g_dma_channel_tft].status & MXC_F_DMA_STATUS_STATUS)) {
        {
        }
    }

    setup_dma_tft((uint32_t *)(raw + IMAGE_XRES * IMAGE_YRES));
    // Send a second half of captured image to TFT
    start_tft_dma((uint32_t *)(raw + IMAGE_XRES * IMAGE_YRES));
    // Wait for DMA to finish
    while ((MXC_DMA->ch[g_dma_channel_tft].status & MXC_F_DMA_STATUS_STATUS)) {
        {
        }
    }
    PR_DEBUG("DMA time : %d", utils_get_time_ms() - dma_time);
    //MXC_TFT_ShowImageCameraRGB565(X_START, Y_START, raw, w, h);

#endif
#endif

    cnn_start();

    //PR_INFO("CNN initialization time : %d", utils_get_time_ms() - pass_time);

    uint8_t *data = raw;

    pass_time = utils_get_time_ms();

    for (int i = y_offset; i < HEIGHT_DET + y_offset; i++) {
        data = raw + ((IMAGE_H - (WIDTH_DET)) / 2) * IMAGE_W * BYTE_PER_PIXEL;
        data += (((IMAGE_W - (HEIGHT_DET)) / 2) + i) * BYTE_PER_PIXEL;
        for (int j = x_offset; j < WIDTH_DET + x_offset; j++) {
            uint8_t ur, ug, ub;
            int8_t r, g, b;
            uint32_t number;

            ub = (uint8_t)(data[j * BYTE_PER_PIXEL * IMAGE_W + 1] << 3);
            ug = (uint8_t)((data[j * BYTE_PER_PIXEL * IMAGE_W] << 5) |
                           ((data[j * BYTE_PER_PIXEL * IMAGE_W + 1] & 0xE0) >> 3));
            ur = (uint8_t)(data[j * BYTE_PER_PIXEL * IMAGE_W] & 0xF8);
            b = ub - 128;
            g = ug - 128;
            r = ur - 128;

            // Loading data into the CNN fifo
            while (((*((volatile uint32_t *)0x50000004) & 1)) != 0) {}
            // Wait for FIFO 0

            number = 0x00FFFFFF & ((((uint8_t)b) << 16) | (((uint8_t)g) << 8) | ((uint8_t)r));

            *((volatile uint32_t *)0x50000008) = number; // Write FIFO 0
        }
    }

    int cnn_load_time = utils_get_time_ms() - pass_time;
    PR_DEBUG("CNN load data time : %dms", cnn_load_time);

    pass_time = utils_get_time_ms();

    // Disable Deep Sleep mode
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

    // CNN interrupt wakes up CPU from sleep mode
    while (cnn_time == 0) {
        asm volatile("wfi"); // Sleep and wait for CNN interrupt
    }

    PR_DEBUG("CNN wait time : %dms", utils_get_time_ms() - pass_time);

    uint32_t post_process_time = utils_get_time_ms();
    get_priors();
#ifdef RETURN_MAX_PROB
    get_max_probable_box();
#else
    localize_objects();
#endif
    PR_DEBUG("CNN post process time : %dms", utils_get_time_ms() - post_process_time);
    // Power off CNN after unloading result to clear all CNN registers.
    // It's needed to load and run other CNN model
    cnn_disable();
}
