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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "mxc.h"
#include "board.h"

#define TS_X_MIN 254
#define TS_X_MAX 3680
#define TS_Y_MIN 193
#define TS_Y_MAX 3661
#define TFT_BUFF_SIZE 32 // TFT buffer size

int image_bitmap = (int)&img_1_rgb565[0];
int font_1 = (int)&Liberation_Sans12x12[0];
int font_2 = (int)&Liberation_Sans24x24[0];
int font_3 = (int)&Liberation_Sans28x28[0];
int font_4 = (int)&Liberation_Sans16x16[0];
int font_5 = (int)&Liberation_Sans19x19[0];
const int font_5_width = 19;
const int font_5_height = 19;
unsigned int seed = 78002;

void TFT_Print(char *str, int x, int y, int font, int length)
{
    text_t text = { .data = str, .len = length };

    MXC_TFT_PrintFont(x, y, font, &text, NULL);
}

void TFT_test(void)
{
    area_t _area;
    area_t *area;
    char buff[TFT_BUFF_SIZE];

    MXC_TFT_ShowImage(0, 0, image_bitmap);

    /* Get a good look at bitmap and allow debugger time to attach */
    MXC_Delay(MXC_DELAY_SEC(3));
    MXC_TFT_SetBackGroundColor(RED);

    area = &_area;
    area->x = 10;
    area->y = 10;
    area->w = 200;
    area->h = 100;

    MXC_TFT_FillRect(area, GREEN);
    MXC_Delay(MXC_DELAY_SEC(2));
    MXC_TFT_ClearScreen();
    MXC_TFT_Line(10, 10, 200, 200, NAVY);
    MXC_Delay(MXC_DELAY_SEC(1));
    MXC_TFT_Rectangle(10, 10, 200, 200, NAVY);
    MXC_Delay(MXC_DELAY_SEC(1));
    MXC_TFT_Circle(100, 100, 50, PURPLE);
    MXC_Delay(MXC_DELAY_SEC(1));
    MXC_TFT_FillCircle(100, 100, 50, PURPLE);
    MXC_Delay(MXC_DELAY_SEC(1));
    MXC_TFT_SetBackGroundColor(BLACK);
    MXC_TFT_SetForeGroundColor(WHITE); // set chars to white
    MXC_TFT_ClearScreen();

    memset(buff, 32, TFT_BUFF_SIZE);
    snprintf(buff, sizeof(buff), "ANALOG DEVICES");
    TFT_Print(buff, 0, 10, font_1, 14);

    snprintf(buff, sizeof(buff), "Analog Devices");
    TFT_Print(buff, 0, 50, font_2, 14);

    snprintf(buff, sizeof(buff), "Analog Devices");
    TFT_Print(buff, 0, 100, font_3, 14);

    snprintf(buff, sizeof(buff), "Analog Devices");
    TFT_Print(buff, 0, 150, font_4, 14);

    snprintf(buff, sizeof(buff), "Analog Devices");
    TFT_Print(buff, 0, 200, font_5, 14);

    MXC_Delay(MXC_DELAY_SEC(3));
    MXC_TFT_SetBackGroundColor(BLACK);
    MXC_TFT_SetForeGroundColor(WHITE);
    MXC_TFT_ClearScreen();
}

void print_xy(unsigned int x, unsigned int y)
{
    char buf[16];

    MXC_TFT_ClearScreen();
    TFT_Print(buf, x, y, font_1, snprintf(buf, sizeof(buf), "(%u,%u)", x, y));
}

int32_t rescale(int32_t x, int32_t min, int32_t max, int32_t a, int32_t b)
{
    x = (x > max) ? max : (x < min) ? min : x;

    return a + (((x - min) * (b - a)) / (max - min));
}

int main(void)
{
    MXC_Delay(MXC_DELAY_SEC(2));
#ifdef TFT_ADAFRUIT
    uint16_t x, y;
    int32_t xx, yy;
#endif

    MXC_ICC_Enable(MXC_ICC0);
    MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
    SystemCoreClockUpdate();

    printf("TFT Demo Example\n");
#ifdef TFT_ADAFRUIT
    /* Initialize ADAFRUIT TFT display */
    MXC_TFT_Init(MXC_SPI0, -1, NULL, NULL);
    MXC_TFT_SetRotation(ROTATE_270);
    TFT_test();
    /* Initialize ADAFRUIT touch screen */
    if (MXC_TS_Init(MXC_SPI0, -1, NULL, NULL))
        printf("Touch screen initialization failed\n");
    MXC_TS_Start();
    TFT_Print("Touch the screen!", 0, 120, font_5, 17);
    TFT_Print("And check serial terminal!", 0, 140, font_5, 27);
#else
    /* Initialize TFT display */
    MXC_TFT_Init(NULL, NULL);
    MXC_TFT_SetRotation(ROTATE_270);
    TFT_test();

    /* Initialize TouchScreen*/
    unsigned int touch_x, touch_y;
    MXC_TS_Init();
    MXC_TS_Start();
    TFT_Print("Touch the screen!", 0, 120, font_5, 17);
#endif

    for (;;) {
#ifdef TFT_ADAFRUIT
        if (ts_event) {
            MXC_TS_GetTouch(&x, &y);
            ts_event = false;
            printf("%d,%d  ", x, y);
            xx = rescale(x, TS_X_MIN, TS_X_MAX, 0, DISPLAY_HEIGHT);
            yy = rescale(y, TS_Y_MIN, TS_Y_MAX, 0, DISPLAY_WIDTH);
            printf("%d,%d\n", xx, yy);
        }
#else
        if (MXC_TS_GetTSEvent()) {
            MXC_TS_ClearTSEvent();
            MXC_TS_GetXY(&touch_x, &touch_y);
            print_xy(touch_x, touch_y);
        }
#endif
    }
}
