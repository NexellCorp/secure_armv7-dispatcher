/*
 * Copyright (C) 2012 Nexell Co., All Rights Reserved
 * Nexell Co. Proprietary & Confidential
 *
 * NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
 * AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Module          : Display Port Controller
 * File            : nx_drex.h
 * Description     :
 * Author          : Firmware Team
 * History         : 2014.08.20 Hans create
 */
#ifndef __NX_DREX_H__
#define __NX_DREX_H__

#define PORTNUM		4


struct NX_DREX_QOS {
	u32 QOSCONTROL;
	u32 _Reserved;
};

struct NX_DREX_ACTIMING
{
	u32 TIMINGROW;		/* 0x034 : AC Timing for SDRAM Row */
	u32 TIMINGDATA;		/* 0x038 : AC Timing for SDRAM Data */
	u32 TIMINGPOWER;	/* 0x03C : AC Timing for Power Mode of SDRAM */
};

struct NX_DREX_BP {
	u32 BP_CONTROL;		/* 0x2x0 : Back Pressure Control */
	u32 BP_CONFIG_R;	/* 0x2x4 : Back Pressure Configuration for Read */
	u32 BP_CONFIG_W;	/* 0x2x8 : Back Pressure Configuration for Write */
	u32 _Reserved;		/* 0x2xC */
};

struct  NX_DREXSDRAM_RegisterSet {
	u32 CONCONTROL;				/* 0x00 */
	u32 MEMCONTROL;				/* 0x04 */
	u32 MEMCONFIG[2];			/* 0x08 ~ 0x0C */
	u32 DIRECTCMD;				/* 0x10 */
	u32 PRECHCONFIG;			/* 0x14 */
	u32 PHYCONTROL[4];			/* 0x18 ~ 0x24 */
	u32 PWRDNCONFIG;			/* 0x28 */
	u32 TIMINGPZQ;				/* 0x2C */
	u32 TIMINGAREF;				/* 0x30 */
	u32 TIMINGROW;				/* 0x34 */
	u32 TIMINGDATA;				/* 0x38 */
	u32 TIMINGPOWER;			/* 0x3C */
	u32 PHYSTATUS;				/* 0x40 */
	u32 PAD_0[1];				/* 0x44 */
	u32 CHIPSTATUS;				/* 0x48 */
	u32 PAD_1[2];				/* 0x4C ~ 0x50 */
	u32 MRSTATUS;				/* 0x54 */
	u32 PAD_2[2];				/* 0x58 ~ 0x5C */
	struct NX_DREX_QOS QOSCONTROL[16];	/* 0x60 ~ 0xDC */
	u32 PAD_19[5];				/* 0xE0 ~ 0xF0 */
	u32 WRTRA_CONFIG;			/* 0xF4 */
	u32 RDLVL_CONFIG;			/* 0xF8 */
	u32 PAD_20[1];				/* 0xFC */
	u32 BRBRSVCONTROL;			/* 0x100 */
	u32 BRBRSVCONFIG;			/* 0x104 */
	u32 BRBQOSCONFIG;			/* 0x108 */
	u32 MEMBASECONFIG[2];			/* 0x10C ~ 0x110 */
	u32 PAD_21[3];				/* 0x114 ~ 0x11C */
	u32 WRLVL_CONFIG[2];			/* 0x120 ~ 0x124 */
	u32 WRLVL_STATUS;			/* 0x128 */
	u32 PAD_22[9];				/* 0x12C ~ 0x14C */
	u32 CTRL_IO_RDATA;			/* 0x150 */
	u32 PAD_23[3];				/* 0x154 ~ 0x15C */
	u32 CACAL_CONFIG[2];			/* 0x160 ~ 0x164 */
	u32 CACAL_STATUS;			/* 0x168 */
};


enum {
	DREX_PORT_DISP		= 0,
	DREX_PORT_CCI		= 1,
	DREX_PORT_BOT		= 2
};

typedef enum {
	SDRAM_CMD_MRS		= 0x0,	/* MRS/EMRS	mode register */
	SDRAM_CMD_EMRS		= 0x0,
	SDRAM_CMD_PALL		= 0x1,	/* all banks precharge */
	SDRAM_CMD_PRE		= 0x2,	/* per bank precharge */
	SDRAM_CMD_REFS		= 0x4,	/* self refresh */
	SDRAM_CMD_REFA		= 0x5,	/* auto refresh */
	/* don't use this command if pb_ref_en is enabled in MemControl register */
	SDRAM_CMD_CKEL		= 0x6,	/* active/precharge power down */
	SDRAM_CMD_NOP		= 0x7,	/* exit from active/precharge power down */
	SDRAM_CMD_REFSX		= 0x8,	/* exit from self refresh */
	SDRAM_CMD_MRR		= 0x9,	/* mode register reading */
	SDRAM_CMD_ZQINIT	= 0xa,	/* ZQ calibration init */
	SDRAM_CMD_ZQOPER	= 0xb,	/* ZQ calibration long */
	SDRAM_CMD_ZQCS		= 0xc,	/* ZQ calibration short */
	SDRAM_CMD_SRR		= 0xd	/* for Wide IO Memory */
}	SDRAM_CMD;


typedef enum {
	WRITE_LEVEL_STATUS_FSM_IDLE	= 1 << 0,
	WRITE_LEVEL_STATUS_FSM_SETUP	= 1 << 1,
	WRITE_LEVEL_STATUS_FSM_ACCESS	= 1 << 2,
	WRITE_LEVEL_STATUS_FSM_DONE	= 1 << 3,
	WRITE_LEVEL_STATUS_FSM_TWLO	= 1 << 4
}	WRITE_LEVEL_STATUS;

enum {
	CHIP_COL_7	= 0,
	CHIP_COL_8	= 1,
	CHIP_COL_9	= 2,
	CHIP_COL_10	= 3,
	CHIP_COL_11	= 4
};

enum {
	CHIP_ROW_12 = 0,
	CHIP_ROW_13 = 1,
	CHIP_ROW_14 = 2,
	CHIP_ROW_15 = 3,
	CHIP_ROW_16 = 4
};

enum {
	CHIP_BANK_4 = 2,
	CHIP_BANK_8 = 3
};
enum {
	CHIP_SIZE_MASK_256MB	= 0x7F0,
	CHIP_SIZE_MASK_512MB	= 0x7E0,
	CHIP_SIZE_MASK_1GB	= 0x7C0,
	CHIP_SIZE_MASK_2GB	= 0x780,
	CHIP_SIZE_MASK_4GB	= 0x700
};

enum {
	BANK_OF_LSB_BIT_POSITION_8	= 0,	/* column low size 256B */
	BANK_OF_LSB_BIT_POSITION_9	= 1,	/* column low size 512B */
	BANK_OF_LSB_BIT_POSITION_10	= 2,	/* column low size 1KB */
	BANK_OF_LSB_BIT_POSITION_11	= 3,	/* column low size 2KB */
	BANK_OF_LSB_BIT_POSITION_12	= 4,	/* column low size 4KB */
	BANK_OF_LSB_BIT_POSITION_13	= 5 	/* column low size 8KB */
};

#endif /* __NX_DREX_H__ */
