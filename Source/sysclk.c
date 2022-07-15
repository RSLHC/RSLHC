/**
  ******************************************************************************
  * @file      sysclk.c
  * @author    MCU Application Team 
  * @version   V1.0.0
  * @date      12-Oct-2016
  * @brief     sysclk
  *            History:
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 Silan Microelectronics </center></h2>
  *
  * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
  * AS A RESULT, SILAN MICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
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
#include "sysclk.h"

/**
  * @brief select the source of MCLK
  * @param  b_clksel 
  *     @arg CLK_SELRCH
  *     @arg CLK_SELOSCH
  * @retval None
  */
void clk_sel(bit b_clksel)
{
    volatile bit bx;
	BYTE read;
	read = MCKSET;
   
	if(b_clksel == CLK_SELOSCH) // CLK_SELOSCH 1       // Exterbal Oscilator
	{
		P0MDL &= 0xF0;
		/* IO enable osc fuction */
		read &= ~(1<<CRYIOEN); 	// P0[1:0] used as external oscillator input/output.
		read |= (1<<CRYEN); 	// enable CRYH.
        HAL_ENTER_CRITICAL(bx);
        WPKEY = 0x37; 			// Writing 37H to WPKEY register will remove write-protection,
					  			// and data should be written to the key register in 3 instruction periods, 
					  			// then the write-protection will reactive.
		MCKSET = read;
        t0_wait(1);        /* to wait osc clock stable */

		/* switch to OSCH£¬close RCH */	
        read |= (1<<MCKS); 		// SYSCLK is from CRYH.
		WPKEY = 0x37;
		MCKSET = read;
	}
	else  //  CLK_SELRCH  0    	// Internal Oscilator
	{
		/* switch to RCH */
		read &= ~ (1<<MCKS); 	//SYSCLK is from RCH (default).
        HAL_ENTER_CRITICAL(bx);
		WPKEY = 0x37; 			// Writing 37H to WPKEY register will remove write-protection,
					  			// and data should be written to the key register in 3 instruction periods, 
					  			// then the write-protection will reactive.
		MCKSET = read;
		read = FALSE;

        /* wait for switching finish */
		while(1) 
		{
			if( (MCKSET&0x04) == 0x00)
				break;
		}		

		/* close osc£¬set as GPIO */
		read = MCKSET; 
		read |= (1<<CRYIOEN); 	// P0[1:0] used as GPIO(default).
		read &= ~(1<<CRYEN);  	// disable CRYH (default).
		WPKEY = 0x37;
		MCKSET = read;
	}
    
    HAL_EXIT_CRITICAL(bx);
}

/**
  * @brief MCLK division
  * @param  div_n: can be 1,2,4, or 8 
  * @retval None
  */
void clk_div(BYTE div_n)
{
	BYTE read;
    volatile bit bx;    
	read = MCKSET;
	read &= ~(1<<MDIV1 | 1<<MDIV0);

	switch(div_n)
	{
								// MDIV[1:0]
		case DEVIDED_FREQ_8:
				read |= 0x00; 	// 00: MCLK frequency is 8 divided-frequency of SYSCLK
				break;				
		case DEVIDED_FREQ_4:
				read |= 0x01;	// 01: MCLK frequency is 4 divided-frequency of SYSCLK.
				break;				
		case DEVIDED_FREQ_2:
				read |= 0x02;	// 10: MCLK frequency is 2 divided-frequency of SYSCLK.
				break;				
		case DEVIDED_FREQ_1:
				read |= 0x03;	// 11: MCLK frequency is same as SYSCLK	
				break;		
		default:
				read = MCKDIV;
				break;
	}
  HAL_ENTER_CRITICAL(bx);
	WPKEY = 0x37;
	MCKDIV = read;
  _nop_();
  _nop_();
  HAL_EXIT_CRITICAL(bx);
}

/******************** (C) COPYRIGHT Silan Microelectronics *****END OF FILE****/