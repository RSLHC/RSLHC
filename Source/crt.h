/**
  ******************************************************************************
  * @file      crt.h
  * @author    MCU Application Team 
  * @version   V1.0.0
  * @date      12-Oct-2016
  * @brief     head file crt
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

#ifndef __CRT_H
#define __CRT_H
#include "define.h"
#include "type_def.h"



extern void CRT_onoff(BYTE bi); 
extern void CRT_modesel(BYTE mode);
extern void CRT_insel(BYTE bcrtis); 
extern void CRT_remops(BYTE bremops); 
extern void CRT_clkdiv(BYTE div);
extern void CRT_REM2P30();
//void CRT_REM2P02();
extern void crt_init();
extern void CRT_carryset(int32u i);

#endif /* __CRT_H */

/******************** (C) COPYRIGHT Silan Microelectronics *****END OF FILE****/