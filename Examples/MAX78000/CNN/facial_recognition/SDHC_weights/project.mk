# This file can be used to set build configuration
# variables.  These variables are defined in a file called 
# "Makefile" that is located next to this one.

# For instructions on how to use this system, see
# https://github.com/analogdevicesinc/VSCode-Maxim/tree/develop#build-configuration

#BOARD=FTHR_RevA
# ^ For example, you can uncomment this line to make the 
# project build for the "FTHR_RevA" board.

# **********************************************************

# Add your config here!

BOARD = FTHR_RevA

LIB_SDHC = 1

ifneq "$(BOARD)" "FTHR_RevA"
$(error ERR_NOTSUPPORTED: This project is only support for the MAX78000FTHR board!)
endif