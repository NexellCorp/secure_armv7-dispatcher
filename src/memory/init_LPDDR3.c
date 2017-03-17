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
#include <sysheader.h>

#include <nx_drex.h>
#include <nx_ddrphy.h>
#include "ddr3_sdram.h"

#define nop() __asm__ __volatile__("mov\tr0,r0\t@ nop\n\t");

static struct NX_DREXSDRAM_RegisterSet *const pReg_Drex =
	(struct NX_DREXSDRAM_RegisterSet * const)PHY_BASEADDR_DREX_MODULE_CH0_APB;

void delay(int milisecond)
{
	register volatile int count;

	for (count = 0; count < milisecond; count++)
		nop();
}

inline void SendDirectCommand(SDRAM_CMD cmd, u8 chipnum,
		SDRAM_MODE_REG mrx, u16 value)
{
	WIO32(&pReg_Drex->DIRECTCMD,
		  (u32)((cmd << 24) | ((chipnum & 1) << 20) |
			((mrx >> 6) & 0x3) | (((mrx >> 3) & 0x7) << 16) |
			((mrx & 0x7) << 10) | ((value & 0xFF) << 2)));
}

void enterSelfRefresh(void)
{
	u32 nTemp;
	u32 nChips = 0;

	if (pSBI->DII.ChipNum > 1)
		nChips = 0x3;
	else
		nChips = 0x1;

	while (RIO32(&pReg_Drex->CHIPSTATUS) & 0xF)
		nop();

	delay(10000);

	/* Send PALL command */
	SendDirectCommand(SDRAM_CMD_PALL, 0, (SDRAM_MODE_REG)CNULL, CNULL);
	if (pSBI->DII.ChipNum > 1)
		SendDirectCommand(SDRAM_CMD_PALL, 1, (SDRAM_MODE_REG)CNULL, CNULL);
	delay(10000);

	/* ODT Off */
	union SDRAM_MR MR;

	MR.LP_MR11.DQ_ODT = 0;	/* 0: Disable, 1: RZQ/4, 2:RZQ/2, 3:RZQ/1 */
	MR.LP_MR11.PD_CON = 0;	/* 0: ODT Disable by during power down. */
				/* 1: ODT Enable  by during power down. */
	/* MR11 */
	SendDirectCommand(SDRAM_CMD_MRS, 0, SDRAM_MODE_REG_MR11, MR.Reg);
	if (pSBI->DII.ChipNum > 1)
		SendDirectCommand(SDRAM_CMD_MRS, 1, SDRAM_MODE_REG_MR11,
			MR.Reg);

	delay(1000 * 3);

	/* Enter self-refresh command */
	SendDirectCommand(SDRAM_CMD_REFS, 0, (SDRAM_MODE_REG)CNULL, CNULL);
	if (pSBI->DII.ChipNum > 1)
		SendDirectCommand(SDRAM_CMD_REFS, 1, (SDRAM_MODE_REG)CNULL, CNULL);

	delay(1000 * 3);

	do {
		nTemp = (RIO32(&pReg_Drex->CHIPSTATUS) & nChips);
	} while (nTemp);

	do {
		nTemp = ((RIO32(&pReg_Drex->CHIPSTATUS) >> 8) & nChips);
	} while (nTemp != nChips);

	delay(1000 * 3);

	/* Auto refresh counter disable */
	/* afre_en[5]. Auto Refresh Counter. Disable:0, Enable:1 */
	CIO32(&pReg_Drex->CONCONTROL, (0x1<< 5));

	/* ACK, ACKB off */
	/* clk_stop_en[0] : Dynamic Clock Control : 1'b0  - Always running */
	SIO32(&pReg_Drex->MEMCONTROL, (0x1 << 0));
	delay(1000 * 3);
}

