
#ifndef __IR_LEARN_C__
#define __IR_LEARN_C__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include 	"main.h"
#include	"common.h"
#include	"function_key.h"
#include	"flash.h"
//#include	"Eeprom.h"
#include	"learn.h"

/**************************************************************************************************
 *                                            DEFINES
 **************************************************************************************************/
#define	CARRIER_TYPE				0
#define	FLASH_TYPE					1

#define	Word_Margine				15
#define	Bit_Margine					4

#define	RC5_SHORT					895
#define	RC5_LONG					1790
#define	RC5_DECIDE_LS				((RC5_SHORT+RC5_LONG)/2)
#define	RC5_MARGINE					100

#define	RC6_TOGGLE_LONG				1333
#define	RC6_TOGGLE_SHORT			444

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/
#if (USE_IR_LEARN == TRUE)

byte RawDataOfLearn_Verify(void)
{
	WORD	addAddr;
	int16u	i,cnt;

	uptr_Timer = (WORD*) &ir_stream.Tunion.Tstruct.pulse.BitStream[0];	//bitstream start address load in uptr_timer

	if((*uptr_Timer&0x7fff) == 0)
	{
		while((*uptr_Timer&0x7fff) == 0) 																// data format �� low|high������ ��� ù bit�� 0�̵ɼ� �ִ�.
		{
			uptr_Timer += 2;
		}
	}

	//1'st
	uptr_TimerEnd = (WORD*) &ir_stream.Tunion.Tstruct.pulse.BitStream[0];
	addAddr       = (WORD) ir_stream.Tunion.Tstruct.bidxEndWord1;
	uptr_TimerEnd += addAddr;
	First_End     = uptr_TimerEnd;

	wTimer = (*uptr_Timer);

	if(uptr_Timer >= &ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH])
	{
		return FALSE;
	}
	if(First_End >= &ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH])
	{
		return FALSE;
	}

	//2'nd
	Repeat_Start=(WORD*) &ir_stream.Tunion.Tstruct.pulse.BitStream[0];	//bitstream start address load in uptr_timer
	addAddr=(WORD) ir_stream.Tunion.Tstruct.bidxRepeatWord;
	Repeat_Start+=addAddr;

	if((*Repeat_Start&0x7fff)==0)
	{
		while((*Repeat_Start&0x7fff)==0) 																// data format �� low|high������ ��� ù bit�� 0�̵ɼ� �ִ�.
		{
			Repeat_Start+=2;
		}
	}

	//end pointer -> uptr_TimerEnd
	Repeat_End =(WORD*) &ir_stream.Tunion.Tstruct.pulse.BitStream[0];
	addAddr=(WORD) ir_stream.Tunion.Tstruct.bidxEndWord2;
	Repeat_End+=addAddr;

	if(Repeat_Start>=&ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH])
	{
		return FALSE;
	}
	if(Repeat_End>=&ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH])
	{
		return FALSE;
	}
	if(ir_stream.Tunion.Tstruct.bidxRepeatWord > ir_stream.Tunion.Tstruct.bidxEndWord2)
	{
		return FALSE;
	}

	cnt=(int16u) (Repeat_End - &ir_stream.Tunion.Tstruct.pulse.BitStream[0]);
	uptr_Timer=(WORD*) &ir_stream.Tunion.Tstruct.pulse.BitStream[0];

	for(i=0; i<cnt; i++)
	{
		if((*uptr_Timer&0x7fff)==0)
		{
			return FALSE;
		}
		uptr_Timer++;
	}

	return TRUE;
}


void Clear_Learn_Receive_Buffer(int8u fillData)
{
	WORD	i = 0;

	pStream16u = &ir_stream.Tunion.Tstruct.pulse.BitStream[0];

	for(i=0; i<(MAX_BITSTREAM+1); i++)
	{
		*pStream16u++ = fillData;
	}

}

void Quit_Learn(void)
{

	IRCC0 = 0;	//IRAEN SENOEN - - REFSEL RSEL2 RSEL1 RSEL0
	IRCC1 = 0;	//IRCEN IRIIF IREDGE1 IREDGE0 - IRPOL SINGLE PHASE
	IRCC2 = 0;	//T3IR T2IR - - T3EDGE1 T3EDGE0 T2EDGE1 T2EDGE0

	WTDR1 = 0;	
	WTDR0 = 0;	
	WTMR = 0;	//WTEN OVFDIS WTCL WTIEN - - WTCK1 WTCK0

	T3HDR = 0;
	T3LDR = 0;  
	T3CR = 0;	//EC3E PWM3E CAP3 T3CK2 T3CK1 T3CK0 T3CN T3ST

/* Interrupt Disable */
	IE1	&= ~(1<<INT6E_IRI);			/* Disable IRI Input Interrupt */
	IE2 &= ~(1<<INT14E_Timer3);		/* Disable Timer 3 Interrupt */
	IE2	&= ~(1<<INT16E_WT);			/* Disable Watch Timer Interrupt */

/* Interrupt Priority */
	IP	&= ~0x04;	/* WT(GROUP2)*/
	IP1	&= ~0x04;	/* WT(GROUP2)*/

	Learn_Pwr_Off;
	
	ReturnNormal();
}

void FailToLearn(void)
{
	wBitStream_Clear();
	Clear_Learn_Receive_Buffer(0);
	device_Led_blink(IR_LEARN_LED, 10, 1, 1);
	Timer0_50ms16=SETUP_TIMEOUT_S;
	//pStream=&Envelop.Tunion.data1[0];

	//device_led_on(IR_SENDING_LED);
	wait_getkey_simple();
}

int8u Check_LearnEnableKey(int8u* key)
{
	LearnKeyNo=0;

	switch(*key)
	{
		case	KEY_TV:
		case	KEY_STB:
		case	KEY_DVD:
		case	KEY_AUDIO:
			return	FALSE;

		case	KEY_POWER:
			LearnKeyNo++;
		case	KEY_CHUP:
			LearnKeyNo++;
		case	KEY_CHDOWN:
			LearnKeyNo++;
		case	KEY_0:
			LearnKeyNo++;
		case	KEY_1:
			LearnKeyNo++;
		case	KEY_2:
			LearnKeyNo++;
		case	KEY_3:
			LearnKeyNo++;
		case	KEY_4:
			LearnKeyNo++;
		case	KEY_5:
			LearnKeyNo++;
		case	KEY_6:
			LearnKeyNo++;
		case	KEY_7:
			LearnKeyNo++;
		case	KEY_8:
			LearnKeyNo++;
		case	KEY_9:
			LearnKeyNo++;
		case	KEY_UP:
			LearnKeyNo++;
		case	KEY_DOWN:
			LearnKeyNo++;
		case	KEY_LEFT:
			LearnKeyNo++;
		case	KEY_RIGHT:
			LearnKeyNo++;
		case	KEY_OK:
			LearnKeyNo++;
		case	KEY_TEXT:
			LearnKeyNo++;
		case	KEY_MENU:
			LearnKeyNo++;
		case	KEY_END:
			LearnKeyNo++;
		case	KEY_INFO:
			LearnKeyNo++;
		case	KEY_RED:
			LearnKeyNo++;
		case	KEY_GREEN:
			LearnKeyNo++;
		case	KEY_YELLOW:
			LearnKeyNo++;
		case	KEY_BLUE:
			LearnKeyNo++;
		case	KEY_PLAY:
			LearnKeyNo++;
		case	KEY_STOP:
			LearnKeyNo++;
		case	KEY_PAUSE:
			LearnKeyNo++;
		case	KEY_REW:
			LearnKeyNo++;
		case	KEY_FF:
			LearnKeyNo++;
		case	KEY_REC:
			LearnKeyNo++;
		case	KEY_F1:
			LearnKeyNo++;
		case	KEY_F2:
			LearnKeyNo++;
		case	KEY_F4:
			LearnKeyNo++;
		case	KEY_F5:
			LearnKeyNo++;
		case	KEY_F6:
			return	TRUE;

		case	KEY_VOLUP:
			LearnKeyNo++;
		case	KEY_VOLDOWN:
			LearnKeyNo++;
		case	KEY_MUTE:
			LearnKeyNo++;
		case	KEY_F3:
			LearnKeyNo+=37;
			if(setting_source==SRC_TV)
				return	TRUE;
			else
				return	FALSE;

		default:
			return	FALSE;
	}
}


void Enter_Delete_Mode(void)
{
	curr_state=SET_MODE_LEARN_DELETE;
	SetMode_StartTimeOut(SETUP_TIMEOUT_S);

	device_led_on(IR_SENDING_LED);
	while(getkey_simple()==1)
	{
		if(!Timer0_50ms16)
			break;
	}

	while(Timer0_50ms16)					//learn time out check. -> 20sec
	{
		getkey(&key_buf[0]);

		if((!key_cnt)&&(!Timer0_50ms16))
		{
			break;
		}

		if(key_cnt==1)
		{
			Timer0_50ms16=SETUP_TIMEOUT_S;

			if((key_buf[0]==KEY_TV)||(key_buf[0]==KEY_STB)||(key_buf[0]==KEY_DVD)||(key_buf[0]==KEY_AUDIO))
			{
				device_Led_InSetUp(SET_LEARN_NG, IR_LEARN_LED);
				ReturnNormal();
				return;
			}
			else if(!Check_LearnEnableKey(key_buf))
			{
				FailToLearn();
				wait_getkey_simple();
				continue;
			}
			else
			{
				Erase_Learn_Key(setting_source);
				device_Led_blink(IR_LEARN_LED, 2, 3, 3);
				//device_led_on(IR_SENDING_LED);
				wait_getkey_simple();
				continue;
			}
		}
	}
	ReturnNormal();
}

void Learn_Start(void)
{
/* 0xffff*2us = 131ms overflow*/
	T3HDR = 0xff;
	T3LDR = 0xff;
/* 8*10^6 / 16 = 500KHz = 2us per */
/* T3CR  : Timer 3 Mode Control Register, Initial value : 00H */
	T3CR 	= 0				/* Timer 3 Mode Control Register */
			| (0<<EC3E)		/* 0:Timer 3 is a normal counter, 1:Timer 3 is an event counter clocked by EC3 */
			| (0<<PWM3E)	/* 0:Timer 3 is Normal Timer/Counter, 1:Timer 1 is PWM */
			| (0<<CAP3)		/* 0:Timer/Counter mode, 1:Capture mode */
							/* fSCLK fSCLK/2^1 fSCLK/2^2 fSCLK/2^3 fSCLK/2^4 fSCLK/2^6 fSCLK/2^8 CRF(Carrier) */
			| (1<<T3CK2)	/*	 0	   0	     0         0 	   	 1		   1	     1        1  		  */
			| (0<<T3CK1)	/*	 0	   0	     1	       1 	   	 0		   0	     1	      1  		  */
			| (0<<T3CK0)	/*	 0	   1	     0	       1 	   	 0		   1	     0	      1  		  */
			| (1<<T3CN)		/* 0:Pause counting temporarily, 1:Continue to count */
			| (0<<T3ST);	/* 0:Stops counting, 1:Clears counter and starts up-counting */

/* 0x0320 * 0.125us = 100us overflow,*/
	WTDR1 = 0x03;
	WTDR0 = 0x20;

/* 8*10^6 / 1 = 8MHz = 0.125us per */
/* WTMR  : Watch Timer Mode Register, Initial value : 00H */
	WTMR 	= 0				/* Watch Timer Mode Register */
			| (1<<WTEN)		/* 0:Disable WT, 1:Enable WT */
			| (0<<OVFDIS)	/* 0:Auto clear counters when overflow, 1:Overflow event is ignored */
			| (1<<WTCL)		/* 0:No operation (Free Run mode), 1:Clear WT counter (Auto-clear after 1 cycle) */
							/* fSCLK/32 fSCLK/64 fSCLK/128 fSCLK/256 : Watch Timer mode */
							/* fSCLK/1  fSCLK/2  fSCLK/3   fSCLK/4   : IR Capture mode  */
			| (0<<WTCK1)	/*	 0	      0	       1         1  		  				*/
			| (0<<WTCK0);	/*	 0	      1	       0	     1  		  				*/

/* IRCC0  : IR Capture Control Register 0, Initial value : 00H */
	IRCC0 	= 0				/* IR Capture Control Register 0 */
			| (1<<IRAEN)	/* 0:Disable IR AMP, 1:Enable IR AMP */
			| (0<<SENOEN)	/* 0:SIGNAL/P32 is normal port, 1:The output of IRAMP is monitored on SIGNLA/P32 port when IRAEN is enabled */
			| (0<<REFSEL)	/* 0:Internally divided voltage becomes reference voltage, 1:External input voltage becomes reference voltage */
							/* 1/16 VDDEXT 2/16 VDDEXT 3/16 VDDEXT 4/16 VDDEXT 12/16 VDDEXT 13/16 VDDEXT 14/16 VDDEXT 15/16 VDDEXT */
			| (1<<RSEL2)	/*	 	0	   		0	     	0         	0 	   	 	 1		   	  1	     	   1            1	   */
			| (1<<RSEL1)	/*	 	0	   		0	     	1	       	1 	   	 	 0		   	  0	     	   1	        1      */
			| (1<<RSEL0);	/*	 	0	   		1	     	0	       	1 	   	 	 0		   	  1	     	   0	        1 	   */


/* IRCC2  : IR Capture Control Register 2, Initial value : 00H */
	//IRCC2 	= 0				/* IR Capture Control Register 2 */
	//		| (0<<T3IR)		/* 0:Timer 3 is in normal operation, 1:Timer 3 calculates the number of incoming carrier signals */
	//		| (0<<T2IR)		/* 0:Timer 2 is in normal operation, 1:Timer 2 calculates the number of incoming carrier signals */
	//						/* No capture Falling edge Rising edge Both edge  */
	//		| (0<<T3EDGE1)	/*	 	0	      0	       		1         1 	  */
	//		| (1<<T3EDGE0)	/*	 	0	      1	       		0	      1  	  */
	//						/* No capture Falling edge Rising edge Both edge  */
	//		| (0<<T2EDGE1)	/*	 	0	      0	       		1         1 	  */
	//		| (0<<T2EDGE0);	/*	 	0	      1	       		0	      1  	  */

/* IRCC1  : IR Capture Control Register 1, Initial value : 00H */
	IRCC1 	= 0				/* IR Capture Control Register 1 */
			| (1<<IRCEN)	/* 0:IR capture mode is disabled, normal timer function, 1:IR capture mode is enabled */
			| (0<<IRIIF)	/* 0:No IRI input is generated, 1:IRI interrupt is generated on the condition by IREDGE[1:0] bits */
							/* disabled  on Falling edge on Rising on edge Both edge  */
			| (1<<IREDGE1)	/*	 	0	      0	       			1         	1 	  	  */
			| (0<<IREDGE0)	/*	 	0	      1	       			0	      	1  	  	  */
			| (0<<IRPOL)	/* 0:The inverted signal from IRAMP output(=COMP_OUT) or SENSOR/P31 input becomes the input source of WT */
							/* 1:The multiplexed output of IRAMP output(=COMP_OUT) or SENSOR/P31 input becomes the input source of WT */
			| (0<<SINGLE)	/* 0:Capture continuously until WTIR overflows, 1:Capture first 3 edges of carrier signal */
			| (1<<PHASE);	/* 0:Capture sequence is 1st Falling->Rising->Falling edge */
							/* 1:Capture sequence is 1st Rising->Falling->Rising edge */

/* Interrupt Priority */
	IP	|= 0x04;	/* WT(GROUP2) highest*/
	IP1	|= 0x04;	/* WT(GROUP2) highest*/

/* Interrupt Enable */
	IE1	|= (1<<INT6E_IRI);			/* Enable IRI Input Interrupt */
	IE2 &= ~(1<<INT14E_Timer3);	/* Disable Timer 3 Interrupt */
	IE2	|= (1<<INT16E_WT);			/* Enable Watch Timer Interrupt */

	EA = 1;

	learn_type = 0xFF;
	CarrierIdx = 0;
	ir_stream.Tunion.Tstruct.Carrier = 0;
	pStream16u = (WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
	ipc_cnt = 0;
	Ready_Low_Capture = 0;
	CntFor2Sec = 0;

	/* 0xffff*2us = 131ms overflow*/
	NoSinalCnt = 8;			// 131ms*1 = �� 1��

	bT3OverFlow = FALSE;
}

#if 1
BOOL ChangePatternByCarrier(void)
{
	ULONG	Utemp;
	WORD	i = 0;

	if(ir_stream.Tunion.Tstruct.Carrier > 255)
	{
		return FALSE;
	}

	pStream16u = &ir_stream.Tunion.Tstruct.pulse.BitStream[0];

/* T3CR : 8*10^6 / 16 = 500KHz = 2us per */
/* Carrier Freq : CarrierCount(106)*250ns = 26.5us = 37.735KHz */
	for(i=0; i<(MAX_BITSTREAM+1); i++)
	{
		Utemp = (ULONG)*pStream16u;
		Utemp = Utemp * 2 * 4;

		if(learn_type == FLASH_TYPE)
		{
			Utemp = Utemp/ir_stream.Tunion.Tstruct.Carrier;
			if(Utemp < 2)	Utemp=5;
			*pStream16u++ = Utemp;
		}
		else
		{
			*pStream16u++ = Utemp/(ir_stream.Tunion.Tstruct.Carrier);
		}
	}

	return TRUE;
}
#endif

void Enter_Learn_Mode(void)
{
	// Turn on  �ϸ鼭 IR �Է� ���ο� �ణ�� ����� �����.
	Learn_Pwr_On;

	device_led_on(IR_SENDING_LED);

	wBitStream_Clear();
	Clear_Learn_Receive_Buffer(0);

	curr_state = SET_MODE_LEARN;

	SetMode_StartTimeOut(SETUP_TIMEOUT_S);	
	
	while(getkey_simple()==1)
	{
		if(!Timer0_50ms16)
			break;
	}	

	while(Timer0_50ms16)						//learn time out check. -> 20sec
	{
		getkey(&key_buf[0]);

		if((!key_cnt)&&(!Timer0_50ms16))
		{
			device_Led_blink(IR_LEARN_LED, 10, 100, 100);
			break;
		}

		if(key_cnt==1)
		{
			Timer0_50ms16=SETUP_TIMEOUT_S;
			//pStream=&Envelop.Tunion.data1[0];

			if((key_buf[0]==KEY_TV)||(key_buf[0]==KEY_STB)||(key_buf[0]==KEY_DVD)||(key_buf[0]==KEY_AUDIO))
			{
				device_Led_InSetUp(SET_LEARN_NG, IR_LEARN_LED);
				Quit_Learn();
				return;
			}

			if(!Check_LearnEnableKey(key_buf))
			{
				FailToLearn();
				wait_getkey_simple();
				continue;
			}

			device_Led_blink(IR_LEARN_LED, 1, 0, 500);
			
			device_led_on(IR_SENDING_LED);
			
			//Timer0_50ms08=0xFF;			// receive time for ir signal 13sec
			Timer0_50ms08=260;			// receive time for ir signal 13sec
			Timer0_50ms16=SETUP_TIMEOUT_S;

			return_int8u=0;
			Learn_Start(); 

			while(!return_int8u && Timer0_50ms08){}		// receive time for ir signal 13sec

			WTMR = 0;
			T3CR &= _1111_1110B;					// Stops counting
 			
			IE2 &= ~(1<<INT14E_Timer3);				/* Disable Timer 3 Interrupt */
			
			if(!return_int8u)
			{
				FailToLearn();
				continue;
			}

			return_int8u=0;

			if(!ChangePatternByCarrier())
			{
				FailToLearn();
				continue;
			}
			
			FindPattern();										//25ms
			if(return_int8u)
			{
				SetPattern();									//96ms
				if(return_int16u)
				{
					CompressBitPattern();						//34ms
					MeltBitPattern(&ir_zip);

//					Learn_Ok=TRUE;
					if(RawDataOfLearn_Verify())
					{
						LearnDataRW(WRITE, curr_source);		 //129ms		learn analyzer -> 284ms

						if(RW!=0xff)
						{
							wBitStream_Clear();
							Clear_Learn_Receive_Buffer(0);

							if(	testMode_KeyIndex == 4 )
							{
								device_Led_blink(IR_LEARN_LED, 4, 200, 200);
								Quit_Learn();
								return;
							}

							device_Led_blink(IR_LEARN_LED, 2, 200, 200);
							device_led_on(IR_SENDING_LED);
							Timer0_50ms16=SETUP_TIMEOUT_S;
							//pStream=&Envelop.Tunion.data1[0];

							//Quit_Learn();
							//return;
							wait_getkey_simple();
						}
						else									//puts in full
						{
							device_Led_blink(IR_LEARN_LED, 10, 100, 100);
							Quit_Learn();
						}//end of flash write
					}
					else
					{
						FailToLearn();
						continue;
					}//end of CompressBitPattern & RawDataOfLearn_Verify
				}
				else
				{
					FailToLearn();
					continue;
				}//end of SetPattern
			}
			else
			{
				FailToLearn();
				continue;
			}//end of FindPattern
		}//end of key in -> cnt=1
	}// end of learn (time out -> 20sec) while
	Quit_Learn(); 
}

void Bit_streamIdx_Setting()
{
	unsigned short  i;

	if(repeat > end2)
		repeat = end2;

	ir_stream.Tunion.Tstruct.bidxEndWord1 = end1;
	ir_stream.Tunion.Tstruct.bidxEndWord2 = end2;
	ir_stream.Tunion.Tstruct.bidxRepeatWord = repeat;
	for( i=end2+1; i < BITSTREAMLENGTH; i++)
		ir_stream.Tunion.Tstruct.pulse.BitStream[i]=0;
}

void FindPattern()
{
	unsigned short 	i;
	int16s			l,l2;
	unsigned short 	temp;
	unsigned char 	Pause_count=0;
	unsigned short 	Pause_point[5] = {0,0,0,0,0};

	unsigned short 	WaitPulseLength = 0;
	unsigned short 	WaitPulseTemp=0;
	unsigned char 	single_ok=FALSE;
	unsigned short  WaitPulseLength_second=0;

	unsigned short 	low_search=0;
	unsigned char 	compare_flag=0;

	for( i=1; i < BITSTREAMLENGTH; i+=2)
	{
		WaitPulseTemp = ir_stream.Tunion.Tstruct.pulse.BitStream[i];
		if(!WaitPulseTemp)
			break;

		if(WaitPulseTemp>=WaitPulseLength)
			WaitPulseLength=WaitPulseTemp;
	}

	for( i=3; i < BITSTREAMLENGTH; i+=2)
	{
		WaitPulseTemp = ir_stream.Tunion.Tstruct.pulse.BitStream[i];
		if(!WaitPulseTemp)
			break;

		if((WaitPulseTemp < WaitPulseLength) && (WaitPulseTemp >= WaitPulseLength_second))
			WaitPulseLength_second=WaitPulseTemp;
	}

	if((WaitPulseLength-WaitPulseLength_second) <= (WaitPulseLength*0.05))
		compare_flag=1;

	for( i=3; i < BITSTREAMLENGTH; i+=2)
	{
		WaitPulseTemp = ir_stream.Tunion.Tstruct.pulse.BitStream[i];
        if(!WaitPulseTemp)
			break;

		if(((WaitPulseTemp*1.1) >= WaitPulseLength)&&(WaitPulseTemp!=ir_stream.Tunion.Tstruct.pulse.BitStream[1]))
		{
			Pause_point[Pause_count]=i;
			Pause_count++;
		}
		else
		{
			if((Pause_count)&&(compare_flag))
			{
				if(((WaitPulseTemp*1.05) >= WaitPulseLength_second)&&(WaitPulseTemp!=ir_stream.Tunion.Tstruct.pulse.BitStream[1]))
				{
					Pause_point[Pause_count]=i;
					Pause_count++;
				}
			}
		}

		if(Pause_count>4)
			break;
	}

	for( i=0; (i+1) < Pause_count; i++)
	{
		if(Pause_point[i+1]-Pause_point[i] < 4)
		{
			single_ok=TRUE;
			break;
		}
	}

	if(single_ok)
	{
		for( i=1; i < BITSTREAMLENGTH; i+=2)
		{
			if(ir_stream.Tunion.Tstruct.pulse.BitStream[i])
				continue;
			else
				break;
		}

		l=ir_stream.Tunion.Tstruct.pulse.BitStream[i-1]&0x7fff;

		if(l)
			ir_stream.Tunion.Tstruct.pulse.BitStream[i]=l;
		else
			i-=2;

		end1 = i;
		end2 = i;
		repeat = i;
		Bit_streamIdx_Setting();
		return_int8u=1;
		return;
	}

	if(Pause_count<2)
	{
		for( i=1; i < BITSTREAMLENGTH; i+=2)
		{
			if(ir_stream.Tunion.Tstruct.pulse.BitStream[i])
				continue;
			else
				break;
		}

		if(i < BITSTREAMLENGTH)
		{
			ir_stream.Tunion.Tstruct.pulse.BitStream[i]=(ir_stream.Tunion.Tstruct.pulse.BitStream[i-1]&0x7fff);
			end1 = i;
			end2 = i;
			repeat = i;
			Bit_streamIdx_Setting();
			return_int8u=1;
			return;
		}
		else
		{
			i = Pause_point[0];
			end1 = i;
			end2 = i;
			repeat = 0;
			Bit_streamIdx_Setting();
			return_int8u=1;
			return;
		}
	}

	WaitPulseLength_second=0;
	temp=Pause_point[0];

	for( i=3; i < temp; i+=2)
	{
		WaitPulseTemp = ir_stream.Tunion.Tstruct.pulse.BitStream[i];
		if(WaitPulseTemp>=WaitPulseLength_second)
			WaitPulseLength_second=WaitPulseTemp;
	}

	if((WaitPulseLength-WaitPulseLength_second)<=(WaitPulseLength*0.05))
	{
		for( i=3; i < temp; i+=2)
		{
			WaitPulseTemp = ir_stream.Tunion.Tstruct.pulse.BitStream[i];
			if(WaitPulseTemp*1.01>=WaitPulseLength_second)
			{
				temp =Pause_point[0];
				Pause_point[0]=i;
				low_search=1;
				break;
			}
		}

		if(low_search)
		{
			for(i=1;i<=Pause_count;i++)
			{
				low_search = Pause_point[i];
				Pause_point[i] =temp;
			}
			Pause_count++;
		}
	}

	if(Pause_count<3)
	{
		temp= Pause_point[0];

		if (Pause_point[0]*2+1 == Pause_point[1])
		{
			for(i=0; i< temp ;i++)
			{
				l=ir_stream.Tunion.Tstruct.pulse.BitStream[i] - ir_stream.Tunion.Tstruct.pulse.BitStream[i+temp+1];
				if((l<-Bit_Margine)||(l>Bit_Margine))
					break;
			}
			if(temp!=i)
			{
				end1 = Pause_point[1];
				end2 = Pause_point[1];  //1.2.1.2
				repeat = 0;
			}
			else
			{
				end1 = Pause_point[0];
				end2 = Pause_point[0]; //1,1,1,1
				repeat = 0;
			}
			Bit_streamIdx_Setting();
			return_int8u=1;
			return;
		}
		else
		{
			end1 = Pause_point[0];
			end2 = Pause_point[1];
			repeat = Pause_point[0]+1;	//1,2,2,2
			Bit_streamIdx_Setting();
			return_int8u=1;
			return;
		}
	}

	temp= Pause_point[0];

	for(i=0; i<temp ;i++)
	{
		l=ir_stream.Tunion.Tstruct.pulse.BitStream[i] - ir_stream.Tunion.Tstruct.pulse.BitStream[i+temp+1];
		if(i)
		{
			if((l<-Bit_Margine)||(l>Bit_Margine))
				break;
		}
		else
		{
			if((l<-Word_Margine)||(l>Word_Margine))
				break;
		}
	}

	if (Pause_point[0]*2+1 == Pause_point[1])
	{
		if(i==temp)
		{
			end1 = Pause_point[0];
			end2 = Pause_point[0];
			repeat = 0; //1,1,1
		}
		else
		{
			if(Pause_point[1]*2+1 == Pause_point[3])
			{
				temp= Pause_point[1];

				for(i=Pause_point[0]+1; i<temp ;i++)
				{
					l=ir_stream.Tunion.Tstruct.pulse.BitStream[i] - ir_stream.Tunion.Tstruct.pulse.BitStream[temp+1];
					if((l<-Bit_Margine)||(l>Bit_Margine))
						break;
				}

				if(i==temp)
				{
					end1 = Pause_point[0];
					end2 = Pause_point[1];
					repeat = Pause_point[0]+1; //1,2,2,2
					Bit_streamIdx_Setting();
					return_int8u=1;
					return;
				}

				l = ir_stream.Tunion.Tstruct.pulse.BitStream[Pause_point[1]+1]-ir_stream.Tunion.Tstruct.pulse.BitStream[Pause_point[2]+1];
				if((l>=-Bit_Margine)&&(l<=Bit_Margine))
				{
					l = ir_stream.Tunion.Tstruct.pulse.BitStream[Pause_point[0]]-ir_stream.Tunion.Tstruct.pulse.BitStream[Pause_point[2]];
					l2=ir_stream.Tunion.Tstruct.pulse.BitStream[Pause_point[1]] - ir_stream.Tunion.Tstruct.pulse.BitStream[Pause_point[3]];

					if( ((l>=-Word_Margine)&&(l<=Word_Margine)) && ((l2>=-Word_Margine)&&(l2<=Word_Margine)))
					{
						end1 = Pause_point[1];
						end2 = Pause_point[1];
						repeat = 0; //1,2,1,2
					}
					else
					{
						end1 = Pause_point[0];
						end2 = Pause_point[1];
						repeat = Pause_point[0]+1; //1,2,2,2
					}
				}
				else
				{
					end1 = Pause_point[1];
					end2 = Pause_point[1];
					repeat = 0; //1,2,1,2
				}
			}
			else
			{
				if(Pause_point[0]*3+2 != Pause_point[2])
				{
					end1 = Pause_point[1];
					end2 = Pause_point[2];
					repeat = Pause_point[1]+1; //1,2,3,3
				}
				else
				{
					end1 = Pause_point[0];
					end2 = Pause_point[1];
					repeat = Pause_point[0]+1; //1,2,2,2
				}
			}
		}
	}
	else
	{
		if(Pause_point[1]*2+1 == Pause_point[3])
		{
			l = ir_stream.Tunion.Tstruct.pulse.BitStream[Pause_point[1]+1]-ir_stream.Tunion.Tstruct.pulse.BitStream[Pause_point[2]+1];
			if((l>=-Bit_Margine)&&(l<=Bit_Margine))
			{
				end1 = Pause_point[0];
				end2 = Pause_point[1];
				repeat = Pause_point[0]+1;	//1,2,2,2
			}
			else
			{
				end1 = Pause_point[1];
				end2 = Pause_point[1];
				repeat = 0;	//1,2,1,2
			}
		}
		else
		{
			if(Pause_point[0]*3+2 != Pause_point[2])
			{
				l=ir_stream.Tunion.Tstruct.pulse.BitStream[Pause_point[2]] - ir_stream.Tunion.Tstruct.pulse.BitStream[Pause_point[1]];
				l2=ir_stream.Tunion.Tstruct.pulse.BitStream[Pause_point[3]] - ir_stream.Tunion.Tstruct.pulse.BitStream[Pause_point[2]];

				//		same										not same
				if(  ((l2>=-Word_Margine)&&(l2<=Word_Margine)) && (( (l<=-Word_Margine)||(l>=Word_Margine) ) )  )
				{
					end1 = Pause_point[1];
					end2 = Pause_point[2];
					repeat = Pause_point[1]+1;	//1,2,3,3
				}
				else
				{
					end1 = Pause_point[0];
					end2 = Pause_point[1];
					repeat = Pause_point[0]+1;	//1,2,2,2
				}
			}
			else
			{
				end1 = Pause_point[0];
				end2 = Pause_point[1];
				repeat = Pause_point[0]+1;	//1,2,2,2
			}
		}
	}
	Bit_streamIdx_Setting();
	return_int8u=1;
	return;
}

//#define OPTIMIZE

//---------------------------------------------------------------------------------
//=============================
// �Ʒ��� ���� ���ʷ� �Լ� ȣ��
//=============================
//  Learn�� bitstream�� New_DrvBitStream����ü�� ������ setpattern ȣ��
// SetPattern();
// CompressBitPattern();
// MeltBitPattern(&ir_zip);        // DATABASE ALGORITHM 1
//==============================
void SetPattern()                                            // DATABASE ALGORITHM 1
{
	int16s 			i,j,l;
	int16s 			PulseCnt;
	unsigned char 	bFoundCnt;
	unsigned short 	wFoundVal;
	double   		ulSumVal;

	unsigned short  barrFoundCnt[2] = {0,0};
	unsigned char   barrFoundVal[2] = {0,0};
	unsigned long 	High_real_time;
	unsigned long 	LOW_real_time;
//	unsigned short 	BitSteamIdx=0;

	//>>
	unsigned short 	Found_cnt;
	unsigned short 	CompressIdx;
	int 			l2;
	double 			gap;
	double 			standard;	//us ����
	double 			margin;	//us ����
	//<<

	int16u 			length;
	int16u 			k;
	int16u 			check_error=0; //1: end1, 2:end1 ok, end2

	IrZipData_Clear();

	if((ir_stream.Tunion.Tstruct.bidxRepeatWord > 0) &&(ir_stream.Tunion.Tstruct.bidxRepeatWord!=ir_stream.Tunion.Tstruct.bidxEndWord2 )) //repeat
	{
		length=ir_stream.Tunion.Tstruct.bidxEndWord1+1;

		for(i=0; i< ir_stream.Tunion.Tstruct.bidxEndWord1+1;i++)
		{
			if(!(ir_stream.Tunion.Tstruct.pulse.BitStream[i]&0x7fff))
				break;
		}

		if(i!=length) //0~end1 0 detect
		{
			if(i%2) //low =0
			{
				ir_stream.Tunion.Tstruct.bidxEndWord1=i-2;

				for(i=ir_stream.Tunion.Tstruct.bidxEndWord1+1;i<ir_stream.Tunion.Tstruct.bidxRepeatWord;i++) //fill 0 new_endword1~repeat
				{
					if(i%2)
						ir_stream.Tunion.Tstruct.pulse.BitStream[i]=0x0000;
					else
						ir_stream.Tunion.Tstruct.pulse.BitStream[i]=0x8000;
				}
				check_error=1;
			}
			else //high =0
			{
				ir_stream.Tunion.Tstruct.bidxEndWord1=i-1;

				for(i=ir_stream.Tunion.Tstruct.bidxEndWord1+1;i<ir_stream.Tunion.Tstruct.bidxRepeatWord;i++) //fill 0 new_endword1~repeat
				{
					if(i%2)
						ir_stream.Tunion.Tstruct.pulse.BitStream[i]=0x0000;
					else
						ir_stream.Tunion.Tstruct.pulse.BitStream[i]=0x8000;
				}
				check_error=1;
			}
			if(ir_stream.Tunion.Tstruct.bidxRepeatWord > ir_stream.Tunion.Tstruct.bidxEndWord2)
				ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
		}

		if(!check_error) //end1 no 0
		{
			length=ir_stream.Tunion.Tstruct.bidxEndWord2-ir_stream.Tunion.Tstruct.bidxRepeatWord+1;

			for(k=0,i=ir_stream.Tunion.Tstruct.bidxRepeatWord;i<ir_stream.Tunion.Tstruct.bidxEndWord2+1;i++,k++)
			{
				if(!(ir_stream.Tunion.Tstruct.pulse.BitStream[i]&0x7fff))
					break;
			}
			if(k!=length)
			{
				if(i%2) //low =0
					ir_stream.Tunion.Tstruct.bidxEndWord2=i-2;
				else //high =0
				{
					if(ir_stream.Tunion.Tstruct.pulse.BitStream[i]==0x8000)
					ir_stream.Tunion.Tstruct.bidxEndWord2=i-1;
					else
					ir_stream.Tunion.Tstruct.bidxEndWord2=i-1;
				}
			}
			if(ir_stream.Tunion.Tstruct.bidxRepeatWord >= ir_stream.Tunion.Tstruct.bidxEndWord2)
			{
				ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
				ir_stream.Tunion.Tstruct.bidxEndWord1=ir_stream.Tunion.Tstruct.bidxEndWord2;
			}
		}
	}
	else //continue
	{
		length=ir_stream.Tunion.Tstruct.bidxEndWord2+1;

		for(i=0; i< ir_stream.Tunion.Tstruct.bidxEndWord2+1;i++)
		{
			//>> 2009.02.21
			if(!(ir_stream.Tunion.Tstruct.pulse.BitStream[i]&0x7fff))
				break;
		}

		if(i!=length)
		{
			if(i%2) //low =0
			ir_stream.Tunion.Tstruct.bidxEndWord2=i-2;
			else //high =0
			ir_stream.Tunion.Tstruct.bidxEndWord2=i-1;

			ir_stream.Tunion.Tstruct.bidxEndWord1=ir_stream.Tunion.Tstruct.bidxEndWord2;
		}
	}


	PulseCnt=ir_stream.Tunion.Tstruct.bidxEndWord2+1;
	//<<

	ir_zip.Tunion.Tstruct.bidxEndWord1=(unsigned char)(ir_stream.Tunion.Tstruct.bidxEndWord1);
	ir_zip.Tunion.Tstruct.bidxEndWord2=(unsigned char)(ir_stream.Tunion.Tstruct.bidxEndWord2);
	ir_zip.Tunion.Tstruct.bidxRepeatWord=(unsigned char)(ir_stream.Tunion.Tstruct.bidxRepeatWord);

	ir_zip.Tunion.Tstruct.bidxEndWord1_time=ir_stream.Tunion.Tstruct.pulse.BitStream[ir_stream.Tunion.Tstruct.bidxEndWord1];
	ir_zip.Tunion.Tstruct.bidxEndWord2_time=ir_stream.Tunion.Tstruct.pulse.BitStream[ir_stream.Tunion.Tstruct.bidxEndWord2];
	ir_zip.Tunion.Tstruct.Carrier=(unsigned char)(ir_stream.Tunion.Tstruct.Carrier);

	for(i=0;i<PulseCnt;i+=2)
			ir_stream.Tunion.Tstruct.pulse.BitStream[i]|=0x8000;

	//high
	for(i=0;i<PulseCnt;i++)
	{
		if(!(ir_stream.Tunion.Tstruct.pulse.BitStream[i]&0x8000))
			continue;

		wFoundVal=(ir_stream.Tunion.Tstruct.pulse.BitStream[i]&0x7fff);

		l=wFoundVal-barrFoundVal[0];
		//if(l==0) continue;  //margin=2
		if((l>=-Bit_Margine)&&(l<=Bit_Margine))
			continue;

		l=wFoundVal-barrFoundVal[1];
		//if(l==0) continue;  //margin=2
		if((l>=-Bit_Margine)&&(l<=Bit_Margine))
			continue;

		bFoundCnt=0;
		ulSumVal=0;

		for(j=0;j<PulseCnt;j++)
		{
			if(!(ir_stream.Tunion.Tstruct.pulse.BitStream[j]&0x8000))
				continue;

			High_real_time=(ir_stream.Tunion.Tstruct.pulse.BitStream[j]&0x7fff);
			l=wFoundVal-High_real_time;

			//if(l==0)   //margin=2
			if((l>=-Bit_Margine)&&(l<=Bit_Margine))
			{
				bFoundCnt++;
#ifdef OPTIMIZE
				ulSumVal+=High_real_time;
#else
				ulSumVal+=(unsigned long)High_real_time;
#endif
			}
		}

		ulSumVal/=bFoundCnt;     // ã�� ���� high ������ ���

		if((bFoundCnt>barrFoundCnt[0])&&(ulSumVal<0xffff))             // 1byte
		{
			barrFoundCnt[1]=barrFoundCnt[0];
			barrFoundVal[1]=barrFoundVal[0];

			barrFoundCnt[0]=bFoundCnt;
			barrFoundVal[0]=(unsigned char)ulSumVal;
		}
		else
		{
			if((bFoundCnt>barrFoundCnt[1])&&(ulSumVal<0xffff))         // 1byte
			{
				barrFoundCnt[1]=bFoundCnt;
				barrFoundVal[1]=(unsigned char)ulSumVal;
			}
		}
	}

	ir_zip.Tunion.Tstruct.barrPatHig[0]=barrFoundVal[0];
	ir_zip.Tunion.Tstruct.barrPatHig[1]=barrFoundVal[1];

	for(i=0;i<2;i++)                                                // ���� ����� �Ͼ�� PULSE�� ����. - LOW
	{
		barrFoundCnt[i]=0;
		barrFoundVal[i]=0;
	}
	for(i=0;i<PulseCnt;i++)
	{
		if((ir_stream.Tunion.Tstruct.pulse.BitStream[i]&0x8000))
			continue;

		wFoundVal=(ir_stream.Tunion.Tstruct.pulse.BitStream[i]);

		l=wFoundVal-barrFoundVal[0];
		//if(l==0)
		if((l>=-Bit_Margine)&&(l<=Bit_Margine))
			continue;  //margin = 2

		l=wFoundVal-barrFoundVal[1];
		//if(l==0)
		if((l>=-Bit_Margine)&&(l<=Bit_Margine))
			continue;  //margin = 2

		bFoundCnt=0;
		ulSumVal=0;

		for(j=0;j<PulseCnt;j++)
		{
			if((ir_stream.Tunion.Tstruct.pulse.BitStream[j]&0x8000))
				continue;

			LOW_real_time=(ir_stream.Tunion.Tstruct.pulse.BitStream[j]);

			l=wFoundVal-LOW_real_time;

			//if(l==0)
			if((l>=-Bit_Margine)&&(l<=Bit_Margine))
			{
				bFoundCnt++;


#ifdef OPTIMIZE
				ulSumVal+=LOW_real_time;
#else
				ulSumVal+=(double)LOW_real_time;
#endif
			}
		}
		ulSumVal/=bFoundCnt;

		if((bFoundCnt>barrFoundCnt[0])&&(ulSumVal<0xffff))             // 1byte
		{
			barrFoundCnt[1]=barrFoundCnt[0];
			barrFoundVal[1]=barrFoundVal[0];

			barrFoundCnt[0]=bFoundCnt;
			barrFoundVal[0]=(unsigned char)ulSumVal;
		}
		else
		{
			if((bFoundCnt>barrFoundCnt[1])&&(ulSumVal<0xffff))         // 1byte
			{
				barrFoundCnt[1]=bFoundCnt;
				barrFoundVal[1]=(unsigned char)ulSumVal;
			}
		}
	}

	ir_zip.Tunion.Tstruct.barrPatLow[0]=barrFoundVal[0];
	ir_zip.Tunion.Tstruct.barrPatLow[1]=barrFoundVal[1];


	Found_cnt=0;
	bFoundCnt=0;

	for(i=0;i<PulseCnt;i++)                                       // ������ PULSEã��.
	{
		wFoundVal=ir_stream.Tunion.Tstruct.pulse.BitStream[i];

		if(!(wFoundVal&0x8000))//i%2)                                                     // low data ��.
		{
			l=wFoundVal-ir_zip.Tunion.Tstruct.barrPatLow[0];
			//if(l==0)
			if((l>=-Bit_Margine)&&(l<=Bit_Margine))
			{
				Found_cnt++;
				continue;
			}
			else
			{
				//k=0;//debug
			}


			l=wFoundVal-ir_zip.Tunion.Tstruct.barrPatLow[1];
			//if(l==0)
			if((l>=-Bit_Margine)&&(l<=Bit_Margine))
			{
				Found_cnt++;
				continue;
			}
			else
			{
				//k=0; //debug
			}
			// low�� ��츸.

//#ifdef OPTIMIZE
			if((i==ir_stream.Tunion.Tstruct.bidxEndWord1)|| (i==ir_stream.Tunion.Tstruct.bidxEndWord2))
			{
				//k=0; //debug
				continue;
			}
//#else
//			if((((ir_stream.Tunion.Tstruct.bidxEndWord1!=0xff)&&
//			i==ir_stream.Tunion.Tstruct.bidxEndWord1))||
//			(i==ir_stream.Tunion.Tstruct.bidxEndWord2))
//			{
//				//k=0; //debug
//				continue;
//			}
//
//#endif
		}
		else                                                        // high data ��.
		{
			wFoundVal=wFoundVal&0x7fff;
			l=wFoundVal-ir_zip.Tunion.Tstruct.barrPatHig[0];
			//if(l==0)
			if((l>=-Bit_Margine)&&(l<=Bit_Margine))
			{
				Found_cnt++;
				continue;
			}
			else
			{
				//k=0; //debug
			}
			l=wFoundVal-ir_zip.Tunion.Tstruct.barrPatHig[1];
			//if(l==0)
			if((l>=-Bit_Margine)&&(l<=Bit_Margine))
			{
				Found_cnt++;
				continue;
			}
			else
			{
				//k=0; //debug
			}
		}

		for(j=0;j<(IRZIP_BUFFER_SIZE/2);j++)                            // data�������� ã��������.
		{
			l=wFoundVal-ir_zip.Tunion.Tstruct.uniData.warrPattern9_F[j];
			//if(l==0)
			if((l>=-Bit_Margine)&&(l<=Bit_Margine))
				break;
		}
		if((j>=(IRZIP_BUFFER_SIZE/2))&&(bFoundCnt<(IRZIP_BUFFER_SIZE/2)))   // Last all, not found!!!
		{
			ir_zip.Tunion.Tstruct.uniData.warrPattern9_F[bFoundCnt++]=wFoundVal; //?
		}
	}
	ir_zip.Tunion.Tstruct.bType|=bFoundCnt;             // D7D6 - TYPE


	bFoundCnt&=0x3f;
	CompressIdx = bFoundCnt*4;
	Found_cnt = Found_cnt/8;

	if( (IRZIP_BUFFER_SIZE - CompressIdx) < Found_cnt)
	{
		//index ����

		l=ir_stream.Tunion.Tstruct.pulse.BitStream[ir_stream.Tunion.Tstruct.bidxEndWord1];
		l2=ir_stream.Tunion.Tstruct.pulse.BitStream[ir_stream.Tunion.Tstruct.bidxEndWord2];
		if(l > l2)
		{
			gap = l - l2;
			standard = l * (ir_stream.Tunion.Tstruct.Carrier/4000000);
		}
		else
		{
			gap = l2 - l;
			standard = l2 * (ir_stream.Tunion.Tstruct.Carrier/4000000);
		}
		gap = gap * (ir_stream.Tunion.Tstruct.Carrier/4000000);
		margin = standard * 0.1;


		if(ir_stream.Tunion.Tstruct.bidxEndWord1 != ir_stream.Tunion.Tstruct.bidxEndWord2)
		{

			 ir_zip.Tunion.Tstruct.bidxEndWord2=ir_zip.Tunion.Tstruct.bidxEndWord1;
			 ir_zip.Tunion.Tstruct.bidxRepeatWord=0;

			if(gap > margin)
			{
				ir_zip.Tunion.Tstruct.bidxEndWord1_time=ir_stream.Tunion.Tstruct.pulse.BitStream[ir_stream.Tunion.Tstruct.bidxEndWord1];
				ir_zip.Tunion.Tstruct.bidxEndWord2_time=ir_zip.Tunion.Tstruct.bidxEndWord1_time;
			}
			else
			{
				ir_zip.Tunion.Tstruct.bidxEndWord1_time=(l+l2)/2;
				ir_zip.Tunion.Tstruct.bidxEndWord2_time=ir_zip.Tunion.Tstruct.bidxEndWord1_time;
			}
		}
		else
		{

		}
	}
	//return TRUE;
	return_int16u=1;
	return;}



void SetNibbleStream()
{
	if(nPosNib%2)
	{
		ir_zip.Tunion.Tstruct.uniData.barrNibbStream[nPosNib/2]&=0x0f0;
		g_temp&=0x0f;
	}
	else
	{
		ir_zip.Tunion.Tstruct.uniData.barrNibbStream[nPosNib/2]&=0x0f;
		g_temp<<=4;
	}
	ir_zip.Tunion.Tstruct.uniData.barrNibbStream[nPosNib/2]|=g_temp;

	if(++nPosNib>=IRZIP_BUFFER_SIZE*2)
		nPosNib--;
}


void SetBitData()
{
	if(g_temp)
		ir_zip.Tunion.Tstruct.uniData.barrBitData[(IRZIP_BUFFER_SIZE-1)-nPosBit/8]|=constBitMask[nPosBit%8];
	else
		ir_zip.Tunion.Tstruct.uniData.barrBitData[(IRZIP_BUFFER_SIZE-1)-nPosBit/8]&=(int8u)(~constBitMask[nPosBit%8]);

	if(++nPosBit>=IRZIP_BUFFER_SIZE*8)
		nPosBit--;
}


// �̹� ã�Ƴ��� PATTERN�� ������ PATTERN ARRAY�� DATA BIT�� �ϼ���Ŵ.
void CompressBitPattern()                                    // DATABASE ALGORITHM 1
{
	int16u   	i,j;
	int16s		l;

	PulseCnt = ir_zip.Tunion.Tstruct.bidxEndWord2+1;
	ppdata = (unsigned short*)&ir_stream.Tunion.Tstruct.pulse.BitStream[0];


#ifdef OPTIMIZE
	nPatCnt = (ir_zip.Tunion.Tstruct.bType)&0x3f;
#else
	nPatCnt = ir_zip.Tunion.Tstruct.bType;
	nPatCnt &= 0x3f;                                                  // bType�� b7|b7���� - only count
#endif

	// warrPattern9_F�� ������ ��ġ�� ����.
	nPosNib = nPatCnt*4;                                              // get the index by byte unit(*2) and by Nib(*2) =*4
	// || nibbel | nibble || nibble | nibble || ...
	//    ^ nPosNib --->

	for(i=nPatCnt*2; i<IRZIP_BUFFER_SIZE; i++)                            //    ^ data clear --->                        |
	{
		ir_zip.Tunion.Tstruct.uniData.barrBitData[i] = 0xff;
	}

	nWidBit = 0;
	nPosBit = 0;

	ppdata = (unsigned short*)&ir_stream.Tunion.Tstruct.pulse.BitStream[0];

	for(i=0; i<PulseCnt; i++)                                       // ������ PULSEã��.
	{
		wCurVal = *ppdata++;

		if((((!(wCurVal&0x8000))&&(((i)==ir_stream.Tunion.Tstruct.bidxEndWord1))))||   // low����.
		((!(wCurVal&0x8000))&&((i)==ir_stream.Tunion.Tstruct.bidxEndWord2)))
		{
			continue;
		}

		for(j=0;j<nPatCnt;j++)                                      // Data�� pattern ��.
		{
			wCurVal = wCurVal&0x7fff;
			l = wCurVal-ir_zip.Tunion.Tstruct.uniData.warrPattern9_F[j];

			if((l>=-Bit_Margine) && (l<=Bit_Margine))
			{
				if(nWidBit==0)
				{
				}
				else if(nWidBit<8)                                  // bit width >= 8
				{                                                   // bit<8�̸� 1nibble�� ����Ͽ� �߰�.
					g_temp = nWidBit;
					SetNibbleStream();
				}
				else
				{
					g_temp = 8;          // bit>=8�̸� 1nibble�� ����Ͽ� 8�� �߰��ϰ�.
					
					SetNibbleStream();
					
					nWidBit -= 8;                                     //            ��, 8�̸� �̾��� 1byte�� data����.
					l=nWidBit >> 4;
					l &= 0x0f;
					g_temp = l;
					
					SetNibbleStream();

					nWidBit &= 0x0f;
					g_temp = nWidBit;
					
					SetNibbleStream();
				}

				nWidBit = 0;
				g_temp = j+9;

				SetNibbleStream();
				break;
			}
		}
		if(j < nPatCnt)	continue;

		if((nPosNib/2)>=((IRZIP_BUFFER_SIZE-1)-(nPosBit/8)))
		{
			return_int16u = 0;
			return;
		}

		if(!(ir_stream.Tunion.Tstruct.pulse.BitStream[i]&0x8000))
		{
			wCurVal = ir_stream.Tunion.Tstruct.pulse.BitStream[i]&0x7fff;
			l = wCurVal-ir_zip.Tunion.Tstruct.barrPatLow[0];

			if((l>=-Bit_Margine) && (l<=Bit_Margine))
			{
				g_temp = 0;
				SetBitData();                      // new bit pos'
				nWidBit++;
				continue;
			}

			l = wCurVal-ir_zip.Tunion.Tstruct.barrPatLow[1];

			if((l>=-Bit_Margine) && (l<=Bit_Margine))
			{
				g_temp = 1;
				SetBitData();                      // new bit pos'
				nWidBit++;
				continue;
			}
		}
		// HIGH ����.
		else                                                        // high data ��.
		{
			wCurVal = ir_stream.Tunion.Tstruct.pulse.BitStream[i]&0x7fff;
			l = wCurVal-ir_zip.Tunion.Tstruct.barrPatHig[0];

			if((l>=-Bit_Margine) && (l<=Bit_Margine))
			{
				g_temp = 0;
				SetBitData();                      // new bit pos'
				nWidBit++;
				continue;
			}

			l=wCurVal-ir_zip.Tunion.Tstruct.barrPatHig[1];

			if((l>=-Bit_Margine) && (l<=Bit_Margine))
			{
				g_temp = 1;
				SetBitData();

				nWidBit++;
				continue;
			}
		}
	}

	// �ٸ� PATTERN�� ���� PATTERN���� �߰����� ��������.
	//      DATA�� ���� �ִ»���.
	if(nWidBit)                                             // bit width >= 8
	{
		if(nWidBit>=8)
		{
			g_temp = 8;
			SetNibbleStream();

			nWidBit-=8;
			l=nWidBit>>4;
			l&=0x0f;
			g_temp = l;
			SetNibbleStream();             // 1byte dataũ��.

			nWidBit &= 0x0f;
		}
		g_temp = nWidBit;
		SetNibbleStream();
	}
	return_int16u=1;
	return;
}
//<< ADD 2008.01.30 ���� ��


void Pulse_time_cal()
{
	//wCurVal=(unsigned short)((float)(ir_stream.Tunion.Tstruct.Carrier/4)*(wCurVal));
	wCurVal=(unsigned short)((double)(ir_stream.Tunion.Tstruct.Carrier/(double)4)*(wCurVal));
}

//=============================================================================================
// type 42B toogle check
//=============================================================================================

void learn_toggle_check_42B()
{
	int16s       l;

	if(!(gl_Toggle[curr_source]%2))
	{
		return_int8u = 0;
		return;
	}

	wCurVal= ir_stream.Tunion.Tstruct.pulse.BitStream[0] & 0x7fff;
	Pulse_time_cal();
	l=wCurVal - 120;   																// 120us //high ���� ��������

	if(l>30 || l < -30 )
	{
		return_int8u = 0;
		return;    																			//42B �� �ƴϴ�.
	}

	wCurVal= ir_stream.Tunion.Tstruct.pulse.BitStream[1];
	Pulse_time_cal();
	l=wCurVal - 7350;   															// 7400us //low ���� ��������

	if(l>120 || l < -120 )
	{
		return_int8u = 0;
		return;    																			//42B �� �ƴϴ�.
	}

	if(ir_stream.Tunion.Tstruct.bidxEndWord1 != ir_stream.Tunion.Tstruct.bidxEndWord2)
	{
		ir_stream.Tunion.Tstruct.bidxEndWord2 = ir_stream.Tunion.Tstruct.bidxEndWord1;
		ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
	}

	wCurVal= ir_stream.Tunion.Tstruct.pulse.BitStream[3] & 0x7fff;
	Pulse_time_cal();

	l=wCurVal - 7350;
	wCurVal= ir_stream.Tunion.Tstruct.pulse.BitStream[1];

	if(l>120 || l < -120 )
		ir_stream.Tunion.Tstruct.pulse.BitStream[3]=wCurVal;
//	else
//		ir_stream.Tunion.Tstruct.pulse.BitStream[3]=(wCurVal/(double)1.5);

	return_int8u = 1;
	return;
}



void MeltBitPattern(IR_ZIP_ST  *pzipdata)        // DATABASE ALGORITHM 1
{
	int16u	i;
	byte	j;

	for(i=0; i<BITSTREAMLENGTH; i++)
	{
		ir_stream.Tunion.Tstruct.pulse.BitStream[i] = 0;
	}

	ir_stream.Tunion.Tstruct.Carrier = pzipdata->Tunion.Tstruct.Carrier;
	npWaddr_1 = &ir_stream.Tunion.Tstruct.pulse.BitStream[0];

	#ifdef OPTIMIZE
	nPosNib=(pzipdata->Tunion.Tstruct.bType)&0x3f;
//	nPosNib&=0x3f;                                                  // bType�� b7|b7���� - only count
	#else
	nPosNib = pzipdata->Tunion.Tstruct.bType;
	nPosNib &= 0x3f;                                                  // bType�� b7|b7���� - only count
	#endif

	nPosNib *= 4;                                                     // get the index by byte unit(*2) and by Nib(*2) =*4

	nPosBit = 0;

	ir_stream.Tunion.Tstruct.bidxEndWord2   = pzipdata->Tunion.Tstruct.bidxEndWord2;
	ir_stream.Tunion.Tstruct.bidxEndWord1   = pzipdata->Tunion.Tstruct.bidxEndWord1;          // bidxEndWord1�� 1word�� wait����.
	ir_stream.Tunion.Tstruct.bidxRepeatWord = pzipdata->Tunion.Tstruct.bidxRepeatWord;

	//>> �߰��
	for(i=0; ;i++)
	{
		if(((i%2)&&((i) == pzipdata->Tunion.Tstruct.bidxEndWord2)))//low
		{
			*npWaddr_1 += pzipdata->Tunion.Tstruct.bidxEndWord2_time;
			ir_stream.Tunion.Tstruct.bidxEndWord2 = npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
			break;
		}

		if(((i%2)&&((i) == pzipdata->Tunion.Tstruct.bidxEndWord1)))
		{
			*npWaddr_1+=pzipdata->Tunion.Tstruct.bidxEndWord1_time;
			ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];

			if(++npWaddr_1>&ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH-1])
			{
				return_int16u=0;
				return;
			}
			continue;
		}

		bNibble = pzipdata->Tunion.Tstruct.uniData.barrNibbStream[nPosNib/2];
	
		if(nPosNib % 2)
		{
			bNibble &= 0x0f;
		}
		else
		{
			bNibble >>= 4;
		}
	
		nPosNib++;

		if(bNibble >= 0x09)                                           // etc
		{
			bNibble -= 0x09;
			*npWaddr_1 = pzipdata->Tunion.Tstruct.uniData.warrPattern9_F[bNibble];

			if((i%2)==0)
			{
				*npWaddr_1 += 0x8000;
			}

			if(++npWaddr_1>&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM-1])
			{
				return_int16u=0;
				return;
			}
		}
		else if(bNibble==0)
		{
			break;
		}
		else                                                        // data
		{
			if(bNibble==8)                                          // 1byteũ�� ����.
			{
				j=8;

				bNibble=pzipdata->Tunion.Tstruct.uniData.barrNibbStream[nPosNib/2];

				if(nPosNib%2)
					bNibble &= 0x0f;
				else
					bNibble >>= 4;

				bNibble <<= 4;
				nPosNib++;
				j+=bNibble;

				bNibble = pzipdata->Tunion.Tstruct.uniData.barrNibbStream[nPosNib/2];

				if(nPosNib%2)
					bNibble &= 0x0f;
				else
					bNibble >>= 4;

				bNibble &= 0x0f;
				nPosNib++;

				j += bNibble;

				bNibble = (unsigned char)j;
			}

			for(j=0; j<bNibble; j++)
			{
				if(((i%2)&&((i) == pzipdata->Tunion.Tstruct.bidxEndWord1)))
				{
					if(i%2)
					{
						*npWaddr_1+=pzipdata->Tunion.Tstruct.bidxEndWord1_time;
						ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];

						if(++npWaddr_1>&ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH-1])
						{
							return_int16u = 0;
							return;
						}
						i++;
					}
					else
					{
						i++;
						if(++npWaddr_1 > &ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH-1])
						{
							return_int16u = 0;
							return;
						}
						*npWaddr_1 = pzipdata->Tunion.Tstruct.bidxEndWord1_time;
						ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];

						i++;

						if(++npWaddr_1>&ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH-1])
						{
							return_int16u = 0;
							return;
						}
						*npWaddr_1 = 0x8000;
					}

					j--;
					continue;
				}

				if(pzipdata->Tunion.Tstruct.uniData.barrBitData[(IRZIP_BUFFER_SIZE-1)-nPosBit/8]&constBitMask[nPosBit%8])
				{
					if(i%2)
					{
						*npWaddr_1 = pzipdata->Tunion.Tstruct.barrPatLow[1];
					}
					else
					{
						*npWaddr_1 = pzipdata->Tunion.Tstruct.barrPatHig[1];
						*npWaddr_1 += 0x8000;
					}
					if(++npWaddr_1> & ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH-1])
					{
						return_int16u = 0;
						return;
					}
				}
				else
				{
					if(i%2)
					{
						*npWaddr_1 = pzipdata->Tunion.Tstruct.barrPatLow[0];
					}
					else
					{
						*npWaddr_1 = pzipdata->Tunion.Tstruct.barrPatHig[0];
						*npWaddr_1 += 0x8000;
					}
					if(++npWaddr_1>&ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH-1])
					{
						return_int16u = 0;
						return;
					}
				}

				i++;                                                // !!! ���� !!!
				nPosBit++;
			}
		i--;
		}
	}


	if((ir_stream.Tunion.Tstruct.bidxRepeatWord > 0) &&(ir_stream.Tunion.Tstruct.bidxRepeatWord != ir_stream.Tunion.Tstruct.bidxEndWord2 )) //repeat
	{
		return;
	}

	learn_toggle_check_42B();
	if(!return_int8u)
	{
		learn_toggle_check_RC5();
		if(!return_int8u)
		{
			learn_toggle_check_RC6();
		}
	}

	return_int16u = 1;

	return;
}

void learn_toggle_check_RC5()
{
	int 	raw0,raw1,raw2,raw3,raw4;
	int 	pattern=0;
	int  	i,cycle;
	unsigned short    temp;

 // toogle �ϴ� grobal ���� check
	if(!(gl_Toggle[curr_source]%2))
	{
			return_int8u = 2;
			return;
	}

	wCurVal=ir_stream.Tunion.Tstruct.pulse.BitStream[0] & 0x7fff;
	Pulse_time_cal();
	raw0=wCurVal;

	wCurVal=ir_stream.Tunion.Tstruct.pulse.BitStream[1] & 0x7fff;
	Pulse_time_cal();
	raw1=wCurVal;

	wCurVal=ir_stream.Tunion.Tstruct.pulse.BitStream[2] & 0x7fff;
	Pulse_time_cal();
	raw2=wCurVal;

	wCurVal=ir_stream.Tunion.Tstruct.pulse.BitStream[3] & 0x7fff;
	Pulse_time_cal();
	raw3=wCurVal;

	wCurVal=ir_stream.Tunion.Tstruct.pulse.BitStream[4] & 0x7fff;
	Pulse_time_cal();
	raw4=wCurVal;

	if((raw0 < (RC5_SHORT-RC5_MARGINE)) || (raw0 > (RC5_SHORT+RC5_MARGINE)))
		if((raw0 < (RC5_LONG-RC5_MARGINE)) || (raw0 > (RC5_LONG+RC5_MARGINE)))
		{
			return_int8u=0;
			return;
		}
	if((raw1 < (RC5_SHORT-RC5_MARGINE)) || (raw1 > (RC5_SHORT+RC5_MARGINE)))
		if((raw1 < (RC5_LONG-RC5_MARGINE)) || (raw1 > (RC5_LONG+RC5_MARGINE)))
		{
			return_int8u=0;
			return;
		}
	if((raw2 < (RC5_SHORT-RC5_MARGINE)) || (raw2 > (RC5_SHORT+RC5_MARGINE)))
		if((raw2 < (RC5_LONG-RC5_MARGINE)) || (raw2 > (RC5_LONG+RC5_MARGINE)))
		{
			return_int8u=0;
			return;
		}
	if((raw3 < (RC5_SHORT-RC5_MARGINE)) || (raw3 > (RC5_SHORT+RC5_MARGINE)))
		if((raw3 < (RC5_LONG-RC5_MARGINE)) || (raw3 > (RC5_LONG+RC5_MARGINE)))
		{
			return_int8u=0;
			return;
		}
	if((raw4 < (RC5_SHORT-RC5_MARGINE)) || (raw4 > (RC5_SHORT+RC5_MARGINE)))
		if((raw4 < (RC5_LONG-RC5_MARGINE)) || (raw4 > (RC5_LONG+RC5_MARGINE)))
		{
			return_int8u=0;
			return;
		}

	if(raw0 > RC5_DECIDE_LS)
		raw0 = 1;
	else
		raw0 = 0;
	if(raw1 > RC5_DECIDE_LS)
		raw1 = 1;
	else
		raw1 = 0;
	if(raw2 > RC5_DECIDE_LS)
		raw2 = 1;
	else
		raw2 = 0;
	if(raw3 > RC5_DECIDE_LS)
		raw3 = 1;
	else
		raw3 = 0;
	if(raw4 > RC5_DECIDE_LS)
		raw4 = 1;
	else
		raw4 = 0;


	if((raw0)&&(!raw1)&&(!raw2)&&(!raw3))
		pattern=0;
	else if((raw0)&&(!raw1)&&(!raw2)&&(raw3))
		pattern=1;
	else if((raw0)&&(raw1)&&(raw2))
		pattern=2;
	else if((raw0)&&(raw1)&&(!raw2))
		pattern=3;
	else if((!raw0)&&(!raw1)&&(raw2)&&(!raw3))
		pattern=4;
	else if((!raw0)&&(!raw1)&&(raw2)&&(raw3))
		pattern=5;
	else if((!raw0)&&(!raw1)&&(!raw2)&&(!raw3)&&(raw4))
		pattern=6;
	else if((!raw0)&&(!raw1)&&(!raw2)&&(!raw3)&&(!raw4))
		pattern=7;
	else
	{
		return_int8u=0;
		return;
	}

	switch(pattern)
	{
		case 0:
			ir_stream.Tunion.Tstruct.pulse.BitStream[2] = ir_stream.Tunion.Tstruct.pulse.BitStream[2] & 0x7fff;
			ir_stream.Tunion.Tstruct.pulse.BitStream[3] = ir_stream.Tunion.Tstruct.pulse.BitStream[3] | 0x8000;
			break;
		case 1:
		case 3:
			temp = ir_stream.Tunion.Tstruct.pulse.BitStream[1];
			ir_stream.Tunion.Tstruct.pulse.BitStream[1] = ir_stream.Tunion.Tstruct.pulse.BitStream[3];
			ir_stream.Tunion.Tstruct.pulse.BitStream[3] = temp;
			break;
		case 2:
			cycle=ir_stream.Tunion.Tstruct.bidxEndWord1;
			for(i=cycle;i>2;i--)
			{
				ir_stream.Tunion.Tstruct.pulse.BitStream[i+2] = ir_stream.Tunion.Tstruct.pulse.BitStream[i];
			}
			ir_stream.Tunion.Tstruct.pulse.BitStream[1] = ir_stream.Tunion.Tstruct.pulse.BitStream[1]/2;
			ir_stream.Tunion.Tstruct.pulse.BitStream[2] = ir_stream.Tunion.Tstruct.pulse.BitStream[1] | 0x8000;
			ir_stream.Tunion.Tstruct.pulse.BitStream[3] = ir_stream.Tunion.Tstruct.pulse.BitStream[1];
			ir_stream.Tunion.Tstruct.pulse.BitStream[4] = ir_stream.Tunion.Tstruct.pulse.BitStream[2];
			ir_stream.Tunion.Tstruct.bidxEndWord1+=2;
			ir_stream.Tunion.Tstruct.bidxEndWord2 = ir_stream.Tunion.Tstruct.bidxEndWord1;
			break;
		case 4:
		case 6:
			temp = ir_stream.Tunion.Tstruct.pulse.BitStream[2];
			ir_stream.Tunion.Tstruct.pulse.BitStream[2] = ir_stream.Tunion.Tstruct.pulse.BitStream[4];
			ir_stream.Tunion.Tstruct.pulse.BitStream[4] = temp;
			break;
		case 5:
			cycle=ir_stream.Tunion.Tstruct.bidxEndWord1;
			for(i=cycle;i>3;i--)
			{
				ir_stream.Tunion.Tstruct.pulse.BitStream[i+2] = ir_stream.Tunion.Tstruct.pulse.BitStream[i];
			}
			ir_stream.Tunion.Tstruct.pulse.BitStream[2] = ir_stream.Tunion.Tstruct.pulse.BitStream[2] & 0x7fff;
			ir_stream.Tunion.Tstruct.pulse.BitStream[2] = ir_stream.Tunion.Tstruct.pulse.BitStream[2]/2;
			ir_stream.Tunion.Tstruct.pulse.BitStream[2] = ir_stream.Tunion.Tstruct.pulse.BitStream[2] | 0x8000;
			ir_stream.Tunion.Tstruct.pulse.BitStream[3] = ir_stream.Tunion.Tstruct.pulse.BitStream[2] & 0x7fff;
			ir_stream.Tunion.Tstruct.pulse.BitStream[4] = ir_stream.Tunion.Tstruct.pulse.BitStream[2];
			ir_stream.Tunion.Tstruct.pulse.BitStream[5] = ir_stream.Tunion.Tstruct.pulse.BitStream[3];
			ir_stream.Tunion.Tstruct.bidxEndWord1+=2;
			ir_stream.Tunion.Tstruct.bidxEndWord2 = ir_stream.Tunion.Tstruct.bidxEndWord1;
			break;
		case 7:
			ir_stream.Tunion.Tstruct.pulse.BitStream[3] = ir_stream.Tunion.Tstruct.pulse.BitStream[3] | 0x8000;
			ir_stream.Tunion.Tstruct.pulse.BitStream[4] = ir_stream.Tunion.Tstruct.pulse.BitStream[4] & 0x7fff;
			break;

	}

	return_int8u=1;
	return;


}


//=============================================================================================
//	type RC6 toogle check
//=============================================================================================

//>>ADD 07.03

void learn_toggle_check_RC6()
{
	int				i,cycle,k;
	unsigned short	 /*pulse_time,*/t;
	//unsigned short	 Carrier;
	unsigned short	 *pToggle;
	int				l;
	unsigned short  toggle_st;

	if(!(gl_Toggle[curr_source]%2))
	{
			return;
	}

	pToggle=(unsigned short *)&RC6[0];
//	Carrier=ir_stream.Tunion.Tstruct.Carrier;
	i=0;
	for(k=0;k<8;k++)
	{
		wCurVal= ir_stream.Tunion.Tstruct.pulse.BitStream[k] & 0x7fff;
		Pulse_time_cal();

		//pulse_time=(unsigned short)((float)(Carrier/4)*(pulse_time));

		l=wCurVal - *(pToggle+k);
		if( l<=110 && l>=-110) continue;	//1'st lead high
		else
		{
			i=0x01;
			break;
		}
	}

	if(i)
		return;

	cycle=ir_stream.Tunion.Tstruct.bidxEndWord1;

	if(ir_stream.Tunion.Tstruct.bidxEndWord1 != ir_stream.Tunion.Tstruct.bidxEndWord2)
	{
		ir_stream.Tunion.Tstruct.bidxEndWord2 = ir_stream.Tunion.Tstruct.bidxEndWord1;
		ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
	}
	toggle_st = cycle*3;

	wCurVal= ir_stream.Tunion.Tstruct.pulse.BitStream[8] & 0x7fff;
	//pulse_time=(unsigned short)((float)(Carrier/4)*(pulse_time));
	Pulse_time_cal();

	l=wCurVal - RC6_TOGGLE_LONG;
	if(l<=110 && l>= -110)
	{
		wCurVal= ir_stream.Tunion.Tstruct.pulse.BitStream[9] & 0x7fff;
		//pulse_time=(unsigned short)((float)(Carrier/4)*(pulse_time));
		Pulse_time_cal();
		l=wCurVal - RC6_TOGGLE_LONG;
		if(l<=110 && l>= -110)
		{
			//copy
			for(i=0;i<(cycle+1);i++)
			{
				ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+i]=ir_stream.Tunion.Tstruct.pulse.BitStream[i];
				ir_stream.Tunion.Tstruct.pulse.BitStream[i]=0;
			}

			for(i=0;i<8;i++)
				ir_stream.Tunion.Tstruct.pulse.BitStream[i]=ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+i];


			for(i=10;i<cycle+1;i++)
			ir_stream.Tunion.Tstruct.pulse.BitStream[i+2] = ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+i];


			ir_stream.Tunion.Tstruct.pulse.BitStream[8]=ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+2];			//8
			ir_stream.Tunion.Tstruct.pulse.BitStream[9]=ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+3];			//9
			ir_stream.Tunion.Tstruct.pulse.BitStream[10]=(ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+3]|0x8000);	//10
			ir_stream.Tunion.Tstruct.pulse.BitStream[11]=ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+5];			//11
			ir_stream.Tunion.Tstruct.bidxEndWord1 = cycle+4;
			ir_stream.Tunion.Tstruct.bidxEndWord2 = cycle+4;

		}
		else
		{
			t=ir_stream.Tunion.Tstruct.pulse.BitStream[10];
			ir_stream.Tunion.Tstruct.pulse.BitStream[10] =ir_stream.Tunion.Tstruct.pulse.BitStream[8];
			ir_stream.Tunion.Tstruct.pulse.BitStream[8] =t;
		}
	}
	else//short
	{
		wCurVal= ir_stream.Tunion.Tstruct.pulse.BitStream[10] & 0x7fff;
		//pulse_time=(unsigned short)((float)(Carrier/4)*(pulse_time));
		Pulse_time_cal();
		l=wCurVal - RC6_TOGGLE_LONG;
		if(l<=110 && l>= -110)
		{
			t=ir_stream.Tunion.Tstruct.pulse.BitStream[10];
			ir_stream.Tunion.Tstruct.pulse.BitStream[10] =ir_stream.Tunion.Tstruct.pulse.BitStream[8];
			ir_stream.Tunion.Tstruct.pulse.BitStream[8] =t;
		}
		else
		{
			for(i=0;i<(cycle+1);i++)
			{
				ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+i]=ir_stream.Tunion.Tstruct.pulse.BitStream[i];
				ir_stream.Tunion.Tstruct.pulse.BitStream[i]=0;
			}

			for(i=0;i<8;i++)
				ir_stream.Tunion.Tstruct.pulse.BitStream[i]=ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+i];


			for(i=10;i<cycle-1;i++)
				ir_stream.Tunion.Tstruct.pulse.BitStream[i] = ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+i+2];

			t=ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+8];
			t+=(ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+9]&0x7fff);
			ir_stream.Tunion.Tstruct.pulse.BitStream[8] = t;
			t=ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+11];
			t+=(ir_stream.Tunion.Tstruct.pulse.BitStream[toggle_st+10]&0x7fff);
			ir_stream.Tunion.Tstruct.pulse.BitStream[9] = t;
		}
	}

	ir_stream.Tunion.Tstruct.bidxEndWord1 -= 2;
	ir_stream.Tunion.Tstruct.bidxEndWord2 -= 2;
}

#endif	//USE_IR_LEARN

#endif	//__IR_LEARN_C__

