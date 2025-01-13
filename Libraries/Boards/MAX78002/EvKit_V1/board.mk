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

ifeq "$(RISCV_CORE)" ""
# Note: RISC-V core does not have access to SPI0, cannot drive TFT
TFT ?= NEWHAVEN
# Supported values for TFT:
# - ADAFRUIT (for use with legacy Adafruit 3315)
# - NEWHAVEN (default - for use with NewHaven NHD-2.4-320240CF))
endif

# Source files for this application (add path to VPATH below)
SRCS += board.c
SRCS += stdio.c
SRCS += led.c
SRCS += pb.c
ifeq "$(TFT)" "ADAFRUIT"
PROJ_CFLAGS+=-DTFT_ADAFRUIT
SRCS += adafruit_3315_tft.c
SRCS += adafruit_3315_touch.c
endif
ifeq "$(TFT)" "NEWHAVEN"
PROJ_CFLAGS+=-DTFT_NEWHAVEN
SRCS += tft_st7789v.c
# NewHaven TFT board has an integrated tsc2046 touchscreen driver
# The TFT display is typically oriented with a 270 degree rotation,
# so we need to flip the screen and swap the X,Y coordinates for
# the touchscreen drivers to match it.
PROJ_CFLAGS += -DFLIP_SCREEN
PROJ_CFLAGS += -DSWAP_XY
SRCS += tsc2046.c
endif
SRCS += camera.c
SRCS += mipi_camera.c

ifeq "$(CAMERA)" "HM0360"
$(warning Using mono HM0360 by default.)
$(warning Use CAMERA=HM0360_COLOR for color drivers, or CAMERA=HM0360_MONO to clear this warning.)
CAMERA=HM0360_MONO
endif

ifeq "$(CAMERA)" "OV5640"
SRCS += ov5640.c
PROJ_CFLAGS+=-DCAMERA_OV5640
else ifeq "$(CAMERA)" "HM01B0"
SRCS += hm01b0.c
PROJ_CFLAGS+=-DCAMERA_HM01B0
PROJ_CFLAGS+=-DCAMERA_MONO
else ifeq "$(CAMERA)" "HM0360_MONO"
SRCS += hm0360_mono.c
PROJ_CFLAGS+=-DCAMERA_HM0360_MONO
PROJ_CFLAGS+=-DCAMERA_MONO
else ifeq "$(CAMERA)" "HM0360_COLOR"
SRCS += hm0360_color.c
SRCS += debayering.c
PROJ_CFLAGS+=-DCAMERA_BAYER
PROJ_CFLAGS+=-DCAMERA_HM0360_COLOR
else ifeq "$(CAMERA)" "OV5642"
SRCS += ov5642.c
PROJ_CFLAGS+=-DCAMERA_OV5642
else ifeq "$(CAMERA)" "OV7692"
SRCS += ov7692.c
PROJ_CFLAGS+=-DCAMERA_OV7692
else ifeq "$(CAMERA)" "PAG7920"
SRCS += pag7920.c
PROJ_CFLAGS+=-DCAMERA_PAG7920
PROJ_CFLAGS+=-DCAMERA_MONO
else ifeq "$(CAMERA)" ""
SRCS += ov7692.c
PROJ_CFLAGS+=-DCAMERA_OV7692
endif
SRCS += sccb.c

# The MAX78002EVKIT has an on-board external APS6404 SRAM.
# Add drivers for it from MiscDrivers.
ifeq "$(RISCV_CORE)" ""
# RISC-V core does not have access to SPI0.  Skip drivers
SRCS += fastspi.c
SRCS += aps6404.c
endif

MISC_DRIVERS_DIR ?= $(MAXIM_PATH)/Libraries/MiscDrivers

# Where to find BSP source files
VPATH += $(BOARD_DIR)/Source
VPATH += $(MISC_DRIVERS_DIR)
VPATH += $(MISC_DRIVERS_DIR)/Camera
VPATH += $(MISC_DRIVERS_DIR)/Display
VPATH += $(MISC_DRIVERS_DIR)/LED
VPATH += $(MISC_DRIVERS_DIR)/PushButton
VPATH += $(MISC_DRIVERS_DIR)/Touchscreen
VPATH += $(MISC_DRIVERS_DIR)/SRAM


# Where to find BSP header files
IPATH += $(BOARD_DIR)/Include
IPATH += $(MISC_DRIVERS_DIR)
IPATH += $(MISC_DRIVERS_DIR)/Camera
IPATH += $(MISC_DRIVERS_DIR)/Display
IPATH += $(MISC_DRIVERS_DIR)/LED
IPATH += $(MISC_DRIVERS_DIR)/PushButton
IPATH += $(MISC_DRIVERS_DIR)/Touchscreen
IPATH += $(MISC_DRIVERS_DIR)/SRAM

include $(MISC_DRIVERS_DIR)/Display/fonts/fonts.mk
