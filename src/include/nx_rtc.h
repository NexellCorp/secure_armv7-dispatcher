/*
 * Copyright (C) 2012 Nexell Co., All Rights Reserved
 * Nexell Co. Proprietary & Confidential
 *
 * NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
 * AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Module          : Real Time Clock
 * File            : nx_rtc.h
 * Description     :
 * Author          : Goofy
 * History         :
 */
#ifndef __NX_RTC_H__
#define __NX_RTC_H__

struct	NX_RTC_RegisterSet {
	u32 RTCCNTWRITE;	/* 0x00 : RTC counter (Write only) */
	u32 RTCCNTREAD;		/* 0x04 : RTC counter (Read only) */
	u32 RTCALARM;		/* 0x08 : RTC alarm */
	u32 RTCCTRL;		/* 0x0C : RTC control */
	u32 RTCINTENB;		/* 0x10 : RTC interrupt enable */
	u32 RTCINTPND;		/* 0x14 : RTC interrupt pending (Read only) */
	u32 RTCCORERSTIMESEL;	/* 0x18 : RTC Core POR time select */
	u32 RTCSCRATCH;
	u32 RTCSCRATCHSTATUS;
};

enum {
	NX_RTC_INT_COUNTER	= 0,	/* RTC Counter interrupt. */
	NX_RTC_INT_ALARM	= 1	/* RTC Alarm interrupt. */
};

typedef enum {
	NX_RTC_RESETDELAY_BYPASS	= 1UL << 0,	/* No Delay(Bypass). Without delay, reset signal is sent to power manager. */
	NX_RTC_RESETDELAY_31MS		= 1UL << 1,	/* After 31ms , reset signal is sent to power manager. */
	NX_RTC_RESETDELAY_62MS		= 1UL << 2,	/* After 62ms	, reset signal is sent to power manager. */
	NX_RTC_RESETDELAY_93MS		= 1UL << 3,	/* After 93ms	, reset signal is sent to power manager. */
	NX_RTC_RESETDELAY_124MS		= 1UL << 4,	/* After 124ms, reset signal is sent to power manager. */
	NX_RTC_RESETDELAY_155MS		= 1UL << 5,	/* After 155ms, reset signal is sent to power manager. */
	NX_RTC_RESETDELAY_186MS		= 1UL << 6,	/* After 186ms, reset signal is sent to power manager. */
	NX_RTC_RESETDELAY_210MS		= 0		/* After 210ms, reset signal is sent to power manager. */

}	NX_RTC_RESETDELAY;

typedef enum {
	NX_RTC_OSCSEL_1HZ	= 0,	/* RTC oscillation selection to 1Hz */
	NX_RTC_OSCSEL_32HZ	= 1,	/* RTC oscillation selection to 32Hz */
}	NX_RTC_OSCSEL;


#endif /* __NX_RTC_H__ */
