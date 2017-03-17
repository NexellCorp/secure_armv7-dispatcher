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
#ifndef __PLAT_PM_H__
#define __PLAT_PM_H__

/* Macro for Suspend/Resume */
#define PSCI_SUSPEND		0
#define PSCI_RESUME		1

u32 __calc_crc(void *addr, int len);
void flushL1cache(void);
void suspend_mark(u32 state, u32 entrypoint);

void s5p4418_l2c_disable(void);
void s5p4418_cpuidle(int cpu_id, int int_id);
void s5p4418_cpu_off_wfi_ready(void);

 int s5p4418_cpu_check(u32 cpu_id);

 int s5p4418_cpu_on(u32 cpu_id);
 int s5p4418_cpu_off(u32 cpu_id);

void s5p4418_reset_cpu(void);

void s5p4418_suspend(void);
void s5p4418_resume(void);

#endif	/* __PLAT_PM_H__ */
