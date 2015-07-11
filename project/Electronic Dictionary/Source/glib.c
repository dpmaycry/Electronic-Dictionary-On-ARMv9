/*********************************************************************************************
* File£º	glib.c
* Author:	Embest	
* Desc£º	LCD_Test
* History:	
*********************************************************************************************/

/*------------------------------------------------------------------------------------------*/
/*	 								include files						 				    */
/*------------------------------------------------------------------------------------------*/
#include "def.h"
#include "lcdlib.h"
#include "glib.h"
#include "lcd.h"

/*------------------------------------------------------------------------------------------*/
/*	 								functions declare                                       */
/*------------------------------------------------------------------------------------------*/
void (*PutPixel)(UINT32T,UINT32T,UINT32T);
void (*BitmapView)(UINT8T *pBuffer);

/*********************************************************************************************
* name:		Glib_Init()
* func:		Glib initialization
* para:		type    --  lcd display mode
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Glib_Init(int type)
{
    switch(type)
    {
	    case MODE_STN_1BIT:
	    	PutPixel=_PutStn1Bit;
	    	BitmapView=BitmapViewStn1Bit;
	    	break;
	    case MODE_STN_2BIT:
	       	PutPixel=_PutStn2Bit;
	       	BitmapView=BitmapViewStn2Bit;
	       	break;
	    case MODE_STN_4BIT:
	        PutPixel=_PutStn4Bit;
	        BitmapView=BitmapViewStn4Bit;
	        break;
	    case MODE_CSTN_8BIT:
	    	PutPixel=_PutCstn8Bit;
	    	BitmapView=BitmapViewCstn8Bit;
	    	break;   
	    case MODE_CSTN_12BIT:
	    	PutPixel=_PutCstn12Bit;
	    	BitmapView=BitmapViewCstn12Bit;
	    	break;   
	    case MODE_TFT_8BIT_240320:
	    	PutPixel=_PutTft8Bit_240320;
	    	BitmapView=BitmapViewTft8Bit_240320;  	
	    	break;   
	    case MODE_TFT_16BIT_240320:
	    	PutPixel=_PutTft16Bit_240320;
	    	BitmapView=BitmapViewTft16Bit_240320;  	
	    	break;   
	    case MODE_TFT_1BIT_640480:
	    	PutPixel=_PutTft1Bit_640480;
	    	BitmapView=BitmapViewTft1Bit_640480;
	    	break;   
	    case MODE_TFT_8BIT_640480:
	    	PutPixel=_PutTft8Bit_640480;
	    	BitmapView=BitmapViewTft8Bit_640480;
	    	break;   
	    case MODE_TFT_16BIT_640480:
	    	PutPixel=_PutTft16Bit_640480;
	    	BitmapView=BitmapViewTft16Bit_640480;
	    	break;   
		case MODE_TFT_16BIT_800480:
	    	PutPixel=_PutTft16Bit_800480;
	    	BitmapView=BitmapViewTft16Bit_800480;
	    	break;   
	    case MODE_TFT_24BIT_640480:
	    	PutPixel=_PutTft24Bit_640480;
	    	BitmapView=BitmapViewTft24Bit_640480;
	    	break;   
	    case MODE_TFT_1BIT_800600:
	    	PutPixel=_PutTft1Bit_800600;
	    	BitmapView=BitmapViewTft1Bit_800600;
	    	break;   
	    case MODE_TFT_8BIT_800600:
	    	PutPixel=_PutTft8Bit_800600;
	    	BitmapView=BitmapViewTft8Bit_800600;
	    	break;   
	    case MODE_TFT_16BIT_800600:
	    	PutPixel=_PutTft16Bit_800600;
	    	BitmapView=BitmapViewTft16Bit_800600;
	    	break;   
	    default: 
	    	break;
   }
}

/*********************************************************************************************
* name:		_PutStn1Bit()
* func:		put pixel to 1bpp stn
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutStn1Bit(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_STN&& y<SCR_YSIZE_STN)
	frameBuffer1Bit[(y)][(x)/32]=( frameBuffer1Bit[(y)][(x)/32]
	& ~(0x80000000>>((x)%32)*1) ) | ( (c&0x00000001)<< ((32-1-((x)%32))*1) );
}

/*********************************************************************************************
* name:		_PutStn2Bit()
* func:		put pixel to 2bpp stn
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutStn2Bit(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_STN&& y<SCR_YSIZE_STN)
        frameBuffer2Bit[(y)][(x)/16]=( frameBuffer2Bit[(y)][x/16]
	& ~(0xc0000000>>((x)%16)*2) ) | ( (c&0x00000003)<<((16-1-((x)%16))*2) );
}

/*********************************************************************************************
* name:		_PutStn4Bit()
* func:		put pixel to 4bpp stn
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutStn4Bit(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_STN&& y<SCR_YSIZE_STN)
        frameBuffer4Bit[(y)][(x)/8]=( frameBuffer4Bit[(y)][x/8]
	& ~(0xf0000000>>((x)%8)*4) ) | ( (c&0x0000000f)<<((8-1-((x)%8))*4) );
}

/*********************************************************************************************
* name:		_PutCstn8Bit()
* func:		put pixel to 8bpp color stn
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutCstn8Bit(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_CSTN&& y<SCR_YSIZE_CSTN)
        frameBuffer8Bit[(y)][(x)/4]=( frameBuffer8Bit[(y)][x/4]
	    & ~(0xff000000>>((x)%4)*8) ) | ( (c&0x000000ff)<<((4-1-((x)%4))*8) );
}

/*********************************************************************************************
* name:		_PutCstn12Bit()
* func:		put pixel to 12bpp color stn
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutCstn12Bit(UINT32T x,UINT32T y,UINT32T c)
{
    UINT32T z;
    z=((x)%8);
    if(x<SCR_XSIZE_CSTN&& y<SCR_YSIZE_CSTN)
    {

        if((z%3)!=2)
            frameBuffer12Bit[(y)][(x)*3/8]=
            ( frameBuffer12Bit[(y)][(x)*3/8] & ~(0xfff00000>>(((z/3)*4)+((z)%3)*12) )
		| ( (c&0xfff)<<(20-(((z/3)*4)+((z)%3)*12))) );
        else
        {
            if(z==2)
            {
                frameBuffer12Bit[(y)][(x)*3/8]=( (frameBuffer12Bit[(y)][(x)*3/8]
                								& ~(0xff)) | ((c&0xff0)>>4)  ); 
                frameBuffer12Bit[(y)][((x)*3/8)+1]=( (frameBuffer12Bit[(y)][((x)*3/8)+1]
		    										& ~(0xf0000000)) | ((c&0xf)<<28)  );
           }   
            else if(z==5)
            {
                frameBuffer12Bit[(y)][(x)*3/8]=( (frameBuffer12Bit[(y)][(x)*3/8]
	            & ~(0xf)) | ((c&0xf00)>>8)  );
                frameBuffer12Bit[(y)][((x)*3/8)+1]=( (frameBuffer12Bit[(y)][((x)*3/8)+1]
	            & ~(0xff000000)) | ((c&0xff)<<24)  );
           }   
       }
   }
}

/*********************************************************************************************
* name:		_PutTft8Bit_240320()
* func:		put pixel to 8bpp 240*320 TFT
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutTft8Bit_240320(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_TFT_240320 && y<SCR_YSIZE_TFT_240320)
        frameBuffer8BitTft240320[(y)][(x)/4]=( frameBuffer8BitTft240320[(y)][x/4]
        & ~(0xff000000>>((x)%4)*8) ) | ( (c&0x000000ff)<<((4-1-((x)%4))*8) );
}

/*********************************************************************************************
* name:		_PutTft16Bit_240320()
* func:		put pixel to 16bpp 240*320 TFT
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutTft16Bit_240320(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_TFT_240320 && y<SCR_YSIZE_TFT_240320)
        frameBuffer16BitTft240320[(y)][(x)/2]=( frameBuffer16BitTft240320[(y)][x/2]
        & ~(0xffff0000>>((x)%2)*16) ) | ( (c&0x0000ffff)<<((2-1-((x)%2))*16) );
}

/*********************************************************************************************
* name:		_PutTft1Bit_640480()
* func:		put pixel to 1bpp 640*480 TFT
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutTft1Bit_640480(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_TFT_640480 && y<SCR_YSIZE_TFT_640480)
        frameBuffer1BitTft640480[(y)][(x)/32]=( frameBuffer1BitTft640480[(y)][x/32]
	& ~(0x80000000>>((x)%32)*1) ) | ( (c&0x00000001)<< ((32-1-((x)%32))*1) );
}

/*********************************************************************************************
* name:		_PutTft8Bit_640480()
* func:		put pixel to 8bpp 640*480 TFT
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutTft8Bit_640480(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_TFT_640480 && y<SCR_YSIZE_TFT_640480)
        frameBuffer8BitTft640480[(y)][(x)/4]=( frameBuffer8BitTft640480[(y)][x/4]
        & ~(0xff000000>>((x)%4)*8) ) | ( (c&0x000000ff)<<((4-1-((x)%4))*8) );
}

/*********************************************************************************************
* name:		_PutTft16Bit_640480()
* func:		put pixel to 16bpp 640*480 TFT
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutTft16Bit_640480(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_TFT_640480 && y<SCR_YSIZE_TFT_640480)
      frameBuffer16BitTft640480[(y)][(x)/2]=( frameBuffer16BitTft640480[(y)][x/2]
        & ~(0xffff0000>>((x)%2)*16) ) | ( (c&0x0000ffff)<<((2-1-((x)%2))*16) );				 

}

void _PutTft16Bit_800480(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_TFT_800480 && y<SCR_YSIZE_TFT_800480)
      frameBuffer16BitTft800480[(y)][(x)/2]=( frameBuffer16BitTft800480[(y)][x/2]
        & ~(0xffff0000>>((x)%2)*16) ) | ( (c&0x0000ffff)<<((2-1-((x)%2))*16) );				 

}

/*********************************************************************************************
* name:		_PutTft24Bit_640480()
* func:		put pixel to 24bpp 640*480 TFT
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutTft24Bit_640480(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_TFT_640480 && y<SCR_YSIZE_TFT_640480)
        frameBuffer24BitTft640480[(y)][(x)]=( frameBuffer24BitTft640480[(y)][(x)]
        & (0x0) | ( c&0xffffff00)); // | ( c&0x00ffffff)); LSB 
}


/*********************************************************************************************
* name:		_PutTft1Bit_800600()
* func:		put pixel to 1bpp 800*600 TFT
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutTft1Bit_800600(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_TFT_800600 && y<SCR_YSIZE_TFT_800600)
        frameBuffer1BitTft800600[(y)][(x)/32]=( frameBuffer1BitTft800600[(y)][x/32]
	& ~(0x80000000>>((x)%32)*1) ) | ( (c&0x00000001)<< ((32-1-((x)%32))*1) );
}


/*********************************************************************************************
* name:		_PutTft8Bit_800600()
* func:		put pixel to 8bpp 800*600 TFT
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutTft8Bit_800600(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_TFT_800600 && y<SCR_YSIZE_TFT_800600)
        frameBuffer8BitTft800600[(y)][(x)/4]=( frameBuffer8BitTft800600[(y)][x/4]
        & ~(0xff000000>>((x)%4)*8) ) | ( (c&0x000000ff)<<((4-1-((x)%4))*8) );
}


/*********************************************************************************************
* name:		_PutTft16Bit_800600()
* func:		put pixel to 16bpp 800*600 TFT
* para:		UINT32T x   --  x coordinate
*           UINT32T y   --  y coordinate
*           UINT32T c   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void _PutTft16Bit_800600(UINT32T x,UINT32T y,UINT32T c)
{
    if(x<SCR_XSIZE_TFT_800600 && y<SCR_YSIZE_TFT_800600)
        frameBuffer16BitTft800600[(y)][(x)/2]=( frameBuffer16BitTft800600[(y)][x/2]
        & ~(0xffff0000>>((x)%2)*16) ) | ( (c&0x0000ffff)<<((2-1-((x)%2))*16) );
}

//add by Y.J.GUO April 13,2005
void BitmapViewStn1Bit(UINT8T *pBuffer)
{
     
}
void BitmapViewStn2Bit(UINT8T *pBuffer)
{
    
}
void BitmapViewStn4Bit(UINT8T *pBuffer)
{

}

/*********************************************************************************************
* name:		BitmapViewCstn8Bit
* func:		display a 8bpp bitmap  on 256 color LCD
* para:		UINT8T *pBuffer --  bitmap data pointer          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void BitmapViewCstn8Bit(UINT8T *pBuffer)
{
    UINT32T i, j;
	UINT32T *pView = (UINT32T*)frameBuffer8Bit;

	for (i = 0; i < SCR_YSIZE_STN; i++)
	{
	    for (j = 0; j < LCD_XSIZE_STN/4; j++)
	    {
		pView[j] = ((*pBuffer) << 24) + ((*(pBuffer+1)) << 16) + ((*(pBuffer+2)) << 8) + (*(pBuffer+3));
		pBuffer += 4;
	   }
	    pView+=SCR_XSIZE_STN/4;
	}

}
void BitmapViewCstn12Bit(UINT8T *pBuffer)
{
    
}
void BitmapViewTft8Bit_240320(UINT8T *pBuffer)
{
    
}
void BitmapViewTft16Bit_240320(UINT8T *pBuffer)
{
    
}  	
void BitmapViewTft1Bit_640480(UINT8T *pBuffer)
{
    
}
void BitmapViewTft8Bit_640480(UINT8T *pBuffer)
{
    
}

void BitmapViewTft16Bit_640480(UINT8T *pBuffer)
{
    UINT32T i, j;
	UINT32T *pView = (UINT32T*)frameBuffer16BitTft640480;

	for (i = 0; i < LCD_YSIZE_TFT_640480; i++)
	{
	   
	   
	   for (j = 0; j < LCD_XSIZE_TFT_640480/2 ; j++)
	    {
	      
	      pView[j] = ((*(pBuffer+3)) << 24) + ((*(pBuffer+2)) << 16) + ((*(pBuffer+1)) << 8) + (*(pBuffer));
		 
		  pBuffer += 4;
	    
	    }
	    pView+=LCD_XSIZE_TFT_640480;
	}

    
}

void BitmapViewTft16Bit_800480(UINT8T *pBuffer)
{
    UINT32T i, j;
	UINT32T *pView = (UINT32T*)frameBuffer16BitTft800480;

	for (i = 0; i < LCD_YSIZE_TFT_800480; i++)
	{
	   for (j = 0; j < LCD_XSIZE_TFT_800480/2 ; j++)
	    {
	      
	      pView[j] = ((*(pBuffer+1)) << 24) + ((*(pBuffer)) << 16) + ((*(pBuffer+3)) << 8) + (*(pBuffer+2));
		 
		  pBuffer += 4;
	    
	    }
	    pView+=LCD_XSIZE_TFT_800480;
	}
   
}
void BitmapViewTft24Bit_640480(UINT8T *pBuffer)
{
    
}
void BitmapViewTft1Bit_800600(UINT8T *pBuffer)
{
    
}
void BitmapViewTft8Bit_800600(UINT8T *pBuffer)
{
    
}
void BitmapViewTft16Bit_800600(UINT8T *pBuffer)
{
    
}

/*********************************************************************************************
* name:		Glib_Rectangle()
* func:		describe a Rectangle
* para:		int x1      --  x coordinate 1
*           int y1      --  y coordinate 1
*           int x2      --  x coordinate 2
*           int y2      --  y coordinate 2
*           int color   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Glib_Rectangle(int x1,int y1,int x2,int y2,int color)
{
    Glib_Line(x1,y1,x2,y1,color);
    Glib_Line(x2,y1,x2,y2,color);
    Glib_Line(x1,y2,x2,y2,color);
    Glib_Line(x1,y1,x1,y2,color);
}

/*********************************************************************************************
* name:		Glib_FilledRectangle()
* func:		describe a FilledRectangle
* para:		int x1      --  x coordinate 1
*           int y1      --  y coordinate 1
*           int x2      --  x coordinate 2
*           int y2      --  y coordinate 2
*           int color   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Glib_FilledRectangle(int x1,int y1,int x2,int y2,int color)
{
    int i;

    for(i=y1;i<=y2;i++)
	Glib_Line(x1,i,x2,i,color);
}



/*********************************************************************************************
* name:		Glib_Line()
* func:		describe a Line
* para:		int x1      --  x coordinate 1
*           int y1      --  y coordinate 1
*           int x2      --  x coordinate 2
*           int y2      --  y coordinate 2
*           int color   --  color value          
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Glib_Line(int x1,int y1,int x2,int y2,int color)
{
	int dx,dy,e;
	dx=x2-x1; 
	dy=y2-y1;
    
	if(dx>=0)
	{
		if(dy >= 0)                                             // dy>=0
		{
			if(dx>=dy)                                          // 1/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else                                                // 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else                                                    // dy<0
		{
			dy=-dy;                                             // dy=abs(dy)

			if(dx>=dy)                                          // 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else                                                // 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
	else                                                        //dx<0
	{
		dx=-dx;                                                 //dx=abs(dx)
		if(dy >= 0)                                             // dy>=0
		{
			if(dx>=dy)                                          // 4/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else                                                // 3/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else                                                    // dy<0
		{
			dy=-dy;                                             // dy=abs(dy)

			if(dx>=dy)                                          // 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else                                                // 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
}


/*********************************************************************************************
* name:		Glib_ClearScr()
* func:		Clear screen with color value c
* para:		UINT32T c   --  color value 
*           int type    --  lcd display type         
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Glib_ClearScr(UINT32T c, int type)
{	
    int i,j;

    if((type&0x4000)&&(type&0x400))                                 // type:MODE_TFT_1BIT_800600|MODE_TFT_8BIT_800600|MODE_TFT_16BIT_800600
        for(j=0;j<SCR_YSIZE_TFT_800600;j++)
            for(i=0;i<SCR_XSIZE_TFT_800600;i++)
		        PutPixel(i,j,c);


   // else if((type&0x4000)&&(type&0x200))                            // type:MODE_TFT_1BIT_640480|MODE_TFT_8BIT_640480|MODE_TFT_16BIT_640480
     //   for(j=0;j<SCR_YSIZE_TFT_640480;j++)
     //       for(i=0;i<SCR_XSIZE_TFT_640480;i++)
	//	        PutPixel(i,j,c);

	    else if((type&0x4000)&&(type&0x200))                            // type:MODE_TFT_1BIT_640480|MODE_TFT_8BIT_640480|MODE_TFT_16BIT_640480
        for(j=0;j<20;j++)
            for(i=0;i<20;i++)
		        PutPixel(i,j,c);


    else if((type&0x4000)&&(type&0x100))                            // type:MODE_TFT_1BIT_240320|MODE_TFT_8BIT_240320|MODE_TFT_16BIT_240320
        for(j=0;j<SCR_YSIZE_TFT_240320;j++)
            for(i=0;i<SCR_XSIZE_TFT_240320;i++)
		        PutPixel(i,j,c);
    else if(type&0x2000)                                            // type:MODE_CSTN_8BIT|MODE_CSTN_12BIT
        for(j=0;j<SCR_YSIZE_CSTN;j++)
    	    for(i=0;i<SCR_XSIZE_CSTN;i++)
		        PutPixel(i,j,c);
    else                                                            // type:MODE_CSTN_4BIT|MODE_STN_2BIT|MODE_STN_1BIT
        for(j=0;j<SCR_YSIZE_STN;j++)
    	    for(i=0;i<SCR_XSIZE_STN;i++)
		        PutPixel(i,j,c);
}
