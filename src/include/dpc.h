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
#ifndef __DPC_H__
#define __DPC_H__

void dpc_set_enable_all(u32 module, u32 enb);
 int dpc_get_pending_all(u32 module);
void dpc_clear_pending_all(u32 module);
int  dpc_enabled(u32 module);

#endif
