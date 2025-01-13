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

# Specify the build directory if not defined by the project
ifeq "$(BUILD_DIR)" ""
ifeq "$(MFLOAT_ABI)" ""
LWIP_BUILD_DIR = ${LWIP_DIR}/bin
else
LWIP_BUILD_DIR = ${LWIP_DIR}/bin/$(MFLOAT_ABI)
endif
else
LWIP_BUILD_DIR=$(BUILD_DIR)/lwIP
endif

# Set the output filename
# This is used in the PROJECT variable in the recursive make rule
ifneq "$(MFLOAT_ABI)" ""
LWIP_LIBRARY_NAME = liblwip_$(MFLOAT_ABI)
else
LWIP_LIBRARY_NAME = liblwip
endif

# Form the filepath to the output library file (note the addition of the .a extension here)
LWIP_LIBRARY_FILE = $(LWIP_BUILD_DIR)/$(LWIP_LIBRARY_NAME).a

# Add to library list
LIBS += $(LWIP_LIBRARY_FILE)

# Add include paths to the build.  This file is used to add lwIP as a library,
# so we just need to add relevant include paths...
include ${LWIP_DIR}/lwip_files.mk
IPATH += ${LWIP_INCLUDE_DIR}
# ... ^ here

# Export other variables needed by the peripheral driver makefile
export TARGET
export COMPILER
export TARGET_MAKEFILE
export PROJ_CFLAGS
export PROJ_LDFLAGS
export MXC_OPTIMIZE_CFLAGS

# Add rule to build the Driver Library
${LWIP_BUILD_DIR}/${LWIP_LIBRARY_NAME}.a: ${LWIP_C_FILES} ${LWIP_H_FILES}
	$(MAKE) -f ${LWIP_DIR}/Makefile  lib BUILD_DIR=${LWIP_BUILD_DIR} PROJECT=${LWIP_LIBRARY_NAME}

clean.lwip:
	@rm -rf ${LWIP_BUILD_DIR}/*

query.lwip:
	@${MAKE} -f ${LWIP_DIR}/Makefile query QUERY_VAR="${QUERY_VAR}"