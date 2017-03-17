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

extern void ResetCon(u32 devicenum, int en);
extern void secure_launch(cbool isresume, u32 secureos_startaddr, u32 non_secure_bl);
extern void non_secure_launch(cbool isresume, u32 non_secure_bl);

extern volatile union cpuonstatus ourcpus;
u32 g_subcpu_ep;

#define CPU_ALIVE_FLAG_ADDR	0xC0010238

cbool subcpu_on_start(u32 cpuid)
{
	if ((cpuid > 3) || (cpuid == 0))
		return CFALSE;

	// High Vector;
	SIO32(&pReg_Tieoff->TIEOFFREG[0], ((1 << cpuid) << 18));
	/* Reset Assert */
	ResetCon(cpuid, CTRUE);

	/* CPUCLKOFF Set to 1 except CPU0 */
	SIO32(&pReg_Tieoff->TIEOFFREG[1], ((1 << cpuid) << (37 - 32)));

	// Reset Negate
	ResetCon(cpuid, CFALSE);

	/*
	 * CPUCLKOFF Set to 0 except CPU0
	 * supply clock to CPUCLK real startup cpu
	 */
	CIO32(&pReg_Tieoff->TIEOFFREG[1], ((1 << cpuid) << (37 - 32)));

	return CTRUE;
}

void SubCPUBoot(u32 cpuid)
{
	ourcpus.cpu[cpuid] = 1;

	if (g_subcpu_ep) {
		if (g_subcpu_ep & 0x1) {	// check from non-secure call mark
			g_subcpu_ep &= ~1;
			non_secure_launch(0, g_subcpu_ep);
		} else {
			secure_launch(0, g_subcpu_ep, 0);
		}
	} else
		__asm__ __volatile__ ("wfi");
	while (1);
}

