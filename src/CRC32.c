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
 * Author	: Hans
 * History	: 2017.03.14 new release
 */
#include <nx_pyrope.h>
#include <nx_type.h>

u32 iget_fcs(u32 fcs, u32 data)
{
	int i;

	fcs ^= data;
	for (i = 0; i < 32; i++) {
		if (fcs & 0x01)
			fcs = (fcs >> 1) ^ POLY;
		else
			fcs >>= 1;
	}

	return fcs;
}

#define CHKSTRIDE 1
u32 __calc_crc(void *addr, int len)
{
	u32 *c = (u32 *)addr;
	u32 crc = 0, chkcnt = ((len + 3) / 4);
	u32 i;

	for (i = 0; chkcnt > i; i += CHKSTRIDE, c += CHKSTRIDE)
		crc = iget_fcs(crc, *c);

	return crc;
}

