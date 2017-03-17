/*
 * Copyright (C) 2012 Nexell Co., All Rights Reserved
 * Nexell Co. Proprietary & Confidential
 *
 * NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
 * AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Module          : PHY for DDR SDRAM
 * File            : nx_ddrphy.h
 * Description     :
 * Author          : Firmware Team
 * History         : 2014.08.20 Hans create
 */
#ifndef __NX_DDRPHY_H__
#define __NX_DDRPHY_H__

struct  NX_DDRPHY_RegisterSet {
	u32 PHY_CON[44];
};

enum {
	DDRMODE_DDR2	= 0,
	DDRMODE_LPDDR1	= 0,
	DDRMODE_DDR3	= 1,
	DDRMODE_LPDDR2	= 2,
	DDRMODE_LPDDR3	= 3
};

enum {
	SHIFT_DEGREE_0		= 0,	/* 0 */
	SHIFT_DEGREE_365	= 1,	/* T */
	SHIFT_DEGREE_180	= 2,	/* T/2 */
	SHIFT_DEGREE_90		= 3,	/* T/4 */
	SHIFT_DEGREE_45		= 4,	/* T/8 */
	SHIFT_DEGREE_22_5	= 5	/* T/16 */
};

enum {
	PHY_DRV_STRENGTH_240OHM	= 0,	/* Div1, 240ohm */
	PHY_DRV_STRENGTH_120OHM	= 1,	/* Div2, 120ohm */
	PHY_DRV_STRENGTH_80OHM	= 2,	/* Div3, 80ohm */
	PHY_DRV_STRENGTH_60OHM	= 3,	/* Div4, 60ohm */
	PHY_DRV_STRENGTH_48OHM	= 4,	/* Div5, 48ohm */
	PHY_DRV_STRENGTH_40OHM	= 5,	/* Div6, 40ohm */
	PHY_DRV_STRENGTH_34OHM	= 6,	/* Div7, 34ohm */
	PHY_DRV_STRENGTH_30OHM	= 7	/* Div8, 30ohm */
};

enum {
	PHY_ODT_120OHM	= 1,	/* 120ohm receiver termination */
	PHY_ODT_60OHM	= 2,	/* 60ohm receiver termination */
	PHY_ODT_40OHM	= 3,	/* 40ohm receiver termination */
	PHY_ODT_30OHM	= 4	/* 30ohm receiver termination */
};

enum {
	CALIBRATION_FORCE	= 0,
	CALIBRATION_LONG	= 1,
	CALIBRATION_SHORT	= 2
};

enum {
    RD_DESKEW_CODE          = 0x1,
    RD_DESKEW_CLEAR         = 0x2,
    WR_DESKEW_CODE          = 0x3,
    WR_DESKEW_CLEAR         = 0x4,

    VWM_LEFT                = 0x5,  /* Valid Window Margin LEFT Value. */
    VWM_RIGHT               = 0x6,  /* Valid Window Margin RIGHT Value. */
    VWM_CENTER              = 0x7,  /* Valid Window Margin CENTER Value. */

    RD_VWMC                 = 0x8,  /* READ Valid Window Margin Center Value. */
    WR_VWMC                 = 0x9,  /* WRITE Valid Window Margin Center Value. */
    DM_VWMC                 = 0xA,  /* DM Valid Window Margin Center Value. */
    GATE_VWMC               = 0xB,  /* GATE Valid Window Margin Center Value. */
    VWM_FAIL_STATUS         = 0xC,  /* Valid Window Margin Search Fail Status. */

    GATE_CENTER_CYCLE       = 0xD,  /* Gate Center Cycle Adjust Value. */
    GATE_CENTER_CODE        = 0xE,  /* Gate Center Code Value. */
};

#endif /*  __NX_DDRPHY_H__ */
