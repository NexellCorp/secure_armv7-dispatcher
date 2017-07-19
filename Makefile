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

include config.mak

LDFLAGS		=	-Bstatic						\
			-Wl,-Map=$(DIR_TARGETOUTPUT)/$(TARGET_NAME).map,--cref	\
			-T$(LDS_NAME).lds					\
			-Wl,--start-group					\
			-Lsrc/$(DIR_OBJOUTPUT)					\
			-Wl,--end-group						\
			-Wl,--build-id=none					\
			-nostdlib

SYS_OBJS	+=	startup.o armv7_libs.o armv7_dispatcher.o subcpu.o	\
			plat_pm.o resetcon.o GPIO.o debug.o CRC32.o		\
			gic.o arm_gic.o dpc.o buildinfo.o printf.o		\
			util.o util_arm.o non_secure.o watchdog.o


SYS_OBJS	+=	smc_entry.o smc_handler.o sip_main.o std_svc_setup.o	\
			arm_topology.o psci_system_off.o psci_off.o psci_on.o 	\
			psci_suspend.o psci_common.o psci_main.o bclk-dfs.o

ifeq ($(MEMTYPE),DDR3)
SYS_OBJS	+=	init_DDR3.o
endif
ifeq ($(MEMTYPE),LPDDR3)
SYS_OBJS	+=	init_LPDDR3.o
endif

SYS_OBJS_LIST	=	$(addprefix $(DIR_OBJOUTPUT)/,$(SYS_OBJS))

SYS_INCLUDES	=	-I src				\
			-I src/services			\
			-I src/services/std_svc		\
			-I src/services/std_svc/psci	\
			-I src/include

################################################################################
$(DIR_OBJOUTPUT)/%.o: src/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
################################################################################
$(DIR_OBJOUTPUT)/%.o: src/%.S
	@echo [assemble....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(ASFLAG) $(CFLAGS) $(SYS_INCLUDES)
################################################################################
$(DIR_OBJOUTPUT)/%.o: src/memory/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/%.S
	@echo [assemble....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(ASFLAG) $(CFLAGS) $(SYS_INCLUDES)
################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/std_svc/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/std_svc/psci/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
################################################################################

all: mkobjdir $(SYS_OBJS_LIST) link bin

mkobjdir:
ifeq ($(OS),Windows_NT)
	@if not exist $(DIR_OBJOUTPUT)			\
		@$(MKDIR) $(DIR_OBJOUTPUT)
	@if not exist $(DIR_TARGETOUTPUT)		\
		@$(MKDIR) $(DIR_TARGETOUTPUT)
else
	@if	[ ! -e $(DIR_OBJOUTPUT) ]; then 	\
		$(MKDIR) $(DIR_OBJOUTPUT);		\
	fi;
	@if	[ ! -e $(DIR_TARGETOUTPUT) ]; then 	\
		$(MKDIR) $(DIR_TARGETOUTPUT);		\
	fi;
endif

link:
	@echo [link.... $(DIR_TARGETOUTPUT)/$(TARGET_NAME).elf]

	$(Q)$(CC) $(SYS_OBJS_LIST) $(LDFLAGS)		\
		-o $(DIR_TARGETOUTPUT)/$(TARGET_NAME).elf

bin:
	@echo [binary.... $(DIR_TARGETOUTPUT)/$(TARGET_NAME).bin]
	$(Q)$(MAKEBIN) -O binary $(DIR_TARGETOUTPUT)/$(TARGET_NAME).elf		\
		$(DIR_TARGETOUTPUT)/$(TARGET_NAME).bin

ifeq ($(OS),Windows_NT)
	@if exist $(DIR_OBJOUTPUT)			\
		@$(RM) $(DIR_OBJOUTPUT)\buildinfo.o
else
	@if	[ -e $(DIR_OBJOUTPUT) ]; then 		\
		$(RM) $(DIR_OBJOUTPUT)/buildinfo.o;	\
	fi;
endif

################################################################################
clean:
ifeq ($(OS),Windows_NT)
	@if exist $(DIR_OBJOUTPUT)			\
		@$(RMDIR) $(DIR_OBJOUTPUT)
	@if exist $(DIR_TARGETOUTPUT)			\
		@$(RMDIR) $(DIR_TARGETOUTPUT)
else
	@if	[ -e $(DIR_OBJOUTPUT) ]; then 		\
		$(RMDIR) $(DIR_OBJOUTPUT);		\
	fi;
	@if	[ -e $(DIR_TARGETOUTPUT) ]; then 	\
		$(RMDIR) $(DIR_TARGETOUTPUT);		\
	fi;
endif

-include $(SYS_OBJS_LIST:.o=.d)
