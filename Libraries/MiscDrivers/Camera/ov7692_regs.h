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

#ifndef LIBRARIES_MISCDRIVERS_CAMERA_OV7692_REGS_H_
#define LIBRARIES_MISCDRIVERS_CAMERA_OV7692_REGS_H_

// clang-format off
// Register address definitions.
#define REG0C       (0x0C)
#define REG12       (0x12)
#define REG0E       (0x0E)
#define PIDH        (0x0a)
#define PIDL        (0x0b)

#define MIDH        (0x1c)
#define MIDL        (0x1d)

#define HSTART      (0x17)
#define HSIZE       (0x18)
#define VSTART      (0x19)
#define VSIZE       (0x1A)

#define OH_HIGH     (0xcc)
#define OH_LOW      (0xcd)
#define OV_HIGH     (0xce)
#define OV_LOW      (0xcf)

#define XSC_MAN_HIGH    (0xc4)
#define XSC_MAN_LOW     (0xc5)
#define YSC_MAN_HIGH    (0xc6)
#define YSC_MAN_LOW     (0xc7)

#define IVERTICAL_LOW   (0xcb)

#define REG14   (0x14)
#define REG81   (0x81)
#define REG82   (0x82)
#define REG28   (0x28)
#define REGD2   (0xd2)
#define REGDA   (0xda)
#define REGDB   (0xdb)

#define REG_LUM2   (0x6d)
#define REG_LUM1   (0x6e)
#define REG_LUM0   (0x6f)

// Register value definitions.
#define REG12_RESET (0x80)

#define COLOR_RGB565    (0x06)
#define COLOR_RGB444    (0x0e)
#define COLOR_YUV422    (0x00)
#define COLOR_BAYER     (0x01)

#define VERTICAL_FLIP       (0X80)
#define HORIZONTAL_FLIP     (0X40)

#define SLEEP_MODE_ENABLE   (0x08)
// clang-format on

#endif // LIBRARIES_MISCDRIVERS_CAMERA_OV7692_REGS_H_
