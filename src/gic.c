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
#include <gic.h>

/*******************************************************************************
 * cpu-interface s/w interface accessors for reading entire registers
 ******************************************************************************/

u8* gicc_get_baseaddr(void)
{
	return ((u8*)GICC_BASEADDR);
}

u32 gicc_get_iar(void* base)
{
	return (RIO32((base + GIC_CPUIF_IAR)) & 0xFFFF);
}

/*******************************************************************************
 * cpu-interface s/w interface accessors for writing entire registers
 ******************************************************************************/
void gicc_set_ctrl(void* base, int val)
{
	SIO32((base + GIC_CPUIF_CTRL), val);
}

void gicc_set_eoir(void* base, int val)
{
	int eoir = val & 0xFFFF;
	WIO32((base + GIC_CPUIF_EOIR), eoir);
}

/*******************************************************************************
 * Distributor interface accessors for reading entire registers
 ******************************************************************************/
u8* gicd_get_baseaddr(void)
{
	return ((u8*)GICD_BASEADDR);
}

/*******************************************************************************
 * Distributor interface accessors for writing entire registers
 ******************************************************************************/
void gicd_set_enable(void* base, int val)
{
	SIO32((base + GIC_DIST_SENABLE), val);
}

void gicd_set_group(void* base, int val)
{
	WIO32((base + GIC_DIST_GROUP), val);
}

void gicd_set_sgir(void* base, int val)
{
	WIO32((base + GIC_DIST_SGIR), val);
}

/*******************************************************************************
 * Setup the ARM GIC CPU and Distributor interfaces.
******************************************************************************/
#if 0
void gic_disp_init(void)
{
	u32 dist_base = gic_disp_get_baseaddr();
	u32 cpu_base = gic_cpuif_get_baseaddr();

	int nonsecure = 1, secure = 1;
	int i;

	/* CPU Interface Enable */
	WIO32((cpu_base + GIC_CPUIF_CTRL), (3 << 0));

	/*
	 * Whether or not to activate the interrupt
	 * occurs GROUP0/GROUP1 on the GIC.
	 */
	WIO32((dist_base + GIC_DISP_CTRL), (nonsecure << 1) | (secure << 0));

	/*
	 * secure/non-secure used to determine
	 * the priority of the interrupt.
	 */
	WIO32((cpu_base + GIC_CPUIF_PRIORTY), 0xFFFFFFFF);

	/*
	 * GIC set the path that is connected to the
	 * internal IP interrupts, generated. (0: Secure, 1: Non-Secure)
	 */
	for (i = 0; i <= 0xC; i += 4)
		WIO32((dist_base + GIC_DISP_GROUP + i), 0xFFFFFFFF);
}
#endif
