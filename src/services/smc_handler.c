/*
 * Copyright (c) 2013-2015, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <sysheader.h>
#include <smcc_helpers.h>
#include <psci.h>
#include <gic.h>

/* External function */
extern u32 std_svc_smc_handler(u32 smc_fid,
	u32 r1, u32 r2, u32 r3);

extern u32 sip_smc_handler(u32 smc_fid,
	u32 r1, u32 r2, u32 r3);

extern int s5p4418_bclk_dfs_handler(void);
extern int psci_cpu_off_handler(void);

#define IS_SMC_TYPE(_fid)	((_fid >> 24) & 0xF)

/* External Variable */
volatile u32 g_smc_id    = 0;
volatile u32 g_fiq_flag  = 0;
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

	eoir = gicc_get_iar(cpu_base);

	if (g_smc_id == (u32)0x84000002)
		psci_cpu_off_handler();

	else if (g_smc_id == (u32)0x82000009)
		s5p4418_bclk_dfs_handler();
	else {
		WARN("unknown parameter smc_id : 0x%08X intid:%x\r\n",
				g_smc_id, eoir);
	}

	gicc_set_eoir(cpu_base, eoir);
}

/*******************************************************************************
 * Top level handler for servicing BL1 SMCs.
 ******************************************************************************/
u32 bl1_smc_handler(u32 smc_fid, u32 r1, u32 r2, u32 r3)
{
	u8 smc_type = IS_SMC_TYPE(smc_fid);

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
