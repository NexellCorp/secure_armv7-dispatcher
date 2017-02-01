/*
 * Copyright (C) 2016  Nexell Co., Ltd.
 * Author: Sangjong, Han <hans@nexell.co.kr>
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
#define __SET_GLOBAL_VARIABLES

#include "sysheader.h"

extern void buildinfo(void);
extern void secure_launch(CBOOL isresume, U32 secureos_startaddr, U32 non_secure_bl);
extern void non_secure_launch(CBOOL isresume, U32 non_secure_bl);
void tieoff_set_secure(void);
void device_reset(void);
extern U32 g_subcpu_ep;

void BootMain(CBOOL isresume, U32 non_secure_bl, U32 secureos_startaddr)
{
	U32 debug_ch = 0;
	g_subcpu_ep = secureos_startaddr;

//#if defined(AVN) || defined(NAVI) || defined(RAPTOR)
	debug_ch = 3;
//#endif

#if 1 // Low Level Debug Message
	/*  Low Level Debug Message */
	DebugInit(debug_ch);
#endif

	/* Build information */
	buildinfo();

	// set dram to support secure address
	pReg_Tieoff->TIEOFFREG[24] |= 0xF << 28 | 0xF << 24 | 1 << 17 | 1 << 16;

	tieoff_set_secure();	// crypto, tieoff, sram ...

	device_reset();		// timer reset

	pReg_INTC[0]->INTMODE = 0;		// all irq mode
	pReg_INTC[0]->INTENABLE = 0;		// all irq disable
	pReg_INTC[0]->INTDISABLE = 0xFFFFFFFF;	// all irq disable
	*(U32*)(PHY_BASEADDR_INTC_MODULE + 0x1000 * 0 + 0x300) = 0;	// vic test control reg?
	pReg_INTC[0]->SWINTCLEAR = 0xFFFFFFFF;	// all sw irq clear

	pReg_INTC[1]->INTMODE = 0;		// all irq mode
	pReg_INTC[1]->INTENABLE = 0;		// all irq disable
	pReg_INTC[1]->INTDISABLE = 0xFFFFFFFF;	// all irq disable
	*(U32*)(PHY_BASEADDR_INTC_MODULE + 0x1000 * 1 + 0x300) = 0;	// vic test control reg?
	pReg_INTC[1]->SWINTCLEAR = 0xFFFFFFFF;	// all sw irq clear

	if (secureos_startaddr) {
		SYSMSG("Launch to secure 0x%08X\r\n", (U32)secureos_startaddr);
		while (!DebugIsUartTxDone())
			;
		secure_launch(isresume, secureos_startaddr, non_secure_bl);
	} else {
		if (isresume)
			non_secure_bl = pSBI->EntryPoint;
		SYSMSG("Launch to non-secure 0x%08X\r\n", (U32)non_secure_bl);
		while (!DebugIsUartTxDone())
			;
		non_secure_launch(isresume, non_secure_bl);
	}
}
