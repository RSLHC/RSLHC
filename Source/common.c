
#ifndef __COMMON_C__
#define __COMMON_C__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include 	"main.h"
#include	"common.h"
#include	"function_key.h"
#include	"hal.h"
#include	"flash.h"
#include	"crt.h"
//#include	"Eeprom.h"

#if (USE_IR_LEARN == TRUE)
#include	"learn.h"
#endif

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/
set_number_t LoadSetNumber(BYTE *p)
{
	set_number_t value;

#if (USE_2BYTE_SETTING_NO == TRUE)
	//value = *p;
	//value |= *(p+1)<<8;
	value = *p<<8;
	value |= *(p+1);
#else
	value = *p;
#endif
	return value;
}

int8u Panasonic_TV_Check(int8u device, set_number_t set_no)
{
	int8u 	rtrn = FALSE;
#if	(USE_ADDRESS_32BIT == TRUE)
	uint32* pAddr;
#else
	WORD* 	pAddr;
#endif
	byte*	pAddr2;
	set_number_t	bLongFrameNo;

	if((IrKeyBuf != KEY_TVPOWER)
		&&(IrKeyBuf != KEY_ALLPOWER)
		)
		return rtrn;

#if	(USE_ADDRESS_32BIT == TRUE)
	pAddr = (uint32 *)(X_LONGFRAME + device);
#else
	pAddr = (WORD *)(X_LONGFRAME + device);
#endif

	pAddr2 = (byte code*) *pAddr;

	while(1)
	{
		bLongFrameNo = LoadSetNumber(pAddr2);
		if(bLongFrameNo == 0)
		{
			rtrn = FALSE;
			break;
		}

		if(bLongFrameNo == set_no) 
		{
			rtrn = TRUE;
			break;
		}

		pAddr2 += INC_PTR_SIZE;
	}
	return rtrn;
}

void SetMode_StartTimeOut(int16u time)
{
	Timer0_65ms16_2 = time;
	Timer0_65msStart();
}

#if (USE_AVSHIFT_MODE == TRUE)
void Timer0_avShiftMode_Update(int16u time)
{
	Timer0_avShiftMode = time;
}

void avShiftMode_StartTimeOut(int16u time)
{
	Timer0_avShiftMode = time;
	Timer0_65msStart();
}
#endif

#if (USE_IR_LEARN == TRUE)
void IrZipData_Clear(void)
{
	BYTE	i;

	for(i=0; i<FULL_IRZIP_BUFFER_SIZE; i++)
	{
		ir_zip.Tunion.data1[i] = 0;
	}
}
#endif

void wBitStream_Clear(void)
{
	int8u* bit_Clear;

	bit_Clear = (int8u*)&ir_stream.Tunion.data1[0];

	for( ; bit_Clear <= (int8u*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]; bit_Clear++)
	{
		*bit_Clear = 0;
	}

//	*(++bit_Clear) = 0;
}

void WDG_SystemReset(void)
{
	ClrWDT();

	WDTCON = 0x07; 	// Bit7~Bit3 : WDTRL[4:0]: 5-bit initial value for down counter with auto-load, the value is 1FH by default.
					// Bit1~Bit0 : WDTPR[1:0]: WDT clock prescale selection bit.
					//			   00: RCL/16.
					//			   01: RCL/32.
					//			   10: RCL/64.
					//			   11: RCL/128 (default).
					//			   RCL(=20KHz) / 128(WDTPR[1:0]=11) => 44ms(Overflow time step)

	xSYSCFG  |= (1<<WDTCFG); //WDT is on in STOP mode.
	xSYSCFG  &= ~(1<<RCLCFG); // RCL is on.
	
	while(1){}
}

void SW_SystemReset(void)
{
	RSTSRC |= (1<<SOFTF); 	// SOFTF: software reset flag bit, set to 1 when MCU excute the reset instruction.
}

//----------------------------------------
// Low Voltage Check
//----------------------------------------
#if (USE_BATT_MONITOR == TRUE)
void Low_Voltage_Check(byte lvi_key)
{
//	byte	i;

	lvi_check_key = 0;

	switch(lvi_key)
	{
		case	KEY_ALLPOWER:
			lvi_check_key = 1;
			break;
		case	KEY_TVPOWER:
		case	KEY_TVVOLUP:
		case	KEY_TVVOLDOWN:
		case	KEY_INPUT:
			lvi_check_key = 0;
			break;
		default:
			lvi_check_key = 2;
			break;
	}

	if(LVDCON & (1<<LVDF))		// LVDF : low voltage detecting flag bit.
	{
		keyCnt_inLow--;		//BOF flag detected !
	}
	else
	{
		keyCnt_inLow=LVI_CHECK_CNT;
	}

	if(keyCnt_inLow>LVI_CHECK_CNT)
		keyCnt_inLow=LVI_CHECK_CNT;
}
#endif

void Delayby10us(int16u nCnt)
{
	int16u	i = 0;

/*	unsigned char xdata i,loopcount;

#if (MAIN_OSC == OSC_4MHZ)
	loopcount =32; //40us / 1 intruction excute time = 0.5 us
#elif (MAIN_OSC == OSC_8MHZ)
	loopcount =64; //40us / 1 intruction excute time = 0.25 us
#elif (MAIN_OSC == OSC_10MHZ)
	loopcount =80; //40us / 1 intruction excute time = 0.2 us
#elif (MAIN_OSC == OSC_12MHZ)
	loopcount =96; //40us / 1 intruction excute time = 0.166 us
#endif
*/
//	loopcount--;
//	for(i=0;i<loopcount;i++) { _nop_(); _nop_(); _nop_();_nop_();}

	for(i=0; i<nCnt; i++)
	{
		//wait_10us();		// warning 발생으로 주석 처리(8MHZ OSC에 대한 nop() 추가
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#if(MAIN_OSC == OSC_16MHZ)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#endif
	}
}

//----------------------------------------
// INPUT: key_buf[0]
// OUTPUT: key_cnt=key press no(갯수)
//         keybuf=key index no.
//----------------------------------------
void getkey(unsigned char* p)
{
	BYTE  keysel;
	BYTE  i, j, keydata;

//	KEYSCAN_Interrupt_di;

	for(i=0; i<KEY_BUFF_CNT; i++)
	{
		*(p+i) = NO_KEY;			//key_buf[4] 초기화
	}

	
	key_cnt = 0;

	// Outputs "H" level to GPIO. 
	P2 |= _0110_0111B;  // P2.0(OUT0), P2.1(OUT1), P2.2(OUT2), P2.5(OUT3), P2.6(OUT4) 
	P0 |= _1100_0000B;  // P0.6(OUT5), P0.7(OUT6)

	P2 &= _1111_1110B; //P2.0 부터 Search 시작
	keysel = P2;       //strobe

	for(i=0; i<KEY_OUTPUT_MAX_COUNT; i++)
	{
		/* minimum 40us needed for high recognition because input port pullup resistor needs 40us rising time */
		Delayby10us(4);				// Key stabilization time

		//keydata = (P0 & _1111_1111B);
		keydata = (P1 & _1111_1111B);

		for(j=0; j<KEY_INPUT_MAX_COUNT; j++)
		{
			if(key_cnt > KEY_BUFF_CNT) break;
			if(keydata == NO_KEY) break;

			if (((1 << j) & keydata) == 0)
			{
				p[key_cnt++] = j + (i * KEY_INPUT_MAX_COUNT) + 1;
			}
		}

		if(i < P2_N_BIT_MAX_COUNT) // P2.n에 대한 처리
		//if(i < 5) // P2.n에 대한 처리
		{
			keysel = _0110_0111B;
			if(i >= 2) // P2.5(OUT3), P2.6(OUT4)에 대한 처리
				keysel &= ~(1<<i+3);
			else
				keysel &= ~(1<<i+1);

			if(i == 4)
			{
				P2 = _0110_0111B;
				P0 &= _1011_1111B;
				keysel = P0;
			}
			else
				P2 = keysel & _0110_0111B;
		}
		else if (i == 5) // P0.n에 대한 처리
		{
			P0 |= (1<<6);
			P0 &= _0111_1111B;
		}	

		else //i == 6
			P0 |= _1100_0000B;
	}

	/* Outputs "L" level to GPIO */
	P2 &= _1001_1000B;
	P0 &= _0011_1111B;

	codeCvt(p);
}

#if 0
void key_changed_check(void)
{
	int8u 	bi;

	key_changed = 0;
	for(bi=0;bi<4;bi++)
	{
		if(key_buf[bi]!=key_buf_old[bi])
			key_changed = 1;

		key_buf_old[bi]=key_buf[bi];
	}
}
#endif


void key_buf_old_clear(void)
{
	unsigned char i;

	for(i=0; i<4; i++) key_buf_old[i]=0;
}

unsigned char getkey_simple(void)
{
	KBI_Interrupt_di;

	/* Outputs "L" level to GPIO. */
	P2 &= ~KEY_OUTPUT_PINS_P2;
	P0 &= ~KEY_OUTPUT_PINS_P0;

	// Key stabilization time

	Delayby10us(4);

	if(P1 == NO_KEY)	return FALSE;
	else				return TRUE;
}


//----------------------------------------;
// wait for key off.
//----------------------------------------
void wait_getkey_simple(void)
{
	while(getkey_simple()){}
	t0_wait(1);
}

//----------------------------------------
// holding key in after ir out.
//----------------------------------------
#if 0
void getkey_wait(void)
{
	while(1)
	{
		getkey(&key_buf[0]);
		if(key_cnt==0) break;
		if(key_cnt>=2) break;
		if(key_cnt==1) continue;
	}
}
#endif

//----------------------------------------
// holding key in after ir out.
//----------------------------------------
#if 0
void get2key_wait(void)
{
	while(1)
	{
		getkey(&key_buf[0]);
		if(key_cnt == 2) continue;
		else	break;
	}
}
#endif

//----------------------------------------
// INPUT: &key_buf[0]
// OUTPUT: key_buf (key index)
//                -> key_buf (define key no)
//----------------------------------------

void codeCvt(BYTE* pKey_buf)
{
	byte	i[4], j;
	BYTE code* cpX_cvt;

	cpX_cvt=(BYTE code*)X_CODECVT;

	if(key_cnt > KEY_BUFF_CNT) key_cnt = KEY_BUFF_CNT;

	for(j=0; j<key_cnt; j++) 						//initial, key_cnt는 max 4까지만 가능 함.
	{
		i[j] = *(pKey_buf+j);
		i[j] = i[j]-1;
	}

	for(j=0; j<key_cnt; j++)
	{
		pKey_buf[j] = *(cpX_cvt + i[j]);
	}
}

//----------------------------------------
// map convert
//----------------------------------------
int8u get_map_pointer(void)
{
	//byte	i = key_buf[0] - 1;
	byte	i = IrKeyBuf - 1;
	byte	idxKey;

#if	(USE_ADDRESS_32BIT == TRUE)
	cpX_keymap=(ULONG *)X_KEYMAP;
#else
	cpX_keymap = (WORD *)X_KEYMAP;
#endif

	cpX_keymap = (cpX_keymap + curr_device);	//each device map pointer

	cpX_keymap2 = (BYTE code*) *cpX_keymap;
	idxKey = *(cpX_keymap2 + i);				//idxKey-> map value

	cpX_keymap2 = (BYTE *) X_MAPDUMMY;
	i = *cpX_keymap2;							//map dummy check.

	if(idxKey != i) return idxKey;

	return MAP_NG;
}

//----------------------------------------
// call from main()
//----------------------------------------
void my_sub(void)
{
	t1_int=1;

	//if(backup_key!=key_buf[0])
	if(backup_IrKeyBuf == 0xff)
	{
		my_newkeyincond();				//first key in

		wBitStream_Clear();

#if (USE_STUCKKEY_TIMER == TRUE)
		StuckKeyTimer_Start(STUCKKEY_TIMEOUT_S);
#endif

		if(Send_UR_Ready()==FALSE)
		{
			//backup_key=0xff;
			ir_ok=FALSE;
			outframeNo=0;
			backup_IrKeyBuf=0xff;
			return;
		}

		gl_Toggle[curr_source]++;

		ir_ok=TRUE;

#if 1
#if (USE_AVSHIFT_MODE == TRUE)
		if(!avShiftMode_active)
#endif
		device_led_on(IR_SENDING_LED);

#else
		if( (curr_state==SET_MODE_DIGIT) || (curr_state==SET_MODE_AUTO) )
			device_led_on(setting_source);
		else
			device_led_on(curr_source);
#endif

		send_ur();
	}
}

// 새로운 키 입력
void my_newkeyincond(void)
{
	keyholdcnt=0;
	backup_key=key_buf[0];
	backup_IrKeyBuf = IrKeyBuf;
	//wBitStream_Clear();			// MULTIPLE CALL TO SEGMENT WARNING ????? ???
}

void Timer0_65ms16_2_Update(int16u time)
{
	Timer0_65ms16_2 = time;
}

//----------------------------------------
// t0 only wait
// input: Timer0_65ms16 (1=65ms)
// output: wiat..
//----------------------------------------
void t0_wait(int16u i)
{
	if(i == 0)
		return;

	TickTimer_65msStart(i);

	while(Timer0_65ms16){}

	//아래 변수에 값이 있으면 65ms 타이머를 멈추지 않는다.(설정모드에서 사용)
	if((Timer0_65ms16_2) 
#if (USE_AVSHIFT_MODE == TRUE)
		|| (Timer0_avShiftMode)
#endif
		)
		return;

	TickTimer_65msStop;
}

BYTE t0_wait_keyin_chk(unsigned short i)
{
	TickTimer_65msStart(i);

	while(Timer0_65ms16)
	{
		if (getkey_simple() == 1)
		{
			Timer0_stop;
			return	TRUE;
		}
	}
	Timer0_stop;
	return FALSE;
}

void carrier_set()
{
	WORD	carrier, i;

#if (USE_SPECIAL_0F4H == TRUE)
	if(ramFormat.strMember.outorder[0] == 0xF4)
		f4_cnt = 0;
#endif

	carrier = ir_stream.Tunion.Tstruct.Carrier;

#if (CRT_OSC == OSC_4MHZ)
	i=carrier/3;
#elif (CRT_OSC == OSC_8MHZ)

	if(carrier > CARRIER_DUTY_OFFSET)
	{
		carrier = carrier * 2;
		i = carrier/2;
	}
	else
	{
#ifdef DUTY_RATIO_1_3
		carrier = carrier * 2;
		i = carrier/3;
#else
		carrier = carrier * 2;
		i = carrier/2;
#endif
	}

#elif (CRT_OSC == OSC_10MHZ)
	carrier=(carrier*5)/2;
	i=carrier/3;
#elif (CRT_OSC == OSC_12MHZ)
	carrier=carrier*3;
	i=carrier/3;
#endif

#if (DUTY_RATIO_1_3 == TRUE) && (DUTY_RATIO_2_3 == TRUE)
	CRTDATA1 = 255 - (carrier-i);
	CRTDATA0 = 255 - i;
#else
	CRTDATA0 = 255 - i;
	CRTDATA1 = 255 - (carrier-i);
#endif

	carrier_time_us =  (125	* carrier)	/	1000; //usec 단위
	//				8MHz = 125ns		  	usec단위로 맞추기 위함

	carrier_time_ns =  ((125 * carrier)	% 1000)/100; //100nsec 단위

	wTimer_Max = (0xFFFF / carrier_time_us) - 2;

	Envelop_time_max = (carrier_time_us*wTimer_Max);
	Envelop_counter_max = 65536 - (Envelop_time_max / 2) - ((carrier_time_ns * (wTimer_Max/10))/2); //Timing 계산으로 인해 발생한 nsec 단위 손실에 대한 보상 값

	if((ramFormat.strMember.outorder[0] == 0xf4) || (ramFormat.strMember.option1.flash && ramFormat.strMember.outorder[0] != 0xf0))
		Before_CRT_State = STOP;
	else
		Before_CRT_State = RUN;
	
	reload_time = TRUE;
	non_special_flash_start = TRUE;
	before_reload_state = FALSE;
	continuous_reload = FALSE;
	
	if(ramFormat.strMember.outorder[0] == 0xf0)
		wTimer = *uptr_Timer;

	CRTCON1 |= (1<<CRTOVEN);  // CRTOVEN: CRT interrupt triggered by overflow enable bit.
	CRT_onoff(CRT_ON);
	Interrupt_Enable;
	Timer2_Start();
}

void EnvelopCarrierOff(void)
{
	RCAP2L = 0xFF;
	RCAP2H = 0xFF;
	CRT_onoff(CRT_OFF);
	P3 &= ~0x01;	
	Envelop_Timer_Stop;
}

//----------------------------------------
// ir out timer stop
//----------------------------------------
void pulse_stop(void)
{
	if(ir_ok)
	{
		if(uptr_Timer == Repeat_Start)	t1_int = 2;

		while(t1_int!=2){}
	}
	Delayby10us(50);
	
	//t2 envelop
	EnvelopCarrierOff();


	ir_ok = FALSE;
	IrKeyBuf=0xff;
	backup_IrKeyBuf=0xff;
	outframeNo=0;
}

#if 1
void CalculateEnvelope(void)
{
	WORD	i;
	WORD	wBitStream = 0;

	if(ramFormat.strMember.outorder[0] == 0xf4)
		return;

	if(ramFormat.strMember.option1.flash == 1)
		return;

	for(i=0; i<BITSTREAMLENGTH; i++)
	{
		//56Khz 이상 ->  (h/l - carrier1) -ea-
		if(ir_stream.Tunion.Tstruct.Carrier <= 75)
		{
			ir_stream.Tunion.Tstruct.pulse.BitStream[i] -= 1;
		}
		else
		{
			wBitStream = (ir_stream.Tunion.Tstruct.pulse.BitStream[i] & 0x8fff);

			// 첫번째 펄스가 low 일 수 있기 때문에 아래 연산 skip
			if(i == 0 && wBitStream == 0x8000) continue;

			if(wBitStream >= 4) // high and low 공통
			{
				//ir_stream.Tunion.Tstruct.pulse.BitStream[i] -= 1;

				if(i%2 && ramFormat.strMember.option1.flash == 0)			// low
				{
					ir_stream.Tunion.Tstruct.pulse.BitStream[i] -= 1;
				}
			}
		}
	}
}
#endif

//----------------------------------------
// ir out
//----------------------------------------
void send_ur(void)
{
	WORD	addAddr;
#if 0
	int16u	xdata Reduce_1stWord;
	int16u	xdata Reduce_2ndWord;
#endif

#if 1
#if (IRZIP_CARR_2BYTES == TRUE)
	if(!ir_zip.Tunion.Tstruct.Carrier)
#else
	if(!ir_zip.Tunion.data1[0])
#endif
		CalculateEnvelope();
#endif
	////////////////////////////////////////////////////////////////
	//1'st
	//start pointer -> uptr_Timer
	uptr_Timer = (WORD*) &ir_stream.Tunion.Tstruct.pulse.BitStream[0];	//bitstream start address load in uptr_timer
	//First_Start=uptr_Timer;

	if((*uptr_Timer & 0x7fff) == 0)
	{
		while((*uptr_Timer & 0x7fff) == 0) 							// data format ?? low|high?????? ??? ? bit?? 0???? ???.
		{
			uptr_Timer += 2;
		}
	}

	//end pointer -> uptr_TimerEnd
	//uptr_TimerEnd=(WORD*) &ir_stream.Tunion.Tstruct.pulse.BitStream[ir_stream.Tunion.Tstruct.bidxEndWord1];
	uptr_TimerEnd = (WORD*) &ir_stream.Tunion.Tstruct.pulse.BitStream[0];
	addAddr       = (WORD) ir_stream.Tunion.Tstruct.bidxEndWord1;
	uptr_TimerEnd += addAddr;
	First_End     = uptr_TimerEnd;

	wTimer = (*uptr_Timer);

	if(uptr_Timer >= &ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH] ||
		First_End >= &ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH])
	{
		ir_ok = FALSE;
		return;
	}

	////////////////////////////////////////////////////////////////
	//2'nd
	//start pointer -> uptr_Timer
	Repeat_Start = (WORD*) &ir_stream.Tunion.Tstruct.pulse.BitStream[0];	//bitstream start address load in uptr_timer
	addAddr      = (WORD) ir_stream.Tunion.Tstruct.bidxRepeatWord;
	Repeat_Start += addAddr;

	if((*Repeat_Start & 0x7fff) == 0)
	{
		while((*Repeat_Start&0x7fff) == 0) 							// data format ?? low|high?????? ??? ? bit?? 0???? ???.
		{
			Repeat_Start +=2;
		}
	}

	//end pointer -> uptr_TimerEnd
	Repeat_End = (WORD*) &ir_stream.Tunion.Tstruct.pulse.BitStream[0];
	addAddr    = (WORD) ir_stream.Tunion.Tstruct.bidxEndWord2;
	Repeat_End += addAddr;

	if(Repeat_Start >= &ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH] ||
		Repeat_End >= &ir_stream.Tunion.Tstruct.pulse.BitStream[BITSTREAMLENGTH])
	{
		ir_ok = FALSE;
		return;
	}

#if 0
//	Reduce_1stWord=(int16u) (4000/ir_stream.Tunion.Tstruct.Carrier);
//	Reduce_2ndWord=(int16u) (4000/ir_stream.Tunion.Tstruct.Carrier);
	Reduce_1stWord=(int16u) (4000/((ir_stream.Tunion.Tstruct.Carrier*5)/8));
	Reduce_2ndWord=(int16u) (4000/((ir_stream.Tunion.Tstruct.Carrier*5)/2));

#if (IRZIP_CARR_2BYTES == TRUE)
	if((!ir_zip.Tunion.Tstruct.Carrier)&&(First_End != Repeat_Start))
#else
	if((!ir_zip.Tunion.data1[0])&&(First_End != Repeat_Start))
#endif
	{
		ir_stream.Tunion.Tstruct.pulse.BitStream[ir_stream.Tunion.Tstruct.bidxEndWord1]-=Reduce_1stWord;
		ir_stream.Tunion.Tstruct.pulse.BitStream[ir_stream.Tunion.Tstruct.bidxEndWord2]-=Reduce_2ndWord;
	}
#endif
	
	carrier_set();
}

//----------------------------------------
// standby ir out
//----------------------------------------
byte Send_UR_Ready(void)
{
	UWORD 	uWord12;							//uWord12 -> 1'st word time
	ULONG 	ulIndex;

	if(curr_state==NORMAL_MODE)
	{
#if (USE_IR_LEARN == TRUE)
		if(curr_device == DEV_TV)
		{
			LearnDataRW(READ, curr_source);		//>>> check to learning code

#if (IRZIP_CARR_2BYTES == TRUE)
			if(ir_zip.Tunion.Tstruct.Carrier)
#else
			if(ir_zip.Tunion.data1[0])
#endif
			{
				//Fill_Learn_RawData();

				MeltBitPattern(&ir_zip);

				if(ir_stream.Tunion.Tstruct.Carrier==400)		//fix to 400 at flash type
				{
					ramFormat.strMember.option1.flash=1;
					//ir_stream.Tunion.Tstruct.Carrier=106;
					ir_stream.Tunion.Tstruct.Carrier=100;		//40KHz
				}
				else
					ramFormat.strMember.option1.flash=0;

				return TRUE;
			}
		}
		else
			IrZipData_Clear();
#endif

#if 0
		pTemp=set_no.set_reg;
		pCodeaddr_1=(int8u*)GetCodeAddress_set(*(pTemp+curr_source));		//pCodeaddr_1=set no?? ????? address in each device       // (idxDevice, code)
#else
		if(curr_device==DEV_STB)
		{
			//setting_no=1;
			setting_no=set_no.set_member.set_no.stb_src;
		}
#if (USE_TEST_MODE == TRUE)
		else if(curr_device==DEV_TESTMODE)
		{
			setting_no=1;
		}
#endif
		else
		{
#if (USE_2BYTE_SETTING_NO == TRUE)
			pTemp = set_no.set_reg;
			//setting_no = *(pTemp+(curr_source*INC_PTR_SIZE));
			//setting_no |= *(pTemp+(curr_source*INC_PTR_SIZE)+1)<<8;
			setting_no = *(pTemp+(curr_source*INC_PTR_SIZE))<<8;
			setting_no |= *(pTemp+(curr_source*INC_PTR_SIZE)+1);
#else
			pTemp = set_no.set_reg;
			setting_no = *(pTemp+(curr_source*INC_PTR_SIZE));
#endif
		}
#endif

#if (USE_BATCH_SS_LG == TRUE)
		if(batch_mode_setting_no)
			setting_no=batch_mode_setting_no;
#endif

#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
		if(curr_device==DEV_STB)
		{
			pCodeaddr_1=(int8u*)GetCodeAddress_set(1);
		}
		else
#endif
			pCodeaddr_1=(int8u*)GetCodeAddress_set(setting_no);
	}//end of normal out mode
	else
	{
#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
		if(curr_device==DEV_STB)
		{
			pCodeaddr_1=(int8u*)GetCodeAddress_set(1);
		}
		else
#endif
			pCodeaddr_1=(int8u*)GetCodeAddress_set(setting_no);
	}//end of setting out mode.

#if (USE_BATCH_SS_LG == TRUE)
	batch_mode_setting_no = 0;
#endif

	if(!LoadSetNumber(pCodeaddr_1)) return FALSE;

	if(!LoadCustDataWithFormat(pCodeaddr_1)) return FALSE;

	ir_stream.Tunion.Tstruct.Carrier=ramCode.bCarrier;

	// 첫번째 출력은 항상 HIGH
	ir_stream.Tunion.Tstruct.pulse.BitStream[0]=0x8000;
	npWaddr_1=(WORD*) &ir_stream.Tunion.Tstruct.pulse.BitStream[0];


	if((ramFormat.strMember.outorder[0]&0xf0)==0xf0)     // special format//tg skip
	{
	  	return (*pfunc_special[ramFormat.strMember.outorder[0]&0x0f])();
	}

	uWord12=ramFormat.strMember.word1;                  // word계산.
	npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);

	if(ramFormat.strMember.option1.non_word)
		uWord12=ramFormat.strMember.word1;  			// an interval of WAIT

	if(*npWaddr_1&0x8000)         // Ending the 2'nd Word
	{
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM])
			npWaddr_1--;
		*npWaddr_1=0;
	}

	*npWaddr_1+=uWord12;
	ir_stream.Tunion.Tstruct.bidxEndWord1=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);   // the wait POINTER of 1'st word(word ????)

	if(ramFormat.strMember.option1.repeat)
	{
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;

		*npWaddr_1=0x8000;                               // repeat start //!!! ???? ??? pulse?? HIGH???? !!!///
		ir_stream.Tunion.Tstruct.bidxRepeatWord=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);  // the wait POINTER of 1'st word

		uWord12=ramFormat.strMember.word2;              // word계산.
		npWaddr_1=FillBitStreamFromOutOrder(8,(WORD*)npWaddr_1,(WORD*)&uWord12);

		if(ramFormat.strMember.option1.non_word)
			uWord12=ramFormat.strMember.word2;  		// an interval of WAIT

		if(*npWaddr_1&0x8000)         // Ending the 2'nd Word
		{
			if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM])
				npWaddr_1--;
			*npWaddr_1=0;
		}
		
		*npWaddr_1+=(uWord12+20);
		ir_stream.Tunion.Tstruct.bidxEndWord2=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);
	}
	else
	{
		ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
		ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;	// the wait POINTER of 1'st word
	}

	if(ramFormat.strMember.option1.single)              // single key존재.
	{
		ulIndex=ramCode.Data;
		ulIndex>>=ramFormat.strMember.datasize.data1;

		ulIndex&=(ULONG)pbMaskIndex[ramFormat.strMember.datasize.index1+ramFormat.strMember.customsize.index2-1];

		if(!ulIndex)                                    // single key
		{
			ir_stream.Tunion.Tstruct.bidxRepeatWord=ir_stream.Tunion.Tstruct.bidxEndWord2;
		}
	}
	return TRUE;
}

//----------------------------------------
// time value get.
//----------------------------------------
WORD* SetBitStreamFromNibble(WORD* npWaddr, BYTE* npbAddr, int16u size, UWORD* npWord12)
{
	WORD	i, j;
	byte	bTemp, bTimePos;

	// Time value 가 nibble 원래 size를 2배로 처리
	size <<= 1;

	for(i=0; i<size; i++)
	{
		j  =  i;
		j >>= 1;

		bTemp = npbAddr[j]; 						//btemp=logic (H001 L002)

		if((i & 0x01) == 0)	bTemp >>= 4;
		else				bTemp &= 0x0f;

		// Time 설정이 x000 일 경우 예외처리
		if((bTemp & _0000_0111B) == 0) break;

		if(bTemp & _0000_1000B)                          // HIGH
		{
			if((*npWaddr & 0x8000) == 0)
			{
				if(++npWaddr >= (WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM])
				{
					npWaddr--;
				}

				*npWaddr = 0x8000;
			}
		}
		else                                    // LOW
		{
			if(*npWaddr & 0x8000)
			{
				if(++npWaddr >= (WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM])
				{
					npWaddr--;
				}

				*npWaddr = 0;
			}
		}

		bTimePos = (bTemp & _0000_0111B) - 1;
		*npWaddr  += ramFormat.strMember.timeval[bTimePos];
		*npWord12 -= (ramFormat.strMember.timeval[bTimePos]-1);
	}

	return npWaddr;
}

//----------------------------------------
// get time value of out order
//----------------------------------------
WORD*   FillBitStreamFromOutOrder(int16u start, WORD* npWaddr, UWORD* npWord12)
{
	int16u	i,j;

	BYTE	bTemp; //->main.h
	ULONG   ulTemp;
	BYTE    repeat[3];

	for(i=start;i<16;i++)                      // output order*2: nibble
	{
		bTemp=ramFormat.strMember.outorder[i/2];
        										//if((i%2)==0)    bTemp>>=4;
		if((i & 0x01)==0)
			bTemp>>=4;
		else
			bTemp&=0x0f;

		switch(bTemp)
		{
			case    0:                          // lead
			npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.lead[0],3,npWord12);
			break;

			case    1:                          // system
			ulTemp=ramCode.Custom;

#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
			if(curr_device == DEV_STB)
			{
				if(curr_state == SET_MODE_DIGIT)
				{
					ulTemp += (setting_no - IR_CUSTOM_NUM_MIN)<<8;
				}
				else if(curr_state == SET_MODE_AUTO)
				{
					ulTemp += (StbNo_AutoSearch - IR_CUSTOM_NUM_MIN)<<8;
				}
				else
				{
					ulTemp += (set_no.set_member.set_no.stb_src - IR_CUSTOM_NUM_MIN)<<8;
				}
			}
#endif

			for(j=0;j<ramFormat.strMember.customsize.custom;j++)
			{
				if(ulTemp&0x01) npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic1[0],3,npWord12);
				else            npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic0[0],3,npWord12);
				ulTemp>>=1;
			}
			break;

			case    2:                          // system bar
			ulTemp=ramCode.Custom;
			for(j=0;j<ramFormat.strMember.customsize.custom;j++)
			{
				if(ulTemp&0x01) npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic0[0],3,npWord12);
				else            npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic1[0],3,npWord12);
				ulTemp>>=1;
			}
			break;

			case    3:                          // data
			ulTemp=ramCode.Data;

			for(j=0;j<ramFormat.strMember.datasize.data1;j++)
			{
				if(ulTemp&0x01) npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic1[0],3,npWord12);
				else            npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic0[0],3,npWord12);
				ulTemp>>=1;
			}
			break;

			case    4:                          // data bar
			ulTemp=ramCode.Data;

			for(j=0;j<ramFormat.strMember.datasize.data1;j++)
			{
				if(ulTemp&0x01) npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic0[0],3,npWord12);
				else            npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic1[0],3,npWord12);
				ulTemp>>=1;
			}
			break;

			case    5:                          // end
			npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.end_pulse,1,npWord12);
			break;

			case    6:                          // *** toggle
			if(gl_Toggle[curr_source]&0x01)//gb_ToggleBit[gu_CurDevice-AUDIO]&0x01)
			{
				npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic1[0],3,npWord12);
			}
			else
			{
				npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic0[0],3,npWord12);
			}
			break;

			case    7:                          // *** toggle2
			if(gl_Toggle[curr_source]&0x02)
			{
				npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic1[0],3,npWord12);
			}
			else
			{
				npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic0[0],3,npWord12);
			}
			break;

			case    8:                          // logic1
			npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic1[0],3,npWord12);
			break;

			case    9:                          // logic0
			npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.logic0[0],3,npWord12);
			break;

			case  0x0a:                         // repeat
			npWaddr=SetBitStreamFromNibble(npWaddr,&ramFormat.strMember.repeat[0],3,npWord12);
			break;

			case  0x0b:                         // spare
			case  0x0c:                         //
			break;

			case  0x0d:                         // BIG TOGGLE
			//  gb_BigToggle=!gb_BigToggle;
			for(j=0;j<3;j++)                //
			{
				repeat[j]=ramFormat.strMember.repeat[j];
				if(gl_Toggle[curr_source]&0x01)//gb_BigToggle==0)
				{
					if(repeat[j]&0x80)  repeat[j]&=0x7f;
					else                repeat[j]|=0x80;

					if(repeat[j]&0x08)  repeat[j]&=0xf7;
					else                repeat[j]|=0x08;
				}
			}
			npWaddr=SetBitStreamFromNibble(npWaddr,&repeat[0],3,npWord12);
			break;

			case  0x0e:                         //
			break;

			case  0x0f:                         // end
			i=8*2;                          // exit;
			break;
		}
	}
	return npWaddr;
}

//----------------------------------------
// code format load
//----------------------------------------
BYTE LoadFormat(BYTE formatno)
{
    WORD    wAddr;
    BYTE    data1;
    BYTE    index;
	BYTE*	pAddr;
#if (USE_DATA_ALIGNMENT == TRUE)
    BYTE    index_even=0;
#endif

#if	(USE_ADDRESS_32BIT == TRUE)
	ULONG*	pX_Format;
#else
	WORD*	pX_Format;
#endif

    int8u	i;

#if	(USE_ADDRESS_32BIT == TRUE)
	pX_Format=(ULONG*)X_FORMAT;
#else
	pX_Format=(WORD*)X_FORMAT;
#endif

	pX_Format=(pX_Format+curr_device);				//each device format pointer
	pAddr=(BYTE code*) *pX_Format;				//


    data1=*pAddr;
    if(!data1)   return  FALSE;				//end pointer

    while(data1!=formatno)
    {
        pAddr=FormatNextAddrCal(pAddr);
        data1=*pAddr;

        if(!data1)   return FALSE;               // not found
    }

    for(i=0;i<38;i++)
    {
        ramFormat.byteArray[i]=*pAddr++;
    }

    index=0;
    index+=ramFormat.strMember.size.time;
    index+=ramFormat.strMember.size.outp;
    index+=ramFormat.strMember.option2.repeat;
    index+=ramFormat.strMember.option2.lead;
    index+=ramFormat.strMember.option2.logic0;
    index+=ramFormat.strMember.option2.logic1;
    index+=10;                                  // 11-1=index

#if (USE_DATA_ALIGNMENT == TRUE)
	if(!(index & 0x01))
	{
		index_even=1;
		index+=1;
	}
#endif

                                                
#if (USE_LITTLE_ENDIAN == FALSE)
    wAddr=(UWORD)ramFormat.byteArray[index];									// WORD2
    ramFormat.strMember.word2=(UWORD)(ramFormat.byteArray[--index]<<8)+wAddr;
	
    wAddr=(UWORD)ramFormat.byteArray[--index];
    ramFormat.strMember.word1=(UWORD)(ramFormat.byteArray[--index]<<8)+wAddr;	
#else
    wAddr=(UWORD)(ramFormat.byteArray[index]<<8);
    ramFormat.strMember.word2=(UWORD)(ramFormat.byteArray[--index])+wAddr;

    wAddr=(UWORD)(ramFormat.byteArray[--index]<<8);
    ramFormat.strMember.word1=(UWORD)(ramFormat.byteArray[--index])+wAddr;
#endif

#if (USE_DATA_ALIGNMENT == TRUE)
	if(index_even)
		index-=1;
#endif
                                                // TIME ARRAY
    data1=ramFormat.strMember.size.time;
    for(i=0;i<data1;i++)
    {
        ramFormat.strMember.timeval[(data1-1)-i]=ramFormat.byteArray[--index];
    }
    for(i=data1;i<7;i++)   ramFormat.strMember.timeval[i]=0;

                                                // END PULSE
    ramFormat.strMember.end_pulse=ramFormat.byteArray[--index];

                                                // REPEAT
    data1=ramFormat.strMember.option2.repeat;
    for(i=0;i<data1;i++)
    {
        ramFormat.strMember.repeat[(data1-1)-i]=ramFormat.byteArray[--index];
    }
    for(i=data1;i<3;i++)   ramFormat.strMember.repeat[i]=0;

                                                // LEAD
    data1=ramFormat.strMember.option2.lead;
    for(i=0;i<data1;i++)
    {
        ramFormat.strMember.lead[(data1-1)-i]=ramFormat.byteArray[--index];
    }
    for(i=data1;i<3;i++)   ramFormat.strMember.lead[i]=0;

                                                // LOGIC0
    data1=ramFormat.strMember.option2.logic0;
    for(i=0;i<data1;i++)
    {
        ramFormat.strMember.logic0[(data1-1)-i]=ramFormat.byteArray[--index];
    }
    for(i=data1;i<3;i++)   ramFormat.strMember.logic0[i]=0;

                                                // LOGIC1
    data1=ramFormat.strMember.option2.logic1;
    for(i=0;i<data1;i++)
    {
        ramFormat.strMember.logic1[(data1-1)-i]=ramFormat.byteArray[--index];
    }
    for(i=data1;i<3;i++)   ramFormat.strMember.logic1[i]=0;

                                                // OUTPUT ORDER
    data1=ramFormat.strMember.size.outp;
    for(i=0;i<data1;i++)
    {
        ramFormat.strMember.outorder[(data1-1)-i]=ramFormat.byteArray[--index];
    }
    for(i=data1;i<8;i++)   ramFormat.strMember.outorder[i]=0xFF;

	return TRUE;
}

//---------------------------------------------------------------
//    FormatRead() subroutines
//-----------------------------------------------------------------
BYTE  *FormatNextAddrCal(BYTE code* pAddr)
{
	byte	sum, buf;

	sum = 0;

	++pAddr;                                    // format no + 1 => size
	buf = *pAddr;                               // buf=size
	sum = (buf & 0x0f);                         // 최대 TIME BYTE수 - 실제TIME SIZE
	sum += (buf >> 4);                          // 최대 출력순서BYTE수 - 실제 출력순서SIZE

	pAddr += 2;                                   // option 2(logic1,logic0,lead,repeat size)

	buf = *pAddr;
	sum += (buf & 0x03);
	sum += (buf>>2 & 0x03);
	sum += (buf>>4 & 0x03);
	sum += (buf>>6);

	pAddr += (sum+8);                             // +8(앞에서 +3) : the number of an indispensable member

#if (USE_DATA_ALIGNMENT == TRUE)
	if(!(sum & 0x01))
		pAddr+=1;
#endif

	return pAddr;

}

BOOL LoadCustDataWithFormat(BYTE  * pCodeaddr)
{
	int16u	i;
	ULONG	ulIndex;
	BYTE	k;
	BYTE* 	cpPowerToggle;
	BYTE	idx_key_local;
	BYTE	idxKey;
	ULONG*	pulPtr;

#if	(USE_ADDRESS_32BIT == TRUE)
	ULONG	uOffset;
#else
	WORD	uOffset;
#endif

	idxKey=get_map_pointer();

	if(idxKey==MAP_NG) return FALSE;                 //idxKey-> map value in each device

	//pMultiCust=(BYTE *)0;
	pTemp=0;

	pCodeaddr+=INC_PTR_SIZE;                                // next

	pCodeLast=pCodeaddr+(*pCodeaddr);
	pCodeaddr++;                                // format no

	if(!LoadFormat(*pCodeaddr)) return FALSE;

	pCodeaddr++;                                // carrier
	ramCode.bCarrier=*pCodeaddr;

	pCodeaddr++;                                // custom area
	
	if(!ramFormat.strMember.option1.multi_cust)
	{		
#if (USE_ADDRESS_32BIT == FALSE)
		pulPtr=(ULONG *)pCodeaddr;
		ramCode.Custom=*pulPtr;		
#else
		//4-byte 주소 체계로 인하여, 만약 address 가 4-byte 단위로 떨어지지 않으면 연산 에러가 발생하여 위 코드는 주석 처리하고
		//대신에 1-byte 단위로 처리함(James.KIM)
		ramCode.Custom =  (pCodeaddr[3] << 24);
		ramCode.Custom |= (pCodeaddr[2] << 16);
		ramCode.Custom |= (pCodeaddr[1] << 8);
		ramCode.Custom |=  pCodeaddr[0];
#endif

#if (USE_LITTLE_ENDIAN == FALSE)
		Convert_cd(&(ramCode.Custom));
#endif
		pCodeaddr+=(ramFormat.strMember.customsize.custom+7)/8;
	}
	else                                        // multi custom
	{
#if	(USE_ADDRESS_32BIT == TRUE)
#if 0
		pWordptr=(ULONG *)pCodeaddr;
		uOffset=(ULONG)*pWordptr;
		if(uOffset>(ULONG)0x1ffff)
			return FALSE;
#else
		//4-byte 주소 체계로 인하여, 만약 address 가 4-byte 단위로 떨어지지 않으면 연산 에러가 발생하여 위 코드는 주석 처리하고
		//대신에 1-byte 단위로 처리함(James.KIM)
		uOffset =  (pCodeaddr[3] << 24);
		uOffset |= (pCodeaddr[2] << 16);
		uOffset |= (pCodeaddr[1] << 8);
		uOffset |=  pCodeaddr[0];
#endif
#else
		pWordptr = (WORD *)pCodeaddr;
		uOffset = (UWORD)*pWordptr;		
#endif

		pTemp = (int8u code*)uOffset;

#if	(USE_ADDRESS_32BIT == TRUE)
		pCodeaddr+=4;     // address
#else
		pCodeaddr+=2;     // address
#endif
	}

	i=ramFormat.strMember.datasize.data1+ramFormat.strMember.datasize.index1+ramFormat.strMember.customsize.index2;
	i=(i+7)/8;

	if(power_toggle&0x01)
	{
		cpPowerToggle=(BYTE*)X_TOGGKEY;
#if 0
		cpPowerToggle+=curr_device;
#endif

		//if(*cpPowerToggle==key_buf[0])
		if((*cpPowerToggle==IrKeyBuf) 
			|| (IrKeyBuf==KEY_ALLPOWER)
			)
		{
			k=i;

			cpPowerToggle=(BYTE*)X_TOGGPOS;
#if 1
			cpPowerToggle+=curr_device;
#endif
			idx_key_local=(*cpPowerToggle);
			k*=idx_key_local;

			if((pCodeaddr+k)>=pCodeLast)	
				i*=idxKey;	//ng
			else							
			{
				i*=idx_key_local;
#if 1			//power_toggle 에 dummy data 일 경우 원래 position 의 data read
				pCodeaddr+=i;
#if (USE_ADDRESS_32BIT == FALSE)
				pulPtr=(ULONG *)pCodeaddr;
				ramCode.Data=*pulPtr;
#else
				//4-byte 주소 체계로 인하여, 만약 address 가 4-byte 단위로 떨어지지 않으면 연산 에러가 발생하여 위 코드는 주석 처리하고
				//대신에 1-byte 단위로 처리함(James.KIM)
				ramCode.Data =  (pCodeaddr[3] << 24);
				ramCode.Data |= (pCodeaddr[2] << 16);
				ramCode.Data |= (pCodeaddr[1] << 8);
				ramCode.Data |=  pCodeaddr[0];
#endif
				pCodeaddr-=i;
#if (USE_LITTLE_ENDIAN == FALSE)
				Convert_cd(&(ramCode.Data));
#endif
			//----------------------------------------------// >>> data dummy compare insert
				k=(BYTE)(ramCode.Data&0x000000ff);

				cpX_keymap2=(BYTE*)X_DATADUMMY;
				cpX_keymap2=(cpX_keymap2+curr_device);				//each device data dummy pointer

				if(k==*cpX_keymap2)
				{
					i*=idxKey;
				}
#endif
			}
		}
		else
			i*=idxKey;
	}
	else
		i*=idxKey;


	pCodeaddr+=i;
	if(pCodeaddr>=pCodeLast)    return FALSE;

#if (USE_ADDRESS_32BIT == FALSE)
	pulPtr=(ULONG *)pCodeaddr;
	ramCode.Data=*pulPtr;
#else
	//4-byte 주소 체계로 인하여, 만약 address 가 4-byte 단위로 떨어지지 않으면 연산 에러가 발생하여 위 코드는 주석 처리하고
	//대신에 1-byte 단위로 처리함(James.KIM)
	ramCode.Data =  (pCodeaddr[3] << 24);
	ramCode.Data |= (pCodeaddr[2] << 16);
	ramCode.Data |= (pCodeaddr[1] << 8);
	ramCode.Data |=  pCodeaddr[0];
#endif

//----------------------------------------------//
// digital tv bs rotate data 07.07.17
//	if(curr_source==DEV_BS) BS_Key_Check();
//----------------------------------------------//
#if (USE_LITTLE_ENDIAN == FALSE)
	Convert_cd(&(ramCode.Data));
#endif
//----------------------------------------------// >>> data dummy compare insert
	k=(BYTE)(ramCode.Data&0x000000ff);

	cpX_keymap2=(BYTE code*)X_DATADUMMY;
	cpX_keymap2=(cpX_keymap2+curr_device);				//each device data dummy pointer

	if(k==*cpX_keymap2) return FALSE;							// <<<
//----------------------------------------------//

	if(pTemp)                              				// multi custom
	{
		ulIndex=ramCode.Data;
		ulIndex>>=ramFormat.strMember.datasize.data1;
		ulIndex&=(ULONG)pbMaskIndex[ramFormat.strMember.datasize.index1+ramFormat.strMember.customsize.index2-1];

		ulIndex*=(ramFormat.strMember.customsize.custom+7)/8;
		pTemp+=ulIndex;
		
#if (USE_ADDRESS_32BIT == FALSE)
		pulPtr=(ULONG *)pTemp;
		ramCode.Custom=*pulPtr;
#else
		//4-byte 주소 체계로 인하여, 만약 address 가 4-byte 단위로 떨어지지 않으면 연산 에러가 발생하여 위 코드는 주석 처리하고
		//대신에 1-byte 단위로 처리함(James.KIM)
		ramCode.Custom =  (pTemp[3] << 24);
		ramCode.Custom |= (pTemp[2] << 16);
		ramCode.Custom |= (pTemp[1] << 8);
		ramCode.Custom |=  pTemp[0];
#endif
		
#if (USE_LITTLE_ENDIAN == FALSE)
		Convert_cd(&(ramCode.Custom));
#endif
	}
    return TRUE;
}


void Convert_cd(ULONG* a)
{
	ULONG t = 0;

	t |= (*a >> 24);
	t |= (*a >> 8) & 0x0000ff00;
	t |= (*a << 8) & 0x00ff0000;
	t |= (*a << 24);

	*a = t;
}

//----------------------------------------------
// setNo address find in setting modeC
//----------------------------------------------
BYTE const*	GetCodeAddress_set(set_number_t bSetNo)
{
	set_number_t	bDataInDB;
	byte*	pCode_fixaddr;

	pCode_fixaddr = (byte code*) X_DEVICE[curr_device];
	bDataInDB = LoadSetNumber(pCode_fixaddr);

	if(!bDataInDB) return  FALSE;

	while(bDataInDB != bSetNo)
	{
		pCode_fixaddr+=INC_PTR_SIZE;                            // next
		bDataInDB     = *pCode_fixaddr;									// next address jump
		pCode_fixaddr += bDataInDB;

		bDataInDB = LoadSetNumber(pCode_fixaddr);

		if(!bDataInDB)
		{
			return pCode_fixaddr;               					// not found
		}
	}

	return pCode_fixaddr;
}

//-----------------------------------------
// 3-digit setting
//-----------------------------------------
void Setting_Mode(void)
{
	BYTE	i;
	BYTE*	p;

	//SetMode_StartTimeOut(SETUP_TIMEOUT_S);

	direction_inAuto=0xff;

	switch(setting_source)
	{
#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
		case	SRC_STB:
			setting_no=set_no.set_member.set_no.stb_src;
			setting_device=set_no.set_member.set_dev.stb_src;
			StbNo_AutoSearch = setting_no;
			break;
#endif
		case	SRC_TV:
			setting_no=set_no.set_member.set_no.tv_src;
			setting_device=set_no.set_member.set_dev.tv_src;
			break;
	}

	refDev_inAuto=setting_device;
	refNo_inAuto=setting_no;

	reg_key_max=0;
	p=(BYTE*)reg_key_buf;

	for(i=0; i<4; i++) *(p+i)=0;
}

//----------------------------------------
// Code_Read_Out
//----------------------------------------
void Code_Read_Out(void)
{
	BYTE	cnt;
	set_number_t set_num;

	reg_key_buf[0]=0;
	reg_key_buf[1]=0;
	reg_key_buf[2]=0;
	reg_key_buf[3]=0;

	set_num = set_no.set_member.set_no.tv_src;

#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
	if(setting_source == SRC_STB)
	{
		set_num = set_no.set_member.set_no.stb_src;
	}
#endif

	reg_key_buf[0] = (set_num/100);
	reg_key_buf[1] = ((set_num/10)%10);
	reg_key_buf[2] = (set_num%10);

#if (USE_2BYTE_SETTING_NO == FALSE)
	if(set_no.set_member.set_dev.tv_src == DEV_TV1)
		reg_key_buf[0] += 3;
#endif

	device_led_off(CODEREADOUT_LED);
	t0_wait(7);

	cnt=reg_key_buf[0];
	if(!cnt)
		device_Led_blink(CODEREADOUT_LED, 10, 5, 5);
	else
		device_Led_blink(CODEREADOUT_LED, cnt, 5, 5);
	device_led_off(CODEREADOUT_LED);
	t0_wait(23);

	cnt=reg_key_buf[1];
	if(!cnt)
		device_Led_blink(CODEREADOUT_LED, 10, 5, 5);
	else
		device_Led_blink(CODEREADOUT_LED, cnt, 5, 5);
	device_led_off(CODEREADOUT_LED);
	t0_wait(23);

	cnt=reg_key_buf[2];
	if(!cnt)
		device_Led_blink(CODEREADOUT_LED, 10, 5, 5);
	else
		device_Led_blink(CODEREADOUT_LED, cnt, 5, 5);
	device_led_off(CODEREADOUT_LED);
}

//----------------------------------------
// 3digit -> 1byte hex change
//----------------------------------------
BYTE Get1CodeFrom3Buf(BYTE* p)
{
	set_number_t 	i;

#if (USE_2BYTE_SETTING_NO == TRUE)
	i=( (*p *100) + (p[1]*10) + (p[2]) );
#else
	WORD	k;
	k=( (*p *100) + (p[1]*10) + (p[2]) );

	if(k>256)
		return FALSE;

	i=(BYTE)k;
#endif

#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
	if((setting_device == DEV_STB)&&(i >= IR_CUSTOM_NUM_MIN)&&(i <= IR_CUSTOM_NUM_MAX))
	{
		setting_no=i;																//현재 입력된 3-digit load to setting_no.

		refNo_inAuto=i;
		refDev_inAuto=setting_device;

		StbNo_AutoSearch = setting_no;

		return TRUE;
	}
#endif

	p=(int8u*)GetCodeAddress_set(i);     //pCodeaddr=set no가 위치한 address in each device       // (idxDevice, code)

	if(!LoadSetNumber(p))
		return FALSE;                       			//db 0 -> end point

	setting_no=i;																//현재 입력된 3-digit load to setting_no.

	refNo_inAuto=i;
	refDev_inAuto=setting_device;

	return TRUE;
}

//-------------------------------------------
// auto 1 & 2 mode에서 next setNo search (u/r only)
//-------------------------------------------
void next_searchNo_find_inAutoSearch(void)
{
	cpX_keymap2+=INC_PTR_SIZE;                           		// next
	setting_no=*cpX_keymap2;                    // next address jump
	chksum=setting_no;
	cpX_keymap2+=setting_no;

	setting_no=LoadSetNumber(cpX_keymap2);

	if(!setting_no)															//meet db 0 (last)
	{
		cpX_keymap2=(byte code*) X_DEVICE[curr_device];	//1'st address.
		setting_no=LoadSetNumber(cpX_keymap2);
	}
}

void prev_searchNo_find_inAutoSearch(void)
{
	set_number_t 	CodeNo=setting_no;
	set_number_t	data1;
	int8u	temp;

	cpX_keymap2=(byte code*) X_DEVICE[curr_device];	//1'st address.

	if(LoadSetNumber(cpX_keymap2) == setting_no)
	{
		cpX_keymap2=(byte code*) X_DEVICE[curr_device];	//1'st address.
		while(TRUE)		//while(data1!=i)
		{
			cpX_keymap2+=INC_PTR_SIZE;                          // next
			data1=*cpX_keymap2;											// next address jump
			temp=data1;
			cpX_keymap2+=data1;

			data1=LoadSetNumber(cpX_keymap2);

			if(!data1)															//can't found.
				break;
		}
#if (USE_2BYTE_SETTING_NO == TRUE)
		//setting_no=*(cpX_keymap2-temp-2);					//last setNo in each devcie.
		//setting_no|=*(cpX_keymap2-temp-1)<<8;					//last setNo in each devcie.
		setting_no=*(cpX_keymap2-temp-2)<<8;					//last setNo in each devcie.
		setting_no|=*(cpX_keymap2-temp-1);					//last setNo in each devcie.
#else
		setting_no=*(cpX_keymap2-temp-1);					//last setNo in each devcie.
#endif
		return;
	}//end of that find last setNo.
	do
	{
		next_searchNo_find_inAutoSearch();
	}while(CodeNo != setting_no);

#if (USE_2BYTE_SETTING_NO == TRUE)
	//setting_no=*(cpX_keymap2-chksum-2);					//last setNo in each devcie.
	//setting_no|=*(cpX_keymap2-chksum-1)<<8;					//last setNo in each devcie.
	setting_no=*(cpX_keymap2-chksum-2)<<8;					//last setNo in each devcie.
	setting_no|=*(cpX_keymap2-chksum-1);					//last setNo in each devcie.
#else
	setting_no=*(cpX_keymap2-chksum-1);
#endif
}

void device_led_off(BYTE i)
{
	//all_led_off;	
	//led_force = 0xff;

	if(led_force!=0xff)
		i=led_force;

	switch(i)
	{
		case	LED_1:
			led1_off;
			break;

		case	LED_2:
			led2_off;
			break;

		case	LED_3:
			led3_off;
			break;

		default:
			all_led_off;
			break;
	}
	
}

void device_led_on(BYTE i)
{
	//all_led_off;

	if(led_force!=0xff)
		i=led_force;

	switch(i)
	{
		case	LED_1:
			led1_on;
			break;

		case	LED_2:
			led2_on;
			break;

		case	LED_3:
			led3_on;
			break;

		default:
			all_led_off;
			break;
	}
}

//----------------------------------------
// setting device led blink.
//----------------------------------------
void device_Led_blink(int8u type, int8u repeat, int16u on_ms, int16u off_ms)
{
	BYTE	k;

	for(k=repeat; k>0 ; k--)
	{
		device_led_off(type);
		t0_wait(off_ms);
		device_led_on(type);
		t0_wait(on_ms);
	}
}

#if (USE_LOW_BATT_LED == TRUE)
BOOL device_Led_blink_key_chk(int8u type, int8u repeat, int16u on_ms, int16u off_ms)
{
	BYTE	k;

	for(k=repeat; k>0 ; k--)
	{
		device_led_off(type);
		if(t0_wait_keyin_chk(off_ms))
			return TRUE;
		device_led_on(type);
		if(t0_wait_keyin_chk(on_ms))
			return TRUE;
	}
	return FALSE;
} 
#endif

void device_Led_InSetUp(int8u result, int8u type)
{
	switch(result)
	{
		case	SET_ENTRY:
			device_Led_blink(type, 1, 0, 8); 
			break;
		case	SET_OK:
			device_Led_blink(type, 2, 4, 4);
			break;
		case	SET_NG:
			device_Led_blink(type, 1, 0, 16);
			break;
		case	SET_LEARN_ENTRY:
			device_Led_blink(type, 1, 0, 4);
			break;
		case	SET_LEARN_OK:
			device_Led_blink(type, 2, 4, 4);
			break;
		case	SET_LEARN_NG:
			device_Led_blink(type, 1, 0, 31);
			break;
	}
}

#if (USE_BACKLIGHT == TRUE)
//backlighting -> pwm.
void BackLight_Dim_On(void)
{

}

void BackLight_Dim_Off(void)
{
}
#endif


void Check_Key_InSetmode(int8u* key1, int8u* key2)
{
	twokey_result=0xff;
	//backup_key=0xff;

	switch(*key1)
	{
		case KEY_OK:
			switch(*key2)
			{
				case	KEY_TVPOWER:
					twokey_result = COMBO_KEYS_UR_SETUP;
					break;
#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
				case	KEY_STBPOWER:
					twokey_result = COMBO_KEYS_STB_SETUP;
					break;
#endif
			}
			break;
		case KEY_RED:
			switch(*key2)
			{
				case	KEY_STBPOWER:
					twokey_result = COMBO_KEYS_FACTORY_DEFAULT;
					break;
			}
			break;
	}

	if(twokey_result!=0xff)
	{
		backup_key = *key2;
		return;
	}

	switch(*key2)
	{
		case KEY_OK:
			switch(*key1)
			{
				case	KEY_TVPOWER:
					twokey_result = COMBO_KEYS_UR_SETUP;
					break;
#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
				case	KEY_STBPOWER:
					twokey_result = COMBO_KEYS_STB_SETUP;
					break;
#endif
			}
			break;
		case KEY_RED:
			switch(*key1)
			{
				case	KEY_STBPOWER:
					twokey_result = COMBO_KEYS_FACTORY_DEFAULT;
					break;
			}
			break;
	}

	if(twokey_result!=0xff)
	{
		backup_key = *key1;
		return;
	}
}

//-------------------------------------------
// direction	1:up
//						0:down	search.
//-------------------------------------------
void auto_search_1(int8u direction)
{
	direction_G=direction;

	reg_key_max=0;
	reg_key_buf[0]=0;
	reg_key_buf[1]=0;
	reg_key_buf[2]=0;
	reg_key_buf[3]=0;

	Timer0_65ms16_2_Update(SETUP_TIMEOUT_S);

	if(setting_device==0xff)
	{
		device_Led_InSetUp(SET_NG, SETTING_MODE_LED);
		return;
	}

	curr_device=setting_device;

	//1'st search -> 설정 진입 후 처음으로 search가 진행되는 경우.//search to current db no
	if(!setting_no)
	{
		cpX_keymap2=(byte code*) X_DEVICE[curr_device];				//1'st address.
		setting_no=LoadSetNumber(cpX_keymap2);
		if(direction_G)																						//up search (next)
			prev_searchNo_find_inAutoSearch();
		direction_inAuto=0xff;
		refDev_inAuto=setting_device;
		refNo_inAuto=setting_no;
		cpX_keymap2=(int8u*)GetCodeAddress_set(refNo_inAuto);		//current set no pointer
	}
	else
	{
#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
		if(curr_device==DEV_STB)
		{
			cpX_keymap2=(int8u*)GetCodeAddress_set(1);
		}
		else
#endif
		cpX_keymap2=(int8u*)GetCodeAddress_set(setting_no);			//설정중인 set no pointer
	}
	backup_settingNo=setting_no;
	prev_direction=direction_inAuto;

	//설정진입 후 3-digit입력이나 search가 이루어진 후 진행되는 경우.
	if(direction_G)													//up search (next)
	{
#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
		if(curr_device==DEV_STB)
		{
			if(StbNo_AutoSearch < IR_CUSTOM_NUM_MAX)
				StbNo_AutoSearch++;
			else
				StbNo_AutoSearch = IR_CUSTOM_NUM_MIN;
			setting_no = StbNo_AutoSearch;
		}
		else
#endif
			next_searchNo_find_inAutoSearch();
		direction_inAuto=1;
	}
	else															//down search (prev)
	{
#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
		if(curr_device==DEV_STB)
		{
			if(StbNo_AutoSearch > IR_CUSTOM_NUM_MIN)
				StbNo_AutoSearch--;
			else
				StbNo_AutoSearch = IR_CUSTOM_NUM_MAX;
			setting_no = StbNo_AutoSearch;
		}
		else
#endif
			prev_searchNo_find_inAutoSearch();
		direction_inAuto=0;
	}
	backup_key=0xff;

	switch(curr_device)
	{
		case	DEV_TV:
		case	DEV_TV1:
			//key_buf[0]=KEY_TVPOWER;
			IrKeyBuf=KEY_TVPOWER;
			power_toggle++;
			break;
#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
		case	DEV_STB:
			//key_buf[0]=KEY_STBPOWER;
			IrKeyBuf=KEY_STBPOWER;
			break;
#endif
		default:
			break;
	}

	key_cnt=1;

	if((Panasonic_TV_Check(curr_device, setting_no))&&(IrKeyBuf==KEY_TVPOWER))
		irout_n(5);
	else
	{
#if (USE_BATCH_SS_LG == TRUE)
		if(!batch_SS_LG_Common(curr_device, setting_no, IrKeyBuf))
#endif
			irout_n(3);
	}

	backup_key=0xff;
	setting_ok=TRUE;

	if(prev_direction!=0xff)
	{
		if(direction_inAuto != prev_direction)		//different search direction between previous search direction
		{
			refNo_inAuto=backup_settingNo;			//현재 출력한 3-digit이전의 설정번호 ref로 저장.
			refDev_inAuto=setting_device;
		}
	}

#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
	if(curr_device==DEV_STB)
	{
		device_Led_blink(SETTING_MODE_LED, 1, 0, 5);
	}
	else
#endif
	{
		if( (refNo_inAuto==setting_no) && (refDev_inAuto==setting_device) )
			device_Led_blink(SETTING_MODE_LED, 10, 2, 2);
		else
			device_Led_blink(SETTING_MODE_LED, 1, 0, 5);
	}

	//t0_wait_keyoff(1);					//300ms중에 key in이 된 경우 바로 return.

//	device_led_on(curr_source);
//	wait_getkey_simple();
}



#endif	//__COMMON_C__