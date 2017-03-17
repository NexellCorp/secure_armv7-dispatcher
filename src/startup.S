/*
 * Copyright (C) 2016  Nexell Co., Ltd. All Rights Reserved.
 * Nexell Co. Proprietary & Confidential
 *
 * Nexell informs that this code and information is provided "as is" base
 * and without warranty of any kind, either expressed or implied, including
 * but not limited to the implied warranties of merchantability and/or
 * fitness for a particular puporse.
 * This code is not allowed to redistribute without permission or
 * open to the public.
 *
 * Module	:
 * File		:
 * Description	:
 * Author	: Hans
 * History	: 2017.03.14 new release
 */
#include <nx_pyrope.h>

        .align
/* Start and e.nd of BSS */

.globl __bss_start__
.globl __bss_end__

/*
 * entry point of main function
 */
.global BootMain
.global SubCPUBoot
.global monitor_mode_init
.text
//==================================================================
// vectors
//==================================================================
.align 5
.global vectors
vectors:
	b	reset_handler	// 00 - Reset
	b	.		// 04 - Undefined instructions
	b	.		// 08 - SWI instructions
	b	.		// 0C - Instruction fetch aborts
	b	.		// 10 - Data access aborts
	b	.		// 14 - Reserved (was address exception)
	b	.		// 18 - IRQ interrupts
	b	.		// 1C - FIQ interrupts

// r0: warm or cold boot
// r1: secure os start address
// lr: non-secure start address
reset_handler:
	mcr	p15, 0, r0, c13, c0, 2	// evacuate boot status to tpidr
	mcr	p15, 0, lr, c13, c0, 3	// evacuate non-secure ptr to tpidr
	mcr	p15, 0, r1, c13, c0, 4	// evacuate secure os ptr to tpidr

//;	/* Set the SVC Vector(VBAR) */
	ldr	r0, =vectors
	mcr	p15, 0, r0, c12, c0, 0	// set svc vector(VBAR)


	bl	Invalidate_entire_branch_predictor
	bl	InvalidateDataCache

	/* Set the initialization for PSCI (Monitor Mode) */
	bl	monitor_mode_init

	//;  Configure CPU
	mrc	p15, 0, r0, c1, c0, 1
	//;  Only try to set SMP mode if not yet set - cannot write AuxCtrl if in normal world
	tst	r0, #(1 << 6)			// SMP bit
	orreq	r0, r0, #(1 << 6)		// Set SMP bit in aux control register
	orreq	r0, r0, #0x7			// L2C control
//	orreq	r0, r0, #(1 << 3)		// Zero
	mcreq	p15, 0, r0, c1, c0, 1		// Write Aux Control Register

	mrc	p15, 0, r0, c1, c0, 0		// Read CP15 System Control register
	orr	r0, r0, #(BIT1_I|BIT1_Z)	// set I:12, Z:11 bit to enable I-Cache, Branch Prediction
	mcr	p15, 0, r0, c1, c0, 0
	/* GIC secure part enable, route to non-secure */
	mrc	p15, 4, r0, c15, c0, 0		// Read CBAR
	orr	r2, r0, #0x1000

	mov	r1, #0xFFFF00FF			// gicd - interrupt secure-route
	str	r1, [r2, #0x080]		// gicd - interrupat Group Register

	mov	r1, #(0x3 << 0)			// gicd Enable NS[1], Enable S[0]
	str	r1, [r2, #0x000]		// gicd secure enable

	mov	r1, #0xFFFFFFFF
	str	r1, [r0, #0x104]		// gicc - interrupt priority to best

	mov	r1, #(0xB << 0)			// FIQ_En[3], EnableGrp1[1], EnableGrp0[0]
	str	r1, [r0, #0x100]		// gicc enable

	/*
	 * Processor is in Secure Mode, when the FIQ occurs
	 * in, which specifies the vector jump to Monitor Mode.
	 */
	mrc	p15, 0, r0, c1, c1, 0		// Read SCR into Rt
	orr	r0, r0, #(1 << 2)		// SCR- FIQ Forward[2]
	mcr	p15, 0, r0, c1, c1, 0		// Write Rt to SCR

	/*
	 * Set the Non-Secure FPU
	 * psw0523 add for fpu setting
	 */
	mrc	p15, 0, r0, c1, c1, 2
	orr	r0, r0, #3 << 10		// enable fpu/neon
	bic	r0, r0, #3 << 14		// temp
	mcr	p15, 0, r0, c1, c1, 2

	mov 	r0, #(0xF << 20)		// cp 11, 10, 11: full access
	mcr 	p15, 0, r0, c1, c0, 2		// Access Control Reg

	mov	r3, #0x40000000
	vmsr	fpexc, r3			// enable vpu

	mrc	p15, 0, r12, c0, c0, 5		// Get our cpu id
	ands	r12, r12, #0xF			// Save CPU id
	movne	r0, r12
	bne	SubCPUBoot

	mrc	p15, 4, r0, c15, c0, 0		// Read CBAR
	orr	r2, r0, #0x1000			// base of gicd

	mov	r1, #0xFFFFFFFF
	str	r1, [r2, #0x084]		// gicd - interrupt route to non-secure
	str	r1, [r2, #0x088]		// gicd - interrupt group
	str	r1, [r2, #0x08C]		// gicd - interrupt group

	/* Set to zero, Clear SRAM */
        ldr	r1, =__bss_start__		// this is auto-relocated!
        ldr	r2, =__bss_end__		// this is auto-relocated!

        mov	r3, #0x00000000			// prepare zero to clear BSS

clbss_l:
	cmp	r1, r2				// while not at end of BSS
	strlo	r3, [r1]			// clear 32-bit BSS word
	addlo	r1, r1, #4			// move to next
	blo	clbss_l

main:
	mrc	p15, 0, r0, c13, c0, 2		// restore boot status from tpidr
	mrc	p15, 0, r1, c13, c0, 3		// restore non-secure os ptr from tpidr
	mrc	p15, 0, r2, c13, c0, 4		// restore secure os ptr from tpidr
// void BootMain(isresume, non_secure, secure);
	bl	BootMain			// no need but system init
	b	.


	// R0 : boot status 0: cold, 1: warm boot
	// R1 : ?
	// R2 : boot config structure
.global secure_launch
//secure_launch(isresume, secureos_startaddr, non_secure_bl, bootarg_addr);
secure_launch:
	mov	r11, r1
	msr CPSR_c,  #(Mode_SVC|I_Bit)	/* switch to SVC mode */

	mov	lr, r2		// non_secure bl launch address
	mov	r1, r3		// g_bootarg_tee

	mov	r3, #0
	mcr	p15, 0, r3, c7, c5, 0	//; invalidate all instruction caches
	mcr	p15, 0, r3, c7, c5, 6	//; BPIALL - Invalidate entire branch predictor array

	bx	r11		// secure os launch address

.global non_secure_launch
// void non_secure_launch(isresume, non_secure);
non_secure_launch:
	mov	r4, r1

	mrs	r0, cpsr
	and	r0, #0x1f
	cmp	r0, #MODE_MON		// check monitor mode
	bne	.			// only mon mode can change secure state

	/* Change to Secure -> Non-Secure */
	bl	set_nonsecure_mode

	bl	Invalidate_entire_branch_predictor
	bl	InvalidateDataCache
	mov	lr, r4

	mov	r1,  #0
	mov	r2,  #0
	mov	r3,  #0
	mov	r4,  #0
	mov	r5,  #0
	mov	r6,  #0
	mov	r7,  #0
	mov	r8,  #0
	mov	r9,  #0
	mov	r10, #0
	mov	r11, #0
	mov	r12, #0

	/* switch to supervisor mode */
	mov	r0, #(Mode_SVC | I_Bit)
	msr	SPSR_cxsf, r0
	mov	r0, #0
	movs	pc, lr

	b	.

.align 5
.global system_sleep
//==================================================================
// suspend/resume routine
//==================================================================
.global s5p4418_suspend
system_sleep:					// r0:alive r1:drex
	mrc	p15, 0, r1, c1, c0, 0		// Read control register
	tst	r1, #BIT1_M			// check mmu on
	beq	physical_start
	bic	r1, r1, #(BIT1_I | BIT1_Z)	// Disable IC.
	mcr	p15, 0, r1, c1, c0, 0

	bic	r1, r1, #(BIT1_C | BIT1_M)	// Disable MMU & DC.

	ldr	r0, =physical_start
	cmp	r0, #0				// make sure no stall on "bx r0" below

	// Disable the MMU.
	mcr	p15, 0, r1, c1, c0, 0

	bx	r0				// jump to new physical address
	nop					// for 5 depth pipeline
	nop
	nop
	nop

	// MMU & Caches Now Disabled.
physical_start:
	mrs	r0, CPSR
	orr	r0, r0, #(I_Bit|F_Bit)		// Disable IRQ & FIQ.
	msr	CPSR_cxsf, r0

	mov	r0, #0
	mcr	p15, 0, r0, c7, c5, 0		//; Invalidate all instruction caches
	mcr	p15, 0, r0, c7, c5, 6		//; Invalidate entire branch predictor array

	// Set to Stack pointer
	ldr	sp, =BASEADDR_SRAM
	add	sp, sp, #INTERNAL_SRAM_SIZE	// Add to internal sram size.

	// Goto sleepMain function.
	bl	s5p4418_suspend
	b	.				// this time, core power will be off and so cpu will die.

	.ltorg
