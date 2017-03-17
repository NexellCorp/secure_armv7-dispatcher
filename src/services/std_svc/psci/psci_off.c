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
#include <plat_pm.h>
#include <gic.h>
#include "psci.h"

/* External Variable */
extern volatile int g_fiq_flag;
extern volatile int g_cpu_kill_num;

void psci_cpu_off_handler(void)
{
	int cpu_id = armv7_get_cpuid();
	int ret;

	/* It is necessary in order to ensure sequential operation.*/
	if (cpu_id != 0) {
		g_fiq_flag |= (1 << cpu_id);
		do {
			cache_delay_ms(0xFFFFF);
		} while(g_fiq_flag & (1 << cpu_id));

		return;
	}

	/* cpu0 operated to the subcpu power off*/
	ret = s5p4418_cpu_off(g_cpu_kill_num);
	if (ret > 0)
		g_fiq_flag = 0;
}

/*************************************************************
 * Must be S5P4418
 * CPU Power Off sequence in S5P4418
 * Reference is made to function psci interface .
 *************************************************************/
int psci_do_cpu_off(u32 target_cpu)
{
	u32 cpu_id = ((target_cpu >> 0) & 0xFF);

	s5p4418_cpu_off(cpu_id);

	return s5p4418_cpu_check(cpu_id);	// 0: ON, 1:OFF, 2:PENDING
}
