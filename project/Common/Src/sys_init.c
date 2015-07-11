/*********************************************************************************************
* File name	: sys_init.c
* Author	: embest
* Descript	: Embest s3c2410 system initialization. 
* History
*********************************************************************************************/

/*------------------------------------------------------------------------------------------*/
/*	 								include files						 				    */
/*------------------------------------------------------------------------------------------*/
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "2410lib.h"
/*------------------------------------------------------------------------------------------*/
/*	 								constant define 						 				*/
/*------------------------------------------------------------------------------------------*/
#define rCPLDBEEPADDR  (*(volatile unsigned char*)0x21180000) 			// Beep Address

/*------------------------------------------------------------------------------------------*/
/*	 								global variable						 				    */
/*------------------------------------------------------------------------------------------*/
int nCNT;								// control sign

/*------------------------------------------------------------------------------------------*/
/*	 								function declare					 				    */
/*------------------------------------------------------------------------------------------*/
// for debugging
void break_point(void);
void debug_undef(void);
void debug_swi(void);
void debug_abort(void);
void debug_fiq(void);
void print_infor(void);
void beep_init(int status);
/*********************************************************************************************
* name:		break_point, debug_xxxx
* func:		states the exception has occured.
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void break_point(void)
{
	uart_printf("!!!Enter break point.");	
}

/*********************************************************************************************
* name:		debug_undef
* func:		undef exception has occured.
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void debug_undef(void)
{
	uart_printf("!!!Enter UNDEFINED exception. %d\r\n", nCNT+=1);	
	break_point();
}

/*********************************************************************************************
* name:		debug_swi
* func:		swi exception has occured.
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void debug_swi(void)
{
	uart_printf("!!!Enter SWI exception. %d\r\n", nCNT+=1);	
	break_point();
}

/*********************************************************************************************
* name:		debug_abort_d
* func:		data abort exception has occured.
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void debug_abort_d(void)
{
	uart_printf("!!!Enter Data ABORT exception. %d\r\n", nCNT+=1);	
	break_point();
}

/*********************************************************************************************
* name:		debug_abort_p
* func:		instruction abort exception has occured.
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void debug_abort_p(void)
{
    uart_printf("!!!Enter Prefetch ABORT exception. %d\r\n", nCNT+=1);	
    break_point();
}

/*********************************************************************************************
* name:		debug_fiq
* func:		fiq exception has occured.
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void debug_fiq(void)
{
	uart_printf("!!!Enter FIQ exception. %d\r\n", nCNT+=1);	
	break_point();
}

/*********************************************************************************************
* name:		interrupts_init
* func:		Initialze interrupts and set interrupt vector routine
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void interrupts_init(void)
{
    rINTMOD     = 0x0;                     	// All=IRQ mode
    rSRCPND     = rSRCPND;                	// clear all interrupt
    rINTPND     = rINTPND;                	// clear all interrupt
    rINTMSK     = BIT_ALLMSK;             	// All interrupt is masked.
    rINTSUBMSK  = BIT_SUB_ALLMSK;          	// All sub-interrupt is masked.

	// External interrupt will be falling edge triggered. 
    rEXTINT0 = 0x22222222;    				// EINT[7:0]
    rEXTINT1 = 0x22222222;    				// EINT[15:8]
    rEXTINT2 = 0x22222222;    				// EINT[23:16]

	// pISR_RESET                           //reserved
	pISR_UNDEF     	= (unsigned) debug_undef;
	pISR_SWI       	= (unsigned) debug_swi;
	pISR_PABORT    	= (unsigned) debug_abort_d;
	pISR_DABORT    	= (unsigned) debug_abort_d;
	// pISR_RESERVED                      	//not used
 	// pISR_IRQ       	= (unsigned) 0;    	//reserved
	pISR_FIQ       	= (unsigned) debug_fiq;

	pISR_EINT0      = (unsigned) break_point;    
	pISR_EINT1     	= (unsigned) break_point;
	pISR_EINT2     	= (unsigned) break_point;
	pISR_EINT3     	= (unsigned) break_point;
	pISR_EINT4_7   	= (unsigned) break_point;
	pISR_EINT8_23  	= (unsigned) break_point;
	pISR_NOTUSED6  	= (unsigned) break_point;
	pISR_BAT_FLT   	= (unsigned) break_point;
	pISR_TICK      	= (unsigned) break_point;
	pISR_WDT       	= (unsigned) break_point;
	pISR_TIMER0    	= (unsigned) break_point;
	pISR_TIMER1    	= (unsigned) break_point;
	pISR_TIMER2    	= (unsigned) break_point;
	pISR_TIMER3    	= (unsigned) break_point;
	pISR_TIMER4    	= (unsigned) break_point;
	pISR_UART2     	= (unsigned) break_point;
	pISR_LCD       	= (unsigned) break_point;
	pISR_DMA0      	= (unsigned) break_point;
	pISR_DMA1      	= (unsigned) break_point;
	pISR_DMA2      	= (unsigned) break_point;
	pISR_DMA3      	= (unsigned) break_point;
	pISR_SDI       	= (unsigned) break_point;
	pISR_SPI0      	= (unsigned) break_point;
	pISR_UART1     	= (unsigned) break_point;
	pISR_NOTUSED24 	= (unsigned) break_point;
	pISR_USBD      	= (unsigned) break_point;
    pISR_USBH      	= (unsigned) break_point;
    pISR_IIC       	= (unsigned) break_point;
    pISR_UART0     	= (unsigned) break_point;
    pISR_SPI1      	= (unsigned) break_point;
    pISR_RTC       	= (unsigned) break_point;
    pISR_ADC       	= (unsigned) break_point;
    pISR_ADC       	= (unsigned) break_point;
                
}

/*********************************************************************************************
* name:		sys_init
* func:		Initilaize interrupt, port and UART
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void sys_init()
{
	change_clock_divider(1,1);					// 1:2:4
	change_value_MPLL(M_MDIV, M_PDIV, M_SDIV);	// Fin=12MHz FCLK=202.8MHz
	delay(0);									// adjust the delay count
	port_init();
	interrupts_init();
	uart_init(PCLK, 115200, UART0);
	uart_init(PCLK, 115200, UART1);
	uart_init(PCLK, 115200, UART2);
#ifdef CLK124_200M
	change_value_MPLL(88,1,1);					// Fin=12MHz FCLK=192MHz
	uart_init(192000000/4, 115200, UART0);
	uart_init(192000000/4, 115200, UART1);
#endif	
	uart_select(UART1);
	print_infor();
	beep_init(0);
	delay(500);
}


/*********************************************************************************************
* name:		print_infor
* func:		print the welcome information
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void print_infor(void)
{
	uart_printf("\n\n\n\n");
	uart_printf(" *******************************************************************\n");
	uart_printf(" **             英蓓特EduKit系列嵌入式教学系统平台                **\n");
	uart_printf(" **        Embest EduKit Series Embedded Teaching Platform        **\n");
	uart_printf(" *******************************************************************\n");
}

/*********************************************************************************************
* name:		beep_init
* func:		init the beep
* para:		status--none zero:beep 0:no beep
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void beep_init(int status)
{
	if (status)
		rCPLDBEEPADDR &= ~(1<<7);
	else
		rCPLDBEEPADDR |= (1<<7);
}
