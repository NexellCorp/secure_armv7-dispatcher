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
 * Author	: DeokJin, Lee <truevirtue@nexell.co.kr>
 * History	: 2017.03.14 new release
 */
#include <sysheader.h>
#include <gic.h>
#include <dpc.h>

#define SGI_IRQ_8		8

/* Direct Command */
#define DIRCMD_TYPE_SHIFT	(24)
#define DIRCMD_CHIP_SHIFT	(20)
#define DIRCMD_PALL		(0x1)
#define DIRCMD_CHIP_0		(0x0)
extern volatile u32 g_smc_id;
extern volatile u32 g_fiq_flag;
extern volatile union cpuonstatus ourcpus;

static volatile union cpuonstatus devdfs;

/* External function */
extern int  armv7_get_cpuid(void);
extern void s5p4418_cpuidle(int cpu_id, int int_id);

void s5p4418_bclk_dfs_handler(void)
{
	int cpu_id = armv7_get_cpuid();

	devdfs.cpu[cpu_id] = 1;

	while (devdfs.cpu[cpu_id])
		;
}

/*******************************************************************************
 * For the BCLK DFS, the remaining CPU changes to the WakeUp state.
 ******************************************************************************/
static void cpu_up_force(void)
{
	g_fiq_flag = 0;
	devdfs.cpus = 0;
}

/*******************************************************************************
 * For the BCLK DFS, the remaining CPU changes to the IDLE state.
 * That prevents the memory access to the other CPU.
 ******************************************************************************/
static void cpu_down_force(void)
{
	int cpu_id = armv7_get_cpuid();

	g_smc_id = 0x82000009;
	devdfs.cpu[cpu_id] = 1;

	/* Using the SGI(GIC), and calls to the idle of the CORE FIQ. */
	g_fiq_flag = (0xF & ~(1 << cpu_id));
	s5p4418_cpuidle(g_fiq_flag, SGI_IRQ_8);

	while (devdfs.cpus != ourcpus.cpus)
		;
}

static void send_directcmd(u32 cmd, u8 chip, u32 mrx, u16 value)
{
	void* drex_reg = (void*)PHY_BASEADDR_DREX_MODULE_CH0_APB;
	WIO32((drex_reg + 0x10), ((cmd << DIRCMD_TYPE_SHIFT)
		| (chip << DIRCMD_CHIP_SHIFT) | (mrx << 16) | value));
}

static void sram_set_auto_refresh(int enb)
{
	void* drex_reg = (void*)PHY_BASEADDR_DREX_MODULE_CH0_APB;
	int value = 0;

	value = RIO32(drex_reg + 0x00);
	value &= ~(0x1 << 5);

	if (enb)
		value |= (0x1 << 5);

	WIO32((drex_reg + 0x00), value);
}

void s5p4418_change_pll(u32 pll_data)
{
	u32 pll_num	= (pll_data & 0x00000003);
	u32 s		= (pll_data & 0x000000fc) >> 2;
	u32 m		= (pll_data & 0x00ffff00) >> 8;
	u32 p		= (pll_data & 0xff000000) >> 24;

	CIO32(&pReg_ClkPwr->PLLSETREG[pll_num], 1 << 28);
	WIO32(&pReg_ClkPwr->CLKMODEREG0, 1 << pll_num);
	while (RIO32(&pReg_ClkPwr->CLKMODEREG0) & (1 << 31))
		;

	WIO32(&pReg_ClkPwr->PLLSETREG[pll_num],
			(1UL << 29) |
			(0UL << 28) |
			(s   << 0)  |
			(m   << 8)  |
			(p   << 18));
	WIO32(&pReg_ClkPwr->CLKMODEREG0, 1 << pll_num);
	while (RIO32(&pReg_ClkPwr->CLKMODEREG0) & (1 << 31))
		;

	CIO32(&pReg_ClkPwr->PLLSETREG[pll_num], 1UL << 29);
	WIO32(&pReg_ClkPwr->CLKMODEREG0, 1 << pll_num);
	while (RIO32(&pReg_ClkPwr->CLKMODEREG0) & (1 << 31))
		;

	SIO32(&pReg_ClkPwr->PLLSETREG[pll_num], 1 << 28);
	WIO32(&pReg_ClkPwr->CLKMODEREG0, 1 << pll_num);
	while (RIO32(&pReg_ClkPwr->CLKMODEREG0) & (1 << 31))
		;
}
/*******************************************************************************
 * Must be S5P4418
 * PLL Change sequence in S5P4418 (Kernel)
 * must go through the following steps:
 *
 * Step 00. Change to cpu idle state
 * Step 01. DRAM Command - Precharge All
 * Step 02. Waiting for DPC Blank
 * Step 03. Change to PLL(P,M,S)
 * Step 04. Clear & Waiting for DPC Blank
 * Step 05. DRAM Command - Precharge All
 * Step 06. Change to cpu wakeup state
 ******************************************************************************/
void s5p4418_bclk_freqchange(u32 pll_data)
{
	int dpc_index = 0;

	/* Step 01. DRAM Command - Precharge All */
	sram_set_auto_refresh(0);

	send_directcmd(DIRCMD_PALL, DIRCMD_CHIP_0, 0, 0);

	/* Step 02. Waiting for DPC Blank */
	if (dpc_enabled(dpc_index)) {
		dpc_set_enable_all(dpc_index, 0);
		while (!dpc_get_pending_all(dpc_index))
			;
	}

	/* Step 03. change to PLL(P,M,S) */
	s5p4418_change_pll(pll_data);

	/* Step 04. Clear & Waiting for DPC Blank */
	if (dpc_enabled(dpc_index)) {
		dpc_clear_pending_all(dpc_index);
		dpc_set_enable_all(dpc_index, 1);
	}

	/* Step 05. DRAM Command - Precharge All */
	send_directcmd(DIRCMD_PALL, DIRCMD_CHIP_0, 0, 0);

	sram_set_auto_refresh(1);
}

void s5p4418_bclk_dfs(u32 pll_data)
{
	/* Step 00. change to cpu idle state */
	cpu_down_force();

	s5p4418_bclk_freqchange(pll_data);

	/* Step 06. change to cpu wakeup state */
	cpu_up_force();
}

volatile u32 tee_wait_bclk_dfs_flag[4] = {0};
void s5p4418_tee_bclkwait(void)
{
	int cpu_id = armv7_get_cpuid();

	tee_wait_bclk_dfs_flag[cpu_id] = 1;
	/* Wait until PLL freq change is done */
	while (tee_wait_bclk_dfs_flag[cpu_id])
		;
}

void s5p4418_tee_bclk(u32 pll_data, u32 wait_flag)
{
	u32 read_flag = 0;
	u32 i = 0;

	/* Waiting for all other cores to be in s5p4418_tee_bclkwait() */
	do {
		read_flag = 0;
		for (i = 0; i < 4; i++)
			read_flag |= tee_wait_bclk_dfs_flag[i] << i;
	} while (wait_flag != read_flag);

	s5p4418_bclk_freqchange(pll_data);

	/* Release the other cores */
	for (i = 0; i < 4; i++)
		tee_wait_bclk_dfs_flag[i] = 0;
}
