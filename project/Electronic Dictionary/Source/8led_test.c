/*********************************************************************************************
* File:		8led_test.c
* Author:	Embest	
* Desc:		8LED_Test
* History:	

*********************************************************************************************/

/*------------------------------------------------------------------------------------------*/
/*									include files											*/
/*------------------------------------------------------------------------------------------*/
#include "2410lib.h"
#include "8led_test.h"
#include "iic_8led.h"

/*------------------------------------------------------------------------------------------*/
/*	 								global variables										*/
/*------------------------------------------------------------------------------------------*/
extern UINT8T t;
unsigned char D[4]={0,4,1,3},E[4]={4,1,8,0},f_szDigital[10] ={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6};	// 0 ~ 9		
/*********************************************************************************************
* name:		led8_test()
* func:		test 8led
* para:		none
* ret:		none
* modify:
* comment:	write data to display buffer of ZLG24,then we can see figure scrolling on LED.	
*********************************************************************************************/
void led8_test(void)
{
	int j, k;
	
    uart_printf("\n 8-Segment Digit LED Test Example (Please look at LED)\n");
	iic_init_8led();
	iic_write_8led(0x70, 0x17, f_szDigital[2]);
	iic_write_8led(0x70, 0x16, f_szDigital[0]);
	iic_write_8led(0x70, 0x15, f_szDigital[1]);
	iic_write_8led(0x70, 0x14, f_szDigital[1]);
	iic_write_8led(0x70, 0x13, f_szDigital[1]);
	iic_write_8led(0x70, 0x12, f_szDigital[9]);
	
			    j=D[t];
				k=E[t];
				iic_write_8led(0x70, 0x11, f_szDigital[j]);		// write to DpRam0~DpRam7 of ZLG24
				iic_write_8led(0x70, 0x10, f_szDigital[k]);				   	

}
