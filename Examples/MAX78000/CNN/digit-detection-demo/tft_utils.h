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
#ifdef BOARD_EVKIT_V1
#include "bitmap.h"
#include "tft_ssd2119.h"
#endif
#ifdef BOARD_FTHR_REVA
#include "tft_ili9341.h"
#endif

#include "example_config.h"

#define THICKNESS 4

void TFT_Print(char *str, int x, int y, int font, int length);
void draw_obj_rect(float *xy, int class_idx, uint32_t w, uint32_t h, uint8_t scale);
void start_tft_dma(uint32_t *src_ptr, uint16_t byte_cnt);
void setup_dma_tft(uint32_t *src_ptr, uint16_t byte_cnt);
void tft_dma_display(int x, int y, int w, int h, uint32_t *data);
