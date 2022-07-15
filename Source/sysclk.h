/**
  ******************************************************************************
  * @file      sysclk.h
  * @author    MCU Application Team 
  * @version   V1.0.0
  * @date      12-Oct-2016
  * @brief     head file sysclk
  *            History:
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Silan Microelectronics </center></h2>
  *
  * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
  * AS A RESULT, SLMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
  * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
  * CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
  * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED.
  * 
  ******************************************************************************
  */
#include <INTRINS.H>
#include "SC51F33XX.h"
#include "define.h"
#include "common.h"

#ifndef __SYSCLK_H
#define __SYSCLK_H


#define CLK_SELRCH  0
#define CLK_SELOSCH 1

extern void clk_sel(bit b_clksel);
extern void clk_div(BYTE div_n);

#if 0
typedef enum
{
	CLKSEL_RCH = 0, //Ĭ��
	CLKSEL_CRY,
}CLK_SEL_OPT;

typedef enum
{
	CLKDIV8 = 0,	//Ĭ��
	CLKDIV4,	
	CLKDIV2,	
	CLKDIV1,
}CLK_DIV_OPT;


typedef struct
{
	CLK_SEL_OPT Clk_Src;
	CLK_DIV_OPT Clk_Div;
}MCLK_CFG_Type;

typedef enum
{
	CRY_GAIN_00 = 0, //Ĭ��
	CRY_GAIN_01,
	CRY_GAIN_10,
	CRY_GAIN_11,
}CRY_GAIN_OPT;

typedef enum
{
	CRY_TIME_15 = 0, //Ĭ��
	CRY_TIME_14,
	CRY_TIME_13,
	CRY_TIME_12,
}CRY_TIME_OPT;


typedef struct
{
	int8u CRY_En;
	int8u CRY_Time;	//����ʱ��
	CRY_GAIN_OPT Cry_Gain;
}CRY_CFG_Type;

typedef enum
{
	CKO_RCH = 0, //Ĭ��
	CKO_RCL,
	CKO_CRY,
	CKO_MCLK,
}CKO_OPT;


void Clk_CryCfg(CRY_CFG_Type *pCfg);
void Clk_MclkCfg(MCLK_CFG_Type *pCfg);
void Clk_Cko(int8u Cko);
#endif

#endif /* __SYSCLK_H */

/******************** (C) COPYRIGHT Silan Microelectronics *****END OF FILE****/