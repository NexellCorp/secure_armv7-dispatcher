/*
 * Copyright (C) 2016  Nexell Co., Ltd. All Rights Reserved.
 * Nexell Co. Proprietary & Confidential
 *
 * Nexell informs that this code and information is provided "as is" base
 * and without warranty of any kind, either expressed or implied, including
 * but not limited to the implied warranties of merchantability and/or
 * fitness for a particular puporse.
 * This code is not allowed to redistribute without permission or
 * open to the public.
 *
 * Module	:
 * File		:
 * Description	:
 * Author	: Hans
 * History	: 2017.03.14 new release
 */
#ifndef __SYS_HEADER_H__
#define __SYS_HEADER_H__

#include <nx_pyrope.h>
#include <nx_type.h>
#include <nx_chip.h>
#include <nx_rstcon.h>
#include <nx_clkpwr.h>
#include <nx_gpio.h>
#include <nx_alive.h>
#include <nx_tieoff.h>
#include <nx_clkgen.h>

#include "secondboot.h"
#include <nx_printf.h>
#include <plat_debug.h>
#include <common.h>

/*------------------------------------------------------------------------------
//  Set DEBUG Macro
//----------------------------------------------------------------------------*/

/* UserDebug Message */
#if 0
#define DBGOUT  printf
#else
#define DBGOUT(x, ...)
#endif

/*------------------------------------------------------------------------------
//  Set global variables
//----------------------------------------------------------------------------*/

#if defined(__SET_GLOBAL_VARIABLES)

struct NX_SecondBootInfo *const pSBI =
	(struct NX_SecondBootInfo * const)BASEADDR_SRAM;
struct NX_ALIVE_RegisterSet *const pReg_Alive =
	(struct NX_ALIVE_RegisterSet * const)PHY_BASEADDR_ALIVE_MODULE;
struct NX_TIEOFF_RegisterSet *const pReg_Tieoff =
	(struct NX_TIEOFF_RegisterSet * const)PHY_BASEADDR_TIEOFF_MODULE;
struct NX_CLKPWR_RegisterSet *const pReg_ClkPwr =
	(struct NX_CLKPWR_RegisterSet * const)PHY_BASEADDR_CLKPWR_MODULE;

#else

extern struct NX_SecondBootInfo *const pSBI; /* second boot info */
extern struct NX_ALIVE_RegisterSet *const pReg_Alive;
extern struct NX_TIEOFF_RegisterSet *const pReg_Tieoff;
extern struct NX_CLKPWR_RegisterSet *const pReg_ClkPwr;
#endif

#endif /* __SYS_HEADER_H__ */
