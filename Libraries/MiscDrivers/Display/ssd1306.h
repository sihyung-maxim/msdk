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

#ifndef LIBRARIES_MISCDRIVERS_DISPLAY_SSD1306_H_
#define LIBRARIES_MISCDRIVERS_DISPLAY_SSD1306_H_

#include <stdio.h>
#include <stdint.h>
#include "display.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************* DEFINES ******************************/
#define BIT_SET(a, b) ((a) |= (1U << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1U << (b)))

#define SSD1306_SET_BUFFER_PIXEL_UTIL(buf, buf_w, buf_max, x, y, color, opa) \
    uint16_t byte_index = x + ((y >> 3) * buf_w);                            \
    uint8_t bit_index = y & 0x7;                                             \
    if (byte_index >= buf_max) {                                             \
        return;                                                              \
    }                                                                        \
                                                                             \
    if (color == 0 && opa) {                                                 \
        BIT_SET(buf[byte_index], bit_index);                                 \
    } else {                                                                 \
        BIT_CLEAR(buf[byte_index], bit_index);                               \
    }

/********************************* TYPE DEFINES ******************************/

typedef struct {
    uint8_t row; // Datasheet Page 38.
    uint8_t col; // Datasheet Page 38.
    display_rotation_t rotation;
    uint8_t color_invert;
} ssd1306_init_param_t;

typedef struct {
    display_comm_api comm_api;
    ssd1306_init_param_t init_param;
} ssd1306_dev;

/********************************* Function Prototypes **************************/
int ssd1306_configure(ssd1306_dev *dev, ssd1306_init_param_t *init_param,
                      display_comm_api *comm_api);
int ssd1306_init(ssd1306_dev *dev);
void ssd1306_set_cursor(ssd1306_dev *dev, uint8_t row, uint16_t col);
void ssd1306_send_pixels(ssd1306_dev *dev, uint8_t *payload, uint32_t payloadlen);
void ssd1306_flush_area(ssd1306_dev *dev, const display_area_t *area, const uint8_t *data);

void ssd1306_set_buffer_pixel_util(uint8_t *buf, uint16_t buf_w, uint32_t buf_max, uint16_t x,
                                   uint16_t y, uint8_t color, uint8_t is_opaque);

#ifdef __cplusplus
}
#endif

#endif // LIBRARIES_MISCDRIVERS_DISPLAY_SSD1306_H_
