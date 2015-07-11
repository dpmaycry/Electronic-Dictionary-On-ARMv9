/*********************************************************************************************
* File：	main.c
* Author:	embest
* Desc：	LCD_Test
* History:	
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
#include "sys_init.h"
#include "8led_test.h"
#include "tsp_test.h"
/*------------------------------------------------------------------------------------------*/
/*                                     function declare                                     */
/*------------------------------------------------------------------------------------------*/
extern void color_lcd_test(void);
extern void keypad_test(void);
extern void timer_test(void);
UINT32T n=0,m=100;
UINT8T t=4,k=0,flag=0,z=5,add=0,mark=0;
char C[10][20]={"apple;n,苹果","boy;n,男孩","box;n,盒子","beautiful;adj,美丽的","psv;n,索尼掌上游戏机"};

/*********************************************************************************************
* name:		main
* func:		c code entry
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
int main()
{	
sys_init();	
color_lcd_test();
timer_test();
    while(1)
	{
	        tsp_test();
		keypad_test();
	}
 
}

