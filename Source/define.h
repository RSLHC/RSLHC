
#ifndef DEFINE_H_
#define DEFINE_H_

/******************************************************************************
/******************************************************************************
/*****************************                 ********************************
/***************************** sc51f33xx_sys.h ********************************
/*****************************                 ********************************
/******************************************************************************
/******************************************************************************

/** @addtogroup Interrupt_Critical
  * @{
  */


#define HAL_ENTER_CRITICAL(x)           {x = EA; EA = 0;}
#define HAL_EXIT_CRITICAL(x)            {EA = x;}
#define HAL_GLOBAL_INTERRUPT_DIS()      {EA = 0;}
#define HAL_GLOBAL_INTERRUPT_ENABLE()   {EA = 1;}
#define HAL_GLOBAL_INTERRUPT_EN         EA


/**************************************************************************************************
 *                                            DEFINES
 **************************************************************************************************/
#define	TRUE      1
#define	FALSE     0

#define DISABLE    0
#define ENABLE     1
#define ERROR      0
#define FAIL       0
#define SUCCESS    (!FAIL)

#define CLEAR_VARIABLE 		0
#define REMOVE_FUNCTION 	0

#define DUMMY_DATA 			1
#define EXIST_DATA 			0

#define RUN             1
#define STOP            0

#define STOP_DETECT     1
#define T2_LAST_TIME    2
#define STOP_SIGNAL     3



// ----------------------------------------------- sysclk -----------------------------------------------
#define CLK_SELRCH  0       // Internal Oscilator
#define CLK_SELOSCH 1       // Exterbal Oscilator

#define DEVIDED_FREQ_1  1   //MCLK frequency is same as SYSCLK
#define DEVIDED_FREQ_2  2   //MCLK frequency is 2 divided-frequency of SYSCLK.
#define DEVIDED_FREQ_4  4   //MCLK frequency is 4 divided-frequency of SYSCLK.
#define DEVIDED_FREQ_8  8   //MCLK frequency is 8 divided-frequency of SYSCLK
// ------------------------------------------------------------------------------------------------------

// ------------------------------------------------ crt -------------------------------------------------
#define CRT_ON          1
#define CRT_OFF         0
#define CRT_MODULATE    0
#define CRT_DEMODULATE  1
#define CRT_INP30       0
#define CRT_INT168      1
#define CRT_REMOPS0     0
#define CRT_REMOPS1     1
// ------------------------------------------------------------------------------------------------------



/**************************************************************************************************
 *                                            FW VERSION
 **************************************************************************************************/
#define APP_FW_VERSION         		0x00

/**************************************************************************************************
 *                                            TEST
 **************************************************************************************************/

/**************************************************************************************************
 *                                            TOOL TYPE
 **************************************************************************************************/
#define RB95_TOOL		1		//

#define TOOL_TYPE	RB95_TOOL

/**************************************************************************************************
 *                                            RCU TYPE
 **************************************************************************************************/
#define KT_OTV_RCU		1		//w RB95_TOOL / SAMSUNG_TOOL

#define RCU_TYPE	KT_OTV_RCU

/**************************************************************************************************
 *                                         SW FEATURES
 **************************************************************************************************/
#define USE_WATCH_DOG_TIMER		FALSE
#define USE_QUICKLY_KEY			FALSE
#define USE_TEST_MODE			TRUE
#define USE_STUCKKEY_TIMER		TRUE

/**************************************************************************************************
 *                                            IRDB
 **************************************************************************************************/
#define USE_2BYTE_SETTING_NO		TRUE
#if (USE_2BYTE_SETTING_NO == TRUE)
	#define INC_PTR_SIZE			2
#else
	#define INC_PTR_SIZE			1
#endif

#define USE_DATA_ALIGNMENT		FALSE
#define USE_LITTLE_ENDIAN		FALSE
#define USE_ADDRESS_32BIT		FALSE

#define USE_SETUP_STB_IR_CUSTOM		TRUE
	#define IR_CUSTOM_NUM_MIN		1
	#define IR_CUSTOM_NUM_MAX		50

#define USE_AVSHIFT_MODE		TRUE

#define USE_BATCH_SS_LG			TRUE

#define USE_SPECIAL_0F0H		TRUE	// M708, SAA1250
#define USE_SPECIAL_0F1H		FALSE	// SAA1350
#define USE_SPECIAL_0F2H		TRUE	// CX7947(12or15BIT),40KHz,43ms,52C
#define USE_SPECIAL_0F3H		FALSE	// SONY OLD
#define USE_SPECIAL_0F4H		TRUE	// U327
#define USE_SPECIAL_0F5H		TRUE	// PIONEER
#define USE_SPECIAL_0F6H		FALSE	// ANYCARRIER
#define USE_SPECIAL_0F7H		FALSE	// PANSONIC ZAP
#define USE_SPECIAL_0F8H		FALSE	// two carrier
#define USE_SPECIAL_0F9H		FALSE	// ONKYO FORMAT
#define USE_SPECIAL_0FAH		FALSE	// /nec 1word - 2word - repeat
#define USE_SPECIAL_0FBH		FALSE	// nec repeat & continue
#define USE_SPECIAL_0FCH		FALSE	// PATTERN112_ 1word - 1word - repeat
#define USE_SPECIAL_0FDH		FALSE	// nec repeat & continue
#define USE_SPECIAL_0FEH_2		FALSE	// DATA2_PREIOD
#define USE_SPECIAL_0FEH_3		FALSE	// Metz_Three_Low
#define USE_SPECIAL_0FEH_4		FALSE	// Bang & Olufsen 455KHz
#define USE_SPECIAL_0FEH_5		TRUE	// Three low..
#define USE_SPECIAL_0FEH_6		FALSE	// Shift_Bit
#define USE_SPECIAL_0FEH_8		FALSE	// xmp
#define USE_SPECIAL_0FEH_9		FALSE	// Canal
#define USE_SPECIAL_0FEH_A		FALSE	// AMINO_AROSA

/**************************************************************************************************
 *                                         LED SET MODE
 **************************************************************************************************/
#define	SET_ENTRY			0
#define	SET_OK				1
#define	SET_NG				2
#define	SET_LEARN_ENTRY		3
#define	SET_LEARN_OK		4
#define	SET_LEARN_NG		5

/**************************************************************************************************
 *                                            BATTERY
 **************************************************************************************************/
#define USE_BATT_MONITOR		TRUE
#define USE_LOW_BATT_IROUT	TRUE
#define USE_LOW_BATT_LED		TRUE

#define	LVI_CHECK_CNT		1 // 4

/**************************************************************************************************
 *                                            TIMES
 **************************************************************************************************/
// ????���? 계산: 65ms * 31 = 2sec
#define SETUP_TIMEOUT_S				  312				//20sec
#define	COMBOKEY_TIMEOUT_S		   31				//2sec
#define	COMBOKEY_TIMEOUT2_S		   77  			//5sec
#define AVSHIFT_TIMEOUT_S			  154 			//10sec

// ????���? 계산: 1s * 60 = 60sec
#define	STUCKKEY_TIMEOUT_S			58				//60sec

/**************************************************************************************************
 *                                            STATE
 **************************************************************************************************/
#define NORMAL_MODE     		0
#define SET_MODE_ENTRY 			1
#define SET_MODE_DIGIT 			2
#define SET_MODE_AUTO 			3
#define SET_MODE_STB_ID 		4
#define SET_MODE_GLOBAL 		5
#define SET_MODE_GLOBAL_OK 		6
#define SET_MODE_PAIRING 		7
#define SET_MODE_UNPAIRING 		8
#define SET_MODE_COMPLETE 		9
#define SET_MODE_QUICK_SEARCH	10
#define SET_MODE_CODEREADOUT	11
#define SET_MODE_LEARN			12
#define SET_MODE_LEARN_RX		13
#define SET_MODE_LEARN_DELETE	14
#define SET_MODE_SETUP_TABLE	15
#define SET_MODE_BRIGHTNESS		16

/**************************************************************************************************
 *                                            DEVICE
 **************************************************************************************************/
#define DEV_STB       	0
#define DEV_TV      	1
#define DEV_TV1     	2
#define DEV_TESTMODE   	3

#define MAX_DEVICE_NO  	2

/**************************************************************************************************
 *                                         SOURCE DEVICE
 **************************************************************************************************/
#define SRC_STB         0
#define SRC_TV       	1
#define SRC_ALL     	2

#define SRC_CNT     	2

/**************************************************************************************************
 *                                          COMBO KEY
 **************************************************************************************************/
#define COMBO_KEYS_UR_SETUP			0 
#define COMBO_KEYS_STB_SETUP		1 
#define COMBO_KEYS_FACTORY_DEFAULT	2 

/**************************************************************************************************
 *                                          DEFAULT UR
 **************************************************************************************************/
#define STANDARD_SAMSUNG_TV		171
#define STANDARD_LG_TV			165
#define DEFAULT_UR_VIRTUAL_NO	164
#define DEFAULT_UR_SET_NO		DEFAULT_UR_VIRTUAL_NO
#define DEFAULT_UR_DEVICE		DEV_TV
#define DEFAULT_STB_SET_NO		1

/**************************************************************************************************
 *                                            MAP NG
 **************************************************************************************************/
#define MAP_NG          90

/**************************************************************************************************
 *                                            FLASH
 **************************************************************************************************/
#define	READ		0
#define	WRITE		1

#define	LEARN_MAX_KEYS		0
#define	LEARN_MAX_DEVICE	0

#if 0
#define	EEPROM_1PAGE		64			//24C256
#define	STARTADDR_LEARN		0x0040

#define	LEARN_DEVICE_SIZE	(EEPROM_1PAGE*LEARN_MAX_KEYS)
#define	LEARN_TOTAL_SIZE	(LEARN_DEVICE_SIZE*LEARN_MAX_DEVICE)
#else
#define EEPROM_PBUFF_SIZE	1024

//#define	EEPROM_START_ADDR	(0xFFC0)	//0x7000
#define EEPROM_END_ADDR		(0xFFFF)

#define	LEARN_DEVICE_SIZE	(EEPROM_PBUFF_SIZE*LEARN_MAX_KEYS)

#define	LEARN_TOTAL_SIZE	(LEARN_DEVICE_SIZE*LEARN_MAX_DEVICE)

// 설정번호 저장
#define	STARTADDR_INFO		(EEPROM_END_ADDR-EEPROM_PBUFF_SIZE+1)

//#define	STARTADDR_INFO		0xFF00

//#define	STARTADDR_LEARN		0xF000
#define	STARTADDR_LEARN		(STARTADDR_INFO-LEARN_TOTAL_SIZE)
#define	EEPROM_START_ADDR	(STARTADDR_LEARN)
#endif

/**************************************************************************************************
 *                                            KEYS
 **************************************************************************************************/
#define KEY_NULL			0
#define KEY_TVPOWER        	1
#define KEY_TVVOLUP        	2
#define KEY_TVVOLDOWN       3
#define KEY_INPUT        	4
#define KEY_ALLPOWER        5
#define KEY_STBPOWER        6
#define KEY_VOLUP        	7
#define KEY_VOLDOWN        	8
#define KEY_MUTE      		9
#define KEY_CHUP      		10
#define KEY_CHDOWN      	11
#define KEY_NAVI_UP    		12
#define KEY_NAVI_DOWN  		13
#define KEY_NAVI_LEFT      	14
#define KEY_NAVI_RIGHT     	15
#define KEY_OK      		16
#define KEY_1        		17
#define KEY_2        		18
#define KEY_3        		19
#define KEY_4        		20
#define KEY_5        		21
#define KEY_6        		22
#define KEY_7        		23
#define KEY_8        		24
#define KEY_9        		25
#define KEY_0        		26
#define KEY_MOVIE        	27
#define KEY_REVIEW      	28
#define KEY_CHGUIDE        	29
#define KEY_MENU        	30
#define KEY_OPTION        	31
#define KEY_BACK      		32
#define KEY_EXIT      		33
#define KEY_REW        		34
#define KEY_PLAYPAUSE  		35
#define KEY_STOP        	36
#define KEY_FF        		37
#define KEY_STAR        	38
#define KEY_SHARP        	39
#define KEY_SEARCH        	40
#define KEY_DELETE        	41
#define KEY_HANENG        	42
#define KEY_RED        		43
#define KEY_GREEN        	44
#define KEY_YELLOW        	45
#define KEY_BLUE        	46
#define KEY_MYMENU        	47
#define KEY_SHOPPING        48
#define KEY_WIDGET        	49
#define KEY_APPSTORE        50

/**************************************************************************************************
 *                                        KEY SEARCH
 **************************************************************************************************/
#define P2_N_BIT_MAX_COUNT      5

/**************************************************************************************************
 *                                        VIRTUAL KEYS
 **************************************************************************************************/
#define KEY_LOWBATT			51

/**************************************************************************************************
 *                                           isKeyIn
 **************************************************************************************************/
#define isNavigationKeyIn	((key_buf[0]==KEY_NAVI_UP)||(key_buf[0]==KEY_NAVI_DOWN)||(key_buf[0]==KEY_NAVI_LEFT)||(key_buf[0]==KEY_NAVI_RIGHT)/*||(key_buf[0]==KEY_OK)*/)
#define isAvShiftKeyIn		((key_buf[0]==KEY_NAVI_UP)||(key_buf[0]==KEY_NAVI_DOWN)||(key_buf[0]==KEY_NAVI_LEFT)||(key_buf[0]==KEY_NAVI_RIGHT)||(key_buf[0]==KEY_OK))

/**************************************************************************************************
 *                                         		HAL
 **************************************************************************************************/
#define Fosc		8000000UL						// main clock is 8.00 MHz
#define Subosc 		32768UL							// sub clock is 32.768 KHz

#define	OSC_4MHZ	0
#define	OSC_8MHZ	1
#define	OSC_10MHZ	2
#define	OSC_12MHZ	3
#define OSC_16MHZ 4

#define CRT_OSC OSC_8MHZ

#define MAIN_OSC	OSC_16MHZ

#define DUTY_RATIO_1_3		TRUE	// 1/3
#define DUTY_RATIO_2_3		FALSE	// 2/3 Duty

#define CARRIER_DUTY_OFFSET			191

/**************************************************************************************************
 *                                            LED
 **************************************************************************************************/
#define LED_1					0
#define LED_2					1
#define LED_3					2
#define LED_4					3
#define LED_5					4

#define RED_LED					LED_1
#define GREEN_LED				LED_2
#define BLUE_LED				LED_3
#define WHITE_LED				LED_4
#define ALL_LED					LED_5

#define IR_SENDING_LED			LED_1
#define SETTING_MODE_LED		LED_1
#define LOW_BATTERY_LED			LED_1
#define CODEREADOUT_LED			LED_1
#define IR_LEARN_LED			LED_1

/**************************************************************************************************
 *                                            BACKLIGHT
 **************************************************************************************************/
#define USE_BACKLIGHT			FALSE
#define BACKLIGHT_ACTIVE_HIGH	FALSE

#if (BACKLIGHT_ACTIVE_HIGH == TRUE)
#define	backlight_off				()
#define	backlight_on				()
#else
#define	backlight_off				()
#define	backlight_on				()
#endif

#define BACKLIGHT_CYCLE				1000
#define BACKLIGHT_MAX				(BACKLIGHT_CYCLE-10)	//(BACKLIGHT_CYCLE/2)
#define DIM_OFF						0
#define DIM_UP						1
#define DIM_DOWN					2
#define DIM_FULL					3

#define	BackLight_Timer_Run			()
#define	BackLight_Timer_Stop		()

/**************************************************************************************************
 *                                          IR Learn
 **************************************************************************************************/
#define USE_IR_LEARN			FALSE
#define ONE_DEVICE_LEARN		FALSE
#define IRZIP_CARR_2BYTES		FALSE

/**************************************************************************************************
 *                                         GPIO pins
 **************************************************************************************************/
#define GPIO_Pin_0   0x01
#define GPIO_Pin_1   0x02
#define GPIO_Pin_2   0x04
#define GPIO_Pin_3   0x08
#define GPIO_Pin_4   0x10
#define GPIO_Pin_5   0x20
#define GPIO_Pin_6   0x40
#define GPIO_Pin_7   0x80

#define KEY_INPUT_MAX_COUNT		(8)

#define KEY_OUTPUT_MAX_COUNT	(7)
#define	MAX_KEY_COUNT			(KEY_INPUT_MAX_COUNT*KEY_OUTPUT_MAX_COUNT)

#define Bit_RESET		(0)
#define Bit_SET			(1)

#define KEY_INPUT_PINS_P1  	( GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 )
#define KEY_OUTPUT_PINS_P2 	( GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6 )
#define KEY_OUTPUT_PINS_P0 	( GPIO_Pin_6 | GPIO_Pin_7 )

//#define LED_OUTPUT_PINS 	( GPIO_Pin_6 | GPIO_Pin_7 )
//#define IR_OUTPUT_PIN 		( GPIO_Pin_1 )
//#define BUZZER_OUTPUT_PIN 	( GPIO_Pin_0 )
#define KEY_NO_OUTPUT_PINS	( 0 )
//#define BACKLIGHT_OUTPUT_PIN 	( GPIO_Pin_0 )
#define KEY_BUFF_CNT	4
#define	NO_KEY			0xFF

//Each interrupt source is individually enabled or disabled by setting its corresponding enable bit
#define	Interrupt_Enable			(EA = 1)
#define	Interrupt_Disable			(EA = 0)

#define	led1_on				(P0 &= _1111_1110B)	
#define	led2_on				(P0 &= _1111_1110B)	
#define	led3_on				(P0 &= _1111_1110B)	
#define	led1_off			(P0 |= _0000_0001B)
#define	led2_off			(P0 |= _0000_0001B)
#define	led3_off			(P0 |= _0000_0001B)

#define	all_led_off			(P0 |= _0000_0001B)
#define	all_led_on			(P0 &= _1111_1110B)	


#define	Learn_Pwr_On		(P2 |= _0001_0000B)
#define	Learn_Pwr_Off		(P2 &= _1110_1111B)

#define	EnvelopOn					  (P3MDL = 0xAA)
#define EnvelopOff					(P3MDL = 0xA8)

#define	CarrierOnOff				CRTCON1
#define	CarrierOn					  (CRTCON1 |= 0x80)
#define	CarrierOff					(CRTCON1 &= ~0x80)

#define	CRT_Run		      (P3 |= 0x01)
#define	CRT_Stop	      (P3 &= ~0x01)

      
#define	CRT_Interrupt_ei	(EIE2|=0x01)
#define	CRT_Interrupt_di	(EIE2&=~0x01)



/**************************************************************************************************
 *                                            TIMER
 **************************************************************************************************/
#define	Timer0_start				{ET0 = 1; TR0 = 1;} //65msec
#define	Timer0_stop					{ET0 = 0; TR0 = 0;}
#define	TickTimer_65msStop		Timer0_stop

#define	Timer1_start				{ET1 = 1; TR1 = 1;} //500us
#define	Timer1_stop					{ET1 = 0; TR1 = 0;}

#define	Timer2_start				{ET2 = 1; TR2 = 1;} //1sec
#define	Timer2_stop					{ET2 = 0; TR2 = 0;} 

#define Envelop_Timer_Start  Timer2_start
#define Envelop_Timer_Stop   Timer2_stop

#define	StuckKeyTimer_Stop			Timer1_stop

/**************************************************************************************************
 *                                            REGISTER
 **************************************************************************************************/
//SYSCFG
#define RSTCFG  7
#define TREN    6
#define TRDS    5
#define WDTCFG  4 
#define CFG1    3
#define CFG0    2 
#define RCLCFG  0

//SYSCFG2
#define DREN    0

//STOPCFG
#define PWRCFG  3

//LVRCON
#define LVREN   7
#define BGEN    5
#define BGOE    2

//LVDCON
#define LVDEN   7
#define LVDF    5
#define LVES    4 
#define LVLS3   3
#define LVLS2   2
#define LVLS1   1
#define LVLS0   0

//MCKSET
#define OSTS1   7
#define OSTS0   6
#define CRYEN   4 
#define CRYIOEN 3
#define MCKS    2
#define MDIV1   1
#define MDIV0   0 

//CRTCON1
#define CRTEN   7
#define CRTMODE 6
#define CRTEXEN 5 
#define CRTOVEN 4
#define CRTIS   3
#define REMOPS  2
#define CRTEXF  1
#define CRTOVF  0

//IOMUX0
#define REMEN   6

//IOMUX2
#define CRTOE   6

//TMOD 
#define T1M1    5
#define T1M0    4
#define T0M1    1
#define T0M0    0

//TIMPRS
#define TIMPRS12  6
#define TIMPRS11  5
#define TIMPRS10  4
#define TIMPRS02  2
#define TIMPRS01  1
#define TIMPRS00  0

//EIE1
#define ECP0F     6
#define ECPOR     5
#define ELVD      3
#define EIIC      1
#define ESPI      0

//EIE2
#define ET168     7
#define EKBI      6
#define EX3       2
#define EX2       1
#define ECRT      0

//T2CON2
#define T2CK2     2
#define T2CK1     1
#define T2CK0     0

//RSTSRC
#define SOFTF     4

/**************************************************************************************************
 *                                     Interrupt control
 **************************************************************************************************/
#define	Interrupt_All_Enable		  (IE|=0x80)
#define	Interrupt_All_Disable		  (IE&=~0x80)

#define	Timer2_Interrupt_ei			  (IE|=0x20)
#define	Timer2_Interrupt_di			  (IE&=~0x20)
#define	UART0_Interrupt_ei			  (IE|=0x10)
#define	UART0_Interrupt_di			  (IE&=~0x10)
#define	Timer1_Interrupt_ei			  (IE|=0x08)
#define	Timer1_Interrupt_di			  (IE&=~0x08)
#define	External_Interrupt1_ei	  (IE|=0x04)
#define	External_Interrupt1_di	  (IE&=~0x04)
#define	Timer0_Interrupt_ei			  (IE|=0x02)
#define	Timer0_Interrupt_di			  (IE&=~0x02)
#define	External_Interrupt0_ei	  (IE|=0x01)
#define	External_Interrupt0_di	  (IE&=~0x01)

#define Comp_Out_FE_Interrupt_ei  (EIE1|=0x40)
#define Comp_Out_FE_Interrupt_di  (EIE1&=~0x40)
#define Comp_Out_RE_Interrupt_ei  (EIE1|=0x20)
#define Comp_Out_RE_Interrupt_di  (EIE1&=~0x20)
#define LVD_Interrupt_ei          (EIE1|=0x08)
#define LVD_Interrupt_di          (EIE1&=~0x08)
#define I2C_Interrupt_ei          (EIE1|=0x02)
#define I2C_Interrupt_di          (EIE1&=~0x02)
#define SPI_Interrupt_ei          (EIE1|=0x01)
#define SPI_Interrupt_di          (EIE1&=~0x01)

#define	T168_Interrupt_ei		      (EIE2|=0x80)
#define	T168_Interrupt_di		      (EIE2&=~0x80)
#define	KBI_Interrupt_ei		      (EIE2|=0x40)
#define	KBI_Interrupt_di		      (EIE2&=~0x40)
#define	External_Interrupt3_ei	  (EIE2|=0x04)
#define	External_Interrupt3_di	  (EIE2&=~0x04)
#define	External_Interrupt2_ei	  (EIE2|=0x02)
#define	External_Interrupt2_di	  (EIE2&=~0x02)
#define	CRT_Interrupt2_ei	        (EIE2|=0x01)
#define	CRT_Interrupt2_di	        (EIE2&=~0x01)

/**************************************************************************************************
 *                                            ETC
 **************************************************************************************************/
void _nop_(void);
void __no_operation(void);

/**************************************************************************************************
 *                                         BIT DEFINITION
 **************************************************************************************************/
#define  _0000_0000B        0
#define  _0000_0001B        1
#define  _0000_0010B        2
#define  _0000_0011B        3
#define  _0000_0100B        4
#define  _0000_0101B        5
#define  _0000_0110B        6
#define  _0000_0111B        7
#define  _0000_1000B        8
#define  _0000_1001B        9
#define  _0000_1010B        10
#define  _0000_1011B        11
#define  _0000_1100B        12
#define  _0000_1101B        13
#define  _0000_1110B        14
#define  _0000_1111B        15
#define  _0001_0000B        16
#define  _0001_0001B        17
#define  _0001_0010B        18
#define  _0001_0011B        19
#define  _0001_0100B        20
#define  _0001_0101B        21
#define  _0001_0110B        22
#define  _0001_0111B        23
#define  _0001_1000B        24
#define  _0001_1001B        25
#define  _0001_1010B        26
#define  _0001_1011B        27
#define  _0001_1100B        28
#define  _0001_1101B        29
#define  _0001_1110B        30
#define  _0001_1111B        31
#define  _0010_0000B        32
#define  _0010_0001B        33
#define  _0010_0010B        34
#define  _0010_0011B        35
#define  _0010_0100B        36
#define  _0010_0101B        37
#define  _0010_0110B        38
#define  _0010_0111B        39
#define  _0010_1000B        40
#define  _0010_1001B        41
#define  _0010_1010B        42
#define  _0010_1011B        43
#define  _0010_1100B        44
#define  _0010_1101B        45
#define  _0010_1110B        46
#define  _0010_1111B        47
#define  _0011_0000B        48
#define  _0011_0001B        49
#define  _0011_0010B        50
#define  _0011_0011B        51
#define  _0011_0100B        52
#define  _0011_0101B        53
#define  _0011_0110B        54
#define  _0011_0111B        55
#define  _0011_1000B        56
#define  _0011_1001B        57
#define  _0011_1010B        58
#define  _0011_1011B        59
#define  _0011_1100B        60
#define  _0011_1101B        61
#define  _0011_1110B        62
#define  _0011_1111B        63
#define  _0100_0000B        64
#define  _0100_0001B        65
#define  _0100_0010B        66
#define  _0100_0011B        67
#define  _0100_0100B        68
#define  _0100_0101B        69
#define  _0100_0110B        70
#define  _0100_0111B        71
#define  _0100_1000B        72
#define  _0100_1001B        73
#define  _0100_1010B        74
#define  _0100_1011B        75
#define  _0100_1100B        76
#define  _0100_1101B        77
#define  _0100_1110B        78
#define  _0100_1111B        79
#define  _0101_0000B        80
#define  _0101_0001B        81
#define  _0101_0010B        82
#define  _0101_0011B        83
#define  _0101_0100B        84
#define  _0101_0101B        85
#define  _0101_0110B        86
#define  _0101_0111B        87
#define  _0101_1000B        88
#define  _0101_1001B        89
#define  _0101_1010B        90
#define  _0101_1011B        91
#define  _0101_1100B        92
#define  _0101_1101B        93
#define  _0101_1110B        94
#define  _0101_1111B        95
#define  _0110_0000B        96
#define  _0110_0001B        97
#define  _0110_0010B        98
#define  _0110_0011B        99
#define  _0110_0100B        100
#define  _0110_0101B        101
#define  _0110_0110B        102
#define  _0110_0111B        103
#define  _0110_1000B        104
#define  _0110_1001B        105
#define  _0110_1010B        106
#define  _0110_1011B        107
#define  _0110_1100B        108
#define  _0110_1101B        109
#define  _0110_1110B        110
#define  _0110_1111B        111
#define  _0111_0000B        112
#define  _0111_0001B        113
#define  _0111_0010B        114
#define  _0111_0011B        115
#define  _0111_0100B        116
#define  _0111_0101B        117
#define  _0111_0110B        118
#define  _0111_0111B        119
#define  _0111_1000B        120
#define  _0111_1001B        121
#define  _0111_1010B        122
#define  _0111_1011B        123
#define  _0111_1100B        124
#define  _0111_1101B        125
#define  _0111_1110B        126
#define  _0111_1111B        127
#define  _1000_0000B        128
#define  _1000_0001B        129
#define  _1000_0010B        130
#define  _1000_0011B        131
#define  _1000_0100B        132
#define  _1000_0101B        133
#define  _1000_0110B        134
#define  _1000_0111B        135
#define  _1000_1000B        136
#define  _1000_1001B        137
#define  _1000_1010B        138
#define  _1000_1011B        139
#define  _1000_1100B        140
#define  _1000_1101B        141
#define  _1000_1110B        142
#define  _1000_1111B        143
#define  _1001_0000B        144
#define  _1001_0001B        145
#define  _1001_0010B        146
#define  _1001_0011B        147
#define  _1001_0100B        148
#define  _1001_0101B        149
#define  _1001_0110B        150
#define  _1001_0111B        151
#define  _1001_1000B        152
#define  _1001_1001B        153
#define  _1001_1010B        154
#define  _1001_1011B        155
#define  _1001_1100B        156
#define  _1001_1101B        157
#define  _1001_1110B        158
#define  _1001_1111B        159
#define  _1010_0000B        160
#define  _1010_0001B        161
#define  _1010_0010B        162
#define  _1010_0011B        163
#define  _1010_0100B        164
#define  _1010_0101B        165
#define  _1010_0110B        166
#define  _1010_0111B        167
#define  _1010_1000B        168
#define  _1010_1001B        169
#define  _1010_1010B        170
#define  _1010_1011B        171
#define  _1010_1100B        172
#define  _1010_1101B        173
#define  _1010_1110B        174
#define  _1010_1111B        175
#define  _1011_0000B        176
#define  _1011_0001B        177
#define  _1011_0010B        178
#define  _1011_0011B        179
#define  _1011_0100B        180
#define  _1011_0101B        181
#define  _1011_0110B        182
#define  _1011_0111B        183
#define  _1011_1000B        184
#define  _1011_1001B        185
#define  _1011_1010B        186
#define  _1011_1011B        187
#define  _1011_1100B        188
#define  _1011_1101B        189
#define  _1011_1110B        190
#define  _1011_1111B        191
#define  _1100_0000B        192
#define  _1100_0001B        193
#define  _1100_0010B        194
#define  _1100_0011B        195
#define  _1100_0100B        196
#define  _1100_0101B        197
#define  _1100_0110B        198
#define  _1100_0111B        199
#define  _1100_1000B        200
#define  _1100_1001B        201
#define  _1100_1010B        202
#define  _1100_1011B        203
#define  _1100_1100B        204
#define  _1100_1101B        205
#define  _1100_1110B        206
#define  _1100_1111B        207
#define  _1101_0000B        208
#define  _1101_0001B        209
#define  _1101_0010B        210
#define  _1101_0011B        211
#define  _1101_0100B        212
#define  _1101_0101B        213
#define  _1101_0110B        214
#define  _1101_0111B        215
#define  _1101_1000B        216
#define  _1101_1001B        217
#define  _1101_1010B        218
#define  _1101_1011B        219
#define  _1101_1100B        220
#define  _1101_1101B        221
#define  _1101_1110B        222
#define  _1101_1111B        223
#define  _1110_0000B        224
#define  _1110_0001B        225
#define  _1110_0010B        226
#define  _1110_0011B        227
#define  _1110_0100B        228
#define  _1110_0101B        229
#define  _1110_0110B        230
#define  _1110_0111B        231
#define  _1110_1000B        232
#define  _1110_1001B        233
#define  _1110_1010B        234
#define  _1110_1011B        235
#define  _1110_1100B        236
#define  _1110_1101B        237
#define  _1110_1110B        238
#define  _1110_1111B        239
#define  _1111_0000B        240
#define  _1111_0001B        241
#define  _1111_0010B        242
#define  _1111_0011B        243
#define  _1111_0100B        244
#define  _1111_0101B        245
#define  _1111_0110B        246
#define  _1111_0111B        247
#define  _1111_1000B        248
#define  _1111_1001B        249
#define  _1111_1010B        250
#define  _1111_1011B        251
#define  _1111_1100B        252
#define  _1111_1101B        253
#define  _1111_1110B        254
#define  _1111_1111B        255
#endif

