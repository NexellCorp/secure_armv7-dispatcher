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
#ifndef __PSCI_MAIN_H__
#define __PSCI_MAIN_H__

/*******************************************************************************
 * Defines for runtime services function ids
 ******************************************************************************/
#define PSCI_VERSION			0x84000000
#define PSCI_CPU_SUSPEND_AARCH32	0x84000001
#define PSCI_CPU_OFF			0x84000002
#define PSCI_CPU_ON_AARCH32		0x84000003
#define PSCI_AFFINITY_INFO_AARCH32	0x84000004
#define PSCI_AFFINITY_INFO_AARCH64	0xc4000004
#define PSCI_MIG_AARCH32		0x84000005
#define PSCI_MIG_INFO_TYPE		0x84000006
#define PSCI_MIG_INFO_UP_CPU_AARCH32	0x84000007
#define PSCI_SYSTEM_OFF			0x84000008
#define PSCI_SYSTEM_RESET		0x84000009
#define PSCI_FEATURES			0x8400000A
#define PSCI_SYSTEM_SUSPEND_AARCH32	0x8400000E

/*******************************************************************************
 * Bit definitions inside the function id as per the SMC calling convention
 ******************************************************************************/
#define FUNCID_TYPE_SHIFT		31
#define FUNCID_CC_SHIFT			30
#define FUNCID_OEN_SHIFT		24
#define FUNCID_NUM_SHIFT		0

#define FUNCID_TYPE_MASK		0x1
#define FUNCID_CC_MASK			0x1
#define FUNCID_OEN_MASK			0x3f
#define FUNCID_NUM_MASK			0xffff

#define FUNCID_TYPE_WIDTH		1
#define FUNCID_CC_WIDTH			1
#define FUNCID_OEN_WIDTH		6
#define FUNCID_NUM_WIDTH		16

#define SMC_32				0

#endif
