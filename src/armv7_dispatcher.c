/*
 * Copyright (C) 2016  Nexell Co., Ltd.
 * Author: Sangjong, Han <hans@nexell.co.kr>
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
#define __SET_GLOBAL_VARIABLES

#include "sysheader.h"

extern void buildinfo(void);
extern void secure_launch(CBOOL isresume, U32 secureos_startaddr,
							U32 non_secure_bl, U32 bootarg_addr);
extern void non_secure_launch(CBOOL isresume, U32 non_secure_bl);

extern void s5p4418_tee_bclkwait(void);
extern void s5p4418_tee_bclk(unsigned int pll_data, int wait_flag);
extern void s5p4418_tee_suspend(unsigned int sec_entrypoint);

void tieoff_set_secure(void);
void device_reset(void);
extern U32 g_subcpu_ep;

struct {
	U32 sec_arg[6];
	void (*tee_bclkwait)(void);
	void (*tee_bclk)(U32, int);
	void (*tee_suspend)(void);
}g_bootarg_tee;

void secureos_bootarg(void)
{
	U32 secureos_id =	((((U32)'W') <<  24)|
						(((U32)'A') <<  16)	|
						(((U32)'R') << 8)	|
						(((U32)'E') << 0));

	if ((U32)pReg_ClkPwr->SCRATCH[2] == secureos_id)
	{
		g_bootarg_tee.sec_arg[0] = SRAM_STACK_BASE;
		g_bootarg_tee.sec_arg[1] = SRAM_STACK_OFFSET;
		g_bootarg_tee.tee_bclkwait = &s5p4418_tee_bclkwait;
		g_bootarg_tee.tee_bclk = &s5p4418_tee_bclk;
		g_bootarg_tee.tee_suspend = &s5p4418_tee_suspend;
	}
	else
	{
		g_bootarg_tee.sec_arg[0] = SECOS_MAGIC_NUM;
		g_bootarg_tee.sec_arg[1] = 0x1;
		g_bootarg_tee.sec_arg[2] = DRAM_BASE;
		g_bootarg_tee.sec_arg[3] = DRAM_SIZE;
		g_bootarg_tee.sec_arg[4] = SEC_DRAM_BASE;
		g_bootarg_tee.sec_arg[5] = SEC_DRAM_SIZE;
		g_bootarg_tee.tee_bclkwait = &s5p4418_tee_bclkwait;
		g_bootarg_tee.tee_bclk = &s5p4418_tee_bclk;
		g_bootarg_tee.tee_suspend = &s5p4418_tee_suspend;
	}
}

void BootMain(CBOOL isresume, U32 non_secure_bl, U32 secureos_startaddr)
{
	U32 debug_ch = 0;
	g_subcpu_ep = secureos_startaddr;

//#if defined(AVN) || defined(NAVI) || defined(RAPTOR)
	debug_ch = 3;
//#endif

#if 1 // Low Level Debug Message
	/*  Low Level Debug Message */
	DebugInit(debug_ch);
#endif

	/* Build information */
	buildinfo();

	/* Set sdram to support secure address */
#if defined(CONFIG_DREX_BUS_SECURE)
	mmio_set_32(&pReg_Tieoff->TIEOFFREG[24],
		(0xF << 28 | 0xF << 24 | 1 << 17 | 1 << 16));
#endif

	tieoff_set_secure();	// crypto, tieoff, sram ...

	device_reset();		// timer reset

	pReg_INTC[0]->INTMODE = 0;		// all irq mode
	pReg_INTC[0]->INTENABLE = 0;		// all irq disable
	pReg_INTC[0]->INTDISABLE = 0xFFFFFFFF;	// all irq disable
	*(U32*)(PHY_BASEADDR_INTC_MODULE + 0x1000 * 0 + 0x300) = 0;	// vic test control reg?
	pReg_INTC[0]->SWINTCLEAR = 0xFFFFFFFF;	// all sw irq clear

	pReg_INTC[1]->INTMODE = 0;		// all irq mode
	pReg_INTC[1]->INTENABLE = 0;		// all irq disable
	pReg_INTC[1]->INTDISABLE = 0xFFFFFFFF;	// all irq disable
	*(U32*)(PHY_BASEADDR_INTC_MODULE + 0x1000 * 1 + 0x300) = 0;	// vic test control reg?
	pReg_INTC[1]->SWINTCLEAR = 0xFFFFFFFF;	// all sw irq clear

	if (secureos_startaddr) {
		INFO("Launch to secure 0x%08X\r\n", (U32)secureos_startaddr);
		if (!isresume)
			secureos_bootarg();
		while (!DebugIsUartTxDone())
			;
		secure_launch(isresume, secureos_startaddr, non_secure_bl, (U32)&g_bootarg_tee);
	} else {
		if (isresume)
			non_secure_bl = pSBI->EntryPoint;
		INFO("Launch to non-secure 0x%08X\r\n", (U32)non_secure_bl);
		while (!DebugIsUartTxDone())
			;
		non_secure_launch(isresume, non_secure_bl);
	}
}
