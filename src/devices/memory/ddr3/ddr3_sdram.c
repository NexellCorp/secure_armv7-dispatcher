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

#include <drex.h>
#include <ddrphy.h>
#include <ddr3_sdram.h>

#define nop() __asm__ __volatile__("mov\tr0,r0\t@ nop\n\t");

struct s5p4418_drex_sdram_reg *const g_drex_reg =
    (struct s5p4418_drex_sdram_reg * const)PHY_BASEADDR_DREX_MODULE_CH0_APB;
struct s5p4418_ddrphy_reg *const g_ddrphy_reg =
    (struct s5p4418_ddrphy_reg * const)PHY_BASEADDR_DREX_MODULE_CH1_APB;

struct dram_device_info g_ddr3_info;

void dmc_delay(int ms)
{
	register volatile int count;

	for (count = 0; count < ms; count++)
		nop();
}

void send_directcmd(SDRAM_CMD cmd, unsigned char chipnum, SDRAM_MODE_REG mrx,
		unsigned short value)
{
	mmio_write_32((unsigned int *)&g_drex_reg->DIRECTCMD,
			(cmd << 24)|(chipnum << 20)|(mrx << 16)|value);
}

#if (CONFIG_SUSPEND_RESUME == 1)
void enter_self_refresh(void)
{
	union SDRAM_MR MR;
	unsigned int nTemp;
	unsigned int nChips = 0;

	int DDR3_CS_NUM, MR1_ODS, MR1_Rtt_Nom, MR1_nAL, nCWL;
	int reg_value;

	/* Alive power gate open */
	mmio_write_32(&pReg_Alive->ALIVEPWRGATEREG, 1);

	/* Read to Memroy Information (Scratch)*/
	reg_value = mmio_read_32(&pReg_Alive->ALIVESCRATCHVALUE6);

	DDR3_CS_NUM = (reg_value >> 0) & 0x3;
	MR1_ODS = (reg_value >> 2) & 0x1;
	MR1_Rtt_Nom = (reg_value >> 4) & 0x7;
	MR1_nAL	= (reg_value >> 8 ) & 0xF;
	nCWL	= (reg_value >> 12) & 0xF;

	if (DDR3_CS_NUM > 1)
		nChips = 0x3;
	else
		nChips = 0x1;

	while (mmio_read_32(&g_drex_reg->CHIPSTATUS) & 0xF) {
		nop();
	}

	/* Step 01. Send PALL Command */
	send_directcmd(SDRAM_CMD_PALL, 0, (SDRAM_MODE_REG)0, 0);
	if (DDR3_CS_NUM > 1)
		send_directcmd(SDRAM_CMD_PALL, 1, (SDRAM_MODE_REG)0, 0);

	dmc_delay(100);

	/* Step 02. (DRAM) ODT OFF */
	MR.Reg = 0;
	MR.MR2.RTT_WR = 0;							// 0: disable, 1: RZQ/4 (60ohm), 2: RZQ/2 (120ohm)
	MR.MR2.SRT = 0;								// self refresh normal range, if (ASR == 1) SRT = 0;
	MR.MR2.ASR = 1;								// auto self refresh enable
	MR.MR2.CWL = (nCWL - 5);

	send_directcmd(SDRAM_CMD_MRS, 0, SDRAM_MODE_REG_MR2, MR.Reg);
	if (DDR3_CS_NUM > 1)
		send_directcmd(SDRAM_CMD_MRS, 1, SDRAM_MODE_REG_MR2, MR.Reg);

	/* Step 03. (DRAM) Set the Drive Strength */
	MR.Reg = 0;
	MR.MR1.DLL = 1;								// 0: Enable, 1 : Disable
	MR.MR1.AL = MR1_nAL;
	MR.MR1.ODS1 = (MR1_ODS >> 1) & 1;
	MR.MR1.ODS0 = (MR1_ODS >> 0) & 1;
	MR.MR1.RTT_Nom2 = (MR1_Rtt_Nom >> 2) & 1;
	MR.MR1.RTT_Nom1 = (MR1_Rtt_Nom >> 1) & 1;
	MR.MR1.RTT_Nom0 = (MR1_Rtt_Nom >> 0) & 1;
	MR.MR1.QOff = 0;
	MR.MR1.WL = 0;
#if 0
	MR.MR1.TDQS     = (_DDR_BUS_WIDTH >> 3) & 1;
#endif

	send_directcmd(SDRAM_CMD_MRS, 0, SDRAM_MODE_REG_MR1, MR.Reg);
	if (DDR3_CS_NUM > 1)
		send_directcmd(SDRAM_CMD_MRS, 1, SDRAM_MODE_REG_MR1, MR.Reg);

	/* Step 04. Enter Self-Refresh Command */
	send_directcmd(SDRAM_CMD_REFS, 0, (SDRAM_MODE_REG)0, 0);
	if (DDR3_CS_NUM > 1)
		send_directcmd(SDRAM_CMD_REFS, 1, (SDRAM_MODE_REG)0, 0);

	/*  Step 05. Check the Busy State */
	do {
		nTemp = (mmio_read_32(&g_drex_reg->CHIPSTATUS) & nChips);
	} while (nTemp);


	/* Step 06. Check the Sel-Refresh State (FSM) */
	do {
		nTemp = ((mmio_read_32(&g_drex_reg->CHIPSTATUS) >> 8) & nChips);
	} while (nTemp != nChips);

#if 0
	/* Step 07. Disable the Auto refresh Counter */
	mmio_clear_32(&g_drex_reg->CONCONTROL, (0x1 << 5));			// afre_en[5]. Auto Refresh Counter. Disable:0, Enable:1
#endif
	/* Step  08. Disable the Dynamic Clock */
	mmio_set_32  (&g_drex_reg->MEMCONTROL, (0x1 << 0));			// clk_stop_en[0] : Dynamic Clock Control   :: 1'b0  - Always running

}

void exit_self_refresh(void)
{
	union SDRAM_MR MR;

	int DDR3_CS_NUM, MR1_ODS, MR1_Rtt_Nom, MR1_nAL, MR2_Rtt_WR, nCWL;
	int reg_value;

	/* Alive power gate open */
	mmio_write_32(&pReg_Alive->ALIVEPWRGATEREG, 1);

	/* Read to Memroy Information (Scratch)*/
	reg_value = mmio_read_32(&pReg_Alive->ALIVESCRATCHVALUE6);

	DDR3_CS_NUM = (reg_value >> 0) & 0x3;
	MR1_ODS = (reg_value >> 2) & 0x1;
	MR1_Rtt_Nom = (reg_value >> 4) & 0x7;
	MR1_nAL	= (reg_value >> 8 ) & 0xF;
	nCWL	= (reg_value >> 12) & 0xF;
	MR2_Rtt_WR = (reg_value >> 16) & 0x2;

	/* Step  01. Enable the Dynamic Clock */
	mmio_clear_32(&g_drex_reg->MEMCONTROL, (0x1 << 0));			// clk_stop_en[0] : Dynamic Clock Control   :: 1'b0  - Always running
	dmc_delay(10);

	/* Step 02. Enable the Auto refresh counter */
	mmio_set_32(&g_drex_reg->CONCONTROL, (0x1 << 5));			// afre_en[5]. Auto Refresh Counter. Disable:0, Enable:1
	dmc_delay(10);

	/* Step 03. Send PALL command */
	send_directcmd(SDRAM_CMD_PALL, 0, (SDRAM_MODE_REG)0, 0);
	if (DDR3_CS_NUM > 1)
		send_directcmd(SDRAM_CMD_PALL, 1, (SDRAM_MODE_REG)0, 0);

	/* Step 04. Set the Drive Strength */
	MR.Reg          = 0;
	MR.MR1.DLL      = 1;							// 0: Enable, 1 : Disable
	MR.MR1.AL       = MR1_nAL;
	MR.MR1.ODS1 	= (MR1_ODS >> 1) & 1;
	MR.MR1.ODS0 	= (MR1_ODS >> 0) & 1;
	MR.MR1.RTT_Nom2 = (MR1_Rtt_Nom >> 2) & 1;
	MR.MR1.RTT_Nom1 = (MR1_Rtt_Nom >> 1) & 1;
	MR.MR1.RTT_Nom0 = (MR1_Rtt_Nom >> 0) & 1;
	MR.MR1.QOff     = 0;
	MR.MR1.WL       = 0;
#if 0
	MR.MR1.TDQS     = (_DDR_BUS_WIDTH>>3) & 1;
#endif

	send_directcmd(SDRAM_CMD_MRS, 0, SDRAM_MODE_REG_MR1, MR.Reg);
	if (DDR3_CS_NUM > 1)
		send_directcmd(SDRAM_CMD_MRS, 1, SDRAM_MODE_REG_MR1, MR.Reg);

	/* Step 05. Set the ODT On */
	MR.Reg          = 0;
	MR.MR2.RTT_WR   = MR2_Rtt_WR;
	MR.MR2.SRT      = 0;							// self refresh normal range
	MR.MR2.ASR      = 0;							// auto self refresh disable
	MR.MR2.CWL      = (nCWL - 5);

	send_directcmd(SDRAM_CMD_MRS, 0, SDRAM_MODE_REG_MR2, MR.Reg);
	if (DDR3_CS_NUM > 1)
		send_directcmd(SDRAM_CMD_MRS, 1, SDRAM_MODE_REG_MR2, MR.Reg);

	/* Step 06. Exit the Self-Refresh Command */
	send_directcmd(SDRAM_CMD_REFSX, 0, (SDRAM_MODE_REG)0, 0);
	if (DDR3_CS_NUM > 1)
		send_directcmd(SDRAM_CMD_REFSX, 1, (SDRAM_MODE_REG)0, 0);

	/* Step 07. Check the Self-Refresh State (FSM) */
	while( mmio_read_32(&g_drex_reg->CHIPSTATUS) & (0xF << 8) ) {
		nop();
	}

}
#endif // #if (CONFIG_SUSPEND_RESUME == 1)
