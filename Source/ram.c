
#ifndef	__RAM_C__
#define	__RAM_C__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "define.h"
#include "type_def.h"
#include "main.h"

/*****************************************************************************/
/* Global variable declarations ('', definition in C source)           */
/*****************************************************************************/
xdata 	TYPE_SETNO 		set_no;
xdata 	TYPE_FORMAT		ramFormat;
xdata 	TYPE_CODE		ramCode;
xdata 	IR_STREAM_ST	ir_stream;
//xdata 	Input_Carrier	Envelop;
xdata 	IR_ZIP_ST			ir_zip;

xdata		byte		chksum;
xdata		byte		curr_device;
xdata		byte		curr_source;
xdata		byte		backup_source;
xdata		byte		curr_state;
xdata		byte		setting_device;
xdata		byte		setting_source;
xdata		byte		backup_key;
xdata		byte		key_buf[KEY_BUFF_CNT];
xdata		byte		key_cnt;
xdata		byte		key_buf_old[KEY_BUFF_CNT];
xdata		byte		key_changed;
xdata		byte		IrKeyBuf;
xdata		byte		backup_IrKeyBuf;
//xdata		byte		gl_Toggle2;
xdata		byte		gl_Toggle[SRC_CNT];
xdata		byte		keyholdcnt;
xdata		byte		power_toggle;
xdata		set_number_t		setting_no;
xdata		byte		reg_key_max;
xdata		byte		reg_key_buf[KEY_BUFF_CNT];
xdata		byte		t1_int;
xdata		WORD		wTimer;
xdata		WORD		Timer0_65ms16;
xdata		WORD		Timer0_65ms16_2;
xdata		WORD		Timer0_keyin_chk;
#if (USE_AVSHIFT_MODE == TRUE)
xdata		WORD		Timer0_avShiftMode;
#endif
xdata		byte		Timer1_1sec08;
xdata		byte		ir_ok;
xdata		byte		setting_ok;
xdata		WORD*		uptr_Timer;									// move to absolute address
xdata		WORD*		uptr_TimerEnd;

#if (USE_IR_LEARN == TRUE)
xdata		WORD 		nPosNib;                   // Pattern을 가리키는 index(Nibble단위)
xdata		WORD 		nWidBit;                   // 현재의 Data크기.
xdata		WORD 		PulseCnt;
xdata		WORD 		nPosBit;                   // Data을 가리키는 index(bit단위)
xdata		WORD 		wCurVal;
xdata		WORD 		nPatCnt;                   // Pattern의 크기.
xdata		byte 		g_temp;
xdata		byte 		bNibble;
xdata		byte 		return_int8u;
xdata		WORD 		return_int16u;
xdata		byte		LearnKeyNo;
#endif

xdata		set_number_t 		refNo_inAuto;
xdata		byte 		refDev_inAuto;
xdata		byte 		direction_inAuto;
xdata		byte 		end1;
xdata		byte 		end2;
xdata		byte 		repeat;
xdata		byte* 	pStream;
xdata		byte		prev_direction;
xdata		set_number_t		backup_settingNo;
xdata		byte		direction_G;
xdata		byte		learn_type;
xdata		byte		RW;
xdata		byte		led_force;
#if (USE_SPECIAL_0FEH_5 == TRUE)
xdata		byte		Three_low_keyoff;
#endif
//xdata		byte		TV2_SetState;							// 3-digit 또는 auto-search 중 set_no.set_member.TV2_SetState 값이 
																					// 변경될 수 있다. 만약, 저장하지 않고 종료할 경우 설정모드 진입시의
																					// set_no.set_member.TV2_SetState 값으로 돌려놓기 위해서 사용함.
#if (USE_BATT_MONITOR == TRUE)
xdata		byte		keyCnt_inLow;
xdata		byte		lvi_check_key;
#endif
xdata		byte		twokey_result;

xdata		byte		FactoryEntryCnt;
xdata		byte		bTestModeEnable;
xdata		byte		ContinueDoubleKey;
#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
xdata		byte		StbNo_AutoSearch;
#endif
#if (USE_AVSHIFT_MODE == TRUE)
xdata		byte		avShiftMode_active;
#endif
#if (USE_BATCH_SS_LG == TRUE)
xdata		byte		batch_mode_keybuf;
xdata		set_number_t		batch_mode_setting_no;
#endif

/////////////////////////////////////////////////////////////////////////////////////
xdata	byte	RC5_Toggle;
xdata	byte	outframeNo;
/////////////////////////////////////////////////////////////////////////////////////


idata 		TYPE_CODE_SPECIALF4F8*	pramCode;

#if	(USE_ADDRESS_32BIT == TRUE)
idata		ULONG*	pWordptr;
idata		ULONG*	cpX_keymap;
#else
idata	WORD*			pWordptr;
idata	WORD*			cpX_keymap;
#endif

idata		WORD*		bidxEndWord1;
idata		WORD*		bidxEndWord2;
idata		WORD*		bidxRepeatWord;
//idata		ULONG*		pulPtr;
idata		byte*		pTemp;
idata		byte*		pCodeLast;
idata		byte*		cpX_keymap2;
idata		byte*		pCodeaddr_1;
idata		WORD*		npWaddr_1;
//idata		WORD* 		First_Start;
idata		WORD* 		First_End;
idata		WORD* 		Repeat_Start;
idata		WORD* 		Repeat_End;
idata		WORD		pflashCode;
//idata		byte* 		pExtRam;
//idata		WORD		temp;
idata		WORD		TransferSize;
//idata		byte		bodIntFlag;
idata		byte		f4_cnt;
//idata		byte		bSingleIndex;		  //DATA 영역에서 단펄스 인덱스

#if (USE_IR_LEARN == TRUE)
idata		WORD* 		ppdata;
idata		WORD*		pStream16u;
idata		byte		CarrierIdx;
idata		byte		Ready_Low_Capture;
idata		byte		NoSinalCnt;
idata		WORD		ipc_cnt;
idata		byte		bT3OverFlow;			// Timer 3 over flow flag
idata		int32u		CntFor2Sec;
idata		int32u		wDutyCycle;
#endif

//------------------------------------------------------------------------
idata		WORD		eeFAILAddress;
idata		byte		eeFAIL;
idata		byte		eeIntFlag;

xdata		int16u 		crt_int_counter;
xdata		WORD		Envelop_time;
xdata		WORD		Envelop_counter;
xdata		WORD		Envelop_time_max;
xdata		WORD		Envelop_counter_max;
xdata		WORD		carrier_time;
xdata		WORD		carrier_time_us;
xdata		WORD		carrier_time_ns;
xdata		BYTE		Envelop_create_flag;
xdata		WORD		wTimer_Max;
xdata 		BYTE		Before_CRT_State;
xdata		BYTE		reload_time;
xdata		BYTE		before_reload_state;
xdata		BYTE 		Before_carrier;
xdata		BYTE		non_special_flash_start;
xdata		BYTE		F4_i;
xdata		BYTE		F4_carrier;
xdata		BYTE		continuous_reload;






#endif	//__RAM_C__

