/*********************************************************************************************
* File name	: 2410lib.h
* Author	: embest
* Descript	: S3C2410 Define Address Register 
* History
*********************************************************************************************/

/*------------------------------------------------------------------------------------------*/
/*	 								include files						 				    */
/*------------------------------------------------------------------------------------------*/
#include "def.h"
#include "memcfg.h"
#include "option.h"
#include "register.h"
#include "2410addr.h"

#ifndef __2410lib_h__
#define __2410lib_h__

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------------------*/
/*	 								constant define						 				    */
/*------------------------------------------------------------------------------------------*/
#define DOWNLOAD_ADDRESS _RAM_STARTADDRESS
#define DEBUG_OUT uart_printf

#define min(x1,x2) (((x1)<(x2))? (x1):(x2))
#define max(x1,x2) (((x1)>(x2))? (x1):(x2))

#define ONESEC0 (62500)								// 16us resolution, max 1.04 sec
#define ONESEC1 (31250)								// 32us resolution, max 2.09 sec
#define ONESEC2 (15625)								// 64us resolution, max 4.19 sec
#define ONESEC3 (7812)								// 128us resolution, max 8.38 sec
#define ONESEC4 (PCLK/128/(0xff+1))					// @60Mhz, 128*4us resolution, max 32.53 sec

#define NULL 0
#define LCD   0xDD									// LCD symbol
#define UART0 0x0									// UART symbol
#define UART1 0x1
#define UART2 0xBB
#define EnterPWDN(clkcon) ((void (*)(int))0x20)(clkcon)

/*------------------------------------------------------------------------------------------*/
/*	 								fuction declare						 				    */
/*------------------------------------------------------------------------------------------*/
void StartPointAfterPowerOffWakeUp(void);  
void sys_init(void);
void delay(int time);								// Watchdog Timer is used.
void port_init(void);
void uart_select(int ch);
void uart_txempty(int ch);
void uart_init(int nMainClk, int nBaud, int nChannel);
char uart_getch(void);
char uart_getkey(void);
int  uart_getintnum(void);
void uart_sendbyte(int data);
void uart_printf(char *fmt,...);
void uart_sendstring(char *pt);
void timer_start(int divider);						// Watchdog Timer is used.
int  timer_stop(void);								// Watchdog Timer is used.

void led_display(int data);

void change_value_MPLL(int m,int p,int s);
void change_clock_divider(int hdivn,int pdivn);
void ChangeUPllValue(int m,int p,int s);
void InitMMU(unsigned int *pTranslationTable);		

/*------------------------------------------------------------------------------------------*/
/*	 								inline fuction define								    */
/*------------------------------------------------------------------------------------------*/
/*********************************************************************************************
* name:		ARM_WriteTTB
* func:		Write Translation table base  register
* para:		TTB Address
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
__inline void ARM_WriteTTB(unsigned int ttb)
{
 	__asm("MCR p15, 0, (ttb & 0xFFFFC000), c2, c0, 0");
}

/*********************************************************************************************
* name:		ARM_WriteDomain
* func:		Write domain access control
* para:		Domain NO.
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
__inline void ARM_WriteDomain(unsigned int domain)
{
 	__asm("MCR p15, 0, domain, c3, c0, 0");
}

/*********************************************************************************************
* name:		ARM_ReadControl
* func:		Read Control register
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
__inline unsigned int ARM_ReadControl()
{
	register unsigned int ctl;
 	__asm("MRC p15, 0, ctl, c1, c0, 0");
 	return ctl;
}

/*********************************************************************************************
* name:		ARM_WriteControl
* func:		Write Control register
* para:		Control Byte
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
__inline void ARM_WriteControl(unsigned int ctl)
{
 	__asm("MCR p15, 0, ctl, c1, c0, 0");
}

#ifdef __cplusplus
}
#endif

#endif  //__2410lib_h__

