/*
 * Copyright (c) 2013-2015, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <sysheader.h>
#include <gic.h>

/* External function */
extern void s5p4418_bclk_dfs(u32 pll_data);
#define PHY_BASEADDR_PL310_MODULE	0xCF000000

#define ADDRMASK_4K			0xFFFFF000
#define ADDRMASK_64K			0xFFFF0000

/* Macro for Secure Write/Read  */
#define S5PXX18_REG_WRITE		0x82000000
#define S5PXX18_REG_READ		0x82000001
#define S5PXX18_L2C_REG_WRITE		0x82000100
#define S5PXX18_L2C_REG_READ		0x82000101
#define S5PXX18_MALI_REG_WRITE		0x82000102
#define S5PXX18_MALI_REG_READ		0x82000103
#define S5PXX18_MIPI_REG_WRITE		0x82000104
#define S5PXX18_MIPI_REG_READ		0x82000105
#define S5PXX18_TIEOFF_REG_WRITE	0x82000106
#define S5PXX18_TIEOFF_REG_READ		0x82000107

#define S5PXX18_REG_BCLK		0x82000009

/*******************************************************************************
 * Registers to access in a secure mode write function.
 ******************************************************************************/
static inline int secure_write(void* hwreg, int value)
{
	WIO32(hwreg, value);
	return 0;
}

/*******************************************************************************
 * Registers to access in a secure mode read function.
 ******************************************************************************/
static inline int secure_read(void* hwreg)
{
	return RIO32(hwreg);
}

/*******************************************************************************
 * For implementing the functions defiend by the user, the SIP interface the
 * main function
 ******************************************************************************/
int sip_smc_handler(u32 smc_fid,
	u32 r1, u32 r2, u32 r3)
{
	r3 = r3;

	switch (smc_fid) {
	case S5PXX18_REG_WRITE:
		return secure_write((void*)r1, (int)r2);

	case S5PXX18_REG_READ:
		return secure_read((void*)r1);

	case S5PXX18_L2C_REG_WRITE:
		if (r1 & ADDRMASK_4K)
			break;
		r1 += PHY_BASEADDR_PL310_MODULE;
		return secure_write((void*)r1, (int)r2);

	case S5PXX18_L2C_REG_READ:
		if (r1 & ADDRMASK_4K)
			break;
		r1 += PHY_BASEADDR_PL310_MODULE;
		return secure_read((void*)r1);

	case S5PXX18_MALI_REG_WRITE:
		if (r1 & ADDRMASK_64K)
			break;
		r1 += PHY_BASEADDR_MALI400_MODULE;
		return secure_write((void*)r1, (int)r2);

	case S5PXX18_MALI_REG_READ:
		if (r1 & ADDRMASK_64K)
			break;
		r1 += PHY_BASEADDR_MALI400_MODULE;
		return secure_read((void*)r1);

	case S5PXX18_MIPI_REG_WRITE:
		if (r1 & ADDRMASK_4K)
			break;
		r1 += PHY_BASEADDR_MIPI_MODULE;
		return secure_write((void*)r1, (int)r2);

	case S5PXX18_MIPI_REG_READ:
		if (r1 & ADDRMASK_4K)
			break;
		r1 += PHY_BASEADDR_MIPI_MODULE;
		return secure_read((void*)r1);

	case S5PXX18_TIEOFF_REG_WRITE:
		if (r1 & ADDRMASK_4K)
			break;
		if ((r1 == 0) || (r1 >= 0x68))
			break;
		r1 += PHY_BASEADDR_TIEOFF_MODULE;
		return secure_write((void*)r1, (int)r2);

	case S5PXX18_TIEOFF_REG_READ:
		if (r1 & ADDRMASK_4K)
			break;
		if ((r1 == 0) || (r1 >= 0x68))
			break;
		r1 += PHY_BASEADDR_TIEOFF_MODULE;
		return secure_read((void*)r1);

	case S5PXX18_REG_BCLK:
		s5p4418_bclk_dfs(r1);
		break;

	default:
		WARN("Unimplemented SIP Service Call: 0x%x\r\n", smc_fid);
		break;
	}

	return 0;
}
