#
# Copyright (C) 2016  Nexell Co., Ltd. All Rights Reserved.
# Nexell Co. Proprietary & Confidential
#
# Nexell informs that this code and information is provided "as is" base
# and without warranty of any kind, either expressed or implied, including
# but not limited to the implied warranties of merchantability and/or
# fitness for a particular puporse.
#
# Module	:
# File		:
# Description	:
# Author	: Hans
# History	: 2017.03.14 new release

###########################################################################
# Build Version info
###########################################################################
VERINFO			= V001

###########################################################################
# Build Environment
###########################################################################
CHIPNAME		= S5P4418

DEBUG			= n

MEMTYPE			= DDR3
#MEMTYPE		= LPDDR3

CRC_CHECK		= n

CFLAGS			:=

BOARD			?= RAPTOR

# cross-tool pre-header
ifeq ($(OS),Windows_NT)
CROSS_TOOL_TOP		=
CROSS_TOOL		= $(CROSS_TOOL_TOP)arm-none-eabi-
else
CROSS_TOOL_TOP		=
CROSS_TOOL		= $(CROSS_TOOL_TOP)arm-eabi-
endif

###########################################################################
# Top Names
###########################################################################
PROJECT_NAME		= $(CHIPNAME)_dispatcher_$(MEMTYPE)_$(VERINFO)
TARGET_NAME		= armv7_dispatcher-$(shell echo $(BOARD) | tr A-Z a-z)
LDS_NAME		= armv7_dispatcher

###########################################################################
# Directories
###########################################################################
DIR_PROJECT_TOP		=

DIR_OBJOUTPUT		= obj
DIR_TARGETOUTPUT	= out

CODE_MAIN_INCLUDE	=

###########################################################################
# Build Environment
###########################################################################
CPU			= cortex-a9
CC			= $(CROSS_TOOL)gcc
LD 			= $(CROSS_TOOL)ld
AS 			= $(CROSS_TOOL)as
AR 			= $(CROSS_TOOL)ar
MAKEBIN			= $(CROSS_TOOL)objcopy
OBJCOPY			= $(CROSS_TOOL)objcopy
RANLIB 			= $(CROSS_TOOL)ranlib

GCC_LIB			= $(shell $(CC) -print-libgcc-file-name)

ifeq ($(DEBUG), y)
CFLAGS			= -DNX_DEBUG -O0
Q			=
else
CFLAGS			= -DNX_RELEASE -Os
Q			= @
endif

###########################################################################
# MISC tools for MS-DOS
###########################################################################
MKDIR			= mkdir
CD			= cd
ECHO			= echo
ifeq ($(OS),Windows_NT)
RM			= del /q /F
MV			= move
CP			= copy
RMDIR			= rmdir /S /Q
else
RM			= rm -f
MV			= mv
CP			= cp
RMDIR			= rm -rf
endif
###########################################################################
# FLAGS
###########################################################################
ARFLAGS			= rcs
ARFLAGS_REMOVE		= -d
ARLIBFLAGS		= -v -s

ASFLAG			= -D__ASSEMBLY__

CFLAGS			+=	-g -Wall				\
				-Wextra -ffreestanding -fno-builtin	\
				-msoft-float				\
				-mlittle-endian				\
				-mcpu=$(CPU)				\
				-mstructure-size-boundary=32		\
				$(CODE_MAIN_INCLUDE)			\
				-DMEMTYPE_$(MEMTYPE)			\
				-D$(BOARD)

