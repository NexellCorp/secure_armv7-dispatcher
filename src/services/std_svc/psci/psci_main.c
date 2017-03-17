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
#include <nx_prototype.h>
#include <plat_pm.h>
#include <psci_main.h>
#include <psci.h>

/* External Function */
extern int  psci_validate_mpidr(u32 mpidr);
extern void psci_power_down_wfi(void);

extern int  psci_cpu_on_start(u32 target_cpu, u32 entrypoint);
extern int  psci_do_cpu_off(u32 cpu_id);
extern void psci_system_off(void);
extern void psci_system_reset(void);
extern int  psci_cpu_suspend_start(u32 entrypoint);

/* External Variable */
extern volatile int g_smc_id;
extern volatile int g_fiq_flag;
extern volatile int g_cpu_kill_num;
extern volatile union cpuonstatus ourcpus;


/* Macro to help build the psci capabilities bitfield */
#define define_psci_cap(x)		(1 << (x & 0x1f))

u32 psci_caps = define_psci_cap(PSCI_CPU_OFF) |
		define_psci_cap(PSCI_CPU_ON_AARCH32) |
		define_psci_cap(PSCI_SYSTEM_OFF) |
		define_psci_cap(PSCI_SYSTEM_SUSPEND_AARCH32);

/*******************************************************************************
 * PSCI frontend api for servicing SMCs. Described in the PSCI spec.
 ******************************************************************************/
void psci_set_sgi_caller(int cpu_id)
{
	u32 target_cpu = 0, int_num = 8;
	cpu_id = cpu_id;
	s5p4418_cpuidle((1 << target_cpu), int_num);
}

u32 psci_version(void)
{
	return PSCI_MAJOR_VER | PSCI_MINOR_VER;
}

int psci_cpu_on(u32 target_cpu,
		u32 entrypoint, u32 context_id)
{
	int rc;

	context_id = context_id;

	/* Determine if the cpu exists of not */
	rc = psci_validate_mpidr(target_cpu);
	if (rc != PSCI_E_SUCCESS)
		return PSCI_E_INVALID_PARAMS;

	/* s5p4418(cortex-a9) Actual secondary cpu on */
	rc = psci_cpu_on_start(target_cpu, entrypoint);

	return rc;
}

int psci_cpu_off(void)
{
	int cpu_id = armv7_get_cpuid();
	int rc = 0;

	/* Check the CPUx to Kill*/
	g_smc_id = PSCI_CPU_OFF;
	g_cpu_kill_num = cpu_id;
	ourcpus.cpu[cpu_id] = 0;

	/* Check the SGI Call CPUx */
	psci_set_sgi_caller(cpu_id);

	s5p4418_cpu_off_wfi_ready();
	psci_power_down_wfi();

	return rc;
}

int psci_system_suspend(u32 entrypoint, u32 context_id)
{
	int rc = 0;

	context_id = context_id;

	rc = psci_cpu_suspend_start(entrypoint);

	return rc;
}

int psci_affinity_info(u32 target_affinity, u32 lowest_affinity_level)
{
	u32 cpu_id = (target_affinity & 0xF);
	u32 status = 0;

	target_affinity = target_affinity;
	lowest_affinity_level = lowest_affinity_level;

	status = s5p4418_cpu_check(cpu_id);

	return status;
}

int psci_features(u32 psci_fid)
{
	u32 local_caps = psci_caps;

	/* TODO: sanity check of psci_fid */

	/* Check if the psci fid is supported or not */
	if (!(local_caps & define_psci_cap(psci_fid)))
		return PSCI_E_NOT_SUPPORTED;

	/* Return 0 for all other fid's */
	return PSCI_E_SUCCESS;
}

int psci_smc_handler(u32 smc_fid, u32 x1, u32 x2, u32 x3)
{

	DBGOUT(" R0: %X, R1: %X, R2: %X, R3: %X \r\n",
		smc_fid, x1, x2, x3);

	/* Check the psci smc_fid  */
	if (((smc_fid >> FUNCID_CC_SHIFT) & FUNCID_CC_MASK) == SMC_32) {
		/* 32-bit PSCI function, clear top parameter bits */

		switch (smc_fid) {
		case PSCI_VERSION:
			return psci_version();

		case PSCI_CPU_OFF:
			return psci_cpu_off();

		case PSCI_CPU_ON_AARCH32:
			return psci_cpu_on(x1, x2, x3);

		case PSCI_AFFINITY_INFO_AARCH32:
			return psci_affinity_info(x1, x2);

		case PSCI_MIG_INFO_TYPE:
			return PSCI_E_NOT_SUPPORTED;

		case PSCI_SYSTEM_SUSPEND_AARCH32:
			return psci_system_suspend(x1, x2);

		case PSCI_SYSTEM_OFF:
			psci_system_off();
			WARN("PSCI_SYSTEM_OFF\r\n");
			/* We should never return from psci_system_off() */
			break;

		case PSCI_SYSTEM_RESET:
			psci_system_reset();
			WARN("PSCI_SYSTEM_RESET\r\n");
			/* We should never return from psci_system_reset() */
			break;

		case PSCI_FEATURES:
			return psci_features(x1);

		default:
			break;
		}
	}

	WARN("Unimplemented PSCI Call: 0x%x \r\n", smc_fid);

	return -1;
}
