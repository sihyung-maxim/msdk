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

#ifndef CAMERA_H
#define CAMERA_H

#include "mxc_errors.h"
#include "mipi_camera.h"

#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 256
#define PIXEL_FORMAT PIXEL_FORMAT_RGB565
#define PIXEL_ORDER PIXEL_ORDER_RGB565_RGB

#define SRAM_ADDRESS 0x0
// ^ This is the address in SRAM that captured images will be stored at.

// Update for future cameras
#if defined(CAMERA_OV5640)
#define CAMERA_ID 0x5640
#else // Default
#define CAMERA_ID 0x5640
#endif

/***** Globals *****/

/***** Functions *****/

bool camera_init(void);
void camera_capture(void);
void camera_capture_and_load_cnn(void);
void camera_display_last_image(void);

#endif
