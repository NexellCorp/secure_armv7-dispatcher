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
 * Author	: Hans
 * History	: 2017.03.14 new release
 */
#ifndef __NX_DDR3_SDRAM_H__
#define	__NX_DDR3_SDRAM_H__

typedef enum {
	SDRAM_MODE_REG_MR0      = 0,
	SDRAM_MODE_REG_MR1      = 1,
	SDRAM_MODE_REG_MR2      = 2,
	SDRAM_MODE_REG_MR3      = 3,
	/* LPDDR3 Features */
	SDRAM_MODE_REG_MR11	= 11,
	SDRAM_MODE_REG_MAX_MRn  = 0xFF
}	SDRAM_MODE_REG;

struct SDRAM_MR0 {
	u16 BL		:2;     /* 0 Burst Length	( 00: Fixed BL8, 01: 4 or 8 (A12), 10: Fixed BC4, 11: Reserved) */
	u16 CL0		:1;     /* 2 CAS Latency 0 bit (0000: Reserved, 0010: 5, 0100: 6, 0110: 7, 1000: 8, 1010: 9, 1100:10, 1110:11, 0001:12, 0011:13 */
	u16 BT		:1;     /* 3 READ Burst Type (0: Sequential(nibble), 1: Interleaved) */
	u16 CL1		:3;     /* 4 CAS Latency 0 bit */
	u16 SBZ0	:1;     /* 7 */
	u16 DLL		:1;     /* 8 DLL Reset ( 0: No, 1: Yes) */
	u16 WR		:3;     /* 9 Write Recovery (000: Reserved, 001: 5, 010: 6, 011: 7, 100: 8, 101: 10, 110: 12, 111: 14) */
	u16 PD		:1;     /* 12 Precharge PD (0:DLL off (slow exit), 1:DLL on (fast exit) */
	u16 SBZ1	:3;     /* 13 ~ 15 */
};
struct SDRAM_MR1 {
	u16 DLL		:1;     /* 0 DLL Enable (0: enable, 1: disable) */
	u16 ODS0	:1;     /* 1 Output Drive Strenghth 0 bit (00: RZQ/6(40ohm), 01: RZQ/7(34ohm), 10, 11: reserved) */
	u16 RTT_Nom0	:1;     /* 2 */
	u16 AL		:2;     /* 3 Additive Latency	(00: disable, 01: AL=CL-1, 10: AL=CL-2, 11: Reserved) */
	u16 ODS1	:1;     /* 5 Output Drive Strenghth 1 bit */
	u16 RTT_Nom1	:1;     /* 6 */
	u16 WL		:1;     /* 7 Write Levelization (0: disable, 1: enable) */
	u16 SBZ0	:1;     /* 8 */
	u16 RTT_Nom2	:1;     /* 9 (000: disable, 001: RZQ/4(60ohm), 010: RZQ/2(120ohm), 011: RZQ/6(40ohm), 100: RZQ/12(20ohm), 101: RZQ/8(30ohm) 110, 111: Reserved) */
	u16 SBZ1	:1;     /* 10 */
	u16 TDQS	:1;     /* 11 TDQS (0: disable, 1: enable) */
	u16 QOff	:1;     /* 12 Q Off (0: enabled, 1: disabled) */
	u16 SBZ2	:3;     /* 13 ~ 15 */
};

struct SDRAM_MR2 {
	u16 SBZ0	:3;     /* 0 */
	u16 CWL		:3;     /* 3 CAS Write Latency (000: 5CK(tCK>=2.5ns), 001: 6CK(2.5ns>tCK>=1.875ns, 010: 7CK(1.875ns>tCK>=1.25ns), 011: 8CK(1.5ns>tCK>=1.25ns), 100: 9CK(1.25ns>tCK>=1.07ns)) */
	u16 ASR		:1;     /* 6 Auto Self Refresh(option) (0: disabled:Manual, 1:Enabled: Automatic) */
	u16 SRT		:1;     /* 7 Self Refresh Temperature (0: Normal(0 to 85 degree), 1: Extended(0 to 95 degree) */
	u16 SBZ1	:1;     /* 8 */
	u16 RTT_WR	:2;     /* 9 Dynamic ODT (00: Rtt disabled, 01: RZQ/4, 10: RZQ/2, 11: reserved) */
	u16 SBZ2	:5;     /* 11 ~ 15 */
};

struct SDRAM_MR3 {
	u16 MPR_RF	:2;     /* 0 MPR Read Function (00: Predefined pattern, 01, 10, 11: reserved) */
	u16 MPR		:1;     /* 2 MPR Enable (0: Normal DRAM operation, 1: Dataflow from MPR) */
	u16 SBZ0	:13;    /* 3 ~ 15 */
};

struct LPDDR3_MR0 {
	u16 DAI		:1;     /* 0 Device Auto-Initialization Status( 0: DAI still in progress) */
	u16 _RFU0	:2;     /* 1 ~ 2 */
	u16 RZQI	:2;     /* 3 ~ 4 RZQ Information */
	u16 _RFU1	:1;     /* 5 */
	u16 WL		:1;     /* 6 */
	u16 RL3		:1;     /* 7 */
	u16 _reserved	:8;     /* 8 ~ 15 */
};
struct LPDDR3_MR1 {
	u16 BL		:3;     /* 0 ~ 2 */
	u16 _RFU0	:2;     /* 3 ~ 4 */
	u16 WR		:3;     /* 5 ~ 7 */
	u16 _reserved	:8;     /* 8 ~ 15 */
};

struct LPDDR3_MR2 {
	u16 RL_WL	:4;     /* 0 ~ 3 */
	u16 WRE		:1;     /* 4 */
	u16 _RFU0	:1;     /* 5 */
	u16 WL_SEL	:1;     /* 6 */
	u16 WR_LVL	:1;     /* 7 */
	u16 _reserved	:8;     /* 8 ~ 15 */
};

struct LPDDR3_MR3 {
	u16 DS		:4;     /* 0 ~ 3 */
	u16 _RFU0	:4;     /* 4 ~ 7 */
	u16 _reserved	:8;     /* 8 ~ 15 */
};

struct LPDDR3_MR11 {
	u16 DQ_ODT	:2;     /* 0 ~ 1 */
	u16 PD_CON	:1;     /* 2 */
	u16 _RFU0	:5;     /* 3 ~ 7 */
	u16 _reserved	:8;     /* 8 ~ 15 */
};

union SDRAM_MR {
	struct SDRAM_MR0    MR0;
	struct SDRAM_MR1    MR1;
	struct SDRAM_MR2    MR2;
	struct SDRAM_MR3    MR3;

	struct LPDDR3_MR0   LP_MR0;
	struct LPDDR3_MR1   LP_MR1;
	struct LPDDR3_MR2   LP_MR2;
	struct LPDDR3_MR3   LP_MR3;
	struct LPDDR3_MR11  LP_MR11;

	u16 Reg;
};

#endif	/* __NX_DDR3_SDRAM_H__ */
