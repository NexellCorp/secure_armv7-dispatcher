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
#include <sysheader.h>
#include <plat_pm.h>
#include <psci.h>

/* External function */
extern unsigned int __calc_crc(void *addr, int len);

/* Macro for Suspend/Resume */
#define PSCI_SUSPEND		0
#define PSCI_RESUME		1

/*************************************************************
 * Before entering suspend and Mark the location and promise Kernel.
 * Reference CRC, Jump Address, Memory Address(CRC), Size(CRC)
 *************************************************************/
static void suspend_mark(unsigned int state, unsigned int entrypoint,
		unsigned int crc, unsigned int mem, unsigned int size)
{
#if 0	// Not determined mark parmeter
	unsigned int crc;
	unsigned int mem = 0x40000000, size = (512*1024*1024);
#endif
	mmio_write_32(&pReg_Alive->ALIVESCRATCHRSTREG, 0xFFFFFFFF);
	mmio_write_32(&pReg_Alive->ALIVESCRATCHRST1, 0xFFFFFFFF);
	mmio_write_32(&pReg_Alive->ALIVESCRATCHRST2, 0xFFFFFFFF);
	mmio_write_32(&pReg_Alive->ALIVESCRATCHRST3, 0xFFFFFFFF);
	mmio_write_32(&pReg_Alive->ALIVESCRATCHRST4, 0xFFFFFFFF);

	if (state == PSCI_SUSPEND) {
		crc = __calc_crc((void *)mem, size);
		mmio_write_32(&pReg_Alive->ALIVESCRATCHSETREG, SUSPEND_SIGNATURE);
		mmio_write_32(&pReg_Alive->ALIVESCRATCHSET1, entrypoint);
		mmio_write_32(&pReg_Alive->ALIVESCRATCHSET2, crc);
		mmio_write_32(&pReg_Alive->ALIVESCRATCHSET3, mem);
		mmio_write_32(&pReg_Alive->ALIVESCRATCHSET4, size);
	}
}

#define L2C_BASE	0xCF000000
void s5p4418_l2c_disable(void)
{
	unsigned int cache_way = (1 << 16) - 1;		// there are 16 cache ways

	if ((ReadIO32(L2C_BASE + 0x100) & 1) == 0)	// check ctrl enabled
		return;

	WriteIO32(L2C_BASE + 0x7fc, cache_way);		// 16 way, clean inv way
	while (ReadIO32(L2C_BASE + 0x7fc) & cache_way)
		;
	WriteIO32(L2C_BASE + 0x730, 0);			// cache sync;

	while (ReadIO32(L2C_BASE + 0x77c) & cache_way)	// check inv way
		;
	while (ReadIO32(L2C_BASE + 0x7bc) & cache_way)	// check clean way
		;
	while (ReadIO32(L2C_BASE + 0x7fc) & cache_way)	// check clean inv way
		;
	WriteIO32(L2C_BASE + 0x100, 0);			// ctrl disable
}

/*************************************************************
 * Designed to meet the the PSCI Common Interface.
 *************************************************************/
void flushL1cache(void);
void plat_suspend(unsigned int entrypoint)
{
	/* s5pxx18 suspend mark */
	suspend_mark(PSCI_SUSPEND, entrypoint, 0, 0x91080000, (128 * 1024));

	/* the function for operation to go sleep */
	system_sleep();
	__asm__ __volatile__ ("wfi");
}

int psci_cpu_suspend_start(unsigned int entrypoint)
{
	flushL1cache();
	s5p4418_l2c_disable();

	plat_suspend(entrypoint);

	return PSCI_E_SUCCESS;
}

void s5p4418_tee_suspend(unsigned int sec_entrypoint)
{
	plat_suspend(sec_entrypoint);
}

/*************************************************************
 * Designed to meet the the PSCI Common Interface.
 *************************************************************/
void psci_cpu_suspend_finish(unsigned int cpu_idx,
			     psci_power_state_t *state_info)
{
	/* Remove warning for futrue externsibility */
	cpu_idx = cpu_idx;
	state_info = state_info;

	/* the function for system wakeup */
	s5p4418_resume();
}
