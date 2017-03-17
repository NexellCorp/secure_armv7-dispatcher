/*
 * Copyright (C) 2012 Nexell Co., All Rights Reserved
 * Nexell Co. Proprietary & Confidential
 *
 * NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
 * AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Module          : Interrupt Controller
 * File            : nx_intc.h
 * Description     :
 * Author          : Firware Team
 * History         :
 */
#ifndef __NX_INTC_H__
#define __NX_INTC_H__

struct NX_INTC_RegisterSet {
	u32 IRQSTATUS;
	u32 FIQSTATUS;
	u32 RAWINTR;
	u32 INTMODE;
	u32 INTENABLE;
	u32 INTDISABLE;
	u32 SWINT;
	u32 SWINTCLEAR;
	u32 PROENBALE;
	u32 SWPRIMASK;
	u32 PRIORITY;
	u8  _Reserved0[0x100 - 0x2C];
	u32 VECTORADDR[32];
	u8  _Reserved1[0x200 - 0x180];
	u32 VECTORPRI[32];
	u8  _Reserved2[0xF00 - 0x280];
	u32 CURRENTVECTOR;
	u8  _Reserved3[0x1000 - 0xF04];
};

typedef enum {
	NX_INTC_INTMODE_IRQ = 0UL,	/* IRQ mode */
	NX_INTC_INTMODE_FIQ = 1UL	/* FIQ mode */
}	NX_INTC_INTMODE ;

#endif /* __NX_INTC_H__ */
