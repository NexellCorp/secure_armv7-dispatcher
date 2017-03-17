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
#include <psci.h>

/*************************************************************
 * Designed to meet the the PSCI Common Interface.
 *************************************************************/
int psci_cpu_suspend_start(u32 entrypoint)
{
	/* s5pxx18 suspend mark */
	suspend_mark(PSCI_SUSPEND, entrypoint);

	flushL1cache();
	s5p4418_l2c_disable();
	/* the function for operation to go sleep */
	system_sleep();
	__asm__ __volatile__ ("wfi");

	return PSCI_E_SUCCESS;
}

void s5p4418_tee_suspend(u32 sec_entrypoint)
{
	psci_cpu_suspend_start(sec_entrypoint);
}

/*************************************************************
 * Designed to meet the the PSCI Common Interface.
 *************************************************************/
void psci_cpu_suspend_finish(u32 cpu_idx,
			     psci_power_state_t *state_info)
{
	/* Remove warning for futrue externsibility */
	cpu_idx = cpu_idx;
	state_info = state_info;

	/* the function for system wakeup */
	s5p4418_resume();
}
