/*
 * Copyright (C) 2016  Nexell Co., Ltd. All Rights Reserved.
 * Nexell Co. Proprietary & Confidential
 *
 * Nexell informs that this code and information is provided "as is" base
 * and without warranty of any kind, either expressed or implied, including
 * but not limited to the implied warranties of merchantability and/or
 * fitness for a particular puporse.
 *
 * Module	:
 * File		:
 * Description	:
 * Author	: Hans
 * History	: 2017.03.14 new release
 */
#define __SET_GLOBAL_VARIABLES

#include <sysheader.h>

#include <nx_intc.h>

extern void buildinfo(void);
extern void secure_launch(cbool isresume, u32 secureos_startaddr,
				u32 non_secure_bl, u32 bootarg_addr);
extern void non_secure_launch(cbool isresume, u32 non_secure_bl);

extern void s5p4418_tee_bclkwait(void);
extern void s5p4418_tee_bclk(u32 pll_data, int wait_flag);
extern void s5p4418_tee_suspend(u32 sec_entrypoint);

void tieoff_set_secure(void);
void device_reset(void);
extern u32 g_subcpu_ep;

struct NX_INTC_RegisterSet (*const pReg_INTC)[1] =
	(struct NX_INTC_RegisterSet (* const)[])PHY_BASEADDR_INTC_MODULE;

volatile union cpuonstatus ourcpus;

struct {
	u32 sec_arg[6];
	void (*tee_bclkwait)(void);
	void (*tee_bclk)(u32, int);
	void (*tee_suspend)(u32);
	u32 rpmb_low;
	u32 rpmb_high;
	u32 huk1_addr;
	u32 huk1_size;
	u32 huk2_addr;
	u32 huk2_size;
}g_bootarg_tee;

void secureos_bootarg(void)
{
	u32 secureos_id =	((((u32)'W') << 24)	|
				 (((u32)'A') << 16)	|
				 (((u32)'R') <<  8)	|
				 (((u32)'E') <<  0));

	if ((u32)RIO32(&pReg_ClkPwr->SCRATCH[2]) == secureos_id) {
		g_bootarg_tee.sec_arg[0] = SRAM_STACK_BASE;
		g_bootarg_tee.sec_arg[1] = SRAM_STACK_OFFSET;
		g_bootarg_tee.tee_bclkwait = &s5p4418_tee_bclkwait;
		g_bootarg_tee.tee_bclk = &s5p4418_tee_bclk;
		g_bootarg_tee.tee_suspend = &s5p4418_tee_suspend;
	} else {
		g_bootarg_tee.sec_arg[0] = SECOS_MAGIC_NUM;
		g_bootarg_tee.sec_arg[1] = 0x1;
		g_bootarg_tee.sec_arg[2] = DRAM_BASE;
		g_bootarg_tee.sec_arg[3] = DRAM_SIZE;
		g_bootarg_tee.sec_arg[4] = SEC_DRAM_BASE;
		g_bootarg_tee.sec_arg[5] = SEC_DRAM_SIZE;
		g_bootarg_tee.tee_bclkwait = &s5p4418_tee_bclkwait;
		g_bootarg_tee.tee_bclk = &s5p4418_tee_bclk;
		g_bootarg_tee.tee_suspend = &s5p4418_tee_suspend;
		g_bootarg_tee.rpmb_low = (*(u32 *)0xB0FB0200) & 0xFFFF;
		g_bootarg_tee.rpmb_high = ((*(u32 *)0xB0FB0200) >> 16) & 0xFFFF;
		g_bootarg_tee.huk1_addr = 0xB0FB0100;
		g_bootarg_tee.huk1_size = 128;
		g_bootarg_tee.huk2_addr = 0xB0FB0000;
		g_bootarg_tee.huk2_size = 256;
	}
}

void BootMain(cbool isresume, u32 non_secure_bl, u32 secureos_startaddr)
{
	u32 debug_ch = 0;
	g_subcpu_ep = secureos_startaddr;

	debug_ch = 3;

	/*  Low Level Debug Message */
	DebugInit(debug_ch);

	/* Build information */
	buildinfo();

	ourcpus.cpu[0] = 1;	/* mark cpu 0 alive; */

	tieoff_set_secure();	/* ddr sdram, crypto, tieoff, sram ... */

	device_reset();		/* timer reset */

	WIO32(&pReg_INTC[0]->INTMODE, 0);		/* all irq mode */
	WIO32(&pReg_INTC[0]->INTENABLE, 0);		/* all irq disable */
	WIO32(&pReg_INTC[0]->INTDISABLE, 0xFFFFFFFF);	/* all irq disable */
	/* vic test control reg? */
	WIO32(PHY_BASEADDR_INTC_MODULE + 0x1000 * 0 + 0x300, 0);
	WIO32(&pReg_INTC[0]->SWINTCLEAR, 0xFFFFFFFF);	/* all sw irq clear */

	WIO32(&pReg_INTC[1]->INTMODE, 0);		/* all irq mode */
	WIO32(&pReg_INTC[1]->INTENABLE, 0);		/* all irq disable */
	WIO32(&pReg_INTC[1]->INTDISABLE, 0xFFFFFFFF);	/* all irq disable */
	/* vic test control reg? */
	WIO32(PHY_BASEADDR_INTC_MODULE + 0x1000 * 1 + 0x300, 0);
	WIO32(&pReg_INTC[1]->SWINTCLEAR, 0xFFFFFFFF);	/* all sw irq clear */

	if (secureos_startaddr) {
		INFO("Launch to secure 0x%08X\r\n", (u32)secureos_startaddr);
		if (!isresume)
			secureos_bootarg();
		while (!DebugIsUartTxDone())
			;
		secure_launch(isresume, secureos_startaddr, non_secure_bl, (u32)&g_bootarg_tee);
	} else {
		if (isresume)
			non_secure_bl = pSBI->EntryPoint;
		INFO("Launch to non-secure 0x%08X\r\n", (u32)non_secure_bl);
		while (!DebugIsUartTxDone())
			;
		non_secure_launch(isresume, non_secure_bl);
	}
}
