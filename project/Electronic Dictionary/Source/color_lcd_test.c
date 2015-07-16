/*********************************************************************************************
* File：	lcdapp.c
* Author:	Embest	
* Desc：	LCD_Test
* History:	
*********************************************************************************************/

/*------------------------------------------------------------------------------------------*/
/*	 								include files						 				    */
/*------------------------------------------------------------------------------------------*/
#include <string.h>
#include "2410lib.h"
#include "lcdlib.h"
#include "glib.h"
#include "lcd.h"
#include <math.h>

/*------------------------------------------------------------------------------------------*/
/*	 								constant define							 			    */
/*------------------------------------------------------------------------------------------*/
#define LCD_BUF_SIZE		(SCR_XSIZE_TFT_640480*SCR_YSIZE_TFT_640480/2)
#define LCD_ACTIVE_BUFFER	(0x33800000)
#define LCD_VIRTUAL_BUFFER  (0x33800000 + LCD_BUF_SIZE)
#define LCD_D_OFF 			rGPCDAT &= ~(1 << 4); 
#define LCD_D_ON 			rGPCDAT |= (1 << 4); 

/*------------------------------------------------------------------------------------------*/
/*	 								extern variables						 			    */
/*------------------------------------------------------------------------------------------*/
#ifdef BOARDTEST_EXH
#undef BOARDTEST
#endif	

#ifndef BOARDTEST	
extern const UINT8T g_ucBitmap[][76800];
extern UINT8T g_ucAscii8x16[];
extern UINT8T g_auc_Ascii6x8[];
extern UINT8T g_auc_hzk24[];
extern UINT32T n,m;	
extern UINT8T t,k,flag,z,add,mark,keychar,p,q,f,g,h,v;			
extern char C[10][20],A[1],B[20];

#endif
 
/*------------------------------------------------------------------------------------------*/
/*	 								function declare                                        */
/*------------------------------------------------------------------------------------------*/
void Lcd_DspAscII6x8(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T * s);
void Lcd_DspAscII8x16(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T * s);
//void Lcd_DspHz12(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T *s);
//void Lcd_DspHz16(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T *s);
void Lcd_DspHz24(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T *s);
//void Lcd_DspHz24s(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T *s);
void Lcd_Draw_HLine(INT16T usX0, INT16T usX1, INT16T usY0, UINT16T ucColor, UINT16T usWidth);
void Lcd_Draw_VLine (INT16T usY0, INT16T usY1, INT16T usX0, UINT16T ucColor, UINT16T usWidth);
void lcd_clr(void);
extern void Lcd_port_init(void);
extern void Lcd_Port_Return(void);
void color_lcd_printf(UINT32T x,UINT32T y);

typedef struct _Rect
{
	int x1;
	int y1;
	int x2;
	int y2;
}Rect;

Rect pos[13]={
{251,121,319,149},{500,100,550,150},{500,160,550,210},{500,220,550,270},{560,100,610,150},{560,160,610,210},{560,220,610,270},
{620,100,670,150},{620,160,670,210},{620,220,670,270},
{440,100,490,150},{440,160,490,210},{440,220,490,270}
};



/*********************************************************************************************
* name:		lcd_init_app
* func:		lcd application initialization code 
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_init_app()
{
    Lcd_port_init();
    Lcd_Init(MODE_TFT_16BIT_800480);
    Glib_Init(MODE_TFT_16BIT_800480);
    Glib_ClearScr(0, MODE_TFT_16BIT_800480);
    Lcd_PowerEnable(0, 1);
    Lcd_EnvidOnOff(1);	  
    uart_printf(" [TFT 64K COLOR(16bit/1pixel) LCD TEST]\n");

}

/*********************************************************************************************
* name:		lcd_init_app_end
* func:		end lcd application initialization
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_init_app_end()
{
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}

#ifndef BOARDTEST	
/*********************************************************************************************
* name:		Lcd_DspHz24
* func:		display 24x24 Chinese Symbol on Lcd  
* para:		UINT16T x0          --  x coordinate of start point
*           UINT16T y0          --  y coordinate of start point
*           UINT16T ForeColor --  foreground color of lcd 
*           UINT8T *s        --  string to display 
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_DspHz24(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T *s)
{
	INT16T i,j,k,x,y,xx;
	UINT8T qm,wm;
	INT32T ulOffset;
	INT8T hzbuf[72];

	for(i = 0; i < strlen((const char*)s); i++)
	{
		if(((UINT8T)(*(s+i))) < 161)
		{
			break;
		}
		else
		{
			qm = *(s+i) - 176;                                  //161;
	    		wm = *(s+i + 1) - 161;
            		ulOffset = (INT32T)(qm * 94 + wm) * 72;
            		for (j = 0; j < 72; j ++)
            		{
            			hzbuf[j] = g_auc_hzk24[ulOffset + j];
             		}
            		for(y = 0; y < 24; y++)
            		{
	               	for(x = 0; x < 24; x++) 
	               	{
           				k = x % 8;
			       	if (hzbuf[y * 3 + x / 8]  & (0x80 >> k))
			       	{
			       		xx = x0 + x + i*12;
			       		PutPixel( xx, y + y0, (UINT16T)ForeColor);
			       	}
			      }
            		}
		    	i++;
         	}
     	}     	
}
#endif

/*********************************************************************************************
* name:		Lcd_DspAscII6x8()
* func:		display 6x8 ASCII character string 
* para:		usX0,usY0 -- ASCII character string's start point coordinate
*			ForeColor -- appointed color value
*			pucChar   -- ASCII character string
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
#define XWIDTH 			6
extern UINT8T g_ucAscii6x8[];
void Lcd_DspAscII6x8(UINT16T usX0, UINT16T usY0,UINT16T ForeColor, UINT8T* pucChar)
{
	UINT32T i,j;
	UINT8T  ucTemp;

	while( *pucChar != 0 )
	{
		for( i=0; i < 8; i++ )
		{
  			ucTemp = g_ucAscii6x8[(*pucChar) * 8 + i];
  			for( j = 0; j < 8; j++ )
  			{
  				if( (ucTemp & (0x80 >> j)) != 0 )
  				{
  					PutPixel(usX0 + i, usY0 + 8 - j, (UINT16T)ForeColor);
  				}  				
  			}
		}
		usX0 += XWIDTH;
		pucChar++;
	}
} 

#ifndef BOARDTEST	
/*********************************************************************************************
* name:		Lcd_DspAscII8X16
* func:		display 8x16 AscII Symbol on Lcd  
* para:		UINT16T x0          --  x coordinate of start point
*           UINT16T y0          --  y coordinate of start point
*           UINT16T ForeColor --  foreground color of lcd 
*           UINT8T *s        --  string to display 
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_DspAscII8x16(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T * s)
{
	INT16T i,j,k,x,y,xx;
	UINT8T qm;
	INT32T ulOffset;
	INT8T ywbuf[16];
    
	for(i = 0; i < strlen((const char*)s); i++)
	{
		if((UINT8T)*(s+i) >= 161)
		{
			return;
		}
		else
		{
			qm = *(s+i);
            		ulOffset = (INT32T)(qm) * 16;	
            		for (j = 0; j < 16; j ++)
            		{
            			ywbuf[j] = g_ucAscii8x16[ulOffset + j];
            		}

            		for(y = 0; y < 16; y++)
            		{
               		for(x = 0; x < 8; x++) 
               		{
                   			k = x % 8;
			       	if (ywbuf[y]  & (0x80 >> k))
			       	{
			       		xx = x0 + x + i*8;
			       		PutPixel( xx, y + y0, (UINT16T)ForeColor);
			       	}
			   	}
            		}
         	}
     	}
}
#endif	

/*********************************************************************************************
* name:		lcd_clr
* func:		clear LCD screen
* para:		none 
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_clr(void)
{
	UINT32T i;
	UINT32T *pDisp = (UINT32T*)LCD_ACTIVE_BUFFER;
	
	for (i = 0; i < (SCR_XSIZE_CSTN * SCR_YSIZE_CSTN /4); i++)
	{
		*pDisp = 0xffffffff;//ALLWHITE;
	}
}

/*********************************************************************************************
* name:		lcd_clr_rect
* func:		fill appointed area with appointed color
* para:		usLeft,usTop,usRight,usBottom -- area's rectangle acme coordinate
*			ucColor -- appointed color value
* ret:		none
* modify:
* comment:	also as clear screen function 
*********************************************************************************************/
void lcd_clr_rect(INT16T usLeft, INT16T usTop, INT16T usRight, INT16T usBottom, UINT16T ucColor)
{
	UINT32T i, j;
//	UINT8T *pDisp = (UINT8T*)LCD_ACTIVE_BUFFER;
	
	for (i = usLeft; i < usRight; i++)
	for (j = usTop; j < usBottom; j++)
	{
		PutPixel(i,j,ucColor);
		//*(pDisp+i+j) = ucColor;
	}
}

/*********************************************************************************************
* name:		Lcd_Draw_Box()
* func:		Draw rectangle with appointed color
* para:		usLeft,usTop,usRight,usBottom -- rectangle's acme coordinate
*			ucColor -- appointed color value
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Draw_Box(INT16T usLeft, INT16T usTop, INT16T usRight, INT16T usBottom, UINT16T ucColor)
{
	Lcd_Draw_HLine(usLeft, usRight,  usTop,    ucColor, 1);
	Lcd_Draw_HLine(usLeft, usRight,  usBottom, ucColor, 1);
	Lcd_Draw_VLine(usTop,  usBottom, usLeft,   ucColor, 1);
	Lcd_Draw_VLine(usTop,  usBottom, usRight,  ucColor, 1);
}

/*********************************************************************************************
* name:		Lcd_Draw_Line()
* func:		Draw line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usX1,usY1 -- line's end point coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Draw_Line(INT16T usX0, INT16T usY0, INT16T usX1, INT16T usY1, UINT16T ucColor, UINT16T usWidth)
{
	INT16T usDx;
	INT16T usDy;
	INT16T y_sign;
	INT16T x_sign;
	INT16T decision;
	INT16T wCurx, wCury, wNextx, wNexty, wpy, wpx;

	if( usY0 == usY1 )
	{
		Lcd_Draw_HLine (usX0, usX1, usY0, ucColor, usWidth);
		return;
	}
	if( usX0 == usX1 )
	{
		Lcd_Draw_VLine (usY0, usY1, usX0, ucColor, usWidth);
		return;
	}
	usDx = abs(usX0 - usX1);
	usDy = abs(usY0 - usY1);
	if( ((usDx >= usDy && (usX0 > usX1)) ||
        ((usDy > usDx) && (usY0 > usY1))) )
    {
        GUISWAP(usX1, usX0);
        GUISWAP(usY1, usY0);
    }
    y_sign = (usY1 - usY0) / usDy;
    x_sign = (usX1 - usX0) / usDx;

    if( usDx >= usDy )
    {
        for( wCurx = usX0, wCury = usY0, wNextx = usX1,
             wNexty = usY1, decision = (usDx >> 1);
             wCurx <= wNextx; wCurx++, wNextx--, decision += usDy )
        {
            if( decision >= usDx )
            {
                decision -= usDx;
                wCury += y_sign;
                wNexty -= y_sign;
            }
            for( wpy = wCury - usWidth / 2;
                 wpy <= wCury + usWidth / 2; wpy++ )
            {
                PutPixel(wCurx, wpy, ucColor);
            }

            for( wpy = wNexty - usWidth / 2;
                 wpy <= wNexty + usWidth / 2; wpy++ )
            {
                PutPixel(wNextx, wpy, ucColor);
            }
        }
    }
    else
    {
        for( wCurx = usX0, wCury = usY0, wNextx = usX1,
             wNexty = usY1, decision = (usDy >> 1);
             wCury <= wNexty; wCury++, wNexty--, decision += usDx )
        {
            if( decision >= usDy )
            {
                decision -= usDy;
                wCurx += x_sign;
                wNextx -= x_sign;
            }
            for( wpx = wCurx - usWidth / 2;
                 wpx <= wCurx + usWidth / 2; wpx++ )
            {
                PutPixel(wpx, wCury, ucColor);
            }

            for( wpx = wNextx - usWidth / 2;
                 wpx <= wNextx + usWidth / 2; wpx++ )
            {
                PutPixel(wpx, wNexty, ucColor);
            }
        }
    }
}

/*********************************************************************************************
* name:		Lcd_Draw_HLine()
* func:		Draw horizontal line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usX1 -- line's end point X-coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Draw_HLine(INT16T usX0, INT16T usX1, INT16T usY0, UINT16T ucColor, UINT16T usWidth)
{
	INT16T usLen;

    if( usX1 < usX0 )
    {
        GUISWAP (usX1, usX0);
    }

    while( (usWidth--) > 0 )
    {
        usLen = usX1 - usX0 + 1;
        while( (usLen--) > 0 )
        {
        	PutPixel(usX0 + usLen, usY0, ucColor);
        }
        usY0++;
    }
}

/*********************************************************************************************
* name:		Lcd_Draw_VLine()
* func:		Draw vertical line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usY1 -- line's end point Y-coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Draw_VLine (INT16T usY0, INT16T usY1, INT16T usX0, UINT16T ucColor, UINT16T usWidth)
{
	INT16T usLen;

    if( usY1 < usY0 )
    {
        GUISWAP (usY1, usY0);
    }

    while( (usWidth--) > 0 )
    {
        usLen = usY1 - usY0 + 1;
        while( (usLen--) > 0 )
        {
        	PutPixel(usX0, usY0 + usLen, ucColor);
        }
        usX0++;
    }
}

/*********************************************************************************************
* name:		color_lcd_test()
* func:		LCD test function
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void color_lcd_test(void)
{
	int i=290;

    uart_printf("\n LCD display Test Example (please look at LCD screen)\n");

	lcd_init_app();
#ifndef BOARDTEST	
//	Lcd_DspHz24(50,10,BLUE,"英蓓特三星实验平台");
#endif	
    
    BitmapViewTft16Bit_800480((UINT8T *)(g_ucBitmap));

	Glib_FilledRectangle(101,121,319,149,WHITE);
	Glib_Rectangle(100,120,250,150,BLACK);
	Glib_Rectangle(250,120,320,150,BLACK);

	Glib_FilledRectangle(100,211,319,349,WHITE);

 Glib_FilledRectangle(440,100,490,150,WHITE);
 Glib_Rectangle(439,99,491,151,BLACK);
  Glib_FilledRectangle(440,160,490,210,WHITE);
  Glib_Rectangle(439,159,491,211,BLACK);
   Glib_FilledRectangle(440,220,490,270,WHITE);
   Glib_Rectangle(439,219,491,271,BLACK);

    Glib_FilledRectangle(500,100,550,150,WHITE);
	Glib_Rectangle(499,99,551,151,BLACK);
    Glib_FilledRectangle(500,160,550,210,WHITE);
	Glib_Rectangle(499,159,551,211,BLACK);
    Glib_FilledRectangle(500,220,550,270,WHITE);
	Glib_Rectangle(499,219,551,271,BLACK);
    Glib_FilledRectangle(560,100,610,150,WHITE);
	Glib_Rectangle(559,99,611,151,BLACK);
    Glib_FilledRectangle(560,160,610,210,WHITE);
	Glib_Rectangle(559,159,611,211,BLACK);
    Glib_FilledRectangle(560,220,610,270,WHITE);
	Glib_Rectangle(559,219,611,271,BLACK);
	Glib_FilledRectangle(620,100,670,150,WHITE);
	Glib_Rectangle(619,99,671,151,BLACK);
	Glib_FilledRectangle(620,160,670,210,WHITE);
	Glib_Rectangle(619,159,671,211,BLACK);
	Glib_FilledRectangle(620,220,670,270,WHITE);
	Glib_Rectangle(619,219,671,271,BLACK);
//	Glib_Rectangle(420,90,690,280,BLACK);

	Lcd_DspHz24(65,90,BLACK,"请输入单词");
	Lcd_DspHz24(65,180,BLACK,"该单词的意思为");
	Lcd_DspHz24(400,300,BLACK,"作者：DP");
	Lcd_DspHz24(262,122,BLACK,"搜索");
	Lcd_DspAscII8x16(450,350,BLUE,"作者github：mayqewasd");

	Lcd_DspAscII8x16(505,105,BLACK,"*");
		Lcd_DspAscII8x16(445,105,BLACK,"-");
			Lcd_DspAscII8x16(445,165,BLACK,"+");
				Lcd_DspAscII8x16(445,225,BLACK,"done");

	Lcd_DspAscII8x16(565,105,BLACK,"abc");
	Lcd_DspAscII8x16(625,105,BLACK,"def");
	Lcd_DspAscII8x16(505,165,BLACK,"ghi");
	Lcd_DspAscII8x16(565,165,BLACK,"jkl");
	Lcd_DspAscII8x16(625,165,BLACK,"mno");
	Lcd_DspAscII8x16(505,225,BLACK,"pqrs");
	Lcd_DspAscII8x16(565,225,BLACK,"tuv");
	Lcd_DspAscII8x16(625,225,BLACK,"wxyz");


#ifdef Hzk16
	Lcd_DspHz16(65,90,BLACK,"英蓓特信息技术有限公司");
	Lcd_DspHz16(65,180,BLACK,"十六点阵汉字");
#endif

#ifdef Hzk12
	Lcd_DspHz12(180,i+=20,BLACK,"英蓓特信息技术有限公司");
	Lcd_DspHz12(180,i+=20,BLACK,"十二点阵汉字");
#endif

#ifdef Ascii8x16
	Lcd_DspAscII8X16(300,i+=30,BLACK,"Embest2410");
	Lcd_DspAscII8X16(300,i+=30,BLACK,"8x16");
	lcd_Disp_str16(75, 25, GREEN, "LCD display Test Example (please look at LCD screen)");
#endif

#ifdef Ascii6x8
	Lcd_DspAscII6X8(500,i+=30,BLACK,"Embest2410");
	Lcd_DspAscII6x8(550,i+=30,BLACK,"6x8");
#endif

 

}
 


void color_lcd_printf(UINT32T x,UINT32T y)
{ 
	 
static unsigned char i=0;
delay(1000);
for(i=0;i<13;i++)
	{
	     if(((x<=pos[i].x2)&&(x>=pos[i].x1))&&((y<=pos[i].y2)&&(y>=pos[i].y1)))
		{
	      
              Glib_FilledRectangle(pos[i].x1,pos[i].y1,pos[i].x2,pos[i].y2, BLUE);	
              Glib_FilledRectangle(pos[i].x1,pos[i].y1,pos[i].x2,pos[i].y2, WHITE); 
			  Lcd_DspHz24(262,122,BLACK,"搜索");
			  Lcd_DspAscII8x16(505,105,BLACK,"*");
		Lcd_DspAscII8x16(445,105,BLACK,"-");
			Lcd_DspAscII8x16(445,165,BLACK,"+");
				Lcd_DspAscII8x16(445,225,BLACK,"done"); 
              Lcd_DspAscII8x16(565,105,BLACK,"abc");
	          Lcd_DspAscII8x16(625,105,BLACK,"def");
	          Lcd_DspAscII8x16(505,165,BLACK,"ghi");
	          Lcd_DspAscII8x16(565,165,BLACK,"jkl");
	          Lcd_DspAscII8x16(625,165,BLACK,"mno");
	          Lcd_DspAscII8x16(505,225,BLACK,"pqrs");
	          Lcd_DspAscII8x16(565,225,BLACK,"tuv");
	          Lcd_DspAscII8x16(625,225,BLACK,"wxyz");
             break;
		
                 }
	}
	 switch(i)
		      {
			     case 0: keychar = '1'; break;
                 case 1: keychar = '*'; break;
                 case 2: {keychar = 'g';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);} break;	  			
			     case 3: {keychar = 'p';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);} break;	
			     case 4: {keychar = 'a';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);} break;	
			     case 5: {keychar = 'j';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);} break;
			     case 6: {keychar = 't';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);} break;	
		         case 7: {keychar = 'd';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);} break;
			     case 8: {keychar = 'm';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);} break;	
			     case 9: {keychar = 'w';k+=1;rWTCON |=(1<<5);rINTMSK &= ~(BIT_WDT);} break;
                 case 10: keychar = '-'; break;			
				 case 11: keychar = '+'; break;
				 case 12: keychar = '0'; break;
		  
			     default: break;
	          }
		
	if(keychar=='1')
{keychar='2'; 
if (add==1)
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
		   {keychar='2'; 
		   add=0;
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
		 { keychar='2'; 
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
		 {keychar='2'; 
		 mark=1;
		 for(v=0;v<n;v++)
		 C[z][v]=B[v];
		 C[z][n]=';';
		 lcd_clr_rect(102,211,319,349,WHITE);
		 Lcd_DspHz24(102,222,BLACK,"请在此处添加词意");
		 }
		 else
		 {
		 if (keychar=='0'&&add==1)
		 {keychar='2'; 
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

