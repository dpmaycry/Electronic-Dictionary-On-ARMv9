/*********************************************************************************************
* File：	keypad_test.c
* Author:	embest
* Desc：	Keypad_Test
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
/*------------------------------------------------------------------------------------------*/
/*                                     constant define	                                    */
/*------------------------------------------------------------------------------------------*/
#define KPLAddr  (*(volatile unsigned char*)0x21140000) 	// Keypad list io address
#define KPRAddr  (*(volatile unsigned char*)0x21080000) 	// Keypad row io address

/*------------------------------------------------------------------------------------------*/
/*                                     global variables	                                    */
/*------------------------------------------------------------------------------------------*/
UINT16T KeyNo ;                             				// Key No.
UINT8T KPRData;
extern UINT32T n,m;	
extern UINT8T t,k,flag,z,add,mark;
extern char C[10][20];
UINT8T keychar,p,q,f,g,h,v; 
char A[1],B[20];											// Read Data from the row of keypad
extern void Lcd_DspHz24(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T *s);
extern void Lcd_DspAscII8x16(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T * s);
extern void lcd_clr(void);
extern void timer_test(void);
/*********************************************************************************************
* name:		keyscan
* func:		scan the keypad
* para:		none
* ret:		1--one key have been pressed 0--no key was pressed
* modify:
* comment:		
*********************************************************************************************/
UINT8T keyscan(void)
{
	UINT8T i, j;

	// Scan the keypad 
	for(i = 0; i < 4; i++)
	{
		KPLAddr = ~(0x4 << i);							   	// Give one list a low lever
		for(j = 0; j < 5; j++)							   	// Check the row ,if have low lever ,that is one key was pressed.
		{
			KPRData = KPRAddr;

			if(!(KPRData & (0x2 << j)))
			{
			    delay(10);
				KPRData = KPRAddr;
				if(!(KPRData & (0x2 << j)))					// Avoid dithering
				{
					do{
						KPRData = KPRAddr;
					}while(!(KPRData & (0x2 << j)));		// Wait the key to be released
					
				
									
					KeyNo = i << 8 | j;

				
					
					//rINTMSK |= BIT_WDT;
					return 1;  								// One key was pressed
				}
			}
		}
	}
	return 0;							  					// No key was pressed
}

/*********************************************************************************************
* name:		keypad_test
* func:		scan the keypad and display the pressed key
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void keypad_test(void)
{




 
	if(keyscan())
	{			

			
		  switch(KeyNo)
		{
			case 0x0000: {keychar = '1';
			Glib_FilledRectangle(251,121,319,149,BLACK);
			Lcd_DspHz24(262,122,WHITE,"搜索");
			Glib_FilledRectangle(251,121,319,149,WHITE); 	
		    Lcd_DspHz24(262,122,BLACK,"搜索");} break;	  			// FUN key
			case 0x0001: keychar = 'D'; break;	
			case 0x0002: {keychar = '-';
			Glib_FilledRectangle(440,100,490,150,BLACK);
			Lcd_DspAscII8x16(445,105,WHITE,"-");
			Glib_FilledRectangle(440,100,490,150,WHITE); 	
		    Lcd_DspAscII8x16(445,105,BLACK,"-");}break;
			case 0x0003: {keychar = '0';
			Glib_FilledRectangle(440,220,490,270,BLACK);
			Lcd_DspAscII8x16(445,225,WHITE,"done");
			Glib_FilledRectangle(440,220,490,270,WHITE); 	
		    Lcd_DspAscII8x16(445,225,BLACK,"done");}break;	
			case 0x0004: {keychar = '+'; 
			Glib_FilledRectangle(440,160,490,210,BLACK);
			Lcd_DspAscII8x16(445,165,WHITE,"+");
			Glib_FilledRectangle(440,160,490,210,WHITE); 	
		    Lcd_DspAscII8x16(445,165,BLACK,"+");}break;
			case 0x0100: {keychar = '*';
			Glib_FilledRectangle(500,100,550,150,BLACK);
			Lcd_DspAscII8x16(505,105,WHITE,"*");
			Glib_FilledRectangle(500,100,550,150,WHITE); 	
		    Lcd_DspAscII8x16(505,105,BLACK,"*");}break;
			case 0x0101: keychar = 'C'; break;
			case 0x0102: {keychar = 'w';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);
			Glib_FilledRectangle(620,220,670,270,BLACK);
			Lcd_DspAscII8x16(625,225,WHITE,"wxyz");
			Glib_FilledRectangle(620,220,670,270,WHITE); 	
		    Lcd_DspAscII8x16(625,225,BLACK,"wxyz");} break;	
			case 0x0103: {keychar = 't';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);
			Glib_FilledRectangle(560,220,610,270,BLACK);
			Lcd_DspAscII8x16(565,225,WHITE,"tuv");
			Glib_FilledRectangle(560,220,610,270,WHITE); 	
		    Lcd_DspAscII8x16(565,225,BLACK,"tuv");} break;
			case 0x0104: {keychar = 'p';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);
			Glib_FilledRectangle(500,220,550,270,BLACK);
			Lcd_DspAscII8x16(505,225,WHITE,"pqrs");
			Glib_FilledRectangle(500,220,550,270,WHITE); 	
		    Lcd_DspAscII8x16(505,225,BLACK,"pqrs");} break;	
			case 0x0200: keychar = 'F'; break;
			case 0x0201: keychar = 'B'; break;	
			case 0x0202: {keychar = 'm';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);
			Glib_FilledRectangle(620,160,670,210,BLACK);
			Lcd_DspAscII8x16(625,165,WHITE,"mno");
			Glib_FilledRectangle(620,160,670,210,WHITE); 	
		    Lcd_DspAscII8x16(625,165,BLACK,"mno");} break;
			case 0x0203: {keychar = 'j';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);
			Glib_FilledRectangle(560,160,610,210,BLACK);
			Lcd_DspAscII8x16(565,165,WHITE,"jkl");
			Glib_FilledRectangle(560,160,610,210,WHITE); 	
		    Lcd_DspAscII8x16(565,165,BLACK,"jkl");} break;	
			case 0x0204: {keychar = 'g';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);
			Glib_FilledRectangle(500,160,550,210,BLACK);
			Lcd_DspAscII8x16(505,165,WHITE,"ghi");
			Glib_FilledRectangle(500,160,550,210,WHITE); 	
		    Lcd_DspAscII8x16(505,165,BLACK,"ghi");} break;
			case 0x0300: keychar = 'E'; break;
			case 0x0301: keychar = 'A'; break;	
			case 0x0302: {keychar = 'd'; k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);
			Glib_FilledRectangle(620,100,670,150,BLACK);
			Lcd_DspAscII8x16(625,105,WHITE,"def");
			Glib_FilledRectangle(620,100,670,150,WHITE); 	
		    Lcd_DspAscII8x16(625,105,BLACK,"def");} break;
			case 0x0303: {keychar = 'a'; k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);
			Glib_FilledRectangle(560,100,610,150,BLACK);
			Lcd_DspAscII8x16(565,105,WHITE,"abc");
			Glib_FilledRectangle(560,100,610,150,WHITE); 	
		    Lcd_DspAscII8x16(565,105,BLACK,"abc");} break;	
			case 0x0304: keychar = '7';  break;
			default: break;
	    }
if (keychar=='A')
{t=0;
led8_test();
}
if (keychar=='B')
{t=1;
led8_test();
}
if (keychar=='C')
{t=2;
led8_test();
}
if (keychar=='D')
{t=3;
led8_test();
}		
	if(keychar=='1')
{ if (add==1)
{ C[z][n+1]=',';
 z+=1;
 add=0;
 mark=0; 
} 

 f=0;
    B[n]=';';
    for (p=0;p<10;p++)
{	
  for (q=0;q<=20;q++)
  { 
   if (B[q]!=C[p][q]&&(B[q]!=';'||C[p][q]!=';'))
   {
   	f=1;break;
   }
   else
   {
   	if(B[q]==';'||C[p][q]==';')
	{
		f=0;
		break;
	}
   }
  }
   if (f==0)
   {
   g=p;
   h=q;
   break;
   }
}  
  if (f==1)
{
lcd_clr_rect(102,211,319,349,WHITE);
Lcd_DspHz24(102,222,BLACK,"查无此词");
add=1; 
}
  if(f==0)
{
lcd_clr_rect(102,211,319,349,WHITE);
Lcd_DspAscII8x16(112,222,BLACK,(C[g]+q+1));
for (p=0,q=h+1;C[g][q]!=',';q++)
{
p+=1;
}
Lcd_DspHz24(142,222,BLACK,(C[g]+q+1));
}
}

		else
		 {
		 if(keychar=='*')
		   {add=0;
		   mark=0;
		   n=0;
		   m=103;
		   for (q=0;q<=20;q++)
           {B[q]='0';}
		   Glib_FilledRectangle(101,121,249,149,WHITE);
		   Glib_FilledRectangle(102,211,319,349,WHITE); 
                   }
		 else
		 {
		 if(keychar=='-')
		 {
		 if(n>0)
		 {if(mark==1)
		 {
		 C[z][n]=',';
		  n-=1;
		 lcd_clr_rect(m,262,m+10,289,WHITE);
		 m-=10;
		 }
		 if(mark==0)
		 {B[n]='0';
		 n-=1;
		 B[n]=';';
		 lcd_clr_rect(m,121,m+10,149,WHITE);
		 m-=10;
		 }
		 }
		 }
		 else
		 {
		 if(keychar=='+'&&add==1)
		 {mark=1;
		 for(v=0;v<n;v++)
		 C[z][v]=B[v];
		 C[z][n]=';';
		 lcd_clr_rect(102,211,319,349,WHITE);
		 Lcd_DspHz24(102,222,BLACK,"请在此处添加词意");
		 }
		 else
		 {
		 if (keychar=='0'&&add==1)
		 {
		 C[z][n+1]=',';
		 lcd_clr_rect(102,211,319,349,WHITE);
		 Lcd_DspHz24(102,222,BLACK,"单词添加完成");
		 z+=1;
		 add=0;
		 mark=0;
		 }
		 }
		 }

		 }
	   	}	 
}

		 if(flag==1&&mark==1)
		 {
		 if(n<14)
		 {A[0]=keychar;
        Lcd_DspAscII8x16(m+=10,272,BLACK,A);
		 n+=1;
		 C[z][n]=A[0];
		 flag=0;
		 }
		 } 
         if (flag==1)
		{ 
		if(n<14)
		{A[0]=keychar;
        Lcd_DspAscII8x16(m+=10,131,BLACK,A);
	    B[n]=A[0];
	    n+=1;
        flag=0;
        }
		} 
}