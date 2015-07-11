/*********************************************************************************************
* File£º	glib.h
* Author:	Embest	
* Desc£º	header file of general lib
* History:	
*			Y.J.Guo, Programming modify, April 12, 2005
*********************************************************************************************/
#ifndef __GLIB_H__
#define __GLIB_H__

/*------------------------------------------------------------------------------------------*/
/*	 								functions declare						                */
/*------------------------------------------------------------------------------------------*/
void Glib_Init(int type);                                       

void Glib_Line(int x1,int y1,int x2,int y2,int color);          
void Glib_Rectangle(int x1,int y1,int x2,int y2,int color);
void Glib_FilledRectangle(int x1,int y1,int x2,int y2,int color);
void Glib_ClearScr(UINT32T c,int type);

void _PutStn1Bit(UINT32T x,UINT32T y,UINT32T c);
void _PutStn2Bit(UINT32T x,UINT32T y,UINT32T c);
void _PutStn4Bit(UINT32T x,UINT32T y,UINT32T c);
void _PutCstn8Bit(UINT32T x,UINT32T y,UINT32T c);
void _PutCstn12Bit(UINT32T x,UINT32T y,UINT32T c);
void _PutTft8Bit_240320(UINT32T x,UINT32T y,UINT32T c);
void _PutTft16Bit_240320(UINT32T x,UINT32T y,UINT32T c);
void _PutTft1Bit_640480(UINT32T x,UINT32T y,UINT32T c);
void _PutTft8Bit_640480(UINT32T x,UINT32T y,UINT32T c);
void _PutTft16Bit_640480(UINT32T x,UINT32T y,UINT32T c);
void _PutTft16Bit_800480(UINT32T x,UINT32T y,UINT32T c);
void _PutTft24Bit_640480(UINT32T x,UINT32T y,UINT32T c);
void _PutTft1Bit_800600(UINT32T x,UINT32T y,UINT32T c);
void _PutTft8Bit_800600(UINT32T x,UINT32T y,UINT32T c);
void _PutTft16Bit_800600(UINT32T x,UINT32T y,UINT32T c);

//add by Y.J.GUO April 13,2005
void BitmapViewStn1Bit(UINT8T *pBuffer);
void BitmapViewStn2Bit(UINT8T *pBuffer);
void BitmapViewStn4Bit(UINT8T *pBuffer);
void BitmapViewCstn8Bit(UINT8T *pBuffer);
void BitmapViewCstn12Bit(UINT8T *pBuffer);
void BitmapViewTft8Bit_240320(UINT8T *pBuffer);
void BitmapViewTft16Bit_240320(UINT8T *pBuffer);
void BitmapViewTft1Bit_640480(UINT8T *pBuffer);
void BitmapViewTft8Bit_640480(UINT8T *pBuffer);
void BitmapViewTft16Bit_640480(UINT8T *pBuffer);
void BitmapViewTft16Bit_800480(UINT8T *pBuffer);
void BitmapViewTft24Bit_640480(UINT8T *pBuffer);
void BitmapViewTft1Bit_800600(UINT8T *pBuffer);
void BitmapViewTft8Bit_800600(UINT8T *pBuffer);
void BitmapViewTft16Bit_800600(UINT8T *pBuffer);

/*------------------------------------------------------------------------------------------*/
/*	 								extern functions declare                                */
/*------------------------------------------------------------------------------------------*/
extern void (*PutPixel)(UINT32T,UINT32T,UINT32T);
extern void (*BitmapView)(UINT8T *pBuffer);

#endif                                                          //__GLIB_H__
