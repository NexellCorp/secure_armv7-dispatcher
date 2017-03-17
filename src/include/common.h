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
#ifndef __COMMON_H__
#define __COMMON_H__

/* "startup.S" startup code function */
void system_sleep(void);

/* "armv7_lib.S" armv7 function */
void set_nonsecure_mode(void);
void set_secure_mode(void);

int get_secure_status(void);

 int arm9_get_scr(void);
void arm9_set_scr(int reg);

 int arm9_get_auxctrl(void);
void arm9_set_auxctrl(int value);

 int armv7_get_cpuid(void);

void cache_delay_ms(int ms);

#endif
