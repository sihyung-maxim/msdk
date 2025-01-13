###############################################################################
 #
 # Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by
 # Analog Devices, Inc.),
 # Copyright (C) 2023-2024 Analog Devices, Inc.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #
 ##############################################################################

ifeq "$(BOARD_DIR)" ""
$(error BOARD_DIR must be set)
endif

# Source files for this test (add path to VPATH below)
SRCS += board.c
SRCS += stdio.c
SRCS += led.c
SRCS += camera.c
ifeq "$(CAMERA)" "HM0360_MONO"
SRCS += hm0360_mono.c
SRCS += debayering.c
PROJ_CFLAGS+=-DCAMERA_HM0360_MONO
PROJ_CFLAGS+=-DCAMERA_MONO
else ifeq "$(CAMERA)" "HM0360_COLOR"
SRCS += hm0360_color.c
SRCS += debayering.c
PROJ_CFLAGS+=-DCAMERA_BAYER
PROJ_CFLAGS+=-DCAMERA_HM0360_COLOR
endif
SRCS += sccb.c

MISC_DRIVERS_DIR=$(BOARD_DIR)/../../../MiscDrivers

# Where to find BSP source files
VPATH += $(BOARD_DIR)/Source
VPATH += $(BOARD_DIR)/../Source # Add core BSP source directory
VPATH += $(MISC_DRIVERS_DIR)
VPATH += $(MISC_DRIVERS_DIR)/Camera
VPATH += $(MISC_DRIVERS_DIR)/Display
VPATH += $(MISC_DRIVERS_DIR)/LED
VPATH += $(MISC_DRIVERS_DIR)/PushButton
VPATH += $(MISC_DRIVERS_DIR)/Touchscreen


# Where to find BSP header files
IPATH += $(BOARD_DIR)/Include
IPATH += $(BOARD_DIR)/../Include # Add core BSP include directory
IPATH += $(MISC_DRIVERS_DIR)
IPATH += $(MISC_DRIVERS_DIR)/Camera
IPATH += $(MISC_DRIVERS_DIR)/Display
IPATH += $(MISC_DRIVERS_DIR)/LED
IPATH += $(MISC_DRIVERS_DIR)/PushButton
IPATH += $(MISC_DRIVERS_DIR)/Touchscreen
