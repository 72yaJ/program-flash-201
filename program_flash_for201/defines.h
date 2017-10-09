/***********************************************************************
       Defines for the TigerSHARC EZ-Kit
       Defines.h
************************************************************************/

#define	WORDCOUNT		0x10							// Total # of 32-bit words to write
#define STARTOFFLASH	0x10000							// Starting address of flash to write to
#define FLAG2_EN		22								// FLAG3 output enable is bit 23 in SQCTL
#define FLAG3_EN		23								// FLAG3 output enable is bit 23 in SQCTL
#define FLAG2_OUT		26								// FLAG3 out pin is bit 27 in SQCTL
#define FLAG3_OUT		27								// FLAG3 out pin is bit 27 in SQCTL
#define _CODEC			0x0c000000

//***********************************************************************

#define _P1_OFFSET    	0x02400000						// Processor ID1 MP memory offset

#define INTDMA0			14
#define INTGLOBAL		60
	
#define	FLAG2_EN		22								// FLAG2 output enable is bit 22 in SQCTL
#define	FLAG_TWO_OUT	26								// FLAG2 out pin is bit 26 in SQCTL

#define	TIME_DELAY_L	45000000						// 45 million LSW of delay counter
#define	TIME_DELAY_H	0								// MSW of delay counter

#define BufNumPoints	1024

#define	N				1024
#define	STAGES			10
#define MIDSTAGE		(STAGES/2 + 1)

#define _MinusOverLn2	-1.4427
#define _OverLog2of10	

//***********************************************************************
//.var _Flags;												// various flags
#define	_FFTBufferReady		0

