/*
	Note: the ldr file must be binary format!!!!
		  the flash is Intel 28F128J3A(128Mbit)
		  this program is verified by wwj,and it's ok.
*/

#include "sysreg.h"
#include "signal.h"
#include "stdio.h"
#include "defts201.h"

#define dprintf //

#define wr_dat(addr,data)	(*(volatile int *)(addr))=(data)
#define rd_dat(addr,data)	(data)=(*(volatile int *)(addr))

#define wr_dat64(addr,data)	(*(volatile long long  *)(addr))=(data)
#define rd_dat64(addr,data)	(data)=(*(volatile long long  *)(addr))
#define TRUE			0x1
#define FALSE			0x0
#define BUFFER_SIZE		16*1024		//InputFileWordNumber
#define ManuCode 		0x89
#define DeviceCode 		0x18
#define N				32
#define BLOCK_SIZE		0x20000


//滨湖厂分块程序烧写
#define  MAX_PROG_BLK		8
#define  PROG_OFFSET_REG	0x38300000
#define  PROG_BANK_0	 0x0
#define  PROG_BANK_1	 0x1
#define  PROG_BANK_2	 0x2
#define  PROG_BANK_3	 0x3
#define  PROG_BANK_4	 0x4
#define  PROG_BANK_5	 0x5
#define  PROG_BANK_6	 0x6
#define  PROG_BANK_7	 0x7



//PROG_OFFSET_REG 的bit7=1,Flash高位地址由寄存器PROG_OFFSET_REG控制，＝0，flash的高位地址由ID_CON 控制
//高位地址如下分配：PROG_OFFSET_REG(2:0) :FLASH_ADDR(24:22)
//PROG_OFFSET_REG(3) 对应两片Flash的片选信号＝0选择第1片，等于1选择第2片
//
//注意，由于这个版本使用的是FLash，因此，FLashAddr24不存在，因此每个flash的程序块只有4个块
//
//


#define SDRAM_OFFSET 0x40000000  

// function prototypes
bool SysInitial();
void GetManuID();
bool ReadFlash( long lOffset, int *pnValue );
bool WriteFlash( long lOffset, int nValue );
bool Flash_Read_Array();
bool Flash_Read_Status_Register();
void Flash_Clear_Status_Register(void);
bool Flash_Clear_Block_LockBits();
bool WriteData( long lStart, long lCount, long lStride, int *pnData );
bool ReadData( long lStart, long lCount, long lStride, int *pnData );
bool Flash_Block_Erase(int Blk_No);
bool Flash_WordByte_Program(int offset,int data);
void dma_int_0();
void FlashPrograming(FILE *fp);
// sets up DMA registers
void do_dma();
struct TCB {
	int *DI;										// index
	int DX;											// count and stride in x direction
	int DY;											// count and stride in y direction
	int DP;											// DMA control word
};

struct TCB TCB_source,TCB_dest;								// Temp structure for programming TCBs

__builtin_quad q;	
// global data for use with the VisualDSP++ plug-in
char AFP_Title[] = "Lion TS201 board";
char AFP_Description[] = "28F128J3A";
int AFP_ManCode = -1;				
int AFP_DevCode = -1;
int AFP_Status_reg=-1;
long AFP_Offset = 0x0;
int AFP_Buffer[BUFFER_SIZE];
//={ 	#include "boot_test_pi.ldr"};
long AFP_Size = BUFFER_SIZE;
long AFP_Count = -1;
long AFP_Stride = -1;

int test_cnt=0;
//int data[BUFFER_SIZE]={0};
int temp_buf;
int temp_stor;
int start_addr=0;
int counter=0;
#define DSP_BOARD_NUMB 8
void main()
{
	
	int i,j,k=0,temp=0,Prog_Sn;	
	int size=0;
	 FILE *fp;
	SysInitial();

//	Flash_Read_Array();
//	GetManuID();

	printf("Start program sn0 \n");
	if((fp=fopen("E:\\8_Boot_0114.ldr","rb"))==NULL)
	{
		printf("can't open this file!\n");
	    printf("*****Programming progmar sn0 error !*****\n");	
        exit(0);
	}
	fseek(fp,0,2);
	size=ftell(fp);
	fseek(fp,0,0);
    
	printf("Erasing Flash *****\n");
	wr_dat64(PROG_OFFSET_REG,0x80+0);
    Flash_Clear_Block_LockBits();

	size=size*4/128/1024+1;
	for(i=0;i<size;i++)
	{
	   Flash_Block_Erase(i); 
	}
	
    printf("Erasing completely *****\n");
    	
	FlashPrograming(fp);
	fclose(fp);
	printf("*****Programming progmar sn0 is successful!*****\n");

////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Start program sn1 \n");
	if((fp=fopen("E:\\8_Boot_0114.ldr","rb"))==NULL)
	{
		printf("can't open this file!\n");
	    printf("*****Programming progmar sn1 error !*****\n");	
        exit(0);
	}
	fseek(fp,0,2);
	size=ftell(fp);
	fseek(fp,0,0);
	printf("Erasing Flash *****\n");
	wr_dat64(PROG_OFFSET_REG,0x80+1);
 //   Flash_Clear_Block_LockBits();

	size=size*4/128/1024+1;
	for(i=0;i<size;i++)
	{
	   Flash_Block_Erase(i); 
	}
	
    printf("Erasing completely *****\n");
    	
	FlashPrograming(fp);
	fclose(fp);
	printf("*****Programming progmar sn1 is successful!*****\n");	
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Start program sn2 \n");
	if((fp=fopen("E:\\8_Boot_0114.ldr","rb"))==NULL)
	{
		printf("can't open this file!\n");
	    printf("*****Programming progmar sn2 error !*****\n");	
        exit(0);
	}
	fseek(fp,0,2);
	size=ftell(fp);
	fseek(fp,0,0);
	printf("Erasing Flash *****\n");
	wr_dat64(PROG_OFFSET_REG,0x80+2);
 //   Flash_Clear_Block_LockBits();

	size=size*4/128/1024+1;
	for(i=0;i<size;i++)
	{
	   Flash_Block_Erase(i); 
	}
	
    printf("Erasing completely *****\n");
    	
	FlashPrograming(fp);
	fclose(fp);
	printf("*****Programming progmar sn2 is successful!*****\n");

////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	printf("Start program sn3 \n");
	if((fp=fopen("E:\\8_Boot_0114.ldr","rb"))==NULL)
	{
		printf("can't open this file!\n");
	    printf("*****Programming progmar sn3 error !*****\n");	
        exit(0);
	}
	fseek(fp,0,2);
	size=ftell(fp);
	fseek(fp,0,0);
	printf("Erasing Flash *****\n");
	wr_dat64(PROG_OFFSET_REG,0x80+3);
 //   Flash_Clear_Block_LockBits();

	size=size*4/128/1024+1;
	for(i=0;i<size;i++)
	{
	   Flash_Block_Erase(i); 
	}
	
    printf("Erasing completely *****\n");
    	
	FlashPrograming(fp);
	fclose(fp);
	printf("*****Programming progmar sn3 is successful!*****\n");
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	printf("Start program sn4 \n");
	if((fp=fopen("E:\\8_Boot_0114.ldr","rb"))==NULL)
	{
		printf("can't open this file!\n");
	    printf("*****Programming progmar sn4 error !*****\n");	
        exit(0);
	}
	fseek(fp,0,2);
	size=ftell(fp);
	fseek(fp,0,0);
	printf("Erasing Flash *****\n");
	wr_dat64(PROG_OFFSET_REG,0x80+4);
    Flash_Clear_Block_LockBits();

	size=size*4/128/1024+1;
	for(i=0;i<size;i++)
	{
	   Flash_Block_Erase(i); 
	}
	
    printf("Erasing completely *****\n");
    	
	FlashPrograming(fp);
	fclose(fp);
	printf("*****Programming progmar sn4 is successful!*****\n");	
////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	printf("Start program sn5 \n");
	if((fp=fopen("E:\\8_Boot_0114.ldr","rb"))==NULL)
	{
		printf("can't open this file!\n");
	    printf("*****Programming progmar sn5 error !*****\n");	
        exit(0);
	}
	fseek(fp,0,2);
	size=ftell(fp);
	fseek(fp,0,0);
	printf("Erasing Flash *****\n");
	wr_dat64(PROG_OFFSET_REG,0x80+5);
 //   Flash_Clear_Block_LockBits();

	size=size*4/128/1024+1;
	for(i=0;i<size;i++)
	{
	   Flash_Block_Erase(i); 
	}
	
    printf("Erasing completely *****\n");
    	
	FlashPrograming(fp);
	fclose(fp);
	printf("*****Programming progmar sn5 is successful!*****\n");	
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Start program sn6 \n");
	if((fp=fopen("E:\\8_Boot_0114.ldr","rb"))==NULL)
	{
		printf("can't open this file!\n");
	    printf("*****Programming progmar sn6 error !*****\n");	
        exit(0);
	}
	fseek(fp,0,2);
	size=ftell(fp);
	fseek(fp,0,0);
	printf("Erasing Flash *****\n");
	wr_dat64(PROG_OFFSET_REG,0x80+6);
 //   Flash_Clear_Block_LockBits();

	size=size*4/128/1024+1;
	for(i=0;i<size;i++)
	{
	   Flash_Block_Erase(i); 
	}
	
    printf("Erasing completely *****\n");
    	
	FlashPrograming(fp);
	fclose(fp);
	printf("*****Programming progmar sn6 is successful!*****\n");	
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Start program sn7 \n");
	if((fp=fopen("E:\\8_Boot_0114.ldr","rb"))==NULL)
	{
		printf("can't open this file!\n");
	    printf("*****Programming progmar sn7 error !*****\n");	
        exit(0);
	}
	fseek(fp,0,2);
	size=ftell(fp);
	fseek(fp,0,0);
	printf("Erasing Flash *****\n");
	wr_dat64(PROG_OFFSET_REG,0x80+7);
 //   Flash_Clear_Block_LockBits();

	size=size*4/128/1024+1;
	for(i=0;i<size;i++)
	{
	   Flash_Block_Erase(i); 
	}
	
    printf("Erasing completely *****\n");
    	
	FlashPrograming(fp);
	fclose(fp);
	printf("*****Programming progmar sn7 is successful!*****\n");	
////////////////////////////////////////////////////////////////////////////////////////////////////////////


			
}

bool SysInitial()
{
	int temp;
	
	__builtin_sysreg_write(__SYSCON, SYSCON_MP_WID64 |
									SYSCON_MEM_WID64 | 
									SYSCON_MSH_PIPE2 | 
									SYSCON_MSH_WT0 | 
									SYSCON_MSH_IDLE |
									SYSCON_MS1_PIPE2 |
									SYSCON_MS1_IDLE |
									SYSCON_MS0_SLOW | 
									SYSCON_MS0_WT1 | 
									SYSCON_MS0_IDLE 
		);	
    __builtin_sysreg_write(__SDRCON,0x000059a3); 

	temp = __builtin_sysreg_read(__SQCTL) & ~0x00000004;
	__builtin_sysreg_write(__SQCTL,temp);
		
	interrupt(SIGDMA0, dma_int_0);	    	// Assign isr to DMA channel 0

	temp = __builtin_sysreg_read(__IMASKL);	// Set the DMA0 interrupt enable bit
	temp |= INT_DMA0;
	__builtin_sysreg_write(__IMASKL, temp);
	
	//enable global interrupt
	temp = __builtin_sysreg_read(__SQCTL) | 0x00000004;
	__builtin_sysreg_write(__SQCTL,temp);
	return TRUE;
}
void GetManuID()
{
//	WriteFlash(0,0xB0 );
//	asm("nop;;");
	
	WriteFlash(0,0x90 );
	asm("nop;;");
	ReadFlash( 0, &AFP_ManCode );
	asm("nop;;");
	AFP_ManCode &= 0x00FF;

	printf("Manufacturer ID is %x\n",AFP_ManCode);
	
	switch(AFP_ManCode)
	{
		case 0x89:
			{
			printf("FLASH Manufacturer is 28F128J3A/28F640J3A/28F320J3A\n");
			ReadFlash(2, &AFP_DevCode );
			asm("nop;;");
			AFP_DevCode &= 0x00FF;
			printf("Device Code is %x\n",AFP_DevCode);
			switch(AFP_DevCode)
			{			
				
				case 0x16:
					printf("Device is 32Mbit\n");
					break;
				case 0x17:
					printf("Device is 64Mbit\n");
					break;
				case 0x18:
					printf("Device is 128Mbit\n");
					break;
				default:
					printf("Unknown Device Code !\n");
					break;			
			}
			break;
			}
		default:
			{	
			printf("Manufacturer_Code is %x\n",AFP_ManCode);
			printf("Unknown FLASH Manufacturer Code !\n");	
			}
			exit(0);
			break;
	}
	
}

//////////////////////////////////////////////////////////////

bool WriteData( long lStart, long lCount, long lStride, int *pnData )
{
	long i = 0,j=0,ppData;				// loop counter
	long lOffset = lStart;	// current offset to write

	for (i = 0; (i < lCount) && (i < BUFFER_SIZE); i++, lOffset += lStride)
	{
		for(j=0;j<4;j++)
		{
			// unlock the flash, do the write, and wait for completion
			WriteFlash(0,0x40);
			asm("nop;;");
			ppData= pnData[i]>>(j*8);
			WriteFlash( lOffset+j, ppData);
			asm("nop;;");
			Flash_Read_Status_Register();
		}
	

	}

	// ok 
	return TRUE;
}
bool ReadData( long lStart, long lCount, long lStride, int *pnData )
{
	long i = 0,j=0;				// loop counter
	long lOffset = lStart,ppData;	// current offset to write
	for (i = 0; (i < lCount) && (i < BUFFER_SIZE); i++, lOffset += lStride)
	{	
		for(j=0;j<4;j++)
		{
			
			ReadFlash( lOffset+j, &ppData);
			asm("nop;;");
			pnData[i]|= (ppData&0xff) <<(j*8);
		}
	
	}
	return TRUE;
}
bool Flash_WordByte_Program(int offset,int data)
{

	WriteFlash(0,0x40);
	asm("nop;;");
	WriteFlash(offset,data);
	asm("nop;;");
	Flash_Read_Status_Register();
//	WaitComplete();
/*

	if(AFP_Status_reg&0x1a)
	{
		//SR.4=1 or SR.3=1 or SR.1=1
		printf("(1) Error in program or block lock-bit !\n");
		printf("(2) Vpp low detect, operation abort !\n");
		printf("(3) Block Lock-Bit and/or WP# lock detected, operation abort !\n");
		Flash_Clear_Status_Register();
	}*/
	return TRUE;
}
	
// Write a value to an offset in flash.
//
// Inputs:	long lOffset - offset to write to
//			int nValue - value to write
//
//////////////////////////////////////////////////////////////

bool WriteFlash( long lOffset, int nValue )
{

	asm("[j31 + _temp_stor] = j5;;");	// put the data into _temp_stor

	asm("xr0 = _temp_stor;;");			// xr0 = source index
	asm("xr1 = 0x00010001;;");			// count = 1, modify = 1
	asm("xr2 = 0x0;;");					// not used
	asm("xr3 = 0x43000000;;");			// int mem,prio=norm,2D=no,word=norm,int=yes,RQ=dsbl,chain=no
	asm("xr4 = j4;;");					// xr4 = destination index
	asm("xr5 = 0x00010001;;");			// count = 1, modify = 1
	asm("xr6 = 0x0;;");					// not used
	asm("xr7 = 0xc3000000;;");			// boot rom,prio=norm,2D=no,word=norm,int=yes,RQ=dsbl,chain=no


	do_dma();							// do dma transfer

	// ok
	return TRUE;
}


bool ReadFlash( long lOffset, int *pnValue )
{
	// temp holder
	int nValue = 0x0;

	asm("xr0 = j4;;");					// xr0 = source index, j4 = nOffset
	asm("xr1 = 0x00010001;;");			// count = 1, modify = 1
	asm("xr2 = 0x0;;");					// not used
	asm("xr3 = 0xc3000000;;");			// boot rom,prio=norm,2D=no,word=norm,int=yes,RQ=dsbl,chain=no
	asm("xr4 = _temp_stor;;");			// xr4 = destination index
	asm("xr5 = 0x00010001;;");			// count = 1, modify = 1
	asm("xr6 = 0x0;;");					// not used
	asm("xr7 = 0x43000000;;");			// int mem,prio=norm,2D=no,word=norm,int=yes,RQ=dsbl,chain=no

	do_dma();

	// put the value at the location passed in
	*pnValue = temp_stor;

	// ok
	return TRUE;
}

 //////////////////////////////////////////////////////////////
// void do_dma()
//
// sets up our DMA registers and does the DMA
//
//////////////////////////////////////////////////////////////
void do_dma()
{
	asm("DCS0 = xr3:0;;");		// setup DMA sourse
	asm("DCD0 = xr7:4;;");		// setup DMA destination
	asm("nop;;");
	asm("nop;;");
	asm("nop;;");
	asm("nop;;");
	asm("idle;;");				// wait for dma to finish
}	
bool Flash_Read_Array()
{		
		WriteFlash( 0, 0xff );
		asm("nop;;");
		return TRUE;
}

bool Flash_Read_Status_Register()
{
	WriteFlash(0,0x70 );
	asm("nop;;");
	do {

		ReadFlash( 0, &AFP_Status_reg );
		AFP_Status_reg&=0x80;
		
	} while(!AFP_Status_reg);
	//SR.7=1

	ReadFlash( 0, &AFP_Status_reg );
	asm("nop;;");
	AFP_Status_reg&=0xff;

	return TRUE;
}
void Flash_Clear_Status_Register()
{
	WriteFlash( 0, 0x50 );
}


 void dma_int_0()
 {
 		asm("nop;;");
 //	printf("DMA0 is finished once\n");
 }	
 

bool Flash_Clear_Block_LockBits()
{


	WriteFlash( 0,0x60);
	asm("nop;;");
	WriteFlash( 0, 0xD0 );
	asm("nop;;");
	printf("FLASH clear Block Lock-Bits ...\n");
	if(Flash_Read_Status_Register()&0x20)
	{
		//SR.5=1;
		printf("(1) Error in block erasure or clear lock-bits !\n");
		Flash_Clear_Status_Register();
		return FALSE;
	}
	else
	{
		//SR.5=0;
		printf("Successful block erase or clear lock-bits\n");			
		return TRUE;
	}
}
bool Flash_Block_Erase(int Blk_No)
{
	int lOffset;
	lOffset=Blk_No*BLOCK_SIZE;
	WriteFlash( lOffset,0x20);
	asm("nop;;");
	WriteFlash( lOffset, 0xD0 );
	asm("nop;;");
	Flash_Read_Status_Register();
	return true;
}

	
void FlashPrograming(FILE *fp)
{
    int i,j,k=0,temp=0,Prog_Sn;	
    printf("writing Flash *****\n");
	start_addr=0;
	counter=0;

  for(;;)
    {	
        for(i=0;i<BUFFER_SIZE;i++)
     	AFP_Buffer[i]=0;

		temp=fread((&AFP_Buffer),1,BUFFER_SIZE,fp);
		if(ferror(fp))
		{
		  printf("read file error !\n");
		  exit(0);	
		}
		
	
		AFP_Count=BUFFER_SIZE;
		AFP_Stride=4;	
		if(temp==0)
		{
//		  printf("read %d word !\n",temp);	
//		  WriteData(start_addr, AFP_Count, AFP_Stride, AFP_Buffer );
//		  start_addr+=AFP_Count;
		  printf("writing Flash completely*****\n");
//		  printf("verifying Flash *****\n");
		  
		  break;
		}
		WriteData(start_addr*4, AFP_Count, AFP_Stride, AFP_Buffer );
//		printf("verifying Flash *****\n");
	  	Flash_Read_Array();
	//		ReadData(0, AFP_Count, AFP_Stride, data );

		for(i=0;i<BUFFER_SIZE;i++)
		{
			ReadFlash(4*(i+start_addr),&k);
			asm("nop;;");
			if((k)!=AFP_Buffer[i])
			printf("%x,%x,%x\n",i,AFP_Buffer[i],k);
		}
		start_addr+=BUFFER_SIZE;
		counter++;
	    printf("%dKB completed*****\n",counter*64);
    }		
    
}
