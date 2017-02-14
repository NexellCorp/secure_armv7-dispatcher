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
#include "nx_pyrope.h"
 
#define SRAM_BASEADDR	0xFFFF0000
#define SRAM_SIZE	(32 * 1024)

.global iram_dev_freq

/*
 * Monitor Mode - Exception Vetor table
 * 
 */
.align 6
.global monitor_vectors
monitor_vectors:
        b       iram_suspend			// 00 - reset will never get here
        b       .    				// 04 -
        b       smc_asm_handler			// 08 - SMC Interrupt
        b       .          			// 0C -
        b       .          			// 10 -
        b       iram_dev_freq			// 14 -	No Exception get here
        b       .                   		// 18 -
//	b       smc_asm_monitor_fiq_handler	// 1C - FIQ Interrupt

/* monitor mode - fiq handler */
.global smc_monitor_fiq_handler
smc_asm_monitor_fiq_handler:
	stmfd	sp!, {r0-r12, lr}
	bl	smc_monitor_fiq_handler
	ldmfd	sp!, {r0-r12, lr}
	subs 	pc, r14, #4

/*
 * SMC Caller Handler
 */
.global bl1_smc_handler
smc_asm_handler:
	stmfd	sp!, {r4-r12, lr}
	bl	bl1_smc_handler
	ldmfd	sp!, {r4-r12, lr}
	movs 	pc, r14


iram_dev_freq:
	b	.
iram_suspend:
	b	psci_cpu_suspend_start
	b	.
/*
 *  Only Monitor Mode - Deoks
 */
.global monitor_mode_init
monitor_mode_init:
	/* -----------------------------------------
	 * Secure - SVC Mode -> Monitor Mode
	 * Changes in the SVC Mode to MON Mode,
	 * and store the link register values.
	 * -----------------------------------------
	 */
	mov	r8, lr
	mov	lr, #0		// svc lr
	mov	sp, #0		// svc sp
	/* Switch to monitor mode  */
	msr	CPSR_c,  #(MODE_MON | I_Bit | F_Bit)

        mrc     p15, 0, r0, c1, c0, 0               	// Read CP15 System Control register
        bic     r0, r0, #(BIT1_V|BIT1_I|BIT1_Z)     	// Clear V:13, I:12, Z:11 bit to disable High Vector, I-Cache, Branch Prediction
        bic     r0, r0, #(BIT1_C|BIT1_M)            	// Clear C:2, M:0 bit to disable D Cache, MMU
        mcr     p15, 0, r0, c1, c0, 0               	// Write value back to CP15 System Control register

	/* Stack size set to monitor mode  */
	ldr     sp, =BASEADDR_SRAM
	add     sp, sp, #INTERNAL_SRAM_SIZE

	mrc     p15, 0, r12, c0, c0, 5		// Get our cpu id
	ands    r12, r12, #0xF			// Save CPU id

	mov     r1, #0x100
	sub     r2, r12, #1
	and     r2, r2, #3
	mul     r1, r1, r2
	sub     sp, sp, r1

set_montior_vector:
	/* (MVBAR) set to monitor vector base address */
	ldr	r0, =monitor_vectors
	mcr	p15, 0, r0, c12, c0, 1

	/* return */
	bx	r8

.global wfi_spill
.global psci_power_down_wfi
psci_power_down_wfi:
	dsb	sy					// ensure write buffer empty
wfi_spill:
	wfi
	b	wfi_spill

