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
 * Author	: DeokJin, Lee <truevirtue@nexell.co.kr>
 * History	: 2017.03.14 new release
 */
#include <sysheader.h>

void tieoff_set_secure(void)
{
	u32 value;

	/* set dram to support secure address */
	SIO32(&pReg_Tieoff->TIEOFFREG[24],
			0xF << 28 | 0xF << 24 | 1 << 17 | 1 << 16);

	value = 0xFFFFFFFF;
	value &= ~(1 << 29 | 1 << 25);	/* tieoff, crypto */
	WIO32(&pReg_Tieoff->TIEOFFREG[26], value);

	value = 0xFFFFFFFF;
	value &= ~(1 << 3);	/* sram */
	WIO32(&pReg_Tieoff->TIEOFFREG[30], value);
}

