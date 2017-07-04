 # Copyright (C) 2016  Nexell Co., Ltd.
 # Author: Sangjong, Han <hans@nexell.co.kr>
 #
 # This program is free software; you can redistribute it and/or
 # modify it under the terms of the GNU General Public License
 #
 # as published by the Free Software Foundation; either version 2
 # of the License, or (at your option) any later version.
 #
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 #
 # You should have received a copy of the GNU General Public License
 # along with this program.  If not, see <http://www.gnu.org/licenses/>.
VERINFO				= V001

#########################################################################
# build environment
##########################################################################
CFLAGS				:=
DEBUG				?= n

# chip name
#CHIPNAME			?= nxp4330
CHIPNAME			?= s5p4418

# (ddr3/lpddr3) memory type
MEMTYPE				?= ddr3
#MEMTYPE				?= lpddr3

# system log Message
SYSLOG				?= n

# secure boot
SECURE_ON			?= n

# arm mode - secure/non-secure
ARM_SECURE			?= n

# drex secure mode
DREX_BUS_SECURE			?= n

# cross-tool pre-header
ifeq ($(OS),Windows_NT)
CROSS_COMPILE_TOP		?=
CROSS_COMPILE			?= $(CROSS_COMPILE_TOP)arm-none-eabi-
else
CROSS_COMPILE_TOP		?=
CROSS_COMPILE			?= $(CROSS_COMPILE_TOP)arm-eabi-
endif

# Top Names
PROJECT_NAME			= $(CHIPNAME)_dispatcher_$(MEMTYPE)_$(VERINFO)
TARGET_NAME			= armv7_dispatcher
LDS_NAME			= armv7_dispatcher

# Directories
DIR_PROJECT_TOP			=

DIR_OBJOUTPUT			= obj
DIR_TARGETOUTPUT		= out

CODE_MAIN_INCLUDE		=

# Build Environment
CPU				= cortex-a9
CC				= $(CROSS_COMPILE)gcc
LD 				= $(CROSS_COMPILE)ld
AS 				= $(CROSS_COMPILE)as
AR 				= $(CROSS_COMPILE)ar
MAKEBIN				= $(CROSS_COMPILE)objcopy
OBJCOPY				= $(CROSS_COMPILE)objcopy
RANLIB 				= $(CROSS_COMPILE)ranlib

GCC_LIB				= $(shell $(CC) -print-libgcc-file-name)

ifeq ($(SECURE_ON), y)
CFLAGS				+=  -DSECURE_ON
endif

ifeq ($(DEBUG), y)
CFLAGS				= -DNX_DEBUG -O0
Q				=
else
CFLAGS				= -DNX_RELEASE -Os
Q				= @
endif

# MISC tools for MS-DOS
ifeq ($(OS),Windows_NT)
MKDIR				= mkdir
RM				= del /q /F
MV				= move
CD				= cd
CP				= copy
ECHO				= echo
RMDIR				= rmdir /S /Q
else
MKDIR				= mkdir
RM				= rm -f
MV				= mv
CD				= cd
CP				= cp
ECHO				= echo
RMDIR				= rm -rf
endif

# flags variables
ARFLAGS				= rcs
ARFLAGS_REMOVE			= -d
ARLIBFLAGS			= -v -s
ASFLAG				= -D__ASSEMBLY__
CFLAGS				+=	-g -Wall					\
					-Wextra -ffreestanding -fno-builtin		\
					-msoft-float					\
					-mlittle-endian					\
					-mfpu=neon					\
					-mcpu=$(CPU)					\
					-mstructure-size-boundary=32			\
					$(CODE_MAIN_INCLUDE)				\
					-D__arm -DLOAD_FROM_$(BOOTFROM)			\
					-DCHIPID_$(shell echo $(CHIPNAME) | tr a-z A-Z)	\
					-DMEMTYPE_$(shell echo $(MEMTYPE) | tr a-z A-Z)

# arm mode - secure/non-secure
ifeq ($(ARM_SECURE), y)
CFLAGS				+=	-DSECURE_MODE
endif

# drex secure mode
ifeq ($(DREX_BUS_SECURE), y)
CFLAGS				+=	-DCONFIG_DREX_BUS_SECURE
endif

# system log messgae
ifeq ($(SYSLOG), y)
CFLAGS				+=	-DSYSLOG_ON
endif
