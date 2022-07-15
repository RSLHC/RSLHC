
#ifndef __HAL_C__
#define __HAL_C__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include 	<INTRINS.H>
#include 	"SC51F33XX.h"
#include 	"main.h"
#include	"common.h"
#include	"flash.h"
#include	"hal.h"
#include	"function_key.h"
#include 	"sysclk.h"
#include 	"crt.h"

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/
void SystemInit(void)
{
	/* Test control Register*/
	xTESTCON = 0x00;

	/* System configuration Register, Initial value : 81H */
  	xSYSCFG = 0           
          | (1<<RSTCFG)  /* 0: P10 set as GPIO, 1: P10 set as external reset(default) */
          | (1<<RCLCFG) /* 0: RCL keep on, 1: The RCL is on in working mode, and off in STOP/IDLE mode (default). */ 
		  | (1<<TREN);


	/* System configuration2 Register, Initial value : 01H */
	xSYSCFG2  = 0
            | (1<<DREN); /* 0: LPVR is off, 1: LPVR is on(default) */

	/* STOP mode configuration register */
  	xSTOPCFG |= (1<<PWRCFG); 
            /*	0: in stop mode, MVR is on, and LVR and LVD are controledl by LVREN and LVDEN.
            	1: in stop mode, LVR and LVD are off. If DREN=1, then MVR and BGR are off, 
                core is supplied by LPVR. Awaking from STOP mode will enable BGR and MVR, 
				and the power-supply will switch to MVR. 
            	If the DREN=0, then MVR and BGR is on, detail please refer to Tab 12-1. */

	crt_init_IR();
	clk_sel(CLK_SELRCH); // Use Internal Oscilator (16Mhz) // Sys Clock RCL : 20KHz / RCH : 16MHz
	clk_div(DEVIDED_FREQ_1); // 16MHz / 1 = 16MHz

	SetDeltaV(); //LVD 설정


#if (USE_WATCH_DOG_TIMER == TRUE)
	WDTCON = 0x07; 	// Bit7~Bit3 : WDTRL[4:0]: 5-bit initial value for down counter with auto-load, the value is 1FH by default.
					// Bit1~Bit0 : WDTPR[1:0]: WDT clock prescale selection bit.
					//			   00: RCL/16.
					//			   01: RCL/32.
					//			   10: RCL/64.
					//			   11: RCL/128 (default).
					//			   RCL(=20KHz) / 128(WDTPR[1:0]=11) => 44ms(Overflow time step)

	SYSCFG 	|= (1<<WDTCFG); //WDT is on in STOP mode.
	SYSCFG  &= ~(1<<RCLCFG); // RCL is on.

#else
	//ClrWDT();
	//WDTCLR0=0x53;	
	//WDTCLR1=0xAC;
#endif
}

void Keyscan_Interrupt_On(void)
{
/********************************************************************/
/* Key Matrix input Interrupt Init 	                                */
/* P1.0 ~ P1.7 (8)                                                  */
/********************************************************************/
	// xP1KBICON : KBI enable register
	// 0 : Disable KBI
	// 1 : Enable KBI
	xP1KBICON |= KEY_INPUT_PINS_P1; /* Input port Keyscan function used */

	// xP1KBIWKP : interrupt polarity selection bit.
	// 0: KBI interrupt is triggered by the falling edge, and KBI wake-up is low level active
	// 1: KBI interrupt is triggered by the rising edge, and KBI wake-up is high level active.
	xP1KBIWKP &= ~(KEY_INPUT_PINS_P1);

/********************************************************************/
/* Key Matrix output Interrupt Init                                 */
/* P2.0 P2.1 P2.2 P2.5 P2.6 P0.6 P0.7 (7)                           */
/********************************************************************/
	// xP0KBICON, xP2KBICON  : KBI enable register
	// 0 : Disable KBI
	// 1 : Enable KBI
	xP2KBICON &= ~(KEY_OUTPUT_PINS_P2); /* Output port Keyscan function unused */
	xP0KBICON &= ~(KEY_OUTPUT_PINS_P0);

	// xP0KBIWKP, xP2KBIWKP : interrupt polarity selection bit.
	// 0: KBI interrupt is triggered by the falling edge, and KBI wake-up is low level active
	// 1: KBI interrupt is triggered by the rising edge, and KBI wake-up is high level active.
	xP2KBIWKP |= KEY_OUTPUT_PINS_P2;
	xP0KBIWKP |= KEY_OUTPUT_PINS_P0;
}

void Keyscan_Interrupt_Off(void)
{
/********************************************************************/
/* Key Matrix input Interrupt Init 	                                */
/* P1.0 ~ P1.7 (8)                                                  */
/********************************************************************/
	// xP1KBICON : KBI enable register
	// 0 : Disable KBI
	// 1 : Enable KBI
	xP1KBICON &= ~(KEY_INPUT_PINS_P1); /* Input port Keyscan function unused */

	// xP1KBIWKP : interrupt polarity selection bit.
	// 0: KBI interrupt is triggered by the falling edge, and KBI wake-up is low level active
	// 1: KBI interrupt is triggered by the rising edge, and KBI wake-up is high level active.
	xP1KBIWKP &= ~(KEY_INPUT_PINS_P1);

/********************************************************************/
/* Key Matrix output Interrupt Init                                 */
/* P2.0 P2.1 P2.2 P2.5 P2.6 P0.6 P0.7 (7)                           */
/********************************************************************/
	// xP0KBICON, xP2KBICON  : KBI enable register
	// 0 : Disable KBI
	// 1 : Enable KBI
	xP2KBICON &= ~(KEY_OUTPUT_PINS_P2); /* Output port Keyscan function unused */
	xP0KBICON &= ~(KEY_OUTPUT_PINS_P0);

	// xP0KBIWKP, xP2KBIWKP : interrupt polarity selection bit.
	// 0: KBI interrupt is triggered by the falling edge, and KBI wake-up is low level active
	// 1: KBI interrupt is triggered by the rising edge, and KBI wake-up is high level active.
	xP2KBIWKP &= ~(KEY_OUTPUT_PINS_P2);
	xP0KBIWKP &= ~(KEY_OUTPUT_PINS_P0);
}


/**
  * @brief  crt initialize and set for IR test
  * @retval None
  */
void crt_init_IR(void)
{
	CRT_onoff(CRT_ON);	// CRT ON/OFF
	CRT_modesel(CRT_MODULATE);	// Modulation, Demodulation 선택
	CRT_insel(CRT_INP30);	// CRT out port 설정 (P3.0)
    CRT_clkdiv(1);  // 16MHz / n+1 = 16MHz / 1+1 = 8MHz
	CRT_REM2P30();
	CRT_remops(CRT_REMOPS0);
}

// Watchdog Register Clear
// 한 번 CLear 하면 이 후에도 계속 자동으로 Counter 값이 셋팅된다.
// 한 번 Clear 하면 Watchdog을 멈출 수 없다.
void ClrWDT()
{
	WDTCLR0=0x00;	 
	WDTCLR1=0x00;	 
	WDTCLR0=0x53;	
	WDTCLR1=0xAC;
}

/**
  * @brief  according to VDD voltage setting CP0VRCON 
  * @note   Update Register: CP0VRCON,SPMC0,VMC0
  */
void SetDeltaV(void)
{
//    int8u i,m;
    //LVRCON |= 0x80;  /*turn on reference voltage,it han been opened before call this function in fact*/
	LVDCON 	= 0 		/*open low tension detection 2.2V*/
			| (1<<LVDEN)  // LVD is on
			| (0<<LVLS3)  // LVLS[3:0] : LVD detecting voltage selection bit
			| (1<<LVLS2)  // VLVD = 1.8V + LVLS[3:0]
			| (0<<LVLS1)  // LVLS3(0), LVLS2(1), LVLS1(0), LVLS0(0) = 4
			| (0<<LVLS0); // VLVD = 1.8V + 0.4V = 2.2V 


	LVRCON = _1000_0000B; // LVR is on, LVR reset voltage is 1.6V

    t0_wait(1);         /*stabilize dozens of us*/ 

	// CP0VRCON : ACMP internal reference voltage control register
	//CP0VRCON = 0xf1; /*>=2.9v*/
	CP0VRCON = 0x89; /*>=3.3v*/

}


void ExtInt_init(void)
{
	EA = 0;
	IE = 0x00;		/* Disable Interrupt(T2, UART0, T1, INT1, T0, INT0) */
	EIE1 = 0x00;	/* Disable Interrupt(COMPARATOR_0_FE, COMPARATOR_0_RE, LVD, I2C, SPI) */
	EIE2 = 0x00; 	/* Disable Interrupt(T168, KBI, INT3, INT2, CRT) */

	EIE2 |= (1<<EKBI); /* Enable KBI Interrupt */
}

void InitPort(void)
{
/*  When Px.n set as output, if register Px[n]=0, the Px.n output logic low; 
    else if Px[n]=1，then the Px.n output logic high(push-pull output) or Hi-Z(open-drain output).  
    Reading register Px[n] is not affected by the P0.n port status.
    When Px.n set as input, writing register Px[n] doesn’t affect Px.n port status. 
    When Reading register Px[n], value 0 means Px.n input logic low, and value 1 means Px.n input logic high.*/

/*  {OE,MD} Port mode control bit: {PxOE[n],PxMD[n]} control port Px.n
    00: floating input;
    01: pull-up input;
    10: push-pull output;
    11: open-drain output. */

/********************************************************************/
/* Key Matrix input pull-up init                                    */
/* P1.0 ~ P1.7 (8)                                                  */
/********************************************************************/
  /* Data register */
  P1 = _0000_0000B;
  
  /* P1 mode control register */
  P1MDH = _0101_0101B; // P1OE7 P1MD7 | P1OE6 P1MD6 | P1OE5 P1MD5 | P1OE4 P1MD4 // 0x55
  P1MDL = _0101_0101B; // P1OE3 P1MD3 | P1OE2 P1MD2 | P1OE1 P1MD1 | P1OE0 P1MD0 // 0x55


/********************************************************************/
/* Key Matrix output push-pull init                                 */
/* P2.0 P2.1 P2.2 P2.5 P2.6 P0.6 P0.7 (7)                           */
/********************************************************************/
  	/* Data register */
	P2 = 0 | _0110_0111B;
	P0 = 0 | _1100_0000B;

	/* P2 mode control register */
	P2MDH = _0110_1001B; // P2OE7 P2MD7 | P2OE6 P2MD6 | P2OE5 P2MD5 | P2OE4 P2MD4 // 0x69
	P2MDL = _0110_1010B; // P2OE3 P2MD3 | P2OE2 P2MD2 | P2OE1 P2MD1 | P2OE0 P2MD0 // 0x6A
	/* P0 mode control register (high byte)*/
	P0MDH = _1010_0101B; // P0OE7 P0MD7 | P0OE6 P0MD6 | P0OE5 P0MD5 | P0OE4 P0MD4 // 0xA5

/********************************************************************/
/* Led output init                                                  */
/* P0.0                                                             */  
/********************************************************************/
	/* Data register */ 
	P0 |= _0000_0001B;

	/* P0 mode control register (low byte)*/
  	P0MDL = _0101_0110B; // P0OE3 P0MD3 | P0OE2 P0MD2 | P0OE1 P0MD1 | P0OE0 P0MD0 // 0x56


/********************************************************************/
/* IR port init                                                     */
/* REM : P3.0                                                       */
/********************************************************************/
  /* Data register */
	P3 = _1111_1110B; 

  /* P3 mode control register (low byte) */     
	P3MDL = _1010_1010B; // P3OE7 P3MD7 | P3OE6 P3MD6 | P3OE5 P3MD5 | P3OE4 P3MD4 // 0xAA


/* P10 mode control register (low byte) */ 
  xP10MDL = _1010_1001B; // - - - - - - P10OE0 P10MD0 // 0xA9

  _nop_();

}

void stop_define(void)
{
	ir_ok            = FALSE;
	setting_ok       = FALSE;
	t1_int           = 2;
	backup_key       = NO_KEY;
	led_force        = 0xff;
	//backlight_info   = DIM_OFF;
	//backlight_cnt    = 0;
#if (USE_SPECIAL_0FEH_5 == TRUE)
	Three_low_keyoff = 0;
#endif
	ContinueDoubleKey=0;
	twokey_result=0xff;
	IrKeyBuf=0xff;
	backup_IrKeyBuf=0xff;
#if (USE_AVSHIFT_MODE == TRUE)
	avShiftMode_active=0;
#endif

#if (USE_STUCKKEY_TIMER == TRUE)
	StuckKeyTimer_Stop;
#endif

	//Outputs "L" level to GPIO.
	P2 &= ~(_0110_0111B);
 	P0 &= ~(_1100_0000B);

	//Learn_Pwr_Off;
	//device_led_off(IR_SENDING_LED);
	all_led_off;

	CRT_onoff(CRT_OFF);
	P3 &= ~0x01;
	
	Timer0_stop;
	Timer1_stop;
	Timer2_stop;

}

#if 1
void ram_check(void)
{
	int8u		retry=3;
	int8u		ram_broken=0;

	//printf("ram_check\n");

	chksum_cal();

	if((chksum!=set_no.set_member.Chksum.sum) || ((int8u)(~chksum)!=set_no.set_member.Chksum.com))		//ram ng. -> default.
	{
		while(retry)
		{
			Information_RW(READ);
			chksum_cal();

			if((chksum==set_no.set_member.Chksum.sum) && ((int8u)(~chksum)==set_no.set_member.Chksum.com))
				break;
			retry--;
		}
		if(!retry)
		{
			FactoryDefault();
		}
	}

#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
	if((set_no.set_member.set_no.stb_src < IR_CUSTOM_NUM_MIN) || (set_no.set_member.set_no.stb_src > IR_CUSTOM_NUM_MAX)
#else
	if((set_no.set_member.set_no.stb_src!=DEFAULT_STB_SET_NO)
#endif
	)
		ram_broken=1;
	if(set_no.set_member.set_dev.stb_src!=DEV_STB)
		ram_broken=1;

	if(ram_broken)
	{
		FactoryDefault();
		WDG_SystemReset();
	}

}
#endif


void reset_define(void)
{
	int8u	retry=3;
	int8u	ram_broken=0;

	ir_ok=FALSE;
	setting_ok=FALSE;
	t1_int=2;
	backup_key=0xff;
	curr_state=NORMAL_MODE;
	Timer0_65ms16=0;
	Timer0_65ms16_2=0;
	Timer1_1sec08 = 0;
	FactoryEntryCnt = 0;
	crt_int_counter = 0;
	RC5_Toggle=0;
	power_toggle=0;
	led_force=0xff;
#if (USE_SPECIAL_0FEH_5 == TRUE)
	Three_low_keyoff=0;
#endif
	bTestModeEnable = 0;
	ContinueDoubleKey=0;
	IrKeyBuf=0xff;
	backup_IrKeyBuf=0xff;
#if (USE_AVSHIFT_MODE == TRUE)
	avShiftMode_active=0;
#endif
#if (USE_BACKLIGHT == TRUE)
//	backlight_info=DIM_OFF;
//	backlight_cnt=0;
	BackLight_Dim_Off();
#endif
	key_buf_old_clear();

	STB_IRoutCommon();
	backup_source=curr_source;

#if (USE_BATT_MONITOR == TRUE)
	keyCnt_inLow=LVI_CHECK_CNT;
#endif

	ram_check();

	wBitStream_Clear();

	while(getkey_simple()){}	
	device_Led_blink(IR_SENDING_LED, 1, 2, 0);
	device_led_off(IR_SENDING_LED);
}

void  FactoryDefault(void)
{
#if (USE_IR_LEARN == TRUE)
	EraseLearnDataToAll();
#endif

	curr_source=SRC_STB;
	backup_source=curr_source;

	set_no.set_member.set_no.stb_src=DEFAULT_STB_SET_NO;
#if (USE_BATCH_SS_LG == TRUE)
	set_no.set_member.set_no.tv_src=DEFAULT_UR_VIRTUAL_NO;
#else
	set_no.set_member.set_no.tv_src=STANDARD_SAMSUNG_TV;
#endif

	set_no.set_member.set_dev.stb_src=DEV_STB;
	set_no.set_member.set_dev.tv_src=DEV_TV;

	Save_Information_All();
}

void Timer0_65msStart(void)
{
	TMOD |=  0 
		 |	(0<<T0M1)	// T0M[1:0] = 01(mode 0)
		 |	(1<<T0M0); 	// 00: mode 0, 13-bit counting mode (default).
						// 01: mode 1, 16-bit counting mode.
						// 10: mode 2, 8-bit counting mode with auto-reload.
						// 11: mode 3, 2 independent 8-bit counters.

	xTIMPRS	|= 0		     // fCLK/2 fCLK/4 fCLK/8 fCLK/16 fCLK/32 fCLK/64 fCLK/128 fCLK/256  
			| (0<<TIMPRS02)  //    0      0      0      0       1       1       1        1   
			| (1<<TIMPRS01)  //    0      0      1      1       0       0       1        1
			| (1<<TIMPRS00); //    0      1      0      1       0       1       0        1
	// 16MHz /  65536 /  16    = 15Hz = 65ms
	// fCLK / TMOD / TIMPR

	TF0 = 0; // 0: no T0 overflow interrup_t request generated.

	Timer0_start; // enable Timer0 interrup_t request, T0 is on.
	Interrupt_Enable;
}

void Timer1_1secStart(void)
{
	
	TMOD |=  0 
		 |	(0<<T1M1)	// T1M[1:0] = 01(mode 2)
		 |	(1<<T1M0); 	// 00: mode 0, 13-bit counting mode (default).
						// 01: mode 1, 16-bit counting mode.
						// 10: mode 2, 8-bit counting mode with auto-reload.
						// 11: mode 3, 2 independent 8-bit counters.

	xTIMPRS	|= 0		     // fCLK/2 fCLK/4 fCLK/8 fCLK/16 fCLK/32 fCLK/64 fCLK/128 fCLK/256  
			| (1<<TIMPRS12)  //    0      0      0      0       1       1       1        1   
			| (1<<TIMPRS11)  //    0      0      1      1       0       0       1        1
			| (1<<TIMPRS10); //    0      1      0      1       0       1       0        1
	// 16MHz / 256 / 256    = 0.953Hz = 1sec
	// fCLK / TMOD / TIMPR

	Timer1_start; // enable Timer1 interrup_t request, T1 is on.
	Interrupt_Enable;
}

void Timer2_Start(void) //Envelop Timer
{
	//T2CON1
	T2CNT = 0; // timer mode, in which T2 is clocked by prescale of FCLK.
	T2CPNRL = 0; //reload mode, triggered by effective edge of T2EX or T2 overflowing.

//	xT2CON2 = 0 				// fCLK/2 fCLK/4 fCLK/8 fCLK/16 fCLK/32 fCLK/64 fCLK/128 fCLK/256  
//			|= (1<<T2CK2)		//    0      0      0      0       1       1       1        1   
//			|= (0<<T2CK1)		//    0      0      1      1       0       0       1        1
//			|= (0<<T2CK0);		//    0      1      0      1       0       1       0        1
	xT2CON2 = 0x04 ;
	// 16MHz / 32    	= 0.5MHz = 2us --------> 한 카운터 당 2us
	// 풀 카운트(65536) 시 130ms
	// fCLK / T2CK[2:0]


	if(curr_device == DEV_STB)
	{
		RCAP2L = 0xF0;	//Read high pulse timing 오류 현상 보완
		RCAP2H = 0xFF;
	}
	else
	{
		RCAP2L = 0xFF;	//Read high pulse timing 오류 현상 보완
		RCAP2H = 0xFF;
	}
	

	IP |= (1<<5);

	Timer2_start; // enable Timer2 interrup_t request, T2 is on.
	Interrupt_Enable;
}


void TickTimer_65msStart(int16u time)
{
	Timer0_65ms16 = time;

	//Timer0_50msStart();
	Timer0_65msStart();
}

#if 0 //t1 = 500us
void BatchTimer_500usStart(int16u time)
{
	Timer1_500us16=time;

	Timer1_500usStart();
}
#endif


#if (USE_STUCKKEY_TIMER == TRUE)
void StuckKeyTimer_Start(int8u time)
{
	Timer1_1sec08 = time;

	Timer1_1secStart();
}
#endif

#if 0
void WDT_clear()
{
	WDTMR	= 0x20;  	// Clear Watch-dog timer
}
#endif

#endif	//__HAL_C__

