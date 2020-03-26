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

include config.mak

LDFLAGS		=	-Bstatic							\
			-Wl,-Map=$(DIR_TARGETOUTPUT)/$(TARGET_NAME).map,--cref		\
			-T$(LDS_NAME).lds						\
			-Wl,--start-group						\
			-Lsrc/$(DIR_OBJOUTPUT)						\
			-Wl,--end-group							\
			-Wl,--build-id=none						\
			-nostdlib

SYS_OBJS	+=	startup.o armv7_libs.o clockinit.o armv7_dispatcher.o subcpu.o	\
			plat_pm.o ema.o resetcon.o GPIO.o debug.o util.o CRC32.o	\
			gic.o arm_gic.o dpc.o buildinfo.o printf.o

ifeq ($(ARM_SECURE), n)
SYS_OBJS	+=	non_secure.o smc_entry.o smc_handler.o sip_main.o std_svc_setup.o	\
			arm_topology.o psci_system_off.o psci_off.o psci_on.o 			\
			psci_suspend.o psci_common.o psci_main.o bclk-dfs.o
endif

ifeq ($(MEMTYPE),ddr3)
SYS_OBJS	+=	ddr3_sdram.o
endif
ifeq ($(MEMTYPE),lpddr3)
SYS_OBJS	+=	lpddr3_sdram.o
endif

SYS_OBJS_LIST	=	$(addprefix $(DIR_OBJOUTPUT)/,$(SYS_OBJS))

SYS_INCLUDES	=	-I src				\
			-I src/devices			\
			-I src/devices/memory		\
			-I src/devices/memory/ddr3	\
			-I src/services			\
			-I src/services/std_svc		\
			-I src/services/std_svc/psci	\
			-I prototype/base 		\
			-I prototype/module

###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/%.S
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(ASFLAG) $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################

###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/pmic/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/devices/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/devices/memory/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/devices/memory/ddr3/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################


###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/%.S
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(ASFLAG) $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/std_svc/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/std_svc/psci/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/std_svc/psci/%.S
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(ASFLAG) $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################

all: mkobjdir startup $(SYS_OBJS_LIST) link bin

mkobjdir:
ifeq ($(OS),Windows_NT)
	@if not exist $(DIR_OBJOUTPUT)			\
		@$(MKDIR) $(DIR_OBJOUTPUT)
	@if not exist $(DIR_TARGETOUTPUT)		\
		@$(MKDIR) $(DIR_TARGETOUTPUT)
else
#	@if [ ! -L prototype ] ; then			\
#		ln -s ../../../prototype/s5p4418/ prototype ; \
	fi
	@if	[ ! -e $(DIR_OBJOUTPUT) ]; then 	\
		$(MKDIR) $(DIR_OBJOUTPUT);		\
	fi;
	@if	[ ! -e $(DIR_TARGETOUTPUT) ]; then 	\
		$(MKDIR) $(DIR_TARGETOUTPUT);		\
	fi;
endif

startup:
	@echo [compile....startup.S]
	$(Q)$(CC) -MMD src/startup.S -c -o $@ $(ASFLAG) $(CFLAGS) $(SYS_INCLUDES)

link: $(SYS_OBJS_LIST)
	@echo [link.... $(DIR_TARGETOUTPUT)/$(TARGET_NAME).elf]

	$(Q)$(CC) $(SYS_OBJS_LIST) $(LDFLAGS) -o $(DIR_TARGETOUTPUT)/$(TARGET_NAME).elf

bin: link
	@echo [binary.... $(DIR_TARGETOUTPUT)/$(TARGET_NAME).bin]
	$(Q)$(MAKEBIN) -O binary $(DIR_TARGETOUTPUT)/$(TARGET_NAME).elf $(DIR_TARGETOUTPUT)/$(TARGET_NAME).bin
ifeq ($(OS),Windows_NT)
	@if exist $(DIR_OBJOUTPUT)			\
		@$(RM) $(DIR_OBJOUTPUT)\buildinfo.o
else
	@if	[ -e $(DIR_OBJOUTPUT) ]; then 		\
		$(RM) $(DIR_OBJOUTPUT)/buildinfo.o;	\
	fi;
endif

###################################################################################################
clean:
ifeq ($(OS),Windows_NT)
	@if exist $(DIR_OBJOUTPUT)			\
		@$(RMDIR) $(DIR_OBJOUTPUT)
	@if exist $(DIR_TARGETOUTPUT)			\
		@$(RMDIR) $(DIR_TARGETOUTPUT)
else
	@if [ -L prototype ] ; then			\
		$(RM) prototype ;			\
	fi
	@if	[ -e $(DIR_OBJOUTPUT) ]; then 		\
		$(RMDIR) $(DIR_OBJOUTPUT);		\
	fi;
	@if	[ -e $(DIR_TARGETOUTPUT) ]; then 	\
		$(RMDIR) $(DIR_TARGETOUTPUT);		\
	fi;
endif

-include $(SYS_OBJS_LIST:.o=.d)
