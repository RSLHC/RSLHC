
#ifndef	__RAM_H__
#define	__RAM_H__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include	"type_def.h"
#include	"define.h"

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/
extern  xdata 	TYPE_SETNO 		set_no;
extern  xdata 	TYPE_FORMAT		ramFormat;
extern  xdata 	TYPE_CODE		ramCode;
extern  xdata 	IR_STREAM_ST	ir_stream;
//extern    xdata 	Input_Carrier	Envelop;
extern  xdata 	IR_ZIP_ST			ir_zip;

extern  xdata		byte		chksum;
extern  xdata		byte		curr_device;
extern  xdata		byte		curr_source;
extern  xdata		byte		backup_source;
extern  xdata		byte		curr_state;
extern  xdata		byte		setting_device;
extern  xdata		byte		setting_source;
extern  xdata		byte		backup_key;
extern  xdata		byte		key_buf[KEY_BUFF_CNT];
extern  xdata		byte		key_cnt;
extern  xdata		byte		key_buf_old[KEY_BUFF_CNT];
extern  xdata		byte		key_changed;
extern  xdata		byte		IrKeyBuf;
extern  xdata		byte		backup_IrKeyBuf;
//extern    xdata		byte		gl_Toggle2;
extern  xdata		byte		gl_Toggle[SRC_CNT];
extern  xdata		byte		keyholdcnt;
extern  xdata		byte		power_toggle;
extern  xdata		set_number_t		setting_no;
extern  xdata		byte		reg_key_max;
extern  xdata		byte		reg_key_buf[KEY_BUFF_CNT];
extern  xdata		byte		t1_int;
extern  xdata		WORD		wTimer;
extern  xdata		WORD		Timer0_65ms16;
extern  xdata		WORD		Timer0_65ms16_2;
extern  xdata		WORD		Timer0_keyin_chk;
#if (USE_AVSHIFT_MODE == TRUE)
extern  xdata		WORD		Timer0_avShiftMode;
#endif
extern  xdata		byte		Timer1_1sec08;
extern  xdata		byte		ir_ok;
extern  xdata		byte		setting_ok;
extern  xdata		WORD*		uptr_Timer;									// move to absolute address
extern  xdata		WORD*		uptr_TimerEnd;

#if (USE_IR_LEARN == TRUE)
extern  xdata		WORD 		nPosNib;                   // Pattern을 가리키는 index(Nibble단위)
extern  xdata		WORD 		nWidBit;                   // 현재의 Data크기.
extern  xdata		WORD 		PulseCnt;
extern  xdata		WORD 		nPosBit;                   // Data을 가리키는 index(bit단위)
extern  xdata		WORD 		wCurVal;
extern  xdata		WORD 		nPatCnt;                   // Pattern의 크기.
extern  xdata		byte 		g_temp;
extern  xdata		byte 		bNibble;
extern  xdata		byte 		return_int8u;
extern  xdata		WORD 		return_int16u;
extern  xdata		byte		LearnKeyNo;
#endif

extern  xdata		set_number_t 		refNo_inAuto;
extern  xdata		byte 		refDev_inAuto;
extern  xdata		byte 		direction_inAuto;
extern  xdata		byte 		end1;
extern  xdata		byte 		end2;
extern  xdata		byte 		repeat;
extern  xdata		byte* 	pStream;
extern  xdata		byte		prev_direction;
extern  xdata		set_number_t		backup_settingNo;
extern  xdata		byte		direction_G;
extern  xdata		byte		learn_type;
extern  xdata		byte		RW;
extern  xdata		byte		led_force;
#if (USE_SPECIAL_0FEH_5 == TRUE)
extern xdata		byte		Three_low_keyoff;
#endif
//extern    xdata		byte		TV2_SetState;							// 3-digit �Ǵ� auto-search �� set_no.set_member.TV2_SetState ���� 
																					// ����� �� �ִ�. ����, �������� �ʰ� ������ ��� ������� ���Խ���
																					// set_no.set_member.TV2_SetState ������ �������� ���ؼ� �����.
#if (USE_BATT_MONITOR == TRUE)
extern  xdata		byte		keyCnt_inLow;
extern  xdata		byte		lvi_check_key;
#endif
extern  xdata		byte		twokey_result;

extern  xdata		byte		FactoryEntryCnt;
extern  xdata		byte		bTestModeEnable;
extern  xdata		byte		ContinueDoubleKey;
#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
extern  xdata		byte		StbNo_AutoSearch;
#endif
#if (USE_AVSHIFT_MODE == TRUE)
extern  xdata		byte		avShiftMode_active;
#endif
#if (USE_BATCH_SS_LG == TRUE)
extern  xdata		byte		batch_mode_keybuf;
extern  xdata		set_number_t		batch_mode_setting_no;
#endif

/////////////////////////////////////////////////////////////////////////////////////
extern  xdata	byte	RC5_Toggle;
extern  xdata	byte	outframeNo;
/////////////////////////////////////////////////////////////////////////////////////


extern  idata 		TYPE_CODE_SPECIALF4F8*	pramCode;

#if	(USE_ADDRESS_32BIT == TRUE)
extern  idata		ULONG*	pWordptr;
extern  idata		ULONG*	cpX_keymap;
#else
extern  idata	WORD*			pWordptr;
extern  idata	WORD*			cpX_keymap;
#endif

extern  idata		WORD*		bidxEndWord1;
extern  idata		WORD*		bidxEndWord2;
extern  idata		WORD*		bidxRepeatWord;
//extern    idata		ULONG*		pulPtr;
extern  idata		byte*		pTemp;
extern  idata		byte*		pCodeLast;
extern  idata		byte*		cpX_keymap2;
extern  idata		byte*		pCodeaddr_1;
extern  idata		WORD*		npWaddr_1;
//extern    idata		WORD* 		First_Start;
extern  idata		WORD* 		First_End;
extern  idata		WORD* 		Repeat_Start;
extern  idata		WORD* 		Repeat_End;
extern    idata		WORD		pflashCode;
//extern    idata		byte* 		pExtRam;
//extern    idata		WORD		temp;
extern  idata		WORD		TransferSize;
//extern    idata		byte		bodIntFlag;
extern  idata		byte		f4_cnt;
//extern    idata		byte		bSingleIndex;		  //DATA 영역에서 단펄스 인덱스

#if (USE_IR_LEARN == TRUE)
extern  idata		WORD* 		ppdata;
extern  idata		WORD*		pStream16u;
extern  idata		byte		CarrierIdx;
extern  idata		byte		Ready_Low_Capture;
extern  idata		byte		NoSinalCnt;
extern  idata		WORD		ipc_cnt;
extern  idata		byte		bT3OverFlow;			// Timer 3 over flow flag
extern  idata		int32u		CntFor2Sec;
extern  idata		int32u		wDutyCycle;
#endif

//------------------------------------------------------------------------
extern  idata		WORD		eeFAILAddress;
extern  idata		byte		eeFAIL;
extern  idata		byte		eeIntFlag;

extern  xdata		int16u 		crt_int_counter;
extern  xdata		WORD		Envelop_time;
extern  xdata		WORD		Envelop_counter;
extern  xdata		WORD		carrier_time;
extern 	xdata		WORD		Envelop_time_max;
extern 	xdata		WORD		Envelop_counter_max;
extern  xdata		WORD		carrier_time_us;
extern  xdata		WORD		carrier_time_ns;
extern  xdata		BYTE		Envelop_create_flag;
extern  xdata		WORD		wTimer_Max;
extern	xdata 		BYTE		Before_CRT_State;
extern	xdata 		BYTE		reload_timer_flag;
extern 	xdata		BYTE		reload_time;
extern	xdata		BYTE		before_reload_state;
extern	xdata		BYTE 		Before_carrier;
extern	xdata		BYTE		non_special_flash_start;
extern	xdata		BYTE		F4_i;
extern	xdata		BYTE		F4_carrier;
extern	xdata		BYTE		continuous_reload;

#endif	//__RAM_H__
