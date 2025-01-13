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

#ifndef LIBRARIES_MISCDRIVERS_CAMERA_OV5642_REGS_H_
#define LIBRARIES_MISCDRIVERS_CAMERA_OV5642_REGS_H_

// clang-format off
#define OV5642_I2C_SLAVE_ADDR   0x3C

#define OV5642_CHIPID_HIGH  0x300a
#define OV5642_CHIPID_LOW   0x300b

#define OV5642_ID           0x5642
#define SYS_IO_PAD_CNTRL_REG_START (0x3000)

#define SYS_CNTRL_REG   (0x3008)
#define RESET_MODE      0x80
#define SLEEP_MODE      0x40

#define TIME_CNTRL_18_REG   (0x3818)
#define HORIZONTAL_MIRROR   0x40
#define VERTICAL_FLIP       0x20
#define THUMBNAIL           0x10
#define COMPRESSION         0x08

#define ARRAY_CTRL_01_REG   (0x3621)

#define H_START_HI_REG      0x3800
#define H_START_LO_REG      0x3801
#define V_START_HI_REG      0x3802
#define V_START_LO_REG      0x3803
#define H_WIDTH_HI_REG      0x3804
#define H_WIDTH_LO_REG      0x3805
#define V_HEIGHT_HI_REG     0x3806
#define V_HEIGHT_LO_REG     0x3807
#define OUT_H_WIDTH_HI_REG  0x3808
#define OUT_H_WIDTH_LO_REG  0x3809
#define OUT_V_HEIGHT_HI_REG 0x380a
#define OUT_V_HEIGHT_LO_REG 0x380b
#define TOTAL_H_SIZE_HI_REG 0x380c
#define TOTAL_H_SIZE_LO_REG 0x380d
#define TOTAL_V_SIZE_HI_REG 0x380e
#define TOTAL_V_SIZE_LO_REG 0x380f
#define AVG_X_START_HI_REG  0x5680
#define AVG_X_START_LO_REG  0x5681
#define AVG_X_END_HI_REG    0x5682
#define AVG_X_END_LO_REG    0x5683
#define AVG_Y_START_HI_REG  0x5684
#define AVG_Y_START_LO_REG  0x5685
#define AVG_Y_END_HI_REG    0x5686
#define AVG_Y_END_LO_REG    0x5687

#define ISP_CNTRL_REG       0x5001

#define ISP_FORMAT_MUX_CNTRL_REG    0x501F

#define COLOR_YUV   0
#define COLOR_RGB   1
#define COLOR_RAW   3

#define FORMAT_CNTRL_REG            0x4300
#define FORMAT_YUV422      0x30
#define FORMAT_YUV444      0x20
#define FORMAT_YUV420      0x40
#define FORMAT_RGB565      0x61
#define FORMAT_RGB888      0x23
#define FORMAT_RGB444      0x97
#define FORMAT_RGB555      0x71
#define FORMAT_RAW         0xF8
// clang-format on

// Special effects
typedef enum { SE_NORMAL = 0, SE_BLUE, SE_GREEN, SE_RED, SE_BLACK_WHITE, SE_NEGATIVE, SE_GRAY, SE_SEPIA } ov5642_effect_t;

#endif // LIBRARIES_MISCDRIVERS_CAMERA_OV5642_REGS_H_
