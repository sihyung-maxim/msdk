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

ifeq "$(LVGL_DIR)" ""
# If LVGL_DIR is not specified, this Makefile will locate itself.
LVGL_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
endif

ifeq ($(ENABLE_DISPLAY),1)
PROJ_CFLAGS += -DENABLE_DISPLAY
endif

# require by library
LVGL_DIR_NAME=lvgl

# include LVGL library make files
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/core/lv_core.mk
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/lv_draw.mk
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/extra/lv_extra.mk
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/font/lv_font.mk
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/hal/lv_hal.mk
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/misc/lv_misc.mk
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/widgets/lv_widgets.mk

# Add library .c sources on SRC
SRCS += $(CSRCS)

# Update Include path
IPATH += $(LVGL_DIR)
IPATH += $(LVGL_DIR)/$(LVGL_DIR_NAME)
IPATH += $(LVGL_DIR)/$(LVGL_DIR_NAME)/src

# Update VPATH
# Located with the command "find -L Libraries/LVGL/lvgl/src -type d" run from MSDK root
VPATH += Libraries/LVGL/lvgl/src
VPATH += Libraries/LVGL/lvgl/src/core
VPATH += Libraries/LVGL/lvgl/src/draw
VPATH += Libraries/LVGL/lvgl/src/draw/arm2d
VPATH += Libraries/LVGL/lvgl/src/draw/nxp
VPATH += Libraries/LVGL/lvgl/src/draw/nxp/pxp
VPATH += Libraries/LVGL/lvgl/src/draw/nxp/vglite
VPATH += Libraries/LVGL/lvgl/src/draw/sdl
VPATH += Libraries/LVGL/lvgl/src/draw/stm32_dma2d
VPATH += Libraries/LVGL/lvgl/src/draw/sw
VPATH += Libraries/LVGL/lvgl/src/draw/swm341_dma2d
VPATH += Libraries/LVGL/lvgl/src/extra
VPATH += Libraries/LVGL/lvgl/src/extra/layouts
VPATH += Libraries/LVGL/lvgl/src/extra/layouts/flex
VPATH += Libraries/LVGL/lvgl/src/extra/layouts/grid
VPATH += Libraries/LVGL/lvgl/src/extra/libs
VPATH += Libraries/LVGL/lvgl/src/extra/libs/bmp
VPATH += Libraries/LVGL/lvgl/src/extra/libs/ffmpeg
VPATH += Libraries/LVGL/lvgl/src/extra/libs/freetype
VPATH += Libraries/LVGL/lvgl/src/extra/libs/fsdrv
VPATH += Libraries/LVGL/lvgl/src/extra/libs/gif
VPATH += Libraries/LVGL/lvgl/src/extra/libs/png
VPATH += Libraries/LVGL/lvgl/src/extra/libs/qrcode
VPATH += Libraries/LVGL/lvgl/src/extra/libs/rlottie
VPATH += Libraries/LVGL/lvgl/src/extra/libs/sjpg
VPATH += Libraries/LVGL/lvgl/src/extra/others
VPATH += Libraries/LVGL/lvgl/src/extra/others/fragment
VPATH += Libraries/LVGL/lvgl/src/extra/others/gridnav
VPATH += Libraries/LVGL/lvgl/src/extra/others/ime
VPATH += Libraries/LVGL/lvgl/src/extra/others/imgfont
VPATH += Libraries/LVGL/lvgl/src/extra/others/monkey
VPATH += Libraries/LVGL/lvgl/src/extra/others/msg
VPATH += Libraries/LVGL/lvgl/src/extra/others/snapshot
VPATH += Libraries/LVGL/lvgl/src/extra/themes
VPATH += Libraries/LVGL/lvgl/src/extra/themes/basic
VPATH += Libraries/LVGL/lvgl/src/extra/themes/default
VPATH += Libraries/LVGL/lvgl/src/extra/themes/mono
VPATH += Libraries/LVGL/lvgl/src/extra/widgets
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/animimg
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/calendar
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/chart
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/colorwheel
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/imgbtn
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/keyboard
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/led
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/list
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/menu
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/meter
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/msgbox
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/span
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/spinbox
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/spinner
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/tabview
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/tileview
VPATH += Libraries/LVGL/lvgl/src/extra/widgets/win
VPATH += Libraries/LVGL/lvgl/src/font
VPATH += Libraries/LVGL/lvgl/src/hal
VPATH += Libraries/LVGL/lvgl/src/misc
VPATH += Libraries/LVGL/lvgl/src/widgets
