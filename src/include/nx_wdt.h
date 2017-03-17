/*
 * Copyright (C) 2012 Nexell Co., All Rights Reserved
 * Nexell Co. Proprietary & Confidential
 *
 * NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
 * AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Module          : Watch Dog
 * File            : nx_wdt.h
 * Description     :
 * Author          : Jonghyuk Park(charles@nexell.co.kr)
 * History         : 2012/09/11 first implementation
 */
#ifndef _NX_WDT_H
#define _NX_WDT_H

struct NX_WDT_RegisterSet {
	u32 WTCON;	/* 0x0000 // R/W	// control */
	u32 WTDAT;	/* 0x0004 // R/W	// data */
	u32 WTCNT;	/* 0x0008 // R/W	// count */
	u32 WTCLRINT;	/* 0x000C // W		// interrupt clear */
};

typedef enum {
	WDT_CLOCK_DIV16	 = 0,
	WDT_CLOCK_DIV32  = 1,
	WDT_CLOCK_DIV64  = 2,
	WDT_CLOCK_DIV128 = 3
}	NX_WDT_CLOCK_DIV;

#endif /* _NX_WDT_H */
