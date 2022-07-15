/**
  ******************************************************************************
  * @file      crt.c
  * @author    MCU Application Team 
  * @version   V1.0.0
  * @date      12-Oct-2016
  * @brief     crt functions
  *            History:
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 Silan Microelectronics </center></h2>
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

#include "SC51F33XX.h"
#include "crt.h"
/**
  * @brief Carrier function ON OFF
  * @param bi enable CRT or disable CRT
  *     @arg 0---off
  *     @arg 1---on
  * @retval None
  */
void CRT_onoff(BYTE bi)
{
    if(bi == CRT_ON)
        CRTCON1 |= (1<<CRTEN);  // enable CRT
    else //CRT_OFF
        CRTCON1 &= ~(1<<CRTEN); // diable CRT
}

/**
  * @brief Carrier mode select
  * @param bmode: modulate or demodulate
  *     @arg 0---modulate
  *     @arg 1---demodulate
  * @retval None
  */
void CRT_modesel(BYTE bmode)
{
	if(bmode == CRT_DEMODULATE)
        CRTCON1 |= (1<<CRTMODE); // demodulation mode.
    else // CRT_MODULATE
        CRTCON1 &= ~(1<<CRTMODE); // modulation mode
}

/**
  * @brief Carrier input source select
  * @param bcrtis
  *     @arg 0---select P3.0
  *     @arg 1---select T168 out
  * @retval None
  */
void CRT_insel(BYTE bcrtis)
{
	if(bcrtis == CRT_INT168)
        CRTCON1 |= (1<<CRTIS); // T168
    else // CRT_INP30
        CRTCON1 &= ~(1<<CRTIS); // P3.0
}

/**
  * @brief Carrier out phase invert
  * @param bremops
  *     @arg 0---positive output: CRTDATA0 is the low width of Carrier T, CRTDATA1 is the High...
  *     @arg 1---inverted output: CRTDATA0 is the High width of Carrier T, CRTDATA1 is the Low...
  * @retval None
  */
void CRT_remops(BYTE bremops)
{
	if(bremops == CRT_REMOPS1)
        CRTCON1 |= (1<<REMOPS); // inverted output.
    else // CRT_REMOPS0
        CRTCON1 &= ~(1<<REMOPS);  // positive output, 
                                  // CRTDATA0 sets the low voltage width and 
                                  // CRTDATA1 sets the high voltage width.
}

/**
  * @brief Carrier clock divider
  * @param div Fclk/div bit3~bit0 of div is valid.
  * @retval None
  */
void CRT_clkdiv(BYTE div)
{
	BYTE read;
	read = CRTCON2;
	read &= 0xF0;
	read |= div;
	CRTCON2 = read;
}

/**
  * @brief REM map to GPIO P3.0
  * @retval None
  */
void CRT_REM2P30()
{
	xIOMUX0 |= (1<<REMEN); // enable REM multiplex.
	xIOMUX2 &= ~(1<<CRTOE); // disable CRTOUT multiplex
}

#if 0
void CRT_carryset(int32u i)
{
	CRTDATA0 = i;
}
#endif


/******************** (C) COPYRIGHT Silan Microelectronics *****END OF FILE****/