/*
 * Copyright (C) 2012 Nexell Co., All Rights Reserved
 * Nexell Co. Proprietary & Confidential
 *
 * NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
 * AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Module	:
 * File		: nx_printf.h
 * Description	:
 * Author	:
 * History	:
 */
#ifndef __NX_PRINTF__
#define __NX_PRINTF__

#include <stdarg.h>
#include <plat_debug.h>

#define putchar DebugPutch
int printf(const char *format, ...);

#endif /* __NX_PRINTF__ */
