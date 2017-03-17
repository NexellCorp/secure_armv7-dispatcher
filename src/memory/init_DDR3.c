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
	WIO32((u32 *)&pReg_Drex->DIRECTCMD,
			cmd << 24 | chipnum << 20 | mrx << 16 | value);
}

void enterSelfRefresh(void)
{
	union SDRAM_MR MR;
	u32 nTemp;
	u32 nChips = 0;

	if (pSBI->DII.ChipNum > 1)
		nChips = 0x3;
	else
		nChips = 0x1;

	while (RIO32(&pReg_Drex->CHIPSTATUS) & 0xF)
		nop();

	/* Send PALL command */
	SendDirectCommand(SDRAM_CMD_PALL, 0, (SDRAM_MODE_REG)CNULL, CNULL);
	if (pSBI->DII.ChipNum > 1)
		SendDirectCommand(SDRAM_CMD_PALL, 1, (SDRAM_MODE_REG)CNULL, CNULL);
	delay(100);

	/* odt off */
	MR.Reg		= 0;
	MR.MR2.RTT_WR	= 0;	/* 0: disable, 1: RZQ/4 (60ohm), 2: RZQ/2 (120ohm) */
	MR.MR2.SRT	= 0;	/* self refresh normal range, if (ASR == 1) SRT = 0; */
	MR.MR2.ASR	= 1;	/* auto self refresh enable */
	MR.MR2.CWL	= (pSBI->DII.CWL - 5);

	SendDirectCommand(SDRAM_CMD_MRS, 0, SDRAM_MODE_REG_MR2, MR.Reg);
	if (pSBI->DII.ChipNum > 1)
		SendDirectCommand(SDRAM_CMD_MRS, 1, SDRAM_MODE_REG_MR2, MR.Reg);

	MR.Reg		= 0;
	MR.MR1.DLL	= 1;    /* 0: Enable, 1 : Disable */
	MR.MR1.AL	= pSBI->DII.MR1_AL;
	MR.MR1.ODS1	= (pSBI->DDR3_DSInfo.MR1_ODS >> 1) & 1;
	MR.MR1.ODS0	= (pSBI->DDR3_DSInfo.MR1_ODS >> 0) & 1;
	MR.MR1.RTT_Nom2	= (pSBI->DDR3_DSInfo.MR1_RTT_Nom >> 2) & 1;
	MR.MR1.RTT_Nom1	= (pSBI->DDR3_DSInfo.MR1_RTT_Nom >> 1) & 1;
	MR.MR1.RTT_Nom0	= (pSBI->DDR3_DSInfo.MR1_RTT_Nom >> 0) & 1;
	MR.MR1.QOff	= 0;
	MR.MR1.WL	= 0;

	SendDirectCommand(SDRAM_CMD_MRS, 0, SDRAM_MODE_REG_MR1, MR.Reg);
	if (pSBI->DII.ChipNum > 1)
		SendDirectCommand(SDRAM_CMD_MRS, 1, SDRAM_MODE_REG_MR1, MR.Reg);

	/* Enter self-refresh command */
	SendDirectCommand(SDRAM_CMD_REFS, 0, (SDRAM_MODE_REG)CNULL, CNULL);
	if (pSBI->DII.ChipNum > 1)
		SendDirectCommand(SDRAM_CMD_REFS, 1, (SDRAM_MODE_REG)CNULL, CNULL);

	do {
		nTemp = RIO32(&pReg_Drex->CHIPSTATUS) & nChips;
	} while (nTemp);

	do {
		nTemp = (RIO32(&pReg_Drex->CHIPSTATUS) >> 8) & nChips;
	} while (nTemp != nChips);


	/* Auto refresh counter disable */
	/* afre_en[5]. Auto Refresh Counter. Disable:0, Enable:1 */
	CIO32(&pReg_Drex->CONCONTROL, (0x1 << 5));

	/* ACK, ACKB off */
	/* clk_stop_en[0] : Dynamic Clock Control   :: 1'b0  - Always running */
	SIO32(&pReg_Drex->MEMCONTROL, (0x1 << 0));

	delay(1000 * 3);
}

