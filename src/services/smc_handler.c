/*
 * Copyright (C) 2016  Nexell Co., Ltd.
 * Author: DeokJin, Lee <truevirtue@nexell.co.kr>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <sysheader.h>
#include <smcc_helpers.h>
#include <psci.h>
#include <gic.h>

/* External function */
extern unsigned int std_svc_smc_handler(unsigned int smc_fid,
	unsigned int r1, unsigned int r2, unsigned int r3);

extern unsigned int sip_smc_handler(unsigned int smc_fid,
	unsigned int r1, unsigned int r2, unsigned int r3);

extern int s5p4418_bclk_dfs_handler(void);
extern int psci_cpu_off_handler(void);

#define IS_SMC_TYPE(_fid)	((_fid >> 24) & 0xF)

/* External Variable */
volatile unsigned int g_smc_id    = 0;
volatile unsigned int g_fiq_flag  = 0;
volatile int g_cpu_kill_num = 0;

/*******************************************************************************
 * Monitor Mode FIQ Hanlder ofr servicing BL1 SMCs.
 * Currently purpose used for the BCLK DFS.
 ******************************************************************************/
void smc_set_monitor_fiq(int enable)
{
	int reg = 0;
	int bit_pos = 2;

	reg = arm9_get_scr();
	reg &= ~(1 << bit_pos);
	if (enable)
		reg |= (enable << bit_pos);
	arm9_set_scr(reg);
}


/*******************************************************************************
 * Monitor Mode FIQ Hanlder ofr servicing BL1 SMCs.
 * Currently purpose used for the BCLK DFS.
 ******************************************************************************/
void smc_monitor_fiq_handler(void)
{
	char* cpu_base = (char*)gicc_get_baseaddr();
	int eoir;

	set_secure_mode();

	eoir = gicc_get_iar(cpu_base);
	if (g_smc_id == (unsigned int)0x84000002)
		psci_cpu_off_handler();

	else if (g_smc_id == (unsigned int)0x82000009)
		s5p4418_bclk_dfs_handler();
	else {
		WARN("unknown parameter smc_id : 0x%08X intid:%x\r\n",
				g_smc_id, eoir);
	}

	gicc_set_eoir(cpu_base, eoir);
	set_nonsecure_mode();
}

/*******************************************************************************
 * Top level handler for servicing BL1 SMCs.
 ******************************************************************************/
unsigned int bl1_smc_handler(unsigned int smc_fid,
	unsigned int r1, unsigned int r2, unsigned int r3)
{
	unsigned char smc_type = IS_SMC_TYPE(smc_fid);

	DBGOUT("R0: %X, R1: %X, R2: %X, R3: %X \r\n", smc_fid, r1, r2, r3);

	switch (smc_type) {
	case OEN_SIP_START:
		return sip_smc_handler(smc_fid, r1, r2, r3);

	case OEN_STD_START:
		return std_svc_smc_handler(smc_fid, r1, r2, r3);
	}

	WARN("Unimplemented BL1 SMC Call: 0x%x\r\n", smc_fid);

	return 0;
}
