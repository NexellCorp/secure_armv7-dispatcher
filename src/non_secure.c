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
#include "sysheader.h"

void tieoff_set_secure(void)
{
	unsigned int value;

	value = 0xFFFFFFFF;
//	value &= ~(1 << 29 | 1 << 25);	// tieoff, crypto
	pReg_Tieoff->TIEOFFREG[26] = value;

//	value = 0x1FF;		//VIP, DISP, SCALER
//	pReg_Tieoff->TIEOFFREG[28] = value;

//	value = 0xFFFFFFFF;
	value &= ~(1 << 3);	// sram
	pReg_Tieoff->TIEOFFREG[30] = value;
}

