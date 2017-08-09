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

#define DPC_BASEADDR		0xC0102800
#define DPCCTRL0		0x118

#define RSTCON_BASEADDR		0xC0010000

static inline void* dpc_get_baseaddr(u32 module)
{
	return (void*)(DPC_BASEADDR + (module * 0x4000));
}


void dpc_set_enable_all(u32 module, u32 enb)
{
	void *base = dpc_get_baseaddr(module);
	u32 value;

	value = RIO32(base + DPCCTRL0);

	value &= ~(1 << 15);
	value |= (u32)enb << 15;

	WIO32((base + DPCCTRL0), value);
}

int dpc_get_pending_all(u32 module)
{
	void *base = dpc_get_baseaddr(module);
	return (RIO32(base + DPCCTRL0) >> 10);
}

void dpc_clear_pending_all(u32 module)
{
	void* base = (void*)dpc_get_baseaddr(module);
	u32 value;

	value = RIO32(base + DPCCTRL0);
	value |= (1 << 10);

	WIO32((base + DPCCTRL0), value);
}

int  dpc_enabled(u32 module)
{
	void* base = (void*)dpc_get_baseaddr(module);
	u32 value;

	value = RIO32(base + DPCCTRL0);

	return value & (1 << 15);
}

int dpc_reset_check(void)
{
	void* base = (void*)RSTCON_BASEADDR;
	int value;

	value = ((RIO32(base + 0x2000) >> 9) & 0x3);
	if (!value)
		return 1;

	return 0;
}
