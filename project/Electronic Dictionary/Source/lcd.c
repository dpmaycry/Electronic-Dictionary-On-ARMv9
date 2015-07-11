/*********************************************************************************************
* File£º	lcd.c
* Author:	Embest	
* Desc£º	LCD_Test 
* History:	
*********************************************************************************************/

/*------------------------------------------------------------------------------------------*/
/*	 								include files						 				    */
/*------------------------------------------------------------------------------------------*/
#include <string.h>
#include "2410lib.h"
#include "lcd.h"
#include "glib.h"
#include "lcdlib.h"
#include "demo256.h"

/*------------------------------------------------------------------------------------------*/
/*	 								functions declare                                       */
/*------------------------------------------------------------------------------------------*/
void Lcd_port_init(void);
void Lcd_Port_Return(void);
void Lcd_Palette1Bit_Init(void);
void Lcd_Palette8Bit_Init(void);
void __irq Lcd_Int_Frame(void);// __attribute__ ((interrupt ("IRQ")));
void __irq Lcd_Int_Fifo(void);// __attribute__ ((interrupt ("IRQ")));
void __irq Lcd_Int_Fifo_640480(void);// __attribute__ ((interrupt ("IRQ")));

/*------------------------------------------------------------------------------------------*/
/*	 								global variables						 			    */
/*------------------------------------------------------------------------------------------*/
unsigned save_rGPCUP,save_rGPCDAT,save_rGPCCON;
unsigned save_rGPDUP,save_rGPDDAT,save_rGPDCON; 
unsigned lcd_count;

/*********************************************************************************************
* name:		Lcd_port_init()
* func:		lcd port initialization
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_port_init(void)
{
    save_rGPCCON=rGPCCON;
    save_rGPCDAT=rGPCDAT;
    save_rGPCUP=rGPCUP;
    save_rGPDCON=rGPDCON;
    save_rGPDDAT=rGPDDAT;
    save_rGPDUP=rGPDUP;
    rGPCUP=0xffffffff;                                          // Disable Pull-up register
   // rGPCCON=0xaaaaa9aa;                                         // Initialize VD[7:0],LCDVF[2:0],VM,VFRAME,VLINE,VCLK,LEND 
    rGPCCON=0xaaaaaaaa;  
	rGPDUP=0xffffffff;                                          // Disable Pull-up register
    rGPDCON=0xaaaaaaaa;                                         // Initialize VD[23:8]
    uart_printf(" Initializing GPIO ports..........\n");
}

/*********************************************************************************************
* name:		Lcd_Port_Return()
* func:		Return lcd port configuration
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Port_Return(void)
{
    rGPCCON=save_rGPCCON;
    rGPCDAT=save_rGPCDAT;
    rGPCUP=save_rGPCUP;
    rGPDCON=save_rGPDCON;
    rGPDDAT=save_rGPDDAT;
    rGPDUP=save_rGPDUP;
}

/*********************************************************************************************
* name:		Lcd_Palette1Bit_Init()
* func:		Lcd 1 bpp palette initialization
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Palette1Bit_Init(void)
{
    UINT32T *palette;
    palette=(UINT32T *)PALETTE;
    *palette++=0x0;                                             // Black
    *palette=0xffff;                                            // White
}

/*********************************************************************************************
* name:		Lcd_Palette8Bit_Init()
* func:		Lcd 8 bpp palette initialization
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Palette8Bit_Init(void)
{
    int i;	
    UINT32T *palette;
    rLCDCON5|=(1<<11);                                          // 5:6:5 Palette Setting
    palette=(UINT32T *)PALETTE;
    for(i=0;i<256;i++)
	*palette++=DEMO256pal[i];
}

/*********************************************************************************************
* name:		Lcd_Int_Frame()
* func:		Lcd frame interrupt
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void __irq Lcd_Int_Frame(void)
{
    rLCDINTMSK|=3;                                              // Mask LCD SUB Interrupt

    // GPG4 is .... 
    rGPGDAT&=(~(1<<4));                                         // GPG4=Low
    delay(50);                                                  // GPG4=Low
    rGPGDAT|=(1<<4);                                            // GPG4=High

    rLCDSRCPND=2;                                               // Clear LCD SUB Interrupt source pending
    rLCDINTPND=2;                                               // Clear LCD SUB Interrupt pending
    rLCDINTMSK&=(~(2));                                         // Unmask LCD FRAME Interrupt
    ClearPending(BIT_LCD);
}

/*********************************************************************************************
* name:		Lcd_Int_Fifo()
* func:		Lcd Fifo interrupt
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void __irq Lcd_Int_Fifo(void)
{
    rLCDINTMSK|=3;                                              // Mask LCD SUB Interrupt

    if((lcd_count%20)==0) uart_printf(" \n");
    uart_printf(" .");
    lcd_count++;

    rLCDSRCPND=1;                                               // Clear LCD SUB Interrupt source pending
    rLCDINTPND=1;                                               // Clear LCD SUB Interrupt pending
    rLCDINTMSK&=(~(1));                                         // Unmask LCD FRAME Interrupt
    ClearPending(BIT_LCD);
}

/*********************************************************************************************
* name:		Test_Lcd_Stn_1Bit()
* func:		test 1bpp stn LCD
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Stn_1Bit(void)
{
    int i,j;
    Lcd_port_init();
    Lcd_Init(MODE_STN_1BIT);
    Glib_Init(MODE_STN_1BIT);
    Lcd_EnvidOnOff(1);                                          // Enable ENVID Bit
    uart_printf("  STN Mono(1bit/1pixel) LCD Test\n");              

    Glib_ClearScr(0, MODE_STN_1BIT);
    for(j=0;j<LCD_YSIZE_STN;j+=16)
		for(i=0;i<LCD_XSIZE_STN;i+=16)
		    Glib_FilledRectangle(i,j,i+15,j+15,((j+i)/16)%2);
    uart_printf(" STN Mono test 1. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_STN_1BIT);
    Glib_FilledRectangle(160,0,319,239,1);                      //Half Screen
    uart_printf(" STN Mono test 2. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_STN_1BIT); 
    Glib_Rectangle(0,0,319,239,1);                              // #0
    Glib_Line(0,0,319,239,1);                                   // 00
    Glib_Line(0,239,319,0,1);

    Glib_Rectangle(0+320,0,319+320,239,1);                      // 0#
    Glib_Line(0+320,0,319+320,239,1);                           // 00
    Glib_Line(0+320,239,319+320,0,1);
    Glib_FilledRectangle(50+320,50,269+320,189,1);

    Glib_Rectangle(0,0+240,319,239+240,1);                      // 00
    Glib_Line(0,0+240,319,239+240,1);                           // #0
    Glib_Line(0,239+240,319,0+240,1);
    Glib_FilledRectangle(50,50+240,269,189+240,1);
    
    Glib_Rectangle(0+320,0+240,319+320,239+240,1);              // 00	
    Glib_Line(0+320,0+240,319+320,239+240,1);                   // 0#
    Glib_Line(0+320,239+240,319+320,0+240,1);
    Glib_Rectangle(50+320,50+240,269+320,189+240,1);

    uart_printf(" Virtual Screen Test(STN Mono). Press any key[ijkm\\r]!\n");
    MoveViewPort(MODE_STN_1BIT);
    Lcd_MoveViewPort(0,0,MODE_STN_1BIT);
    Lcd_EnvidOnOff(0);                                          // Disable ENVID Bit
    Lcd_Port_Return();
}


/*********************************************************************************************
* name:		Test_Lcd_Stn_2Bit()
* func:		test 2bpp stn LCD
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Stn_2Bit(void)
{
    int i,j,k;
    Lcd_port_init();
    Lcd_Init(MODE_STN_2BIT);
    Glib_Init(MODE_STN_2BIT);
    Lcd_EnvidOnOff(1);                                          // Enable ENVID Bit
    uart_printf("  STN 4 gray(2bit/1pixel) LCD Test\n");

    Glib_ClearScr(0, MODE_STN_2BIT);
    j=0;
    for(i=0;i<320;i+=80)
        Glib_FilledRectangle(0+i,0,79+i,239,j++);
    uart_printf(" 4 gray mode test 1. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_STN_2BIT);
    j=0;
    for(i=0;i<320;i+=80)
    {
    	Glib_FilledRectangle(0+i,0,79+i,119,j);
    	Glib_FilledRectangle(0+i,120,79+i,239,3-j);
    	j++;
   }
    uart_printf(" 4 gray mode test 2. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_STN_2BIT);
    j=0;
    for(i=0;i<240;i+=60)
    {
    	Glib_FilledRectangle(i,i,i+59,i+59,j);
    	j++;
   }
    uart_printf(" 4 gray mode test 3. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_STN_2BIT);
    k=0;
    for(i=160;i<480;i+=80)
    {
    	for(j=120;j<360;j+=60)
    	{
    	    Glib_FilledRectangle(i,j,i+79,j+59,k%4);
    	    k++;
    	}
    	k+=2;;
   }

    // #0
    // 00
    Glib_Rectangle(0,0,319,239,3);   
    Glib_Line(0,0,319,239,3);        
    Glib_Line(0,239,319,0,3);

    // 0#
    // 00
    Glib_Rectangle(0+320,0,319+320,239,3);          
    Glib_Line(0+320,0,319+320,239,3);        
    Glib_Line(0+320,239,319+320,0,3);

    // 00
    // #0
    Glib_Rectangle(0,0+240,319,239+240,3);          
    Glib_Line(0,0+240,319,239+240,3);        
    Glib_Line(0,239+240,319,0+240,3);

    // 00
    // 0#
    Glib_Line(0+320,0+240,319+320,239+240,3);        
    Glib_Line(0+320,239+240,319+320,0+240,3);
    Glib_Rectangle(50+320,50+240,269+320,189+240,3);

    uart_printf(" Virtual Screen Test(STN 4 gray). Press any key[ijkm\\r]!\n");
    MoveViewPort(MODE_STN_2BIT);
    Lcd_MoveViewPort(0,0,MODE_STN_2BIT);
    Lcd_EnvidOnOff(0);                                          // Disable ENVID Bit
    Lcd_Port_Return();
}


/*********************************************************************************************
* name:		Test_Lcd_Stn_4Bit()
* func:		test 4bpp stn LCD
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Stn_4Bit(void)
{
    int i,j,k;
    Lcd_port_init();
    Lcd_Init(MODE_STN_4BIT);
    Glib_Init(MODE_STN_4BIT);
    Lcd_EnvidOnOff(1);
    uart_printf("  STN 16 gray(4bit/1pixel) LCD Test\n");

    Glib_ClearScr(0, MODE_STN_4BIT);
    j=0;
    for(i=0;i<320;i+=20)
        Glib_FilledRectangle(0+i,0,19+i,239,j++);
    uart_printf(" 16 gray mode test 1. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_STN_4BIT);
    j=0;
    for(i=0;i<320;i+=20)
    {
    	Glib_FilledRectangle(0+i,0,19+i,119,j);
    	Glib_FilledRectangle(0+i,120,19+i,239,15-j);
    	j++;
   }
    uart_printf(" 16 gray mode test 2. Press any key!\n");
    uart_getch();  	


    Glib_ClearScr(0, MODE_STN_4BIT);
    j=0;
    for(i=0;i<240;i+=30)
    {
    	Glib_FilledRectangle(i,i,i+29,i+29,j);
    	Glib_FilledRectangle(i+110,i,i+29+110,i+29,j+8);
    	j++;
   }
    uart_printf(" 4 gray mode test 3. Press any key!\n");
    uart_getch();  	 


    Glib_ClearScr(0, MODE_STN_4BIT);
    k=0;
    for(i=160;i<480;i+=40)
    	for(j=120;j<360;j+=30)
    	{
    	    Glib_FilledRectangle(i,j,i+39,j+29,k%16);
    	    k++;
    	}
    		    
    // #0		    
    // 00		    
    Glib_Rectangle(0,0,319,239,15);   
    Glib_Line(0,0,319,239,15);        
    Glib_Line(0,239,319,0,15);

    // 0#
    // 00
    Glib_Rectangle(0+320,0,319+320,239,15);
    Glib_Line(0+320,0,319+320,239,15);        
    Glib_Line(0+320,239,319+320,0,15);

    // 00
    // #0
    Glib_Rectangle(0,0+240,319,239+240,15);
    Glib_Line(0,0+240,319,239+240,15);        
    Glib_Line(0,239+240,319,0+240,15);

    // 00
    // 0#
    Glib_Rectangle(0+320,0+240,319+320,239+240,15);
    Glib_Line(0+320,0+240,319+320,239+240,15);     
    Glib_Line(0+320,239+240,319+320,0+240,15);
    Glib_Rectangle(50+320,50+240,269+320,189+240,15);

    uart_printf(" Virtual Screen Test(STN 16 gray). Press any key[ijkm\\r]!\n");
    MoveViewPort(MODE_STN_4BIT);
    Lcd_MoveViewPort(0,0,MODE_STN_4BIT);
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}

/*********************************************************************************************
* name:		Test_Lcd_Cstn_8Bit()
* func:		test 8bpp Cstn LCD
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Cstn_8Bit(void)
{
    int i,j,k;
    Lcd_port_init();
    Lcd_Init(MODE_CSTN_8BIT);
    Glib_Init(MODE_CSTN_8BIT);
    Lcd_CstnOnOff(1);                                           // Enable ENVID Bit and GPB5 output
    uart_printf("  CSTN 256 COLOR(8bit/1pixel) LCD TEST\n");
    uart_printf("      R:0   ...    7 \n");
    uart_printf(" G:0  B0:1 B0:1 B0:1 \n");
    uart_printf(" G:.   2:3  2:3  2:3 \n");
    uart_printf(" G:.  B0:1 B0:1 B0:1 \n");
    uart_printf(" G:.   2:3  2:3  2:3 \n");
    uart_printf(" G:.  B0:1 B0:1 B0:1 \n");
    uart_printf(" G:7   2:3  2:3  2:3 \n");

    
    Glib_ClearScr(0, MODE_CSTN_8BIT);
    for(j=0;j<240;j++)
		for(i=0;i<320;i++)                                      //RRRGGGBB
		    PutPixel(i,j,((i/40)<<5)+((j/30)<<2)+(((j/15)%2)<<1)+((i/20)%2));
    uart_printf(" CSTN 256 color mode test 1. Press any key!\n");
    uart_getch();  	
  
    Glib_ClearScr(0, MODE_CSTN_8BIT);
    k=0;
    for(i=160;i<480;i+=20)
    	for(j=120;j<360;j+=15)
    	{
    	    Glib_FilledRectangle(i,j,i+19,j+14,k);
    	    k++;
    	}

    // #0		    
    // 00		    
    Glib_Rectangle(0,0,319,239,255);   
    Glib_Line(0,0,319,239,255);        
    Glib_Line(0,239,319,0,255);

    // 0#
    // 00
    Glib_Rectangle(0+320,0,319+320,239,255);
    Glib_Line(0+320,0,319+320,239,255);        
    Glib_Line(0+320,239,319+320,0,255);

    // 00
    // #0
    Glib_Rectangle(0,0+240,319,239+240,255);
    Glib_Line(0,0+240,319,239+240,255);        
    Glib_Line(0,239+240,319,0+240,255);

    // 00
    // 0#
    Glib_Rectangle(0+320,0+240,319+320,239+240,255);
    Glib_Line(0+320,0+240,319+320,239+240,255);     
    Glib_Line(0+320,239+240,319+320,0+240,255);
    Glib_Rectangle(50+320,50+240,269+320,189+240,255);

    uart_printf(" Virtual Screen Test(CSTN 256 color). Press any key[ijkm\\r]!\n");
    MoveViewPort(MODE_CSTN_8BIT);
    Lcd_MoveViewPort(0,0,MODE_CSTN_8BIT);
    Lcd_CstnOnOff(0);                                           // Disable ENVID Bit and GPB5 output
    Lcd_Port_Return();
}


/*********************************************************************************************
* name:		Test_Lcd_Cstn_8Bit_On()
* func:		test 8bpp Cstn LCD
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Cstn_8Bit_On(void)
{
    int i,j;
    Lcd_port_init();
    Lcd_Init(MODE_CSTN_8BIT);
    Glib_Init(MODE_CSTN_8BIT);
    Lcd_CstnOnOff(1);                                           // Enable ENVID Bit and GPB5 output
    uart_printf("  CSTN LCD(8bit/1pixel) WILL BE TURNED ON EVEN DURING OTHER TEST\n");
    uart_printf(" If you'd like to off the CSTN LCD, then excute other LCD test program,\n");     
    for(j=0;j<240;j++)
    	for(i=0;i<320;i++)                                      //RRRGGGBB
    	    PutPixel(i,j,((i/40)<<5)+((j/30)<<2)+(((j/15)%2)<<1)+((i/20)%2));
}


/*********************************************************************************************
* name:		Test_Lcd_Cstn_12Bit()
* func:		test 12bpp Cstn LCD
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Cstn_12Bit(void)
{
    int i,j,k;
    Lcd_port_init();
    Lcd_Init(MODE_CSTN_12BIT);
    Glib_Init(MODE_CSTN_12BIT);
    Lcd_CstnOnOff(1);                                           // Enable ENVID Bit and GPB5 output
    uart_printf("  CSTN 4K COLOR(12bit/1pixel) LCD TEST\n");
    uart_printf(" R:0 ...    16\n");
    uart_printf(" G:0 ...    16\n");
    uart_printf(" B:0 ...    16\n");
 
    Glib_ClearScr(0, MODE_CSTN_12BIT);
    k=0;
    for(j=0;j<240;j+=80)
        for(i=0;i<320;i+=20)
		{   
	 	   if((k<16)|(k>31))
		    {
		        Glib_FilledRectangle(i,j,i+19,j+79,((k%16)<<(4*(k/16))));
		        k++;
		   }
		    else
		    {
		        Glib_FilledRectangle(i,j,i+19,j+79,((k-(1+2*(k%16)))<<(4*(k/16))));
		        k++;
		   }
		}
    uart_printf(" CSTN 4K color mode test 1. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_CSTN_12BIT);
    for(j=0;j<240;j++)
		for(i=0;i<320;i++)
		    PutPixel(i,j,((i/40)<<9)+((j/30)<<5)+(((j/15)%2)<<1)+((i/20)%2));
    uart_printf(" CSTN 4K color mode test 2. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_CSTN_12BIT);
    k=0;
    for(i=160;i<480;i+=20)
    	for(j=120;j<360;j+=15)
    	{
    	    Glib_FilledRectangle(i,j,i+19,j+14,(((k&0xf)<<8)|((k%4)<<4)|(k&0xf0)) );
    	    k++;
    	}

    // #0		    
    // 00		    
    Glib_Rectangle(0,0,319,239,0xf00);   
    Glib_Line(0,0,319,239,0x0f0);        
    Glib_Line(0,239,319,0,0x00f);
    
    // 0#
    // 00
    Glib_Rectangle(0+320,0,319+320,239,0xfff);
    Glib_Line(0+320,0,319+320,239,0xfff);        
    Glib_Line(0+320,239,319+320,0,0xfff);
    
    // 00
    // #0
    Glib_Rectangle(0,0+240,319,239+240,0xfff);
    Glib_Line(0,0+240,319,239+240,0xfff);        
    Glib_Line(0,239+240,319,0+240,0xfff);
    
    // 00
    // 0#
    Glib_Rectangle(0+320,0+240,319+320,239+240,0xfff);
    Glib_Line(0+320,0+240,319+320,239+240,0xfff);     
    Glib_Line(0+320,239+240,319+320,0+240,0xfff);
    Glib_Rectangle(50+320,50+240,269+320,189+240,0xfff);

    uart_printf(" Virtual Screen Test(CSTN 4K color). Press any key[ijkm\\r]!\n");
    MoveViewPort(MODE_CSTN_12BIT);
    Lcd_MoveViewPort(0,0,MODE_CSTN_12BIT);
    Lcd_CstnOnOff(0);                                           // Disable ENVID Bit and GPB5 output
    Lcd_Port_Return();
}


/*********************************************************************************************
* name:		Test_Lcd_Tft_8Bit_240320()
* func:		test 8bpp 240*320 TFT LCD
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_8Bit_240320(void)
{
    int i,j,k;
    Lcd_port_init();
    Lcd_Palette8Bit_Init();                                     // Initialize 256 palette 
    Lcd_Init(MODE_TFT_8BIT_240320);
    Glib_Init(MODE_TFT_8BIT_240320);
    Lcd_Lpc3600Enable();                                        // Enable LPC3600
    Lcd_PowerEnable(0, 1);
    uart_printf("  TFT 256 COLOR(8bit/1pixel) LCD TEST\n");
    rTPAL = (1<<24)|((0xff)<<16);                               // Enable Temporary Palette : Red
    Lcd_EnvidOnOff(1);                                          // Enable ENVID Bit
    uart_printf(" TFT 256 color mode test 1. Press any key!\n");
    uart_getch(); 

    Glib_ClearScr(0, MODE_TFT_8BIT_240320);                     // Fill the LCD panel with Black Color
    k=0;
    for(i=0;i<320;i+=20)
		for(j=0;j<240;j+=15)
		{ 
		    Glib_FilledRectangle(j,i,j+14,i+19,(k%256));
		    k++;
		}
    rTPAL = 0;                                                  // Disable Temporary Palette
    uart_printf(" TFT 256 color mode test 2. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_TFT_8BIT_240320);                     // Fill the LCD panel with Black Color
    k=0;
    for(i=0;i<320;i+=10)
		for(j=0;j<240;j+=10)
		{ 
		    Glib_FilledRectangle(j,i,j+9,i+9,(k%256));
		    k+=30;
		}
    uart_printf(" TFT 256 color mode test 3. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_TFT_8BIT_240320);
    k=0;
    for(i=80;i<240;i+=10)
    {
    	for(j=60;j<180;j+=10)
    	{
    	    Glib_FilledRectangle(j,i,j+9,i+9,k);
    	    k++;
    	}
   }
    Glib_Rectangle(0,0,239,319,255);   
    Glib_Line(0,0,239,319,255);        
    Glib_Line(0,319,239,0,255);
    
    Glib_Rectangle(0+240,0,239+240,319,255);
    Glib_Line(0+240,0,239+240,319,255);        
    Glib_Line(0+240,319,239+240,0,255);
    
    Glib_Rectangle(0,0+320,239,319+320,255);
    Glib_Line(0,0+320,239,319+320,255);        
    Glib_Line(0,319+320,239,0+320,255);
    
    Glib_Rectangle(0+240,0+320,239+240,319+320,255);
    Glib_Line(0+240,0+320,239+240,319+320,255);     
    Glib_Line(0+240,319+320,239+240,0+320,255);

    Glib_Rectangle(10+240,10+320,480-11,640-11,255);

    uart_printf(" Virtual Screen Test(TFT 256 color). Press any key[ijkm\\r]!\n");
    MoveViewPort(MODE_TFT_8BIT_240320);
    Lcd_MoveViewPort(0,0,MODE_TFT_8BIT_240320);
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}


/*********************************************************************************************
* name:		Test_Lcd_Tft_16Bit_240320()
* func:		test 16bpp 240*320 TFT LCD
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_16Bit_240320(void)
{
    int i,j,k;
    Lcd_port_init();
    Lcd_Init(MODE_TFT_16BIT_240320);
    Glib_Init(MODE_TFT_16BIT_240320);
    Lcd_Lpc3600Enable();                                        // Enable LPC3600
    Lcd_PowerEnable(0, 1);
    Lcd_EnvidOnOff(1);
    uart_printf("  TFT 64K COLOR(16bit/1pixel) LCD TEST\n");

    Glib_ClearScr(0, MODE_TFT_16BIT_240320);
    Glib_FilledRectangle(0,0,239,159,0xf800);    
    Glib_FilledRectangle(0,160,239,320,0xf800);    
    uart_printf(" TFT 64K color mode test 1. Press any key!\n");
    uart_getch();  	
    

    Glib_ClearScr(0, MODE_TFT_16BIT_240320);
    Glib_Rectangle(0,0,239,319,0x07e0);   
    Glib_FilledRectangle(0,0,20,20,65535);   
    Glib_Rectangle(220,300,239,319,65535);   
    Glib_Line(0,0,239,319,0x1f);        
    Glib_Line(239,0,0,319,0xf800);
    uart_printf(" TFT 64K color mode test 2. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_TFT_16BIT_240320);
    k=0;
    for(i=80;i<240;i+=10)
    {
    	for(j=60;j<180;j+=10)
    	{
    	    Glib_FilledRectangle(j,i,j+9,i+9,k);
    	    k++;
    	}
   }

    Glib_Rectangle(0,0,239,319,65535);   
    Glib_Line(0,0,239,319,65535);        
    Glib_Line(0,319,239,0,65535);
    
    Glib_Rectangle(0+240,0,239+240,319,255);
    Glib_Line(0+240,0,239+240,319,255);        
    Glib_Line(0+240,319,239+240,0,255);
    
    Glib_Rectangle(0,0+320,239,319+320,255);
    Glib_Line(0,0+320,239,319+320,255);        
    Glib_Line(0,319+320,239,0+320,255);
    
    Glib_Rectangle(0+240,0+320,239+240,319+320,255);
    Glib_Line(0+240,0+320,239+240,319+320,255);     
    Glib_Line(0+240,319+320,239+240,0+320,255);
    Glib_Rectangle(40+240,40+320,480-41,640-41,0x1f);

    uart_printf(" Virtual Screen Test(TFT 64K color). Press any key[ijkm\\r]!\n");
    MoveViewPort(MODE_TFT_16BIT_240320);
    Lcd_MoveViewPort(0,0,MODE_TFT_16BIT_240320);
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}


/*********************************************************************************************
* name:		Test_Lcd_Tft_8Bit_240320_On()
* func:		test 8bpp 240*320 TFT LCD
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_8Bit_240320_On(void)
{
    int i,j,k=0;
    Lcd_port_init();
    Lcd_Palette8Bit_Init();
    Lcd_Init(MODE_TFT_8BIT_240320);
    Glib_Init(MODE_TFT_8BIT_240320);
    Lcd_Lpc3600Enable();                                        // Enable LPC3600
    Lcd_PowerEnable(0, 1);    
    uart_printf("  TFT LCD(240x320 ,8bpp) WILL BE TURNED ON EVEN DURING OTHER TEST\n");
    uart_printf(" If you'd like to off the TFT LCD, then excute other LCD test program,\n");     

    for(j=0;j<320;j++)
        for(i=0;i<240;i++)
            PutPixel(i,j,((int)DEMO256[k++]));
            
    Lcd_EnvidOnOff(1);
}


/*********************************************************************************************
* name:		Test_Lcd_Tft_8Bit_240320_Bmp()
* func:		test 8bpp 240*320 bmp on TFT LCD
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_8Bit_240320_Bmp(void)
{
    int i,j,k=0;
    Lcd_port_init();
    Lcd_Palette8Bit_Init();
    Lcd_Init(MODE_TFT_8BIT_240320);
    Glib_Init(MODE_TFT_8BIT_240320);
    Lcd_Lpc3600Enable();                                        // Enable LPC3600
    Lcd_PowerEnable(0, 1);    

    for(j=0;j<320;j++)
        for(i=0;i<240;i++)
            PutPixel(i,j,((int)DEMO256[k++]));

    Lcd_EnvidOnOff(1);                                          // Enable ENVID    

    uart_printf(" Press any key to quit!\n");        
    uart_getch();
    Glib_ClearScr(0, MODE_TFT_8BIT_240320);
    delay(5000);
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}


/*********************************************************************************************
* name:		Test_Lcd_Tft_8Bit_240320_IntFr()
* func:		test 8bpp 240*320 TFT LCD Frame interrupt
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_8Bit_240320_IntFr(void)
{
    int i,j,k=0;
    Lcd_port_init();
    Lcd_Palette8Bit_Init();
    Lcd_Init(MODE_TFT_8BIT_240320);
    Glib_Init(MODE_TFT_8BIT_240320);
    Lcd_Lpc3600Enable();                                        // Enable LPC3600
    Lcd_PowerEnable(0, 1);    
    rLCDCON5|=(1<<11);                                          // 5:6:5 Palette Setting
    
    for(j=0;j<320;j++)
        for(i=0;i<240;i++)
            PutPixel(i,j,((int)DEMO256[k++]));

    //LCD frame interrupt test
    rGPGUP=rGPGUP&(~(1<<4))|(1<<4);                             // Pull-up disable
    rGPGDAT=rGPGDAT&(~(1<<4))|(1<<4);                           // GPG4=High
    rGPGCON=rGPGCON&(~(3<<8))|(1<<8);                           // GPG4=Output
   
    pISR_LCD=(unsigned)Lcd_Int_Frame;
    rINTMSK &=~(BIT_LCD);
    rLCDINTMSK=(1<<2)|(0<<1)|(1);                               // 8Words Trigger Level,Unmask Frame int,mask Fifo int
 
    Lcd_EnvidOnOff(1);                                          // Enable ENVID    
    uart_printf(" Press any key to quit!\n");        
    uart_getch();
    rLCDINTMSK |=3;                                             // Mask LCD SUB Interrupt
    rINTMSK    |=(BIT_LCD);                                     // Mask LCD INT
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}


/*********************************************************************************************
* name:		Test_Lcd_Tft_8Bit_240320_IntFi()
* func:		test 8bpp 240*320 TFT LCD Fifo interrupt
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_8Bit_240320_IntFi(void)
{
    int i,j,k=0;
    Lcd_port_init();
    Lcd_Palette8Bit_Init();
    Lcd_Init(MODE_TFT_8BIT_240320);
    Glib_Init(MODE_TFT_8BIT_240320);
    Lcd_Lpc3600Enable();                                        // Enable LPC3600
    Lcd_PowerEnable(0, 1);    
    rLCDCON5|=(1<<11);                                          // 5:6:5 Palette Setting
    
    for(j=0;j<320;j++)
        for(i=0;i<240;i++)
            PutPixel(i,j,((int)DEMO256[k++]));

    Lcd_EnvidOnOff(1);                                          // Enable ENVID    
    // LCD FIFO interrupt test  
    pISR_LCD=(unsigned)Lcd_Int_Fifo;
    rINTMSK  &=~(BIT_LCD);
    rLCDINTMSK=(0<<2)|(1<<1)|(0);                               // 4Words Trigger Level,Mask Frame int,Unmask Fifo int
  
    uart_printf(" Press any key to quit!\n");        
    uart_getch();
    rLCDINTMSK |=3;                                              // Mask LCD SUB Interrupt
    rINTMSK    |=(BIT_LCD);                                         // Mask LCD INT
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}



/*********************************************************************************************
* name:		Lcd_Int_Fifo_640480()
* func:		640480 LCD Fifo interrupt
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void __irq Lcd_Int_Fifo_640480(void)
{
    int i;
    rLCDINTMSK|=3;                                              // Mask LCD SUB Interrupt

    // GPG4 is .... 
    rGPGDAT&=(~(1<<4));                                         // GPG4=Low
    for(i=0;i<1;i++);                                           // GPG4=Low
    rGPGDAT|=(1<<4);                                            //GPG4=High

    rLCDSRCPND=1;                                               // Clear LCD SUB Interrupt source pending
    rLCDINTPND=1;                                               // Clear LCD SUB Interrupt pending
    rLCDINTMSK&=(~(1));                                         // Unmask LCD FRAME Interrupt
    ClearPending(BIT_LCD);
}


/*********************************************************************************************
* name:		Test_Lcd_Tft_1Bit_640480()
* func:		test 1bpp TFT LCD
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_1Bit_640480(void)
{
    int i,j;
    Lcd_Palette1Bit_Init();
    Lcd_port_init();
    Lcd_Init(MODE_TFT_1BIT_640480);
    Glib_Init(MODE_TFT_1BIT_640480);
    Lcd_PowerEnable(0, 1);    
    Lcd_EnvidOnOff(1);
    uart_printf("  TFT Mono(1bit/1pixel) LCD TEST\n");
    Glib_ClearScr(0,MODE_TFT_1BIT_640480);

    rTPAL = (1<<24)|((0xff)<<8);                                // Enable Temporary Palette : Green
    uart_printf(" TFT Mono mode test 1. Press any key!\n");
    uart_getch();  	

    rTPAL = 0;
    Glib_FilledRectangle(0,0,639,239,1);
    Glib_FilledRectangle(0,239,639,479,0);   
    uart_printf(" TFT Mono mode test 2. Press any key!\n");
    uart_getch();  	

    Glib_FilledRectangle(0,0,319,479,1);
    Glib_FilledRectangle(320,0,639,479,0);   
    uart_printf(" TFT Mono mode test 3. Press any key!\n");
    uart_getch();  	

    for(j=0;j<LCD_YSIZE_TFT_640480;j+=20)
	for(i=320;i<640;i+=20)
	    Glib_FilledRectangle(i,j,i+19,j+19,((j+i)/20)%2);
    uart_printf(" TFT Mono mode test 4. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_TFT_1BIT_640480);
    Glib_FilledRectangle(0+320,0+240,1279-320,959-240,1);
    
    // #0		    
    // 00		    
    Glib_Rectangle(0,0,639,479,1);   
    Glib_Line(0,0,639,479,1);        
    Glib_Line(0,479,639,0,1);

    // 0#
    // 00
    Glib_Rectangle(0+640,0,639+640,479,1);   
    Glib_Line(0+640,0,639+640,479,1);        
    Glib_Line(0+640,479,639+640,0,1);

    // 00
    // #0
    Glib_Rectangle(0,0+480,639,479+480,1);   
    Glib_Line(0,0+480,639,479+480,1);        
    Glib_Line(0,479+480,639,0+480,1);

    // 00
    // 0#
    Glib_Rectangle(0+640,0+480,639+640,479+480,1);   
    Glib_Line(0+640,0+480,639+640,479+480,1);        
    Glib_Line(0+640,479+480,639+640,0+480,1);
    Glib_Rectangle(50+640,50+480,639+640-50,479+480-50,1);   

    uart_printf(" Virtual Screen Test(TFT 256 color). Press any key[ijkm\\r]!\n");
    MoveViewPort(MODE_TFT_1BIT_640480);
    Lcd_MoveViewPort(0,0,MODE_TFT_1BIT_640480);
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}

/*********************************************************************************************
* name:		Test_Lcd_Tft_8Bit_640480()
* func:		test 8bpp TFT LCD
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_8Bit_640480(void)
{
    int i,j,k;
    Lcd_Palette8Bit_Init();
    Lcd_port_init();
    Lcd_Init(MODE_TFT_8BIT_640480);
    Glib_Init(MODE_TFT_8BIT_640480);
    Lcd_PowerEnable(0, 1);
    Lcd_EnvidOnOff(1);
    uart_printf("  TFT 256 COLOR(8bit/1pixel) LCD TEST\n");
    Glib_ClearScr(0, MODE_TFT_8BIT_640480);
    rTPAL = (1<<24)|((0xff)<<0);                                // Enable Temporary Palette : Blue
    uart_printf(" TFT 256 color mode test 1. Press any key!\n");
    uart_getch();  	

    k=0;
    for(i=0;i<640;i+=40)
	for(j=0;j<480;j+=30)
	{ 
	    Glib_FilledRectangle(i,j,i+39,j+29,(k%256));
	    k++;
	}
    rTPAL = 0;
    uart_printf(" TFT 256 color mode test 2. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_TFT_8BIT_640480);
    k=0;
    for(i=0;i<640;i+=20)
		for(j=0;j<480;j+=20)
		{ 
		    Glib_FilledRectangle(i,j,i+19,j+19,(k%256));
		    k+=30;
		}
    uart_printf(" TFT 256 color mode test 3. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_TFT_8BIT_640480);
    k=0;
    for(i=160;i<480;i+=20)
    	for(j=120;j<360;j+=15)
    	{
    	    Glib_FilledRectangle(i,j,i+19,j+14,k);
    	    k++;
    	}

    // #0		    
    // 00		    
    Glib_Rectangle(0,0,319,239,255);   
    Glib_Line(0,0,319,239,255);        
    Glib_Line(0,239,319,0,255);

    // 0#
    // 00
    Glib_Rectangle(0+320,0,319+320,239,255);
    Glib_Line(0+320,0,319+320,239,255);        
    Glib_Line(0+320,239,319+320,0,255);

    // 00
    // #0
    Glib_Rectangle(0,0+240,319,239+240,255);
    Glib_Line(0,0+240,319,239+240,255);        
    Glib_Line(0,239+240,319,0+240,255);

    // 00
    // 0#
    Glib_Rectangle(0+320,0+240,319+320,239+240,255);
    Glib_Line(0+320,0+240,319+320,239+240,255);     
    Glib_Line(0+320,239+240,319+320,0+240,255);
    Glib_Rectangle(50+320,50+240,269+320,189+240,255);

    uart_printf(" TFT 256 color mode test 4. Press any key!\n");
    uart_getch();  	
   
    Glib_ClearScr(0, MODE_TFT_8BIT_640480);
    k=0;
    for(i=320;i<960;i+=40)
    	for(j=240;j<720;j+=30)
    	{
    	    Glib_FilledRectangle(i,j,i+39,j+29,(k%256));
    	    k++;
    	}

    // #0		    
    // 00		    
    Glib_Rectangle(0,0,639,479,255);   
    Glib_Line(0,0,639,479,255);        
    Glib_Line(0,479,639,0,255);

    // 0#
    // 00
    Glib_Rectangle(0+640,0,639+640,479,255);   
    Glib_Line(0+640,0,639+640,479,255);        
    Glib_Line(0+640,479,639+640,0,255);

    // 00
    // #0
    Glib_Rectangle(0,0+480,639,479+480,255);   
    Glib_Line(0,0+480,639,479+480,255);        
    Glib_Line(0,479+480,639,0+480,255);

    // 00
    // 0#
    Glib_Rectangle(0+640,0+480,639+640,479+480,255);   
    Glib_Line(0+640,0+480,639+640,479+480,255);        
    Glib_Line(0+640,479+480,639+640,0+480,255);
    Glib_Rectangle(50+640,50+480,639+640-50,479+480-50,255);   

    uart_printf(" Virtual Screen Test(TFT 256 color). Press any key[ijkm\\r]!\n");
    MoveViewPort(MODE_TFT_8BIT_640480);
    Lcd_MoveViewPort(0,0,MODE_TFT_8BIT_640480);
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}

/*********************************************************************************************
* name:		Test_Lcd_Tft_8Bit_640480_Palette()
* func:		test 8bpp 640480 TFT Palette
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_8Bit_640480_Palette(void)
{
    int i,j,k;
    unsigned char cdata, p_red, p_green, p_blue;
    UINT32T *palette;
    Lcd_Palette8Bit_Init();
    Lcd_port_init();
    Lcd_Init(MODE_TFT_8BIT_640480);
    Glib_Init(MODE_TFT_8BIT_640480);
    uart_printf("  TFT 256 COLOR(8bit/1pixel) Palette TEST\n");
    Glib_ClearScr(0, MODE_TFT_8BIT_640480);
    rTPAL = (1<<24)|((0xff)<<0);                                // Enable Temporary Palette : Blue
    Lcd_PowerEnable(0, 1);
    Lcd_EnvidOnOff(1);
    uart_printf(" Temporary palette test. Press any key!\n");    
    uart_getch();  	

    k=0;
    for(i=0;i<640;i+=40)
	for(j=0;j<480;j+=30)
	{ 
	    Glib_FilledRectangle(i,j,i+39,j+29,(k%256));
	    k++;
	}
    rTPAL = 0;
    uart_printf(" Press any key to test writing palette during non-active.\n");

    palette=(UINT32T *)PALETTE;
    for(cdata=0;cdata<255;cdata++)
    {
	p_red=(cdata | 0xe0);
	p_green=(cdata | 0x1c);
	p_blue=(cdata | 0x03);

        while((rLCDCON5>>19)==2);
    	*palette++=((UINT32T)((p_red<<8)|(p_green<<6)|(p_blue<<3)));
    	uart_printf(" VSTATUS=%x\n",(rLCDCON5>>19));
    	uart_printf(" cdata=%d\n",(int)cdata);
   }
    *palette=0x0;    
    uart_getch();
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}


/*********************************************************************************************
* name:		Test_Lcd_Tft_8Bit_640480_Bswp()
* func:		test 8bpp 640480 TFT Bswp
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_8Bit_640480_Bswp(void)
{
    int k;
    Lcd_Palette8Bit_Init();
    Lcd_port_init();
    Lcd_Init(MODE_TFT_8BIT_640480);
    Glib_Init(MODE_TFT_8BIT_640480);
    Lcd_EnvidOnOff(1);
    Lcd_PowerEnable(0, 1);
    rLCDCON5 &= 0x1ffffd;                                       //BSWP Disable
    uart_printf("  TFT 256 COLOR(16bit/1pixel) BSWP TEST\n");

    Glib_ClearScr(0, MODE_TFT_8BIT_640480);
    Glib_Rectangle(0,0,639,479,0xff);
    Glib_Rectangle(1,1,638,478,0x03);
    Glib_Rectangle(0+100,0+100,639-100,479-100,0xff);
    Glib_Rectangle(1+100,1+100,638-100,478-100,0x03);
    Glib_Line(0,0,639,479,0xe0);     
    Glib_Line(639,0,0,479,0x1c);
    uart_printf(" TFT 256 color BSWP test. Press any key excet return!\n");

    while(uart_getch()!='\r')
    {
        rLCDCON5 &= 0x1ffffd;                                   //BSWP Disable
        rLCDCON5 |= ((k%2)<<1);                                 //BSWP Enable
        k++;
   }

    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}


/*********************************************************************************************
* name:		Test_Lcd_Tft_16Bit_640480()
* func:		test 16bpp 640480 TFT  
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_16Bit_640480(void)
{
    int i,j,k;
    Lcd_port_init();
    Lcd_Init(MODE_TFT_16BIT_640480);
    Glib_Init(MODE_TFT_16BIT_640480);
    Lcd_PowerEnable(0, 1);
    Lcd_EnvidOnOff(1);
    uart_printf("  TFT 64K COLOR(16bit/1pixel) LCD TEST\n");

    Glib_ClearScr(0, MODE_TFT_16BIT_640480);
    Glib_FilledRectangle(0,0,639,479,0xf800);    
 
    Glib_ClearScr(0, MODE_TFT_16BIT_640480);
    Glib_Rectangle(0,0,639,479,0x07e0);   
    Glib_FilledRectangle(0,0,20,20,65535);   
    Glib_Rectangle(620,460,639,479,65535);   
    Glib_Line(0,0,639,479,0x1f);        
    Glib_Line(639,0,0,479,0xf800);
 
    Glib_ClearScr(0, MODE_TFT_16BIT_640480);
    k=0;
    for(i=160;i<480;i+=20)
    	for(j=120;j<360;j+=15)
    	{
	    	while((rLCDCON5>>19)==2);
    	    Glib_FilledRectangle(i,j,i+19,j+14,k);
    	    k+=500;
    	}

    // #0		    
    // 00		    
    Glib_Rectangle(0,0,319,239,65535);   
    Glib_Line(0,0,319,239,65535);        
    Glib_Line(0,239,319,0,65535);

    // 0#
    // 00
    Glib_Rectangle(0+320,0,319+320,239,255);
    Glib_Line(0+320,0,319+320,239,255);        
    Glib_Line(0+320,239,319+320,0,255);

    // 00
    // #0
    Glib_Rectangle(0,0+240,319,239+240,255);
    Glib_Line(0,0+240,319,239+240,255);        
    Glib_Line(0,239+240,319,0+240,255);

    // 00
    // 0#
    Glib_Rectangle(0+320,0+240,319+320,239+240,255);
    Glib_Line(0+320,0+240,319+320,239+240,255);     
    Glib_Line(0+320,239+240,319+320,0+240,255);
    Glib_Rectangle(50+320,50+240,269+320,189+240,255);

    Glib_ClearScr(0, MODE_TFT_16BIT_640480);
    k=0;
    for(i=320;i<960;i+=40)
    	for(j=240;j<720;j+=30)
    	{
    	    Glib_FilledRectangle(i,j,i+39,j+29,k);
    	    k+=500;
    	}

    // #0		    
    // 00		    
    Glib_Rectangle(0,0,639,479,65535);   
    Glib_Line(0,0,639,479,65535);        
    Glib_Line(0,479,639,0,65535);

    // 0#
    // 00
    Glib_Rectangle(0+640,0,639+640,479,65535);   
    Glib_Line(0+640,0,639+640,479,65535);        
    Glib_Line(0+640,479,639+640,0,65535);

    // 00
    // #0
    Glib_Rectangle(0,0+480,639,479+480,65535);   
    Glib_Line(0,0+480,639,479+480,65535);        
    Glib_Line(0,479+480,639,0+480,65535);

    // 00
    // 0#
    Glib_Rectangle(0+640,0+480,639+640,479+480,65535);   
    Glib_Line(0+640,0+480,639+640,479+480,65535);        
    Glib_Line(0+640,479+480,639+640,0+480,65535);
    Glib_Rectangle(50+640,50+480,639+640-50,479+480-50,65535);   

    uart_printf(" Virtual Screen Test(TFT 64K color). Press any key[ijkm\\r]!\n");
    MoveViewPort(MODE_TFT_16BIT_640480);
    Lcd_MoveViewPort(0,0,MODE_TFT_16BIT_640480);
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}


/*********************************************************************************************
* name:		Test_Lcd_Tft_16Bit_640480_Hwswp()
* func:		test 16bpp 640480 TFT Hwswp
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_16Bit_640480_Hwswp(void)
{
    int k;
    Lcd_port_init();
    Lcd_Init(MODE_TFT_16BIT_640480);
    Glib_Init(MODE_TFT_16BIT_640480);
    Lcd_PowerEnable(0, 1);
    Lcd_EnvidOnOff(1);
    rLCDCON5 &= 0x1ffffe;                                       //HWSWP Disable
    uart_printf("  TFT 64K COLOR(16bit/1pixel) HWSWP TEST\n");

    Glib_ClearScr(0, MODE_TFT_16BIT_640480);
    Glib_Rectangle(0,0,639,479,0xffff);
    Glib_Rectangle(1,1,638,478,0xf800);
    Glib_Rectangle(0+100,0+100,639-100,479-100,0xffff);
    Glib_Rectangle(1+100,1+100,638-100,478-100,0xf800);
    Glib_Line(0,0,639,479,0x7e0);     
    Glib_Line(639,0,0,479,0x1f);
    uart_printf(" TFT 64K color HWSWP test. Press Enter key excet return!\n");

    while(uart_getch()!='\r')
    {
        rLCDCON5 &= 0x1ffffe;                                   //HWSWP Disable
        rLCDCON5 |= (k%2);                                      //HWSWP Enable
        k++;
   }

    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}


/*********************************************************************************************
* name:		Test_Lcd_Tft_1Bit_800600()
* func:		test 1bpp 800600 TFT  
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_1Bit_800600(void)
{
   // int i,j;
    Lcd_port_init();
    Lcd_Palette1Bit_Init(); 
    Lcd_Init(MODE_TFT_1BIT_800600);
    Glib_Init(MODE_TFT_1BIT_800600);
    uart_printf("  TFT Mono(1bit/1pixel) LCD TEST\n");

    rTPAL = (1<<24)|((0xff)<<0); // Enable Temporary Palette : Blue
    // LCD FIFO interrupt test 
	Lcd_EnvidOnOff(1);                                          // Enable ENVID   
	rGPGUP=rGPGUP&(~(1<<4))|(1<<4);                             // Pull-up disable
	rGPGDAT=rGPGDAT&(~(1<<4))|(1<<4);                           // GPG4=High
	rGPGCON=rGPGCON&(~(3<<8))|(1<<8);                           // GPG4=Output

	pISR_LCD=(unsigned)Lcd_Int_Fifo_640480;
	rINTMSK &=~(BIT_LCD);
	rLCDINTMSK=(0<<2)|(1<<1)|(0);                               // 4Words Trigger Level,Mask Frame int,Unmask Fifo int
    // LCD FIFO interrupt test 
    uart_printf(" TFT Mono mode test 1. Press any key!\n");
    uart_getch();  	

    rTPAL = 0;                                                  // Disable TPAL
    Glib_FilledRectangle(0,0,LCD_XSIZE_TFT_800600-1,LCD_YSIZE_TFT_800600-1,1);
    uart_printf(" TFT Mono mode test 2. Press any key!\n");
    uart_getch();  	

    Glib_FilledRectangle(200,200,LCD_XSIZE_TFT_800600-201,LCD_YSIZE_TFT_800600-201,0);   
    uart_printf(" TFT Mono mode test 3. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_TFT_1BIT_800600);
    Glib_Rectangle(200,200,SCR_XSIZE_TFT_800600-201,SCR_YSIZE_TFT_800600-201,1);
    
    // #0		    
    // 00		    
    Glib_Rectangle(0,0,LCD_XSIZE_TFT_800600-1,LCD_YSIZE_TFT_800600-1,1);   
    Glib_Line(0,0,LCD_XSIZE_TFT_800600-1,LCD_YSIZE_TFT_800600-1,1);        
    Glib_Line(0,LCD_YSIZE_TFT_800600-1,LCD_XSIZE_TFT_800600-1,0,1);

    // 0#
    // 00
    Glib_Rectangle(LCD_XSIZE_TFT_800600,0,SCR_XSIZE_TFT_800600-1,LCD_YSIZE_TFT_800600-1,1);   
    Glib_Rectangle(LCD_XSIZE_TFT_800600+100,100,SCR_XSIZE_TFT_800600-101,LCD_YSIZE_TFT_800600-101,1);   
    
    // 00
    // #0
    Glib_Rectangle(0,LCD_YSIZE_TFT_800600,LCD_XSIZE_TFT_800600-1,SCR_YSIZE_TFT_800600-1,1);   
    Glib_Rectangle(200,LCD_YSIZE_TFT_800600+200,LCD_XSIZE_TFT_800600-201,SCR_YSIZE_TFT_800600-201,1);   

    // 00
    // 0#
    Glib_Rectangle(LCD_XSIZE_TFT_800600,LCD_YSIZE_TFT_800600,SCR_XSIZE_TFT_800600-1,SCR_YSIZE_TFT_800600-1,1);   
    Glib_Line(LCD_XSIZE_TFT_800600,LCD_YSIZE_TFT_800600,SCR_XSIZE_TFT_800600-1,SCR_YSIZE_TFT_800600-1,1);        
    Glib_Line(LCD_XSIZE_TFT_800600,SCR_YSIZE_TFT_800600-1,SCR_XSIZE_TFT_800600-1,LCD_YSIZE_TFT_800600,1);

    uart_printf(" Virtual Screen Test(Mono). Press any key[ijkm\\r]!\n");
    MoveViewPort(MODE_TFT_1BIT_800600);
    Lcd_MoveViewPort(0,0,MODE_TFT_1BIT_800600);
    // LCD FIFO interrupt test 
    rLCDINTMSK |=3;                                              // Mask LCD SUB Interrupt
    rINTMSK    |=(BIT_LCD);                                         // Mask LCD INT
    // LCD FIFO interrupt test   
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}


/*********************************************************************************************
* name:		Test_Lcd_Tft_8Bit_800600()
* func:		test 8bpp 800600 TFT  
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Test_Lcd_Tft_8Bit_800600(void)
{
    int i,j,k;
    Lcd_port_init();
    Lcd_Palette8Bit_Init();
    Lcd_Init(MODE_TFT_8BIT_800600);
    Glib_Init(MODE_TFT_8BIT_800600);
    uart_printf("  TFT 256 COLOR(8bit/1pixel) LCD TEST\n");

    rTPAL = (1<<24)|((0xff)<<0);                                // Enable Temporary Palette : Blue
    // LCD FIFO interrupt test 
	Lcd_EnvidOnOff(1);                                          // Enable ENVID   
    rGPGUP=rGPGUP&(~(1<<4))|(1<<4);                             // Pull-up disable
	rGPGDAT=rGPGDAT&(~(1<<4))|(1<<4);                           // GPG4=High
	rGPGCON=rGPGCON&(~(3<<8))|(1<<8);                           // GPG4=Output

	pISR_LCD=(unsigned)Lcd_Int_Fifo_640480;
	rINTMSK   &=~(BIT_LCD);
	rLCDINTMSK =(0<<2)|(1<<1)|(0);                               // 4Words Trigger Level,Mask Frame int,Unmask Fifo int
    // LCD FIFO interrupt test 
    uart_printf(" TFT 256 color mode test 1. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_TFT_8BIT_800600);
    rTPAL = 0;
    k=0;
    for(i=0;i<LCD_XSIZE_TFT_800600;i+=40)
	for(j=0;j<LCD_YSIZE_TFT_800600;j+=30)
	{ 
	    Glib_FilledRectangle(i,j,i+39,j+29,(k%256));
	    k++;
	}

    uart_printf(" TFT 256 color mode test 2. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0xff, MODE_TFT_8BIT_800600);
    uart_printf(" TFT 256 color mode test 3. Press any key!\n");
    uart_getch();  	

    Glib_ClearScr(0, MODE_TFT_8BIT_800600);
    k=0;
    for(i=(SCR_XSIZE_TFT_800600/4);i<(SCR_XSIZE_TFT_800600*(3/4));i+=40)
    	for(j=SCR_YSIZE_TFT_800600/4;j<(SCR_YSIZE_TFT_800600*(3/4));j+=30)
    	{
    	    Glib_FilledRectangle(i,j,i+39,j+29,(k%256));
    	    k++;
    	}

    // #0		    
    // 00		    
    Glib_Rectangle(0,0,LCD_XSIZE_TFT_800600-1,LCD_YSIZE_TFT_800600-1,0xff);   
    Glib_Line(0,0,LCD_XSIZE_TFT_800600-1,LCD_YSIZE_TFT_800600-1,0xff);        
    Glib_Line(0,LCD_YSIZE_TFT_800600-1,LCD_XSIZE_TFT_800600-1,0,0xff);

    // 0#
    // 00
    Glib_Rectangle(LCD_XSIZE_TFT_800600,0,SCR_XSIZE_TFT_800600-1,LCD_YSIZE_TFT_800600-1,0xf0);   
    Glib_Rectangle(LCD_XSIZE_TFT_800600+100,100,SCR_XSIZE_TFT_800600-101,LCD_YSIZE_TFT_800600-101,0xf0);   
    
    // 00
    // #0
    Glib_Rectangle(0,LCD_YSIZE_TFT_800600,LCD_XSIZE_TFT_800600-1,SCR_YSIZE_TFT_800600-1,0x0f);   
    Glib_Rectangle(200,LCD_YSIZE_TFT_800600+200,LCD_XSIZE_TFT_800600-201,SCR_YSIZE_TFT_800600-201,0x0f);   

    // 00
    // 0#
    Glib_Rectangle(LCD_XSIZE_TFT_800600,LCD_YSIZE_TFT_800600,SCR_XSIZE_TFT_800600-1,SCR_YSIZE_TFT_800600-1,0xff);   
    Glib_Line(LCD_XSIZE_TFT_800600,LCD_YSIZE_TFT_800600,SCR_XSIZE_TFT_800600-1,SCR_YSIZE_TFT_800600-1,0xff);        
    Glib_Line(LCD_XSIZE_TFT_800600,SCR_YSIZE_TFT_800600-1,SCR_XSIZE_TFT_800600-1,LCD_YSIZE_TFT_800600,0xff);

    uart_printf(" Virtual Screen Test(TFT 256 color). Press any key[ijkm\\r]!\n");
    MoveViewPort(MODE_TFT_8BIT_800600);
    Lcd_MoveViewPort(0,0,MODE_TFT_8BIT_800600);

    rLCDINTMSK |=3;                                            // Mask LCD SUB Interrupt
    rINTMSK    |=(BIT_LCD);                                    // Mask LCD INT
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}


