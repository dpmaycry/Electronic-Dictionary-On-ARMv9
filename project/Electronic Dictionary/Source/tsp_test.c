/*********************************************************************************************
* File:		tsp_test.c
* Author:	embest
* Desc:		TSP_Test
* History:	
*********************************************************************************************/

/*------------------------------------------------------------------------------------------*/
/*									include files											*/
/*------------------------------------------------------------------------------------------*/
#include "2410lib.h"
#include "tsp_test.h"

/*------------------------------------------------------------------------------------------*/
/*                                     constants define	                                    */
/*------------------------------------------------------------------------------------------*/
#define ADCPRS 39
int nSampleNo=5;
UINT32T g_nPosX,g_nPosY;
int g_nKeyPress;
extern void color_lcd_printf(UINT32T x,UINT32T y);
/*********************************************************************************************
* name:		tsp_int
* func:		Touch screen interrupt handler (ADC)
* para:		none
* ret:			none
* modify:	
* comment:		
*********************************************************************************************/
void __irq tsp_int(void)
{
    int i;
	UINT32T szPos[40];		
	rINTSUBMSK  |=  (BIT_SUB_ADC | BIT_SUB_TC);// Mask sub interrupt (ADC and TC)
	
    // TC(Touch screen Control) Interrupt	
	if( rADCTSC & 0x100)
	{
		rADCTSC&=0xff;							// Set stylus down interrupt
	}
	else
	{
		uart_printf(" Stylus Down!!\n");
		szPos[30] = g_nPosX;
		szPos[34] = g_nPosY;
				
		// <X-Position Read>
		//Hi-Z,AIN5,GND,Ext vlt,Pullup Dis,Normal,X-position
		rADCTSC = (0<<8)|(0<<7)|(1<<6)|(1<<5)|(0<<4)|(0<<3)|(0<<2)|(1);
		//adc input ain5
		rADCCON = (1<<14)|(39<<6)|(5<<3)|(0<<2)|(1<<1)|(0);
		rADCDAT0;
		delay(10);
		for(i = 0,g_nPosX=0; i<nSampleNo; i++)
		{
			while(!(0x8000 & rADCCON));										// Check ECFLG
			szPos[i] = (0x3ff & rADCDAT0);
			g_nPosX += szPos[i];
		}
		g_nPosX = g_nPosX/nSampleNo;
//		g_nPosX = (g_nPosX-45)*640/685;
		uart_printf(" X-Posion[AIN5] is %04d\n", g_nPosX);

		// <Y-Position Read>
		//GND,Ext vlt,Hi-Z,AIN7,Pullup Dis,Normal,Y-position
		rADCTSC = (0<<8)|(1<<7)|(0<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(2);
		//adc input ain7
		rADCCON = (1<<14)|(39<<6)|(7<<3)|(0<<2)|(1<<1)|(0);
		rADCDAT1;
		delay(10);		
		for(i = 0,g_nPosY=0; i<nSampleNo; i++)
		{
			while(!(0x8000 & rADCCON));										// Check ECFLG
			szPos[i] = (0x3ff & rADCDAT1);
			g_nPosY += szPos[i];
			
		}
		g_nPosY = g_nPosY/nSampleNo;
  //  	g_nPosY = (g_nPosY-70)*480/340;
		uart_printf(" Y-Posion[AIN7] is %04d\n", g_nPosY);
		


        color_lcd_printf(15+800-g_nPosX*4/5, 25+480-g_nPosY/2);

		//GND,AIN,Hi-z,AIN,Pullup En,Normal,Waiting mode
    	rADCTSC=(1<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3);
	}
#ifdef BOARDTEST
	sprintf(&szPos, "(X1,Y1):(%d,%d)",szPos[30],szPos[34]);
	print_lcd(195,170,0x20,&szPos);
	sprintf(&szPos, "(X2,Y2):(%d,%d)",g_nPosX,g_nPosY);
	print_lcd(195,178,0x1c,&szPos);
#endif		
	rSUBSRCPND |= BIT_SUB_TC;		
	rINTSUBMSK = ~(BIT_SUB_TC);												// Unmask sub interrupt (TC)     
	ClearPending(BIT_ADC);
}
			
/*********************************************************************************************
* name:		tsp_test
* func:		
* para:		none
* ret:		   none
* modify:	
* comment:		
*********************************************************************************************/
void tsp_test(void)
{
  	uart_printf(" Touch Screen Test Example.\n");

  	rADCDLY = 50000;												// ADC Start or Interval Delay
 	rGPGCON |= 0xFF000000;
	rGPGUP = 0xFFFF;

	rADCTSC = (0<<8) | (1<<7) | (1<<6) | (0<<5) | (1<<4) | (0<<3) | (1<<2) | (0);
	//auto sequential x/y position conversion,no operation,	XP pull-up
	
    rADCCON  =  (1<<14) | (ADCPRS<<6) | (5<<3) | (0<<2) | (0<<1) | (0);
	// Enable Prescaler,Prescaler,AIN7/5 fix,Normal,Disable read start,No operation	
	
	rADCTSC = (0<<8) | (1<<7) | (1<<6) | (0<<5) | (1<<4) | (0<<3) | (0<<2) | (3);
	//YM:GND,YP:AIN5,XM:Hi-z,XP:external voltage,XP pullup En,AUTO sequential,Waiting for interrupt mode
	delay(100);
	pISR_ADC = (UINT32T)tsp_int;							
	rINTMSK &= ~(BIT_ADC);
	rINTSUBMSK = ~(BIT_SUB_TC);

		if(uart_getkey()) return;									// or press any key to exit
		
	rINTSUBMSK |= BIT_SUB_TC;
	rINTMSK |= BIT_ADC;
	uart_printf(" end.\n");
}
