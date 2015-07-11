/*********************************************************************************************
* File£º	timer_test.c
* Author:	embest
* Desc£º	Timer_Test
* History:	
*			
*********************************************************************************************/
/*------------------------------------------------------------------------------------------*/
/*                                     include files	                                    */
/*------------------------------------------------------------------------------------------*/
#include "2410lib.h"
#include <string.h>
#include "lcdlib.h"
#include "glib.h"
#include "lcd.h"
#include <math.h>
/*------------------------------------------------------------------------------------------*/
/*               global varibles and interrupte service routine declare                     */
/*------------------------------------------------------------------------------------------*/
void __irq watchdog_int(void);
static volatile INT8T f_ucSecondNo;
extern UINT8T k,j,i,flag,keychar;
/*------------------------------------------------------------------------------------------*/
/*                                     function code	                                    */
/*------------------------------------------------------------------------------------------*/
/*********************************************************************************************
* name:		watchdog_test
* func:		watchdog timer test function
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void timer_test(void)
{
	ClearPending(BIT_WDT);							// clear interrupt pending bit
	pISR_WDT = (unsigned)watchdog_int;				// Initialize WDT interrupt handler entry
	rWTCON = ((PCLK/1000000-1)<<8)|(3<<3)|(1<<2);	//(0<<5)|	// 1M,1/128, enable interrupt
	rWTDAT = 7812;									// 1M/128=7812
	rWTCNT = 7812;
//	rWTCON |=(1<<5);
	
	rINTMOD &= ~(BIT_WDT);
	rINTMSK &= ~(BIT_WDT);

	rINTMSK |= BIT_WDT;								// mask watchdog timer interrupt
	
}

/*********************************************************************************************
* name:		watchdog_int
* func:		watchdog interrupt service routine
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void __irq watchdog_int(void)
{   
	ClearPending(BIT_WDT);
	if (keychar=='a')
	{if (k==1)
	 keychar='a';
	 if (k==2)
	 keychar='b';
	 if (k>=3)
	 keychar='c'; 
	}
	if (keychar=='d')
	{if (k==1)
	 keychar='d';
	 if (k==2)
	 keychar='e';
	 if (k>=3)
	 keychar='f'; 
	}
	if (keychar=='g')
	{if (k==1)
	 keychar='g';
	 if (k==2)
	 keychar='h';
	 if (k>=3)
	 keychar='i'; 
	}
	if (keychar=='j')
	{if (k==1)
	 keychar='j';
	 if (k==2)
	 keychar='k';
	 if (k>=3)
	 keychar='l'; 
	}
	if (keychar=='m')
	{if (k==1)
	 keychar='m';
	 if (k==2)
	 keychar='n';
	 if (k>=3)
	 keychar='o'; 
	}
	if (keychar=='p')
	{if (k==1)
	 keychar='p';
	 if (k==2)
	 keychar='q';
	 if (k==3)
	 keychar='r';
	 if(k>=4)
	 keychar='s'; 
	}
	if (keychar=='t')
	{if (k==1)
	 keychar='t';
	 if (k==2)
	 keychar='u';
	 if (k>=3)
	 keychar='v'; 
	}
	if (keychar=='w')
	{if (k==1)
	 keychar='w';
	 if (k==2)
	 keychar='x';
	 if (k==3)
	 keychar='y';
	 if (k>=4)
	 keychar='z'; 
	}
	k=0;
    flag=1; 

	rINTMSK |= BIT_WDT;
	rWTCON &=~(1<<5);
}
