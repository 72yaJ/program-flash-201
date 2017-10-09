#include "LION_TS201_Lib.h"

//**************************************************
// global variablesb
TCB link0_T_TCB;
TCB link1_T_TCB;
TCB link2_T_TCB;
TCB link3_T_TCB;
TCB link0_R_TCB;
TCB link1_R_TCB;
TCB link2_R_TCB;
TCB link3_R_TCB;

TCB dma1_source;
TCB dma1_destination;

__builtin_quad q;

//**************************************************
// system
void Enable_cache(void)
{
	asm("cache_enable(750);");	
}

void System_Init(void)
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
	//---------------------- Enable Interrupts ------------------------------
    temp = __builtin_sysreg_read(__SQCTL);
    temp = temp | SQCTL_GIE;
    __builtin_sysreg_write(__SQCTL, temp);
}

//**************************************************
// link
void LinkPort_Init(int L0_speed,int L1_speed,int L2_speed,int L3_speed,int delay_mscount)
{                                  
	
	__builtin_sysreg_write(__LTCTL0,  ~LTCTL_TEN);	// disable an LP Tx channel before writing an active value to LTCTLx
	__builtin_sysreg_write(__LTCTL1,  ~LTCTL_TEN);	// disable an LP Tx channel before writing an active value to LTCTLx
	__builtin_sysreg_write(__LTCTL2,  ~LTCTL_TEN);	// disable an LP Tx channel before writing an active value to LTCTLx
	__builtin_sysreg_write(__LTCTL3,  ~LTCTL_TEN);	// disable an LP Tx channel before writing an active value to LTCTLx
	
	__builtin_sysreg_write(__LRCTL0,  ~LRCTL_REN);	// disable an LP Rx channel before writing an active value to LRCTLx
	__builtin_sysreg_write(__LRCTL1,  ~LRCTL_REN);	// disable an LP Rx channel before writing an active value to LRCTLx
	__builtin_sysreg_write(__LRCTL2,  ~LRCTL_REN);	// disable an LP Rx channel before writing an active value to LRCTLx
	__builtin_sysreg_write(__LRCTL3,  ~LRCTL_REN);	// disable an LP Rx channel before writing an active value to LRCTLx
	
	
	Delay_ms(delay_mscount);	//delay for all DSP is copmlete boot and run here;
	
	__builtin_sysreg_write(__LTCTL0,  LTCTL_TEN
		//| LTCTL_TVERE
		| LTCTL_TDSIZE
		| L0_speed//LTCTL_TCLKDIV1
		);			// setup LP0 TX channel
	
	__builtin_sysreg_write(__LTCTL1,  LTCTL_TEN
		//| LTCTL_TVERE
		| LTCTL_TDSIZE
		| L1_speed//LTCTL_TCLKDIV1P5
		);			// setup LP0 TX channel
	
	__builtin_sysreg_write(__LTCTL2,  LTCTL_TEN
		//| LTCTL_TVERE
		| LTCTL_TDSIZE
		| L2_speed//LTCTL_TCLKDIV1
		);			// setup LP0 TX channel
	
	__builtin_sysreg_write(__LTCTL3,  LTCTL_TEN
		//| LTCTL_TVERE
		| LTCTL_TDSIZE
		| L3_speed//LTCTL_TCLKDIV1
		);			// setup LP0 TX channel    
	
	Delay_ms(delay_mscount);	//delay for all DSP is copmlete boot and run here;
	
	__builtin_sysreg_write(__LRCTL0,  LRCTL_REN
		//| LRCTL_RVERE
		| LRCTL_RDSIZE
		);    
	
	__builtin_sysreg_write(__LRCTL1,  LRCTL_REN
		//| LRCTL_RVERE
		| LRCTL_RDSIZE
		);    
	
	__builtin_sysreg_write(__LRCTL2,  LRCTL_REN
		//| LRCTL_RVERE
		| LRCTL_RDSIZE
		);    
	
	__builtin_sysreg_write(__LRCTL3,  LRCTL_REN
		//| LRCTL_RVERE
		| LRCTL_RDSIZE
		);                                                        
}

void link0_T(int * TX_BaseAdd,int TX_Length)
{
    link0_T_TCB.DI = TX_BaseAdd;			
    link0_T_TCB.DX = 4 | (TX_Length << 16); 
    link0_T_TCB.DY = 0;				
    link0_T_TCB.DP =  TCB_INTMEM | TCB_QUAD | TCB_INT ;
	
	q = __builtin_compose_128((long long)link0_T_TCB.DI | (long long)link0_T_TCB.DX << 32, (long long)(link0_T_TCB.DY | (long long)link0_T_TCB.DP << 32));
	__builtin_sysreg_write4(__DC4, q);	
}

void link1_T(int * TX_BaseAdd,int TX_Length)
{
    link1_T_TCB.DI = TX_BaseAdd;
    link1_T_TCB.DX = 4 | (TX_Length << 16);
    link1_T_TCB.DY = 0;		
    link1_T_TCB.DP =  TCB_INTMEM | TCB_QUAD | TCB_INT ;	
    
	q = __builtin_compose_128((long long)link1_T_TCB.DI | (long long)link1_T_TCB.DX << 32, (long long)(link1_T_TCB.DY | (long long)link1_T_TCB.DP << 32));
	__builtin_sysreg_write4(__DC5, q);		  	
}

void link2_T(int * TX_BaseAdd,int TX_Length)
{
    link2_T_TCB.DI = TX_BaseAdd;	
    link2_T_TCB.DX = 4 | (TX_Length << 16);  
    link2_T_TCB.DY = 0;					
    link2_T_TCB.DP =  TCB_INTMEM | TCB_QUAD | TCB_INT ;	
    
	q = __builtin_compose_128((long long)link2_T_TCB.DI | (long long)link2_T_TCB.DX << 32, (long long)(link2_T_TCB.DY | (long long)link2_T_TCB.DP << 32));
	__builtin_sysreg_write4(__DC6, q);			    	
}

void link3_T(int * TX_BaseAdd,int TX_Length)
{
    link3_T_TCB.DI = TX_BaseAdd;		
    link3_T_TCB.DX = 4 | (TX_Length << 16); 
    link3_T_TCB.DY = 0;			
    link3_T_TCB.DP =  TCB_INTMEM | TCB_QUAD | TCB_INT ;	
    
	q = __builtin_compose_128((long long)link3_T_TCB.DI | (long long)link3_T_TCB.DX << 32, (long long)(link3_T_TCB.DY | (long long)link3_T_TCB.DP << 32));
	__builtin_sysreg_write4(__DC7, q);		     	
}

void link0_R(int * RX_BaseAdd,int RX_Length)
{
    link0_R_TCB.DI = RX_BaseAdd;		
    link0_R_TCB.DX = 4 | (RX_Length << 16); 
    link0_R_TCB.DY = 0;					
    link0_R_TCB.DP =  TCB_INTMEM | TCB_QUAD | TCB_INT;
	
	q = __builtin_compose_128((long long)link0_R_TCB.DI | (long long)link0_R_TCB.DX << 32, (long long)(link0_R_TCB.DY | (long long)link0_R_TCB.DP << 32));
    __builtin_sysreg_write4(__DC8, q);	
}

void link1_R(int * RX_BaseAdd,int RX_Length)
{        
    link1_R_TCB.DI = RX_BaseAdd;		
    link1_R_TCB.DX = 4 | (RX_Length << 16);  
    link1_R_TCB.DY = 0;						
    link1_R_TCB.DP =  TCB_INTMEM | TCB_QUAD | TCB_INT ;	
    
   	q = __builtin_compose_128((long long)link1_R_TCB.DI | (long long)link1_R_TCB.DX << 32, (long long)(link1_R_TCB.DY | (long long)link1_R_TCB.DP << 32));
    __builtin_sysreg_write4(__DC9, q);    
}

void link2_R(int * RX_BaseAdd,int RX_Length)
{        
    link2_R_TCB.DI = RX_BaseAdd;	
    link2_R_TCB.DX = 4 | (RX_Length << 16);  
    link2_R_TCB.DY = 0;					
    link2_R_TCB.DP =  TCB_INTMEM | TCB_QUAD | TCB_INT ;
	
   	q = __builtin_compose_128((long long)link2_R_TCB.DI | (long long)link2_R_TCB.DX << 32, (long long)(link2_R_TCB.DY | (long long)link2_R_TCB.DP << 32));
    __builtin_sysreg_write4(__DC10, q);    
}

void link3_R(int * RX_BaseAdd,int RX_Length)
{        
    link3_R_TCB.DI = RX_BaseAdd;
    link3_R_TCB.DX = 4 | (RX_Length << 16);
    link3_R_TCB.DY = 0;
    link3_R_TCB.DP =  TCB_INTMEM | TCB_QUAD | TCB_INT ;
    
   	q = __builtin_compose_128((long long)link3_R_TCB.DI | (long long)link3_R_TCB.DX << 32, (long long)(link3_R_TCB.DY | (long long)link3_R_TCB.DP << 32));
    __builtin_sysreg_write4(__DC11, q);    
}  

void link0_T_fe(int * TX_BaseAdd,int TX_Length)
{
    link0_T_TCB.DI = TX_BaseAdd;
    link0_T_TCB.DX = 4 | (TX_Length << 16);
    link0_T_TCB.DY = 0;	
    link0_T_TCB.DP =  TCB_EXTMEM | TCB_QUAD | TCB_INT ;	
    
	q = __builtin_compose_128((long long)link0_T_TCB.DI | (long long)link0_T_TCB.DX << 32, (long long)(link0_T_TCB.DY | (long long)link0_T_TCB.DP << 32));
	__builtin_sysreg_write4(__DC4, q);	
}

void link1_T_fe(int * TX_BaseAdd,int TX_Length)
{
    link1_T_TCB.DI = TX_BaseAdd;	
    link1_T_TCB.DX = 4 | (TX_Length << 16); 
    link1_T_TCB.DY = 0;					
    link1_T_TCB.DP =  TCB_EXTMEM | TCB_QUAD | TCB_INT ;	
	q = __builtin_compose_128((long long)link1_T_TCB.DI | (long long)link1_T_TCB.DX << 32, (long long)(link1_T_TCB.DY | (long long)link1_T_TCB.DP << 32));
	__builtin_sysreg_write4(__DC5, q);			     	
}

void link2_T_fe(int * TX_BaseAdd,int TX_Length)
{
    link2_T_TCB.DI = TX_BaseAdd;		
    link2_T_TCB.DX = 4 | (TX_Length << 16); 
    link2_T_TCB.DY = 0;				
    link2_T_TCB.DP =  TCB_EXTMEM | TCB_QUAD | TCB_INT ;	
	q = __builtin_compose_128((long long)link2_T_TCB.DI | (long long)link2_T_TCB.DX << 32, (long long)(link2_T_TCB.DY | (long long)link2_T_TCB.DP << 32));
	__builtin_sysreg_write4(__DC6, q);
}

void link3_T_fe(int * TX_BaseAdd,int TX_Length)
{
    link3_T_TCB.DI = TX_BaseAdd;		
    link3_T_TCB.DX = 4 | (TX_Length << 16); 
    link3_T_TCB.DY = 0;				
    link3_T_TCB.DP =  TCB_EXTMEM | TCB_QUAD | TCB_INT ;	
	q = __builtin_compose_128((long long)link3_T_TCB.DI | (long long)link3_T_TCB.DX << 32, (long long)(link3_T_TCB.DY | (long long)link3_T_TCB.DP << 32));
	__builtin_sysreg_write4(__DC7, q);
}

void link0_R_te(int * RX_BaseAdd,int RX_Length)
{
    link0_R_TCB.DI = RX_BaseAdd;	
    link0_R_TCB.DX = 4 | (RX_Length << 16); 
    link0_R_TCB.DY = 0;				
    link0_R_TCB.DP =  TCB_EXTMEM | TCB_QUAD | TCB_INT;
	
	q = __builtin_compose_128((long long)link0_R_TCB.DI | (long long)link0_R_TCB.DX << 32, (long long)(link0_R_TCB.DY | (long long)link0_R_TCB.DP << 32));
    __builtin_sysreg_write4(__DC8, q);	   
}

void link1_R_te(int * RX_BaseAdd,int RX_Length)
{        
    link1_R_TCB.DI = RX_BaseAdd;		
    link1_R_TCB.DX = 4 | (RX_Length << 16); 
    link1_R_TCB.DY = 0;					
    link1_R_TCB.DP =  TCB_EXTMEM | TCB_QUAD | TCB_INT ;	
    
   	q = __builtin_compose_128((long long)link1_R_TCB.DI | (long long)link1_R_TCB.DX << 32, (long long)(link1_R_TCB.DY | (long long)link1_R_TCB.DP << 32));
    __builtin_sysreg_write4(__DC9, q);    
}

void link2_R_te(int * RX_BaseAdd,int RX_Length)
{        
    link2_R_TCB.DI = RX_BaseAdd;		
    link2_R_TCB.DX = 4 | (RX_Length << 16); 
    link2_R_TCB.DY = 0;					
    link2_R_TCB.DP =  TCB_EXTMEM | TCB_QUAD | TCB_INT ;	
    
   	q = __builtin_compose_128((long long)link2_R_TCB.DI | (long long)link2_R_TCB.DX << 32, (long long)(link2_R_TCB.DY | (long long)link2_R_TCB.DP << 32));
    __builtin_sysreg_write4(__DC10, q);    
}

void link3_R_te(int * RX_BaseAdd,int RX_Length)
{        
    link3_R_TCB.DI = RX_BaseAdd;		
    link3_R_TCB.DX = 4 | (RX_Length << 16); 
    link3_R_TCB.DY = 0;					
    link3_R_TCB.DP =  TCB_EXTMEM | TCB_QUAD | TCB_INT ;	
    
   	q = __builtin_compose_128((long long)link3_R_TCB.DI | (long long)link3_R_TCB.DX << 32, (long long)(link3_R_TCB.DY | (long long)link3_R_TCB.DP << 32));
    __builtin_sysreg_write4(__DC11, q);    
}

//*** interrupt function **************************************************************
/*
void ISR_Init(void)
{
	//------------------------ Setup Interrupt ISR --------------------------
	//		interrupt(SIGDMA0, dma0_int);
	//		interrupt(SIGDMA1, dma1_int);	// Assign isr to DMA channel 1 (external port 0)
	//		interrupt(SIGDMA2, dma2_int);
	//		interrupt(SIGDMA3, dma3_int);
	//		interrupt(SIGDMA4, dma4_int);	// Assign isr to DMA channel 4 (Link port 0 transmit)
	//	    interrupt(SIGDMA5, dma5_int);	// Assign isr to DMA channel 5 (Link port 1 transmit)
	//    	interrupt(SIGDMA6, dma6_int);
	//		interrupt(SIGDMA7, dma7_int);
	//		interrupt(SIGDMA8, dma8_int);	// Assign isr to DMA channel 8 (Link port 0 receive)
	//		interrupt(SIGDMA9, dma9_int);	// Assign isr to DMA channel 9 (Link port 1 receive)
	//		interrupt(SIGDMA10, dma10_int); 
	//		interrupt(SIGDMA11, dma11_int);
	//    	interrupt(SIGIRQ0, irq0_int);
	//    	interrupt(SIGIRQ1, irq1_int);
	//    	interrupt(SIGIRQ2, irq2_int);
	//    	interrupt(SIGIRQ3, irq3_int);
	//    	interrupt(SIGTIMER0LP, timer0lp_int);
	//    	interrupt(SIGTIMER1LP, timer1lp_int);
	//    	interrupt(SIGTIMER0HP, timer0hp_int);
	//    	interrupt(SIGTIMER1HP, timer1hp_int);
}

void irq0_int()	
{
	//	dprintf("receive interrupt from irq0\n");
	return;	
}
void irq1_int()	
{
	//	dprintf("receive interrupt from irq1\n");
	return;	
}
void irq2_int()	
{
	//	dprintf("receive interrupt from irq2\n");
	return;	
}
void irq3_int()	
{
	//	dprintf("receive interrupt from irq3\n");
	return;	
}

void dma0_int()	// external port DMA ISR
{
	//	dprintf("transmit data from/to internal\n");	
	
    return;	
}
void dma1_int()	// external port DMA ISR
{
	//	dprintf("transmit data from/to internal\n");	
	
	return;	
}
void dma2_int()	// external port DMA ISR
{
	//	dprintf("transmit data from/to internal\n");	
	
	return;	
}
void dma3_int()	// external port DMA ISR
{
	//	dprintf("transmit data from/to internal\n");	
	
		  return;	
}

void dma4_int()	//linl0 T
{
	//	dprintf("transmit data to link0\n");
	
	return;	
}
void dma5_int()	//link1 T
{	
	//	dprintf("transmit data to link1\n");
	
				return;
}  
void dma6_int()	//link2 T
{		
				//	dprintf("transmit data to link2\n");
				
	return;
}
void dma7_int()	//link3 T
{	
	//	dprintf("transmit data to link3\n");
	
	return;
}

void dma8_int()	//link0 R
{
	//	dprintf("receive data from link0\n");	
	
	return;
}
void dma9_int()	//link1 R
{	
	//	dprintf("receive data from link1\n");	
	
	return;
}
void dma10_int()	//link2 R
{	
	//	dprintf("receive data from link2\n");	
	
	return;
}
void dma11_int()	//link3 R
{	
	
	//	dprintf("receive data from link3\n");	
	
	return;
}

void timer0lp_int()	
{
	//	dprintf("receive interrupt from timer0\n");
	return;	
}
void timer1lp_int()	
{
	//	dprintf("receive interrupt from timer1\n");
	return;	
}
void timer0hp_int()	
{
	//	dprintf("receive interrupt from timer0\n");
	return;	
}
void timer1hp_int()	
{
	//	dprintf("receive interrupt from timer1\n");
	return;	
}
*/

//**************************************************
// DMA
void dma1_i2e(long *iAddress,long *eAddress,int n)	// DMA from internal memory to external memory
{
	dma1_source.DI 	= (int *)iAddress;
    dma1_source.DX 	 = 4|(n<<16);     			 
    dma1_source.DY 	 = 0;							
    dma1_source.DP 	 = TCB_INTMEM| TCB_QUAD| TCB_INT ;
	
    dma1_destination.DI = (int *)eAddress;
    dma1_destination.DX = 4|(n<<16);
    dma1_destination.DY = 0;			
    dma1_destination.DP = TCB_EXTMEM| TCB_QUAD| TCB_INT;
    
    start_dma1(dma1_source,dma1_destination);                     
}

void dma1_e2i(long *eAddress,long *iAddress,int n)	// DMA from external memory to internal memory
{
	dma1_source.DI = (int *)eAddress;	
    dma1_source.DX 	 = 4|(n<<16);     			 
    dma1_source.DY 	 = 0;			
    dma1_source.DP 	 = TCB_EXTMEM| TCB_QUAD| TCB_INT ;	
	
    dma1_destination.DI = (int *)iAddress;		
    dma1_destination.DX = 4|(n<<16);     			 
    dma1_destination.DY = 0;								
    dma1_destination.DP = TCB_INTMEM| TCB_QUAD| TCB_INT;	
	
    start_dma1(dma1_source,dma1_destination);
}

inline void start_dma1(TCB Source,TCB Destin)
{	
    q = __builtin_compose_128(((long long)Source.DX << 32)| (long long)Source.DI, ((long long)Source.DP << 32) | (long long)Source.DY);
    __builtin_sysreg_write4(__DCS1, q); 
    q = __builtin_compose_128(((long long)Destin.DX<< 32) | (long long)Destin.DI, ((long long)Destin.DP << 32) | (long long)Destin.DY );
    __builtin_sysreg_write4(__DCD1, q); 
	
    return;
}

//**************************************************
// dsp timer
void runTime1(void)
{
	int temp;
	__builtin_sysreg_write(__TMRIN1H, 0x0000ffff);
	__builtin_sysreg_write(__TMRIN1L, 0xffffffff);
	
	temp = __builtin_sysreg_read(__INTCTL);
    temp = temp | INTCTL_TMR1RN;
    __builtin_sysreg_write(__INTCTL, temp);
}

void readTimeStart(unsigned long long *Stime)
{
	int th1,th2,tl1;
	
	th1 = __builtin_sysreg_read(__TIMER1H);
	tl1 = __builtin_sysreg_read(__TIMER1L);
	th2 = __builtin_sysreg_read(__TIMER1H);
	if (th1 != th2)
	{
		th1 = th2;
		tl1 = __builtin_sysreg_read(__TIMER1L);
	}
	
	*Stime = __builtin_compose_64u(tl1, th1);
}

void readTimeEnd(float *result,unsigned long long *Stime) //计算用时，单位微妙
{
	int th1,th2,tl1;
	
	th1 = __builtin_sysreg_read(__TIMER1H);
	tl1 = __builtin_sysreg_read(__TIMER1L);
	th2 = __builtin_sysreg_read(__TIMER1H);
	if (th1 != th2)
	{
		th1 = th2;
		tl1 = __builtin_sysreg_read(__TIMER1L);
	}
	
	*result = (float)(*Stime - __builtin_compose_64u(tl1, th1))/300.0;	
}

long long read_GCount(void)
{
	long long temp;
	rd_dat64(freg+0100,temp);
	return temp;	
}

float read_GCount_Time(long long Start_Count)
{
	long long temp;
	float result;
	rd_dat64(freg+0x100,temp);		
	result = (float)(temp - Start_Count) / 10;	//计算用时，单位毫妙
	return result;
}

//*** flag function **************************************************************
void set_flag_out(int n)	//n为flag的序号
{
	if(n<0 || n>3)
		return;

	int	temp = __builtin_sysreg_read(__FLAGREG);
	if(n==0x00)
		temp = temp | FLAGREG_FLAG0_EN;                         // Enable output...
	else if(n==0x01)
		temp = temp | FLAGREG_FLAG1_EN;                         // Enable output...
	else if(n==0x02)
		temp = temp | FLAGREG_FLAG2_EN;                         // Enable output...
	else if(n==0x03)
		temp = temp | FLAGREG_FLAG3_EN;                         // Enable output...
	__builtin_sysreg_write(__FLAGREG, temp);
}

void set_flag_in(int n)	//n为flag的序号
{
	if(n<0 || n>3)
		return;

	int temp = __builtin_sysreg_read(__FLAGREG);
	if(n==0x00)
		temp = temp & ~FLAGREG_FLAG0_EN;                     
	else if(n==0x01)
		temp = temp & ~FLAGREG_FLAG1_EN;                     
	else if(n==0x02)
		temp = temp & ~FLAGREG_FLAG2_EN;                     
	else if(n==0x03)
		temp = temp & ~FLAGREG_FLAG3_EN;                     
	__builtin_sysreg_write(__FLAGREG, temp);
}

void set_flag_H(int n)	//n为flag的序号
{
	if(n<0 || n>3)
		return;

	int temp = __builtin_sysreg_read(__FLAGREG);
	if(n==0x00)
		temp = temp | FLAGREG_FLAG0_OUT;                      
	else if(n==0x01)
		temp = temp | FLAGREG_FLAG1_OUT;                      
	else if(n==0x02)
		temp = temp | FLAGREG_FLAG2_OUT;                      
	else if(n==0x03)
		temp = temp | FLAGREG_FLAG3_OUT;                       
	__builtin_sysreg_write(__FLAGREG, temp);
}

void set_flag_L(int n)	//n为flag的序号
{
	if(n<0 || n>3)
		return;

	int temp = __builtin_sysreg_read(__FLAGREG);
	if(n==0x00)
		temp = temp & ~FLAGREG_FLAG0_OUT;                     
	else if(n==0x01)
		temp = temp & ~FLAGREG_FLAG1_OUT;                     
	else if(n==0x02)
		temp = temp & ~FLAGREG_FLAG2_OUT;                     
	else if(n==0x03)
		temp = temp & ~FLAGREG_FLAG3_OUT;
	__builtin_sysreg_write(__FLAGREG, temp);
}

void flash_led(int n,int delay_ms)
{			
	set_flag_L(n);
	Delay_ms(delay_ms);
	set_flag_H(n);
	Delay_ms(delay_ms);			
}

void Delay_us(int count_us)
{
    asmdelay(count_us*10-2);
}

void Delay_ms(int count_ms)
{
    asmdelay(count_ms*10000-2);
}

void Delay_s(int count_s)
{
    asmdelay(count_s*10000000);
}
