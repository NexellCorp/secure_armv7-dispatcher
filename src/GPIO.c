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
 * File		: GPIO.c
 * Description	:
 * Author	: Hans
 * History	: 2017.02.28 new release
 */

#include <nx_pyrope.h>
#include <nx_type.h>

#include <nx_chip.h>
#include <nx_gpio.h>

#if !DIRECT_IO
void GPIOSetAltFunction(u32 AltFunc)
{
	struct NX_GPIO_RegisterSet (*const pReg_GPIO)[1] =
		(struct NX_GPIO_RegisterSet (*const)[])PHY_BASEADDR_GPIOA_MODULE;

	register struct NX_GPIO_RegisterSet *pGPIOReg =
		pReg_GPIO[(AltFunc >> 8) & 0x7];

	WIO32(&pGPIOReg->GPIOxALTFN[(AltFunc >> 7) & 0x1],
		(RIO32(&pGPIOReg->GPIOxALTFN[(AltFunc >> 7) & 0x1]) &
			 ~(0x3UL << (((AltFunc >> 3) & 0xF) * 2))) |
		((AltFunc & 0x3) << (((AltFunc >> 3) & 0xF) * 2)));
}
#endif
