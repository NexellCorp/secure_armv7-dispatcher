/*
 * Copyright (C) 2012 Nexell Co., All Rights Reserved
 * Nexell Co. Proprietary & Confidential
 * 
 * NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
 * AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Module          : ALIVE
 * File            : nx_alive.c
 * Description     :
 * Author          :
 * History         :
 */
#ifndef __NX_ALIVE_H__
#define	__NX_ALIVE_H__


#define NX_ALIVE_NUMBER_OF_GPIO 6

struct NX_ALIVE_RegisterSet {
	u32 ALIVEPWRGATEREG;				/* 0x00 : Alive Power Gating */
	u32 ALIVEGPIOASYNCDETECTMODERSTREG0;		/* 0x04 : Alive GPIO ASync Detect Mode Reset 0 */
	u32 ALIVEGPIOASYNCDETECTMODESETREG0;		/* 0x08 : Alive GPIO ASync Detect Mode Set 0 */
	u32 ALIVEGPIOLOWASYNCDETECTMODEREADREG;		/* 0x0C : Alive GPIO Low ASync Detect Mode Read */

	u32 ALIVEGPIOASYNCDETECTMODERSTREG1;		/* 0x10 : Alive GPIO ASync Detect Mode Reset 1 */
	u32 ALIVEGPIOASYNCDETECTMODESETREG1;		/* 0x14 : Alive GPIO ASync Detect Mode Set 1 */
	u32 ALIVEGPIOHIGHASYNCDETECTMODEREADREG;	/* 0x18 : Alive GPIO High ASync Detect Mode Read */

	u32 ALIVEGPIODETECTMODERSTREG0;			/* 0x1C : Alive GPIO Detect Mode Reset 0 */
	u32 ALIVEGPIODETECTMODESETREG0;			/* 0x20 : Alive GPIO Detect Mode Reset 0 */
	u32 ALIVEGPIOFALLDETECTMODEREADREG;		/* 0x24 : Alive GPIO Falling Edge Detect Mode Read */

	u32 ALIVEGPIODETECTMODERSTREG1;			/* 0x28 : Alive GPIO Detect Mode Reset 1 */
	u32 ALIVEGPIODETECTMODESETREG1;			/* 0x2C : Alive GPIO Detect Mode Reset 1 */
	u32 ALIVEGPIORISEDETECTMODEREADREG;		/* 0x30 : Alive GPIO Rising Edge Detect Mode Read */

	u32 ALIVEGPIODETECTMODERSTREG2;			/* 0x34 : Alive GPIO Detect Mode Reset 2 */
	u32 ALIVEGPIODETECTMODESETREG2;			/* 0x38 : Alive GPIO Detect Mode Reset 2 */
	u32 ALIVEGPIOLOWDETECTMODEREADREG;		/* 0x3C : Alive GPIO Low Level Detect Mode Read */

	u32 ALIVEGPIODETECTMODERSTREG3;			/* 0x40 : Alive GPIO Detect Mode Reset 3 */
	u32 ALIVEGPIODETECTMODESETREG3;			/* 0x44 : Alive GPIO Detect Mode Reset 3 */
	u32 ALIVEGPIOHIGHDETECTMODEREADREG;		/* 0x48 : Alive GPIO High Level Detect Mode Read */

	u32 ALIVEGPIODETECTENBRSTREG;			/* 0x4C : Alive GPIO Detect Enable Reset */
	u32 ALIVEGPIODETECTENBSETREG;			/* 0x50 : Alive GPIO Detect Enable Set */
	u32 ALIVEGPIODETECTENBREADREG;			/* 0x54 : Alive GPIO Detect Enable Read */

	u32 ALIVEGPIOINTENBRSTREG;			/* 0x58 : Alive GPIO Interrupt Enable Reset */
	u32 ALIVEGPIOINTENBSETREG;			/* 0x5C : Alive GPIO Interrupt Enable Set */
	u32 ALIVEGPIOINTENBREADREG;			/* 0x60 : Alive GPIO Interrupt Enable Read */

	u32 ALIVEGPIODETECTPENDREG;			/* 0x64 : Alive GPIO Detect Pending */

	u32 ALIVESCRATCHRSTREG;				/* 0x68 : Alive Scratch Reset */
	u32 ALIVESCRATCHSETREG;				/* 0x6C : Alive Scratch Set */
	u32 ALIVESCRATCHREADREG;			/* 0x70 : Alive Scratch Read */

	u32 ALIVEGPIOPADOUTENBRSTREG;			/* 0x74 : Alive GPIO PAD Out Enable Reset */
	u32 ALIVEGPIOPADOUTENBSETREG;			/* 0x78 : Alive GPIO PAD Out Enable Set */
	u32 ALIVEGPIOPADOUTENBREADREG;			/* 0x7C : Alive GPIO PAD Out Enable Read */

	u32 ALIVEGPIOPADPULLUPRSTREG;			/* 0x80 : Alive GPIO PAD Pullup Reset */
	u32 ALIVEGPIOPADPULLUPSETREG;			/* 0x84 : Alive GPIO PAD Pullup Set */
	u32 ALIVEGPIOPADPULLUPREADREG;			/* 0x88 : Alive GPIO PAD Pullup Read */

	u32 ALIVEGPIOPADOUTRSTREG;			/* 0x8C : Alive GPIO PAD Out Reset */
	u32 ALIVEGPIOPADOUTSETREG;			/* 0x90 : Alive GPIO PAD Out Set */
	u32 ALIVEGPIOPADOUTREADREG;			/* 0x94 : Alive GPIO PAD Out Read */

	u32 VDDCTRLRSTREG;				/* 0x98 : VDD Control Reset */
	u32 VDDCTRLSETREG;				/* 0x9C : VDD Control Set */
	u32 VDDCTRLREADREG;				/* 0xA0 : VDD Control Read */

	u32 CLEARWAKEUPSTATUS;
	u32 WAKEUPSTATUS;

	u32 ALIVESCRATCHRST1;
	u32 ALIVESCRATCHSET1;
	u32 ALIVESCRATCHVALUE1;

	u32 ALIVESCRATCHRST2;
	u32 ALIVESCRATCHSET2;
	u32 ALIVESCRATCHVALUE2;

	u32 ALIVESCRATCHRST3;
	u32 ALIVESCRATCHSET3;
	u32 ALIVESCRATCHVALUE3;

	u32 ALIVESCRATCHRST4;
	u32 ALIVESCRATCHSET4;
	u32 ALIVESCRATCHVALUE4;

	u32 ALIVESCRATCHRST5;
	u32 ALIVESCRATCHSET5;
	u32 ALIVESCRATCHVALUE5;

	u32 ALIVESCRATCHRST6;
	u32 ALIVESCRATCHSET6;
	u32 ALIVESCRATCHVALUE6;

	u32 ALIVESCRATCHRST7;
	u32 ALIVESCRATCHSET7;
	u32 ALIVESCRATCHVALUE7;

	u32 ALIVESCRATCHRST8;
	u32 ALIVESCRATCHSET8;
	u32 ALIVESCRATCHVALUE8;

	u32 VDDOFFCNTVALUERST;
	u32 VDDOFFCNTVALUESET;

	u32 VDDOFFCNTVALUE0;
	u32 VDDOFFCNTVALUE1;

	u32 ALIVEGPIOINPUTVALUE;
};

enum {
	NX_ALIVE_INT_ALIVEGPIO0	= 0,	/* ALIVE GPIO 0 Interrupt */
	NX_ALIVE_INT_ALIVEGPIO1	= 1,	/* ALIVE GPIO 1 Interrupt */
	NX_ALIVE_INT_ALIVEGPIO2	= 2,	/* ALIVE GPIO 2 Interrupt */
	NX_ALIVE_INT_ALIVEGPIO3	= 3,	/* ALIVE GPIO 3 Interrupt */
	NX_ALIVE_INT_ALIVEGPIO4	= 4,	/* ALIVE GPIO 4 Interrupt */
	NX_ALIVE_INT_ALIVEGPIO5	= 5,	/* ALIVE GPIO 5 Interrupt */
	NX_ALIVE_INT_ALIVEGPIO6	= 6,	/* ALIVE GPIO 6 Interrupt */
	NX_ALIVE_INT_ALIVEGPIO7	= 7	/* ALIVE GPIO 7 Interrupt */
};

typedef enum {
	NX_ALIVE_DETECTMODE_ASYNC_LOWLEVEL	= 0,	/* Async Low Level */
	NX_ALIVE_DETECTMODE_ASYNC_HIGHLEVEL	= 1,	/* Async High Level */
	NX_ALIVE_DETECTMODE_SYNC_FALLINGEDGE	= 2,	/* Sync Falling Edge */
	NX_ALIVE_DETECTMODE_SYNC_RISINGEDGE	= 3,	/* Sync Rising Edge */
	NX_ALIVE_DETECTMODE_SYNC_LOWLEVEL	= 4,	/* Sync Low Level */
	NX_ALIVE_DETECTMODE_SYNC_HIGHLEVEL	= 5	/* Sync High Level */

} NX_ALIVE_DETECTMODE;

typedef enum {
	NX_ALIVE_PADGROUP0 = 0,		/* IO Power Group 0 ( RX0 ~ RX4 ) */
	NX_ALIVE_PADGROUP1 = 1,		/* IO Power Group 1 ( USB VBUS ) */
	NX_ALIVE_PADGROUP2 = 2,		/* IO Power Group 2 ( GPIO ) */
	NX_ALIVE_PADGROUP3 = 3,		/* IO Power Group 2 ( GPIO ) */
	NX_ALIVE_PADGROUP_NUMBER = 4

} NX_ALIVE_PADGROUP;

#endif	/* __NX_ALIVE_H__ */
