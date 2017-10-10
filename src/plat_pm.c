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
#include <sysheader.h>
#include <gic.h>
#include <plat_pm.h>

#include <nx_rtc.h>

/* External Function */
extern void ResetCon(u32 devicenum, cbool en);
extern void enterSelfRefresh(void);
extern u32 __calc_crc(void *addr, int len);
extern void delay(int milisecond);

static struct NX_RTC_RegisterSet *const pReg_RTC =
	(struct NX_RTC_RegisterSet * const)PHY_BASEADDR_RTC_MODULE;

static void smp_enable(int enable)
{
	int value;

	value = arm9_get_auxctrl();
	value &= ~(1 << 6);

	if (enable)
		value |= (1 << 6);

	arm9_set_auxctrl(value);
}

static inline void dmb(void)
{
	__asm__ __volatile__ ("dmb");
}

#if 0
/*
 * Must be S5P44418
 * For CPU Power Off, STANDBY_WFI[n]
 * the signal must wait until be High.
 */
static int core_get_standby_wfi(u32 cpu_id)
{
	cpu_id = cpu_id;

	return 0;
}
#endif

/*
  * Optionally available function S5P4418
  * This function is that the CORE Active/Power Down state,
  * it is not known for sure.
  * But It is that you can see at least try.
  * Return Parameter : 0: ON, 1:OFF, 2:PENDING
  */
int s5p4418_cpu_check(u32 cpu_id)
{
	return (RIO32(&pReg_Tieoff->TIEOFFREG[0])
			>> (cpu_id + 6)) & 0x1;
}

/*************************************************************
 * Must be S5P4418
 * CPU Power On sequence in S5P4418
 * A sequence that satisfies both the Power Off
 * and Hotplug.
 * must go through the following steps:
 *
 * Step 01. CPUx Block Reset Assert
 * Step 02. CPUx Power Active (Gating Cells)
 * Step 03. Have to wait more than 5us(tPC).
 * Step 04. CPUCLKOFF Set to 0 except CPU0
 * Step 05. Have to wait more than 15Cycle (tRC1).
 * Step 06. CPUCLKOFF Set to 1 except CPU0
 * Step 07. Have to wait more than 10us(tCR).
 * Step 08. CPUx Block Reset Negate
 * Step 09. Set to (CPUx) Clmap Signal Low.
 * Step 10. Have to wait more than 20us(tRC2).
 * Step 11. CPUx Power On (CPUCLKOFF Set to 0)
 *************************************************************/

int s5p4418_cpu_on(u32 cpu_id)
{
	/* Secondary CPU Wakeup Start Point (High Vector) */
	SIO32(&pReg_Tieoff->TIEOFFREG[0], ((1 << cpu_id) << 18));

	/* Step 01. CPUx Block Reset Assert */
	ResetCon(cpu_id, CTRUE);
	dmb();

	/* Step 02. CPUx Power Active (Power Gating Cells)  (1: Power Down, 0:Active) */
	CIO32(&pReg_Tieoff->TIEOFFREG[0], ((1 << cpu_id) << 6));
	dmb();

	/* Step 03 Waiting for 5us(tPC) */
	cache_delay_ms(0x2000);
	dmb();

	/* Step 04 CPUCLKOFF Set to 0 except CPU0 */
	CIO32(&pReg_Tieoff->TIEOFFREG[1], ((1 << cpu_id) << (37 - 32)));
	dmb();

	/* Step 05 Waiting for 15Cycle(tRC1) */
	cache_delay_ms(0x2000);
	dmb();

	/* Step 06. CPUCLKOFF Set to 1 except CPU0 */
	SIO32(&pReg_Tieoff->TIEOFFREG[1], ((1 << cpu_id) << (37 - 32)));
	dmb();

	/* Step 07. Waiting for 10us (tCR) */
	cache_delay_ms(0x2000);
	dmb();

	/* Step 08. CPUx Block Reset Negate */
	ResetCon(cpu_id, CFALSE);
	dmb();

	/* Step 09. Set to (CPUx) Clamp Signal Low */
	CIO32(&pReg_Tieoff->TIEOFFREG[0], (1 << (cpu_id + 1)));
	dmb();

	/* Step 10. Waiting for 20us (tRC2) */
	cache_delay_ms(0xFFFF);
	dmb();

	/* Step 11. CPUCLKOFF Set to 0 except CPU0 */
	CIO32(&pReg_Tieoff->TIEOFFREG[1], ((1 << cpu_id) << (37 - 32)));
	dmb();

	return 0;		// return - 0: Success
}

void s5p4418_cpu_off_wfi_ready(void)
{
 	/* CPUx Interface Control - Group 0, 1 OFF */
	CIO32((void*)gicc_get_baseaddr(), (3 << 0));

	/* All Function Clean & SMP Detach */
	smp_enable(0);
}

/*************************************************************
 * Must be S5P4418
 * CPU Power Off sequence in S5P4418
 * must go through the following steps:
 *
 * Step 01. Set the Clam Signal (High)
 * Step 02. Have to wait more than 20us.
 * Step 03. Waiting for the Standard WFI Signal Gating.
 * Step 04. Set the CPUx Power Down
 * Step 05. Reset to the CPUx Block.
 *************************************************************/
int s5p4418_cpu_off(u32 cpu_id)
{
	int ret = 1;

	/* Step 01. Check to Core Standard WFI Signal */
#if 0
	while(!core_get_standby_wfi(cpu_id));
#else
	cache_delay_ms(0xFFFFF);
#endif
	dmb();

	/* Step XX. Waiting for tCC Paramter */
	cache_delay_ms(0xFFF);
	dmb();

	/* Step 02. Set to (CPUx) Clamp Signal High */
	SIO32(&pReg_Tieoff->TIEOFFREG[0], (1 << (cpu_id + 1)));
	dmb();

	/* Step 03. Waiting for 20us */
	cache_delay_ms(0xFFFF);
	dmb();

	/* Step 04. Waiting for the Standard WFI Signal Gating. */
#if 0
	while(core_get_standby_wfi(cpu_id));
#else
	cache_delay_ms(0xFFFF);
#endif
	dmb();

	/* Step 05. CPUx Power DeActive (Power Gating Cells)  (1: Power Down, 0:Active) */
	SIO32(&pReg_Tieoff->TIEOFFREG[0], ((1 << cpu_id) << 6));
	dmb();

	return ret;		// 0: ON, 1:OFF, 2:PENDING
}

/*************************************************************
 * s5p4418 system reset (method: power control)
 *************************************************************/
void s5p4418_reset_cpu(void)
{
	void *base = (void *)PHY_BASEADDR_CLKPWR_MODULE;
	int pwrcont = 0x224;
	int pwrmode = 0x228;
	u32 reg = RIO32((void *)(base + pwrcont));

	WIO32(base + pwrcont, reg | 1 << 3);
	WIO32(base + pwrmode, 1 << 12);
}

void s5p4418_resume(void)
{
	NOTICE("system resume\r\n");
}

static void suspend_vdd_pwroff(void)
{
	register unsigned int alive_pend;

	// Clear USE_WFI & USE_WFE bits for STOP mode.
	CIO32(&pReg_ClkPwr->PWRCONT, (0xFF << 8));
	// alive power gate open
	WIO32(&pReg_Alive->ALIVEPWRGATEREG, 0x00000001);

	WIO32(&pReg_RTC->RTCSCRATCH, pSBI->BL2_START);

	WIO32(&pReg_Alive->VDDOFFCNTVALUERST, 0xFFFFFFFF);	/* clear delay counter, refrence rtc clock */
	WIO32(&pReg_Alive->VDDOFFCNTVALUESET, 0x00000001);	/* set minimum delay time for VDDPWRON pin. 1 cycle per 32.768Kh (about 30us) */

	__asm__ __volatile__ ("cpsid i");			/* core interrupt off. */
	WIO32(&pReg_Alive->VDDCTRLSETREG, 0x000003FC);		/* Retention off (Pad hold off) */
	WIO32(&pReg_Alive->VDDCTRLRSTREG, 0x00000001);		/* vddpoweron off, start counting down. */

	delay(600);     /* 600 : 110us, Delay for Pending Clear. When CPU clock is 400MHz, this value is minimum delay value. */

	WIO32(&pReg_Alive->ALIVEGPIODETECTPENDREG, 0xFF);	/* all alive pend pending clear until power down. */
	WIO32(&pReg_Alive->ALIVEPWRGATEREG, 0x00000000);	/* alive power gate close */

	/* Suspend A defense code for the failure sequence. */
	delay(0x20000);

	delay(0x200);

	watchdog_start(0x3333);
	while(1);
}


/*************************************************************
 * Before entering suspend and Mark the location and promise Kernel.
 * Reference CRC, Jump Address, Memory Address(CRC), Size(CRC)
 *************************************************************/
void suspend_mark(u32 state, u32 entrypoint)
{
	WIO32(&pReg_Alive->ALIVESCRATCHRSTREG, 0xFFFFFFFF);
	WIO32(&pReg_Alive->ALIVESCRATCHRST1, 0xFFFFFFFF);
	WIO32(&pReg_Alive->ALIVESCRATCHRST2, 0xFFFFFFFF);
	WIO32(&pReg_Alive->ALIVESCRATCHRST3, 0xFFFFFFFF);
	WIO32(&pReg_Alive->ALIVESCRATCHRST4, 0xFFFFFFFF);

	if (state == PSCI_SUSPEND) {
		void *mem = (void*)0x91080000;
		u32 size = 128 * 1024;
		u32 crc = __calc_crc((void *)mem, size);
		WIO32(&pReg_Alive->ALIVESCRATCHSETREG, SUSPEND_SIGNATURE);
		WIO32(&pReg_Alive->ALIVESCRATCHSET1, entrypoint);
		WIO32(&pReg_Alive->ALIVESCRATCHSET2, crc);
		WIO32(&pReg_Alive->ALIVESCRATCHSET3, mem);
		WIO32(&pReg_Alive->ALIVESCRATCHSET4, size);
	}
}

void s5p4418_l2c_disable(void)
{
//	u32 sysip_base = get_sysipbase();
	u32 cache_way = (1 << 16) - 1;

	if ((RIO32(0xcf000100) & 1) == 0)	/* check ctrl enabled */
		return;

	WIO32(0xcf0007fc, cache_way);		/* 16 way, clean inv way */
	while (RIO32(0xcf0007fc) & cache_way)
		;
	WIO32(0xcf000730, 0);			/* cache sync; */

	while (RIO32(0xcf00077c) & cache_way)	/* inv way */
		;
	while (RIO32(0xcf0007bc) & cache_way)	/* clean way */
		;
	while (RIO32(0xcf0007fc) & cache_way)	/* clean inv way */
		;
	WIO32(0xcf000100, 0);			/* ctrl disable */
}
/************************************************************
 * Must be S5P4418
 * Susepnd Off sequence in S5P4418
 * must go through the following steps:
 *
 * Step 01. (SDRAM) Enter the Self Refresh.
 * Step 02. (BOARD & CORE) VDD Power OFF
 ************************************************************/
void s5p4418_suspend(void)
{
	enterSelfRefresh();
	suspend_vdd_pwroff();
}
