
#ifndef __MAIN_C__
#define __MAIN_C__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "main.h"
#include "hal.h"
#include "common.h"
#include "function_key.h"
#include "crt.h"

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/
/*****************************************************************************/
/* Main Routine */
/*****************************************************************************/
//1st commit test

void main(void)
{
	HAL_GLOBAL_INTERRUPT_DIS();

	SystemInit();
	InitPort();
	reset_define();
	rsAppMain();
}


/*****************************************************************************
 *  DESCRIPTION:    Begins Stop mode,
 *
 *  PARAMETERS:     None
 *
 *  RETURNS:        None
 *****************************************************************************/
void Stop_Mode(void)
{
	EA = 1;

	/* to reduce power consumption, close internal VBG, closed LVR, LVD functions */
    /* But if xSTOPCFG.3(PWRCFG)==1, the LVRCON must be kept enabled before go to stop mode, 
       then the LVR(VBG) automatically closed by hardware when entered stop mode, and it will be 
       enabled again at the time waking from stop mode by hardware */
    if(xSTOPCFG & 0x08) /* very important here */
    {
	    LVRCON |= 0x20; 
        /* if LVR not enabled before, code here to be wrote: delay 20us */
        LVRCON |= 0x80;
    }
    /* make sure the two WPKEY and PCON consecutive instructions 
       are not interrupted*/
    /* make sure no KBI key pressed, if not, the entering to stop
       mode won't be succeed */
	WPKEY = 0x37;
	PCON = 0x02;  /* goto stop mode, 
                     if succeed, instuction will stop here, 
                     if failed, go to the next instruction */
    /* waked up , LVR enabled again by hardware if((xSTOPCFG.3==1) and (LVR enabled before go to stop mode)) */
    _nop_();
    _nop_();
}


void External_Interrupt_0() interrupt 0
{
}

void Timer0_Interrupt(void) interrupt 1 //65ms 
{
	if(Timer0_65ms16)		Timer0_65ms16--;

	if(Timer0_65ms16_2)		Timer0_65ms16_2--;

	if(Timer0_keyin_chk)	Timer0_keyin_chk--;
	
#if (USE_AVSHIFT_MODE == TRUE)
	if(Timer0_avShiftMode)	
		Timer0_avShiftMode--;

	if(!Timer0_avShiftMode)	
		avShiftMode_active = 0;
#endif

}

void External_Interrupt_1() interrupt 2
{
}


void Timer1_Interrupt(void) interrupt 3 //1sec
{
	#if (USE_STUCKKEY_TIMER == TRUE)
	if(curr_state == NORMAL_MODE)
	{
		#if 1
		// StuckKey time
		if(Timer1_1sec08)
		{
			Timer1_1sec08--;
		}
		else
		{
			StuckKeyTimer_Stop;
			CRT_Stop;
			CRT_onoff(CRT_OFF);
			ir_ok = FALSE;
			t1_int = 2;
			device_led_off(IR_SENDING_LED);
			while(getkey_simple()){}
			my_newkeyincond();
		}
		#endif
	}
#endif

#if (USE_IR_LEARN == TRUE)
	else if(curr_state == SET_MODE_LEARN)
	{		
		if(NoSinalCnt)
		{
			NoSinalCnt--;
			bT3OverFlow = TRUE;
		}
		else
		{
			/* Interrupt Disable */
			//IE2 &= ~(1<<INT16E_Timer3);	/* Disable Timer 3 Interrupt */

			// Any carriery
			if(ipc_cnt == 0 && learn_type == CARRIER_TYPE)
			{
				//IR capture mode is disabled
				IRCC1 &= _0111_1111B;
				*pStream16u++ = 0xFFFF;
				ipc_cnt++;
			}

			return_int8u = 1;
		}
	}
#endif

	
}

void UART0_Interrupt() interrupt 4
{
}

void Spectial_F0_High_Envelop()
{
	CRT_Run;													//high pointer
	if (ramFormat.strMember.timeval[0] == 3)
		Delayby10us(11);
	else						
		Delayby10us(1);
	CRT_Stop;
}

void Timer2_Interrupt(void) interrupt 5 using 1 //Envelop Timer
{
	//flash type.
	if(ramFormat.strMember.option1.flash)
	{
#if (USE_SPECIAL_0F0H == TRUE)
		if(ramFormat.strMember.outorder[0] == 0xf0) 		// 0FO Special 
		{
			if( (reload_time == FALSE))
			{
				if(before_reload_state == TRUE)
				{
					before_reload_state = FALSE;
					CRT_Stop;
				}
				else
					Spectial_F0_High_Envelop();
			}
			else
			{
				if(before_reload_state == TRUE)
				{
					if(continuous_reload == FALSE)					
						Spectial_F0_High_Envelop();
				}
			}
			
			if( (wTimer&0x7FFF) > wTimer_Max)
			{
				Envelop_time = Envelop_time_max;
				Envelop_counter = Envelop_counter_max; 
				wTimer -= wTimer_Max;
				if(before_reload_state == TRUE)
					continuous_reload = TRUE;
				else
					continuous_reload = FALSE;
				reload_time = TRUE;
				before_reload_state = TRUE;
			}
			else
			{
				Envelop_time = (carrier_time_us*(wTimer&0x7FFF)); 
				Envelop_counter = 65536 - (Envelop_time / 2) - ((carrier_time_ns * (wTimer&0x7FFF)/10)/2); //Timing 계산으로 인해 발생한 nsec 단위 손실에 대한 보상 값
				uptr_Timer++;											//next high pointer
				wTimer = *uptr_Timer;
				reload_time = FALSE;
			}

			RCAP2H = (Envelop_counter >> 8);
			RCAP2L = Envelop_counter & 0x00FF;
		}
		else
#endif
		{
			if( Before_CRT_State == RUN)
				CRT_Run;

			Delayby10us(1); 
			CRT_Stop;
		
			if(wTimer & 0x8000)
				Before_CRT_State = RUN;
			else 
				Before_CRT_State = STOP;


			if(reload_time == FALSE)
			{
				uptr_Timer++;											//next high pointer
				wTimer = *uptr_Timer;
			}

			if(non_special_flash_start == TRUE)
			{
				non_special_flash_start = FALSE;
				uptr_Timer++;											//next high pointer
				wTimer = *uptr_Timer;
			}


			if( (wTimer&0x7FFF) > wTimer_Max)
			{
				Envelop_time = Envelop_time_max;
				Envelop_counter = Envelop_counter_max; //Timing 계산으로 인해 발생한 nsec 단위 손실에 대한 보상 값
				wTimer -= wTimer_Max;
				reload_time = TRUE;
			}
			else
			{
				Envelop_time = (carrier_time_us*(wTimer&0x7FFF)); 
				Envelop_counter = 65536 - (Envelop_time / 2) - ((carrier_time_ns * (wTimer&0x7FFF)/10)/2); //Timing 계산으로 인해 발생한 nsec 단위 손실에 대한 보상 값
				uptr_Timer++;											//next high pointer
				wTimer = *uptr_Timer;
				reload_time = FALSE;
			}

			RCAP2H = (Envelop_counter >> 8);
			RCAP2L = Envelop_counter & 0x00FF;
		}
	}
	//carrier type
	else
	{
#if (USE_SPECIAL_0F4H == TRUE)
		if(ramFormat.strMember.outorder[0] == 0xf4) 	// 0F4 Special 
		{
			F4_carrier = pramCode->Tunion.Tstruct.bCarrierArray[f4_cnt++];

			if (reload_time == TRUE)
			{
				f4_cnt--;
				reload_time = FALSE;
			}

			if(F4_carrier == 0x70 || F4_carrier == 0x6f)	F4_carrier = 115;	//34.7KHz
			else											F4_carrier = 104;	//38.4KHz

			F4_carrier = F4_carrier*2;
			
			carrier_time_us =  (125	* F4_carrier)	/	1000; //usec 단위
			//				8MHz = 125ns		  	usec단위로 맞추기 위함

			carrier_time_ns =  ((125 * F4_carrier)	% 1000)/100; //100nsec 단위

			Envelop_time_max = (carrier_time_us*wTimer_Max);
			Envelop_counter_max = 65536 - (Envelop_time_max / 2) - ((carrier_time_ns * (wTimer_Max/10))/2); //Timing 계산으로 인해 발생한 nsec 단위 손실에 대한 보상 값

			wTimer_Max = (0xFFFF / carrier_time_us) - 2;

			CRTDATA0 = 255 - F4_carrier/3;
			CRTDATA1 = 255 - (F4_carrier-F4_carrier/3);
		}
#endif
		if( Before_CRT_State == RUN)
			CRT_Run;
		else
			CRT_Stop;
		
		
		if(wTimer & 0x8000)
			Before_CRT_State = RUN;
		else 
			Before_CRT_State = STOP;

		if( (wTimer&0x7FFF) > wTimer_Max)	// Envelop_time 계산 결과가 16bit를 초과하였을 경우
		{
			Envelop_time = Envelop_time_max; 
			Envelop_counter = Envelop_counter_max; 
			wTimer -= wTimer_Max;
			if(ramFormat.strMember.outorder[0] == 0xf4)
				f4_cnt--;
		}
		else
		{
			Envelop_time = (carrier_time_us*(wTimer&0x7FFF)); 
			Envelop_counter = 65536 - (Envelop_time / 2) - ((carrier_time_ns * (wTimer&0x7FFF)/10)/2); //Timing 계산으로 인해 발생한 nsec 단위 손실에 대한 보상 값
			uptr_Timer++;											//next high pointer
			wTimer = *uptr_Timer;
		}

		RCAP2H = (Envelop_counter >> 8);
		RCAP2L = Envelop_counter & 0x00FF;
	}

	// output 1'st frame
	if(uptr_Timer > uptr_TimerEnd)
	{
		t1_int = 2;
		keyholdcnt++;

		if(keyholdcnt > 0xfe) keyholdcnt = 4;

		uptr_Timer    = Repeat_Start;
		uptr_TimerEnd = Repeat_End;

		wTimer = *uptr_Timer;

#if (USE_SPECIAL_0F4H == TRUE)
		f4_cnt = 0;
#endif
	}
	
	if(!wTimer)
	{
		t1_int = 2;
		pulse_stop();
	}

	TF2 = 0;	
}

void SPI_Interrupt(void) interrupt 6
{
}

void I2C_Interrupt(void) interrupt 7
{
}

void LVD_Interrupt(void) interrupt 9
{
}

void Comp_Out_RE_Interrupt(void) interrupt 11
{
}

void Comp_Out_FE_Interrupt(void) interrupt 12
{
}


/**
  * @brief  CRT demodulation interruption function ,sample carrier wave
  * @note   it will sample six value,and useless of the first one
  * @retval None
  */
void CRT_Interrupt(void) interrupt 14 
{
}


void External_Interrupt_2() interrupt 15
{
}

void External_Interrupt_3() interrupt 16
{
}

void KBI_Interrupt(void) interrupt 20 
{
}


#endif	//__MAIN_C__

