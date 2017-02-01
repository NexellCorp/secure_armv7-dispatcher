/*
 * Copyright (C) 2016  Nexell Co., Ltd.
 * Author: Sangjong, Han <hans@nexell.co.kr>
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
// Memory List
#define 	K4B4G0846B_HCK0		0	//  8bit, 512MByte, Lynx DDR3x4
#define 	K4B4G1646B_HCMA		1	// 16bit, 512MByte, Cadence, 933MHz
#define 	H5TQ4G63AFR		2	// 16bit, 512MByte, Lynx DDR3x2, UrBetter
#define		TEMP			3

//#define 	VTK			2
//#define 	ELCOMTECH	3

// Clock List
#define		DDR3_800	5
#define		DDR3_666	6
#define		DDR3_533	7
#define		DDR3_TEST	8


// User Define
#define		MEMORY 		H5TQ4G63AFR
#define		MEM_CLK		DDR3_TEST

#define		RDFETCH		0x1			// CONCONTROL.rd_fetch[14:12]
#define   	PHYCON4		0x0C0C0C0C		//- ctrl_offsetr, 0x0C0C0C0C, 0x18181818
#define   	PHYCON6		0x02020202		//- ctrl_offsetw, 0x04040404, 0x02020202



// Option
#define		CONFIG_ODTOFF_GATELEVELINGON		0
#define 	REG_MSG					1
#define 	MEMTEST					0		// Not support yet.
#define		QOS1					1

#define		chip_row	3		// ROW address bit : 15bit : 3(Tabletx2), 16bit : 4(Tabletx4, Elcomtech)
#define 	chip_mask	0x7C0		// Capacity per nCS: 2G=0x780(Tabletx4, Elcomtech), 1G=0x7C0(Tabletx2, VTK)
#define 	CWL		3		// CAS Write Latency(CWL).
						// 0:5ck(tCK>2.5ns), 1:6ck(2.5ns>tCK>1.875ns), 2:7ck(1.875ns>tCK>2.5ns), 3:8ck(1.5ns>tCK>1.25ns),
						// 4:9ck(1.25ns>tCK>1.07ns), 5:10ck(1.07ns>tCK>0.935ns), 6:11ck(0.935ns>tCK>0.833ns), 7:12ck(0.833ns>tCK>0.75ns)
#define		WL		0x8		// WL=CWL+AL
#define		RL		0xB		// RL=CAS latency + AL.
#define		ChipNum		1		// nCS Number : Tablet DDR3x2=1, VTK, Tablet DDR3x4=2
#define 	BUSWIDTH 	16		// 16bit, 8bit


/*********************************************************
   16bit bus memory, K4B4G1646B_HCMA, H5TQ4G63AFR
*********************************************************/

#if (MEMORY == H5TQ4G63AFR)

#if 0
#if (MEM_CLK == DDR3_TEST)
// It is dependent on Customer Board(DDR3)
// for Samsung VD(DDR 666)
#define		tREF		0x513		// 0x513

#define 	tRFC		0x57		// 0x57
#define 	tRRD		0x4		// 0x4	
#define		tRP		0x5		// 0x5	
#define 	tRCD		0x5		// 0x6	
#define		tRC		0x11		// 0x10
#define		tRAS		0xD		// 0xC	

#define		tWTR		0x4		// 0x4	
#define		tWR		0x5		// 0x5	
#define		tRTP		0x4		// 0x4	
#define		W2W_C2C		0x1		// 0x1	
#define		R2R_C2C		0x1		// 0x1	

#define 	tFAW		0xC		// 0xC	
#define 	tXSR		0x5A		// 0x5A
#define 	tXP		0x3		// 0x3	
#define		tCKE		0x3		// 0x3	
#define		tMRD		0x4		// 0x4	
#endif

#else
#if (MEM_CLK == DDR3_TEST)
// It is dependent on Customer Board(DDR3)
// for Samsung VD(DDR 533)
#define		tREF		0x618		// 0x40E	0x71A

#define 	tRFC		0x68		// 0x46	0x7A
#define 	tRRD		0x4		// 0x4	0x4	
#define		tRP		0x6		// 0x4	0x7	
#define 	tRCD		0x6		// 0x5	0x7	
#define		tRC		0x14		// 0xD	0x17
#define		tRAS		0xF		// 0xA	0x11

#define		tWTR		0x4		// 0x4	0x4	
#define		tWR		0x6		// 0x4	0x7	
#define		tRTP		0x4		// 0x4	0x4	
#define		W2W_C2C		0x1		// 0x1	0x1	
#define		R2R_C2C		0x1		// 0x1	0x1	

// from SOC
/*
#define 	tFAW			0x10		// 0xA	0x11
#define 	tXSR			0x20	// 0x48	0x7E
#define 	tXP			0xA		// 0x3	0x3	
#define		tCKE		0x2		// 0x3	0x3	
#define		tMRD		0x4		// 0x4	0x4	
*/

#define 	tFAW		0xC		// 0xA	0x11
#define 	tXSR		0x6C		// 0x48	0x7E
#define 	tXP		0x3		// 0x3	0x3	
#define		tCKE		0x3		// 0x3	0x3	
#define		tMRD		0x4		// 0x4	0x4	

#endif


#endif


// AC Timing
#if 1	// 800MHz Over
#if (MEM_CLK == DDR3_800)

#define		tREF		0x618		// 7.8us

#define 	tRFC		0x50
#define 	tRRD		0x2
#define		tRP		0x4
#define 	tRCD		0X5		//0x6
#define		tRC		0xe
#define		tRAS		0xd

#define		tWTR		0x2
#define		tWR		0x4
#define		tRTP		0x2
#define		W2W		0x1
#define		R2R		0x1

#define 	tFAW		0xC
#define 	tXSR		0x6C
#define 	tXP		0x3
#define		tCKE		0x3
#define		tMRD		0x4

#endif

#else	// 800MHz Typ
#if (MEM_CLK == DDR3_800)

#define		tREF		0x618		// 7.8us

#define 	tRFC		0x68
#define 	tRRD		0x4
#define		tRP		0x6
#define 	tRCD		0X7		//0x6
#define		tRC		0x14
#define		tRAS		0xF

#define		tWTR		0x4
#define		tWR		0x6
#define		tRTP		0x4
#define		W2W		0x1
#define		R2R		0x1

#define 	tFAW		0xC
#define 	tXSR		0x6C
#define 	tXP		0x3
#define		tCKE		0x3
#define		tMRD		0x4

#endif

#endif


#if (MEM_CLK == DDR3_666)

#define		tREF		0x513

#define 	tRFC		0x57
#define 	tRRD		0x4
#define		tRP		0x5
#define 	tRCD		0x6
#define		tRC		0x10
#define		tRAS		0xC

#define		tWTR		0x4
#define		tWR		0x5
#define		tRTP		0x4
#define		W2W_C2C		0x1
#define		R2R_C2C		0x1

#define 	tFAW		0xA
#define 	tXSR		0x5A
#define 	tXP		0x2
#define		tCKE		0x3
#define		tMRD		0x4

#endif


#if (MEM_CLK == DDR3_533)

#define		tREF		0x40E

#define 	tRFC		0x46
#define 	tRRD		0x4
#define		tRP		0x4
#define 	tRCD		0x5
#define		tRC		0xD
#define		tRAS		0xA

#define		tWTR		0x4
#define		tWR		0x4
#define		tRTP		0x4
#define		W2W_C2C		0x1
#define		R2R_C2C		0x1

#define 	tFAW		0x8
#define 	tXSR		0x48
#define 	tXP		0x2
#define		tCKE		0x3
#define		tMRD		0x4

#endif

#if 0
#if (MEM_CLK == DDR3_TEST)
// It is dependent on Customer Board(DDR3)
// for Samsung VD(DDR 666)
#define		tREF		0x513

#define 	tRFC		0x51
#define 	tRRD		0x4
#define		tRP		0x5
#define 	tRCD		0x6
#define		tRC		0x10
#define		tRAS		0xC

#define		tWTR		0x4
#define		tWR		0x5
#define		tRTP		0x4
#define		W2W_C2C		0x1
#define		R2R_C2C		0x1

#define 	tFAW		0xC
#define 	tXSR		0x1B
#define 	tXP		0x2
#define		tCKE		0x3
#define		tMRD		0x4
#endif
#endif

#if 0
#if  (MEM_CLK == DDR3_TEST)
// It is dependent on Customer Board(DDR3)
// for Elcomtech
//						// 800		666		533		400
#define		tREF		0x668		//0x618		0x513	0x40E	0x30C

#define 	tRFC		0x6B		//0x68		0x57		0x46		0x34
#define 	tRRD		0x4
#define		tRP		0x5		//0x5		0x5		0x4		0x3
#define 	tRCD		0x6		//0x5		0x6		0x5		0x4
#define		tRC		0x12		//0x10		0x10		0xD		0xA
#define		tRAS		0xB		//0xA		0xC		0xA		0x8

#define		tWTR		0x4		//0x3		0x4		0x4		0x4
#define		tWR		0x5		//0x4		0x5		0x4		0x6
#define		tRTP		0x4		//0x3		0x4		0x4		0x4
#define		W2W_C2C		0x1
#define		R2R_C2C		0x1

#define 	tFAW		0x11		//0x10		0x9		0x8		0x6
#define 	tXSR		0x6F		//0x6C		0x5A		0x48		0x36
#define 	tXP		0x3		//0x3		0x3		0x3		0x3
#define		tCKE		0x3
#define		tMRD		0x4

#endif
#endif
#endif


/*********************************************************
   8bit bus memory, K4B4G0846B_HCK0
*********************************************************/

#if (MEMORY == K4B4G0846B_HCK0)
// Refer to Memory Datasheet
#define		chip_row	4		// ROW address bit : 15bit : 3(Tabletx2), 16bit : 4(Tabletx4, Elcomtech)
#define 	chip_mask	0x780	// Capacity per nCS: 2G=0x780(Tabletx4, Elcomtech), 1G=0x7C0(Tabletx2, VTK)
#define 	CWL		3		// CAS Write Latency(CWL).
								// 0:5ck(tCK>2.5ns), 1:6ck(2.5ns>tCK>1.875ns), 2:7ck(1.875ns>tCK>2.5ns), 3:8ck(1.5ns>tCK>1.25ns),
								// 4:9ck(1.25ns>tCK>1.07ns), 5:10ck(1.07ns>tCK>0.935ns), 6:11ck(0.935ns>tCK>0.833ns), 7:12ck(0.833ns>tCK>0.75ns)
#define		WL		0x8		// WL=CWL+AL
#define		RL		0xB		// RL=CAS latency + AL.
#define		ChipNum		2		// nCS Number : Tablet=1, VTK=2
#define 	BUSWIDTH 	8		// 16bit, 8bit

#if (MEM_CLK == DDR3_800)
#define		tREF		0x618	// 7.8us

#define 	tRFC		0x68
#define 	tRRD		0x4
#define		tRP		0x6
#define 	tRCD		0X6
#define		tRC		0x14
#define		tRAS		0xF

#define		tWTR		0x4
#define		tWR		0x6
#define		tRTP		0x4
#define		W2W		0x1
#define		R2R		0x1

#define 	tFAW		0xC
#define 	tXSR		0x20
#define 	tXP		0x2
#define		tCKE		0x3
#define		tMRD		0x4
#endif

#if (MEM_CLK == DDR3_666)
#define		tREF		0x513	// 7.8us

#define 	tRFC		0x57
#define 	tRRD		0x4
#define		tRP		0x5
#define 	tRCD		0X6
#define		tRC		0x11
#define		tRAS		0xD

#define		tWTR		0x4
#define		tWR		0x5
#define		tRTP		0x4
#define		W2W		0x1
#define		R2R		0x1

#define 	tFAW		0xA
#define 	tXSR		0xB
#define 	tXP		0x1
#define		tCKE		0x3
#define		tMRD		0x4
#endif
#endif
