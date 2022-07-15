
#ifndef __FUNCTION_KEY_C__
#define __FUNCTION_KEY_C__
/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include    <INTRINS.H>
#include    "SC51F33XX.h"
#include 	"type_def.h"
#include	"define.h"
#include	"rom.h"
#include	"ram.h"
#include  	"main.h"
#include	"common.h"
#include	"function_key.h"
#include	"hal.h"
#include	"flash.h"
#if (USE_IR_LEARN == TRUE)
#include	"learn.h"
#endif

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/
void STB_IRoutCommon(void)
{
	curr_source=SRC_STB;
	//curr_device=DEV_STB;
	//setting_no=1;
	curr_device=set_no.set_member.set_dev.stb_src;
	setting_no=set_no.set_member.set_no.stb_src;
}

void STB_SettingCommon(void)
{
	setting_source=SRC_STB;
	curr_source=setting_source;
	//backup_source=curr_source;
}

#if (USE_TEST_MODE == TRUE)
void TestMode_IRoutCommon(void)
{
	curr_source=SRC_STB;
	curr_device=DEV_TESTMODE;
	setting_no=1;
}
#endif

void UR_SettingCommon(int8u set_source)
{
	setting_source=set_source;
	curr_source=setting_source;
	//backup_source=curr_source;
}	

#if (USE_LOW_BATT_IROUT == TRUE)
void Low_Voltage_CodeOut(void)
{
	STB_IRoutCommon();
	IrKeyBuf=KEY_LOWBATT;
	backup_IrKeyBuf=0xff;
	irout_n(1);
	pulse_stop();
}
#endif

void PulseOut_nFrame(int8u i)
{
	outframeNo=i;
	backup_key=0xff;

	my_sub();

	if(ir_ok)
		while(keyholdcnt<outframeNo){}

	pulse_stop();
	device_led_off(IR_SENDING_LED);
}

void PulseOut_Above_nFrame(int8u i)
{
	backup_key=0xff;

	my_sub();

	if(ir_ok)
	{
		while(keyholdcnt<i-1){}
		backup_key=key_buf[0];
		while(1)
		{
			getkey(&key_buf[0]);
			if(key_cnt==1)
			{
				if(backup_key!=key_buf[0])
					break;
				else
					continue;
			}
			else
				break;
		}
		if(ir_ok)
		{
			t1_int=1;
			while(t1_int!=2){}
		}

		pulse_stop();
		device_led_off(IR_SENDING_LED);
	}
	else
	{
		pulse_stop();
		device_led_off(IR_SENDING_LED);
	}
}

#if (USE_BATCH_SS_LG == TRUE)
BOOL batch_SS_LG_Common(int8u device, set_number_t set_no, int8u key)
{
	if((device == DEFAULT_UR_DEVICE)&&(set_no == DEFAULT_UR_SET_NO))
	{
		curr_source=SRC_TV;
		curr_device=DEV_TV;
		setting_no=STANDARD_LG_TV;
		batch_mode_setting_no=setting_no;
		batch_mode_keybuf = key;
		IrKeyBuf = key;
		PulseOut_nFrame(1);
#if (USE_AVSHIFT_MODE == TRUE)
		if(!avShiftMode_active)
#endif
		device_led_on(IR_SENDING_LED);

	//	t0_wait(1);
		Delayby10us(3860);

		curr_source=SRC_TV;
		curr_device=DEV_TV;
		setting_no=STANDARD_SAMSUNG_TV;
		batch_mode_setting_no=setting_no;
		batch_mode_keybuf = key;
		IrKeyBuf = key;
		PulseOut_nFrame(1);

		setting_no=DEFAULT_UR_VIRTUAL_NO;
		batch_mode_setting_no=0;
		return TRUE;
	}
	return FALSE;
}
#endif

//--------------------------------------------------------------
BOOL SetModeIn_TimeCheck(int16u time)
{
	TickTimer_65msStart(time);

	while(Timer0_65ms16)
	{
		getkey(&key_buf[0]);

		
		if(!Timer0_65ms16)
		{
			//Setting_Mode();
			return	TRUE;
		}
		if(key_cnt==2)
			continue;
		else
		{
			backup_key=0xff;
			break;
		}
	}//end of while (2key in)
	return	FALSE;
}

#if (USE_QUICKLY_KEY == TRUE)
void QuicklyKey_Delay(void)
{
	int16u bi;

	for(bi=0;bi<5000;bi++) wait_10us();
}
#endif


#if (USE_TEST_MODE == TRUE)
void NecDataQuicklyLoad_FillPattern(int8u codedata)
{
	int8u	i, idxKey;
	const	int8u				LOGIC0=21;
	const	int8u				LOGIC1=63;
	const	int8u				LOGIC0_LOW=21;

	idxKey=codedata;

	for(i=34; i<50; i++)												//data
	{
		if(idxKey&0x01)														//logic 1
		{
			ir_stream.Tunion.Tstruct.pulse.BitStream[i++]=(0x8000+(LOGIC0));	//high
			ir_stream.Tunion.Tstruct.pulse.BitStream[i]=(LOGIC1);						//low
		}
		else																			//logic 0
		{
			ir_stream.Tunion.Tstruct.pulse.BitStream[i++]=(0x8000+(LOGIC0));	//high
			ir_stream.Tunion.Tstruct.pulse.BitStream[i]=(LOGIC0_LOW);						//low
		}
		idxKey>>=1;
	}

	codedata>>=4;
	for(i=58; i<66; i++)												//data bar high nibble.
	{
		if(codedata&0x01)															//logic 0
		{
			ir_stream.Tunion.Tstruct.pulse.BitStream[i++]=(0x8000+(LOGIC0));	//high
			ir_stream.Tunion.Tstruct.pulse.BitStream[i]=(LOGIC0_LOW);						//low
		}
		else																			//logic 1
		{
			ir_stream.Tunion.Tstruct.pulse.BitStream[i++]=(0x8000+(LOGIC0));	//high
			ir_stream.Tunion.Tstruct.pulse.BitStream[i]=(LOGIC1);						//low
		}
		codedata>>=1;
	}
}

void Factory_Version_Reporting(void)
{
	my_newkeyincond();
	wBitStream_Clear();

	TestMode_IRoutCommon();
	IrKeyBuf=KEY_STBPOWER;
	Send_UR_Ready();

	ir_ok=TRUE;
	NecDataQuicklyLoad_FillPattern(SW_VERSION[0]);
    //device_led_on(IR_SENDING_LED);
	backup_IrKeyBuf=0xff;
	outframeNo = 3;
	send_ur();

	if(ir_ok)
		while(keyholdcnt<outframeNo){}

	pulse_stop();
}

void rsApp_EnterTestModeFunction(void)
{
	Factory_Version_Reporting();
	backup_key=0xff;

	device_Led_blink(IR_SENDING_LED, 3, 2, 2);
}

void rsApp_ExitTestModeFunction(void)
{
	if(ir_ok)
	{
		t1_int=1;
		while(t1_int!=2){}
	}
	pulse_stop();
	device_led_off(IR_SENDING_LED);
	FactoryDefault();

	ReturnNormal();

	TestMode_IRoutCommon();
	IrKeyBuf = FACTORY_TEST_KEY[4];
	backup_IrKeyBuf=0xff;
	irout_n(1);
	pulse_stop();
	bTestModeEnable=0;
	FactoryEntryCnt=0;
	STB_IRoutCommon();
	key_buf[0] = KEY_NULL;

	t0_wait(10);
	SW_SystemReset();
}

void Factory_Test_Process(void)
{
	static int8u code* 	pKey=FACTORY_TEST_KEY;

	if(bTestModeEnable)
	{
		TestMode_IRoutCommon();

		backup_key=0xff;

		IrKeyBuf = key_buf[0];
		//backup_IrKeyBuf=0xff;

		irout_n(0xff);

		//backup_key=key_buf[0];
		backup_key=0xff;

		if(*(pKey+4)==key_buf[0]) 
		{
			rsApp_ExitTestModeFunction();
		}
		return;
	}

	if(FactoryEntryCnt>4)
		return;
	
	if(*(pKey+FactoryEntryCnt)==key_buf[0])
		FactoryEntryCnt++;
	else if(*(pKey+FactoryEntryCnt-1)==key_buf[0]){}
	else
		FactoryEntryCnt=0xff;

	if(FactoryEntryCnt==4)
	{
		rsApp_EnterTestModeFunction();
		bTestModeEnable = 1;
	}

}
#endif


void functionKeyProcess(void)
{
	if(curr_state == NORMAL_MODE)
	{
		curr_source = SRC_STB;
		backup_source = SRC_STB;

#if (USE_AVSHIFT_MODE == TRUE)
		if(!isAvShiftKeyIn)
			avShiftMode_active = 0;
#endif
	}
#if (USE_AVSHIFT_MODE == TRUE)
	else
	{
		avShiftMode_active = 0;
	}
#endif

	curr_device=*(set_no.set_reg+curr_source+(SRC_CNT*INC_PTR_SIZE));

	(*pfunc_ptr[key_buf[0]]) ();
}

//----------------------------------------;
// remote program start.
//----------------------------------------;
void rsAppMain(void)
{
#if (USE_BATT_MONITOR == TRUE) || (USE_LOW_BATT_IROUT == TRUE) || (USE_LOW_BATT_LED == TRUE)
	byte		temp_backup_key;
#endif

	while(1)
	{
		getkey(&key_buf[0]);

		// normal state ir out.
		if(curr_state == NORMAL_MODE)
		{
			if(!key_cnt)
			{
#if (USE_SPECIAL_0FEH_5 == TRUE)
				if((Three_low_keyoff)&&(ramFormat.strMember.outorder[0]==0xfe)&&(ramFormat.strMember.outorder[1]==5))	//Three low option0
				{
					pulse_stop();
					key_buf[0]=backup_key;
					backup_key=0xff;
					irout_n(1);
					Three_low_keyoff=0;
				}
#endif

#if (USE_STUCKKEY_TIMER == TRUE)
				StuckKeyTimer_Stop;
#endif
				curr_source=backup_source;
				temp_backup_key = backup_key;
				backup_key=0xff;
				ContinueDoubleKey=0;

				if(ir_ok)
				{
					t1_int=1;
					while(t1_int!=2){}
					pulse_stop();
				}
				//device_led_off(IR_SENDING_LED);
#if (USE_QUICKLY_KEY == TRUE)
				QuicklyKey_Delay();
#endif

#if (USE_AVSHIFT_MODE == TRUE)
				if(avShiftMode_active)
				{
					device_led_on(IR_SENDING_LED);
					continue;
				}
#endif

				device_led_off(IR_SENDING_LED);

				if(getkey_simple())
					continue;

#if (USE_BATT_MONITOR == TRUE)
				Low_Voltage_Check(temp_backup_key);
#endif

#if (USE_LOW_BATT_IROUT == TRUE)
				if((!keyCnt_inLow)&&(lvi_check_key == 2))
				{
					if(t0_wait_keyin_chk(2))
					{
						keyCnt_inLow=LVI_CHECK_CNT;
						continue;
					}
					else
						Low_Voltage_CodeOut();
					
				}
#endif
#if (USE_LOW_BATT_LED == TRUE)
				else if((!keyCnt_inLow)&&(lvi_check_key == 1))
				{
					if(device_Led_blink_key_chk(LOW_BATTERY_LED, 10, 5, 5))
					{
						keyCnt_inLow=LVI_CHECK_CNT;
						device_led_off(LOW_BATTERY_LED);
						continue;
					}
				}
#endif
				keyCnt_inLow=LVI_CHECK_CNT;

				stop_define();
				ExtInt_init();
				Keyscan_Interrupt_On();

				Stop_Mode();
				
				Keyscan_Interrupt_Off();
				continue;
			}
			else
			{
				if(key_cnt==1)
				{		
					if((ContinueDoubleKey)&&(backup_key!=0xff))
					{
						if(isNavigationKeyIn)
							key_buf[0]=backup_key;
					}
					
					if(backup_key==0xff)
					{
						backup_source=curr_source;
#if (USE_TEST_MODE == TRUE)
						Factory_Test_Process();
						if(bTestModeEnable)
							continue;
#endif
					}
					else
					{
						if(key_buf[0]!=backup_key)
						{
							if(ir_ok)
							{
								t1_int=1;
								while(t1_int!=2){}
								pulse_stop();
#if (USE_QUICKLY_KEY == TRUE)
								QuicklyKey_Delay();
#endif	
							}
						}
					}

					functionKeyProcess();
					continue;
				}//end of 1key				
				else if(key_cnt==2) //2key compare & setting in.
				{
					if(Check_Key_2keyNoStop(key_buf,key_buf+1))
					{
						if(ir_ok)
						{
							t1_int=1;
							while(t1_int!=2){}
						}
						curr_source=backup_source;
						continue;
					}

					pulse_stop();
					device_led_off(IR_SENDING_LED);
					curr_source=backup_source;
					backup_key=0xff;
#if (USE_QUICKLY_KEY == TRUE)
					QuicklyKey_Delay();
#endif

					Check_Key_InSetmode(key_buf,key_buf+1);

					switch(twokey_result)
					{
						case	COMBO_KEYS_UR_SETUP:									//TVPOWER & OK
							if(SetModeIn_TimeCheck(COMBOKEY_TIMEOUT_S))
							{
								UR_SettingCommon(SRC_TV);
								curr_state=SET_MODE_ENTRY;
								Setting_Mode();
								SetMode_StartTimeOut(SETUP_TIMEOUT_S);
								device_Led_InSetUp(SET_ENTRY, SETTING_MODE_LED);
								wait_getkey_simple();
							}
							break;

						case	COMBO_KEYS_STB_SETUP:									//STBPOWER & OK
							if(SetModeIn_TimeCheck(COMBOKEY_TIMEOUT_S))
							{
								STB_SettingCommon();
								curr_state=SET_MODE_ENTRY;
								Setting_Mode();
								SetMode_StartTimeOut(SETUP_TIMEOUT_S);
								device_Led_InSetUp(SET_ENTRY, SETTING_MODE_LED);
								wait_getkey_simple();
							}
							break;

						case	COMBO_KEYS_FACTORY_DEFAULT:								//STBPOWER & RED
							if(SetModeIn_TimeCheck(COMBOKEY_TIMEOUT2_S))
							{
								FactoryDefault();
								//device_Led_InSetUp(SET_OK, SETTING_MODE_LED);
								device_Led_blink(SETTING_MODE_LED, 3, 4, 4);
								device_led_off(SETTING_MODE_LED);
								wait_getkey_simple();
							}
							break;

						default:
							backup_key=0xff;
							break;

					}
				}//end of 2key check

				else if(key_cnt==3)
				{
					if(Check_Key_3keyNoStop(key_buf,key_buf+1,key_buf+2))
					{
						if(ir_ok)
						{
							t1_int=1;
							while(t1_int!=2){}
						}
						curr_source=backup_source;
						continue;
					}
					pulse_stop();
					device_led_off(IR_SENDING_LED);
					curr_source=backup_source;
					backup_key=0xff;
				}

				else if(key_cnt==4)
				{
					if(Check_Key_4keyNoStop(key_buf,key_buf+1,key_buf+2,key_buf+3))
					{
						if(ir_ok)
						{
							t1_int=1;
							while(t1_int!=2){}
						}
						curr_source=backup_source;
						continue;
					}
					pulse_stop();
					device_led_off(IR_SENDING_LED);
					curr_source=backup_source;
					backup_key=0xff;
				}

#if 0
				else if(key_cnt==5)
				{
					if(Check_Key_5keyNoStop(key_buf,key_buf+1,key_buf+2,key_buf+3,key_buf+4))
					{
						if(ir_ok)
						{
							t1_int=1;
							while(t1_int!=2){}
						}
						curr_source=backup_source;
						continue;
					}
					pulse_stop();
					device_led_off(IR_SENDING_LED);
					curr_source=backup_source;
					backup_key=0xff;
				}
#endif
				else
				{
					pulse_stop();
					device_led_off(IR_SENDING_LED);
					curr_source=backup_source;
					backup_key=0xff;
				}
			}//end of key in
		}//end of normal mode
		//set mode
		else
		{
			if(Timer0_65ms16_2)
			{
				if(key_cnt==1)
				{
					functionKeyProcess();
					continue;
				}
			}
			else
			{
				ReturnNormal();
			}
		}//end of set mode
	}// end of while
}

#if 1
void chksum_cal(void)
{
	byte	i, sum = 0;
	byte* pStartSetNo = set_no.set_reg;

	for(i=0; i<(BACKUP_SIZE-2); i++)		//BACKUP_SIZE-ChksumBYTE
	{
		sum += *pStartSetNo++;
	}

	chksum = sum;
}
#endif

//---------------------------
// key null
//---------------------------
void Key_NULL(void)
{
	//	__no_operation();
}

//---------------------------
// ir out n frame..
// input -> i=0 -> max 3frame out.
//			i=2 -> continous out.
//			i=3 ~~ n -> n frame out.
//---------------------------
void irout_n(unsigned char i)
{
	if(backup_IrKeyBuf == IrKeyBuf)
		return;

	outframeNo=i;

	if(i==1)								//frame out. -> C&M
	{
		my_sub();
#if (USE_IR_LEARN == TRUE)
#if (IRZIP_CARR_2BYTES == TRUE)
		if(!ir_zip.Tunion.Tstruct.Carrier)
#else
		if(!ir_zip.Tunion.data1[0])
#endif
#endif
		{
			if(ir_ok)
				while(keyholdcnt<outframeNo){}
			pulse_stop();
			device_led_off(IR_SENDING_LED);
#if (USE_QUICKLY_KEY == TRUE)
			QuicklyKey_Delay();
#endif
			//getkey_wait();
		}
	}
	else if(i==0xff)
		my_sub();

	else if(i>=2)							//n frame out. -> auto search.
	{
		my_sub();
		if(ir_ok)
			while(keyholdcnt<outframeNo){}
		pulse_stop();
		//getkey_wait();
	}
}

void ReturnNormal(void)
{
	curr_state=NORMAL_MODE;
	device_led_off(SETTING_MODE_LED);
	wait_getkey_simple();
	Timer0_65ms16_2=0;
}

void IgnoreKeyin(void)
{
	Timer0_65ms16_2_Update(SETUP_TIMEOUT_S);
	wait_getkey_simple();
}

//------------------------------------
// complete setting mdoe (digit,auto,punch,2'nd)
//------------------------------------
void Complete_SetMode(void)
{
	device_led_off(SETTING_MODE_LED);

#if (USE_IR_LEARN == TRUE)
	if(curr_state == SET_MODE_DIGIT || curr_state == SET_MODE_AUTO)
	{
		Erase_LearnDevice();
	}
#endif

	Save_Information_All();

	device_Led_InSetUp(SET_OK, SETTING_MODE_LED);
	ReturnNormal();
	wait_getkey_simple();
}

void Save_TvPower(void)
{
	set_no.set_member.set_no.tv_src=setting_no;
	set_no.set_member.set_dev.tv_src=setting_device;
}

void Complete_Setup_TvPower(void)
{
	Save_TvPower();
}

#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
void Save_StbPower(void)
{
	set_no.set_member.set_no.stb_src=setting_no;
	set_no.set_member.set_dev.stb_src=setting_device;
}

void Complete_Setup_StbPower(void)
{
	Save_StbPower();
}
#endif

void Complete_UrSetup(void)
{
	switch(setting_source)
	{
#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
		case	SRC_STB:
			Complete_Setup_StbPower();
			break;
#endif
		case	SRC_TV:
			Complete_Setup_TvPower();
			break;

		default:
			Complete_Setup_TvPower();
			break;
	}

	backup_key=0xff;
	Complete_SetMode();
}

//------------------------------------
// check setting devcie
//------------------------------------
#if 0
void Check_SettingDevice(void)
{
	switch(setting_source)
	{
		case	SRC_TV:
#if (USE_2BYTE_SETTING_NO == FALSE)
			if(reg_key_buf[0]>=3)
			{
				setting_device=DEV_TV1;
				reg_key_buf[0]-=3;
			}
			else
#endif
				setting_device=DEV_TV;
			break;

		case	SRC_STB:
			setting_device=DEV_STB;
			break;

		default:
			setting_device=0xff;
			break;
	}
}
#endif

//------------------------------------
// digit keys
//------------------------------------
void	DigitKeys_Common_SetMode(int8u digit)
{
	if( (curr_state==SET_MODE_ENTRY) || (curr_state==SET_MODE_DIGIT) || (curr_state==SET_MODE_AUTO) )
	{
		curr_state=SET_MODE_DIGIT;
		setting_ok=FALSE;

		if(reg_key_max==2)
		{
			reg_key_buf[reg_key_max]=digit;

			//Check_SettingDevice();

		/* !!!important!!! - HardFault_Handler */
			if((setting_device==0xff)||(setting_device>MAX_DEVICE_NO))
			{
				device_Led_InSetUp(SET_NG, SETTING_MODE_LED);
				reg_key_max=0;
				reg_key_buf[0]=0;
				reg_key_buf[1]=0;
				reg_key_buf[2]=0;
				reg_key_buf[3]=0;

				wait_getkey_simple();
				return;
			}
		/* !!!important!!! - HardFault_Handler */

			curr_device=setting_device;

			if( Get1CodeFrom3Buf((BYTE*)&reg_key_buf[0]) )		//3-frame out.
			{
				backup_key=0xff;
				switch(curr_device)
				{
					case	DEV_TV:
					case	DEV_TV1:
						//key_buf[0]=KEY_TVPOWER;
						IrKeyBuf = KEY_TVPOWER;
						break;
#if (USE_SETUP_STB_IR_CUSTOM == TRUE)
					case	DEV_STB:
						//key_buf[0]=KEY_STBPOWER;
						IrKeyBuf = KEY_STBPOWER;
						break;
#endif
					default:
						break;
				}
				power_toggle++;

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
				device_Led_blink(SETTING_MODE_LED, 1, 0, 3);
			}
			else
				device_Led_InSetUp(SET_NG, SETTING_MODE_LED);

			reg_key_max=0;
			reg_key_buf[0]=0;
			reg_key_buf[1]=0;
			reg_key_buf[2]=0;
			reg_key_buf[3]=0;

			wait_getkey_simple();
			return;
		}
		else
		{
			reg_key_buf[reg_key_max]=digit;
		}
		Timer0_65ms16_2_Update(SETUP_TIMEOUT_S);

		reg_key_max++;
		device_Led_blink(SETTING_MODE_LED, 1, 0, 3);
		wait_getkey_simple();
	}
	else
		ReturnNormal();
}

//----------------------------------------
// holding key in after ir out.
//----------------------------------------
void getSameKey_wait(void)
{
	backup_key=key_buf[0];
	while(1)
	{
		getkey(&key_buf[0]);
		if(key_cnt==1)
		{
			if(backup_key!=key_buf[0])
				break;
			else
				continue;
		}
		else
			break;
	}
}

int8u Check_Key_2keyNoStop(int8u* key1, int8u* key2)
{
	switch(*key1)
	{
		case KEY_NAVI_UP:
		case KEY_NAVI_DOWN:
		case KEY_NAVI_LEFT:
		case KEY_NAVI_RIGHT:
		//case KEY_OK:
			switch(*key2)
			{
				case	KEY_NAVI_UP:
				case	KEY_NAVI_DOWN:
				case	KEY_NAVI_LEFT:
				case	KEY_NAVI_RIGHT:
				//case	KEY_OK:
					twokey_result=0;
					break;
				default:
					twokey_result=0xff;
					break;
			}
			break;

		default:
			twokey_result=0xff;
			break;

	}

	if(twokey_result)
	{
		ContinueDoubleKey=0;
		return	FALSE;
	}
	else
	{
		ContinueDoubleKey=1;
		return	TRUE;
	}
}

int8u Check_Key_3keyNoStop(int8u* key1, int8u* key2, int8u* key3)
{
	switch(*key1)
	{
		case KEY_NAVI_UP:
		case KEY_NAVI_DOWN:
		case KEY_NAVI_LEFT:
		case KEY_NAVI_RIGHT:
		//case KEY_OK:
			switch(*key2)
			{
				case	KEY_NAVI_UP:
				case	KEY_NAVI_DOWN:
				case	KEY_NAVI_LEFT:
				case	KEY_NAVI_RIGHT:
				//case	KEY_OK:
					switch(*key3)
					{
						case	KEY_NAVI_UP:
						case	KEY_NAVI_DOWN:
						case	KEY_NAVI_LEFT:
						case	KEY_NAVI_RIGHT:
						//case	KEY_OK:
							twokey_result=0;
							break;
						default:
							twokey_result=0xff;
							break;
					}
					break;
				default:
					twokey_result=0xff;
					break;
			}
			break;

		default:
			twokey_result=0xff;
			break;

	}

	if(twokey_result)
	{
		ContinueDoubleKey=0;
		return	FALSE;
	}
	else
	{
		ContinueDoubleKey=1;
		return	TRUE;
	}
}

int8u Check_Key_4keyNoStop(int8u* key1, int8u* key2, int8u* key3, int8u* key4)
{
	switch(*key1)
	{
		case KEY_NAVI_UP:
		case KEY_NAVI_DOWN:
		case KEY_NAVI_LEFT:
		case KEY_NAVI_RIGHT:
		//case KEY_OK:
			switch(*key2)
			{
				case	KEY_NAVI_UP:
				case	KEY_NAVI_DOWN:
				case	KEY_NAVI_LEFT:
				case	KEY_NAVI_RIGHT:
				//case 	KEY_OK:
					switch(*key3)
					{
						case	KEY_NAVI_UP:
						case	KEY_NAVI_DOWN:
						case	KEY_NAVI_LEFT:
						case	KEY_NAVI_RIGHT:
						//case 	KEY_OK:
							switch(*key4)
							{
								case	KEY_NAVI_UP:
								case	KEY_NAVI_DOWN:
								case	KEY_NAVI_LEFT:
								case	KEY_NAVI_RIGHT:
								//case 	KEY_OK:
									twokey_result=0;
									break;
								default:
									twokey_result=0xff;
									break;
							}
							break;
						default:
							twokey_result=0xff;
							break;
					}
					break;
				default:
					twokey_result=0xff;
					break;
			}
			break;

		default:
			twokey_result=0xff;
			break;

	}

	if(twokey_result)
	{
		ContinueDoubleKey=0;
		return	FALSE;
	}
	else
	{
		ContinueDoubleKey=1;
		return	TRUE;
	}
}

#if 0
int8u Check_Key_5keyNoStop(int8u* key1, int8u* key2, int8u* key3, int8u* key4, int8u* key5)
{
	switch(*key1)
	{
		case KEY_NAVI_UP:
		case KEY_NAVI_DOWN:
		case KEY_NAVI_LEFT:
		case KEY_NAVI_RIGHT:
		case KEY_OK:
			switch(*key2)
			{
				case	KEY_NAVI_UP:
				case	KEY_NAVI_DOWN:
				case	KEY_NAVI_LEFT:
				case	KEY_NAVI_RIGHT:
				case 	KEY_OK:
					switch(*key3)
					{
						case	KEY_NAVI_UP:
						case	KEY_NAVI_DOWN:
						case	KEY_NAVI_LEFT:
						case	KEY_NAVI_RIGHT:
						case 	KEY_OK:
							switch(*key4)
							{
								case	KEY_NAVI_UP:
								case	KEY_NAVI_DOWN:
								case	KEY_NAVI_LEFT:
								case	KEY_NAVI_RIGHT:
								case 	KEY_OK:
									switch(*key5)
									{
										case	KEY_NAVI_UP:
										case	KEY_NAVI_DOWN:
										case	KEY_NAVI_LEFT:
										case	KEY_NAVI_RIGHT:
										case 	KEY_OK:
										twokey_result=0;
										break;
										default:
											twokey_result=0xff;
											break;
									}
									break;
								default:
									twokey_result=0xff;
									break;
							}
							break;
						default:
							twokey_result=0xff;
							break;
					}
					break;
				default:
					twokey_result=0xff;
					break;
			}
			break;

		default:
			twokey_result=0xff;
			break;

	}

	if(twokey_result)
	{
		ContinueDoubleKey=0;
		return	FALSE;
	}
	else
	{
		ContinueDoubleKey=1;
		return	TRUE;
	}
}
#endif

void Other_Keys_Common(void)
{
	if(curr_state == NORMAL_MODE)
	{
		IrKeyBuf = key_buf[0];
		irout_n(0xff);
	}
	else if( (curr_state==SET_MODE_DIGIT) || (curr_state==SET_MODE_AUTO) )
	{
		if(setting_device != DEV_STB)
		{
			IgnoreKeyin();
			return;
		}

		switch(key_buf[0])
		{
			case	KEY_STBPOWER:
				curr_device = setting_device;
				IrKeyBuf = key_buf[0];
				backup_key=0xff;
				if(setting_ok)
				{
					irout_n(0xff);
				}
				getSameKey_wait();
				if(ir_ok)
				{
					t1_int=1;
					while(t1_int!=2){}
				}
				pulse_stop();
				backup_key=key_buf[0];
				reg_key_max=0;
				reg_key_buf[0]=0;
				reg_key_buf[1]=0;
				reg_key_buf[2]=0;
				reg_key_buf[3]=0;
				Timer0_65ms16_2_Update(SETUP_TIMEOUT_S);
				device_Led_blink(SETTING_MODE_LED, 1, 0, 3);
				break;
			case	KEY_EXIT:
				ReturnNormal();
				break;
			default:
				//ReturnNormal();
				backup_key=0xff;
				IgnoreKeyin();
				break;
		}
	}
	else
	{
		IgnoreKeyin();
	}
}

void Other_TV_Keys_Common(void)
{
	IrKeyBuf = key_buf[0];

	if(curr_state == NORMAL_MODE)
	{
		switch(key_buf[0])
		{
			case	KEY_TVPOWER:
			case	KEY_INPUT:
#if (USE_BATCH_SS_LG == TRUE)
				if(batch_SS_LG_Common(curr_device, set_no.set_member.set_no.tv_src, key_buf[0]))
				{
					getSameKey_wait();
#if (USE_QUICKLY_KEY == TRUE)
					QuicklyKey_Delay();
#endif
				}
				else
#endif
					irout_n(0xff);
				break;
			case	KEY_TVVOLUP:
			case	KEY_TVVOLDOWN:
#if (USE_BATCH_SS_LG == TRUE)
				if(batch_SS_LG_Common(curr_device, set_no.set_member.set_no.tv_src, key_buf[0]))
				{
					device_led_on(IR_SENDING_LED);

					//T0CR&=~0x01;
					//t0_wait(1);
					Delayby10us(3171);
				}
				else
#endif
					irout_n(0xff);
				break;
			default:
				irout_n(0xff);
				break;
		}
	}
	else if( (curr_state==SET_MODE_DIGIT) || (curr_state==SET_MODE_AUTO) )
	{
		if( !((setting_device == DEV_TV) || (setting_device == DEV_TV1)) )
		{
			IgnoreKeyin();
			return;
		}
		switch(key_buf[0])
		{
			case	KEY_TVPOWER:
				curr_device = setting_device;
				IrKeyBuf = key_buf[0];
				backup_key=0xff;
				if(setting_ok)
				{
#if (USE_BATCH_SS_LG == TRUE)
					if(!batch_SS_LG_Common(curr_device, setting_no, IrKeyBuf))
#endif
						irout_n(0xff);
				}
				getSameKey_wait();
				if(ir_ok)
				{
					t1_int=1;
					while(t1_int!=2){}
				}
				pulse_stop();
				backup_key=key_buf[0];
				reg_key_max=0;
				reg_key_buf[0]=0;
				reg_key_buf[1]=0;
				reg_key_buf[2]=0;
				reg_key_buf[3]=0;
				Timer0_65ms16_2_Update(SETUP_TIMEOUT_S);
				device_Led_blink(SETTING_MODE_LED, 1, 0, 3);
				break;
			case	KEY_EXIT:
				ReturnNormal();
				break;
			default:
				//ReturnNormal();
				backup_key=0xff;
				IgnoreKeyin();
				break;
		}
	}
	else
	{
		//ReturnNormal();
		IgnoreKeyin();
	}
}

void Key_Common_Digit(int8u digit)
{
	if(curr_state!=NORMAL_MODE)
	{
		DigitKeys_Common_SetMode(digit);
	}
	else
	{
		Other_Keys_Common();
	}
}

void  Key_0(void)
{
	Key_Common_Digit(0);
}

void  Key_1(void)
{
	Key_Common_Digit(1);
}

void  Key_2(void)
{
	Key_Common_Digit(2);
}

void  Key_3(void)
{
	Key_Common_Digit(3);
}

void  Key_4(void)
{
	Key_Common_Digit(4);
}

void  Key_5(void)
{
	Key_Common_Digit(5);
}

void  Key_6(void)
{
	Key_Common_Digit(6);
}

void  Key_7(void)
{
	Key_Common_Digit(7);
}

void  Key_8(void)
{
	Key_Common_Digit(8);
}

void  Key_9(void)
{
	Key_Common_Digit(9);
}


//------------------------------------
// auto_search keys
//------------------------------------
void auto_search_Common(void)
{
	if(curr_state==SET_MODE_ENTRY)
	{
		curr_state=SET_MODE_AUTO;

		if(key_buf[0]==KEY_CHUP)
			auto_search_1(1);
		else
			auto_search_1(0);
	}
	else if((curr_state==SET_MODE_AUTO)||(curr_state==SET_MODE_DIGIT))
	{
		curr_state=SET_MODE_AUTO;

		if(key_buf[0]==KEY_CHUP)
			auto_search_1(1);
		else
			auto_search_1(0);
	}
	else
	{
		Other_Keys_Common();
	}
}

void Key_CHUP(void)
{
	auto_search_Common();
}
void Key_CHDOWN(void)
{
	auto_search_Common();
}

void Key_SEARCH(void)
{
	if(curr_state==SET_MODE_ENTRY)
	{
		Code_Read_Out();
		ReturnNormal();
	}
	else
		Other_Keys_Common();
}

#if (USE_AVSHIFT_MODE == TRUE)
void avShiftMode_Keys_Common(void)
{
	if(curr_state==NORMAL_MODE)
	{
		if(avShiftMode_active)
		{
			if(set_no.set_member.set_no.tv_src)
			{
				curr_source=SRC_TV;
				setting_no=set_no.set_member.set_no.tv_src;
				curr_device=set_no.set_member.set_dev.tv_src;
				IrKeyBuf = key_buf[0];

				device_led_off(IR_SENDING_LED);

#if (USE_BATCH_SS_LG == TRUE)
				if(batch_SS_LG_Common(curr_device, setting_no, IrKeyBuf))
				{
					if(isNavigationKeyIn)
					{
						//device_led_on(IR_SENDING_LED);

						//T0CR&=~0x01;
						//t0_wait(1);
						Delayby10us(6700);
					}
					else
						getSameKey_wait();
				}
				else
#endif
					irout_n(0xff);

				Timer0_avShiftMode_Update(AVSHIFT_TIMEOUT_S);
			}
			else
				Other_Keys_Common();
		}
		else
			Other_Keys_Common();

	}
	else
		Other_Keys_Common();
}
#endif

void Key_OK(void)
{
	if((curr_state==SET_MODE_ENTRY)||(curr_state==SET_MODE_DIGIT)||(curr_state==SET_MODE_AUTO))
	{
		if(setting_ok)
			Complete_UrSetup();
		else
			ReturnNormal();
	}
	else
	{
#if (USE_AVSHIFT_MODE == TRUE)
		avShiftMode_Keys_Common();
		avShiftMode_active = 0;
#else
		Other_Keys_Common();
#endif
	}
}

void Key_ALLPOWER(void)
{
	if(curr_state==NORMAL_MODE)
	{
		if(backup_key!=0xff)
			return;

		power_toggle++;

		curr_source=SRC_TV;
		setting_no=set_no.set_member.set_no.tv_src;
		curr_device=set_no.set_member.set_dev.tv_src;

		IrKeyBuf=KEY_TVPOWER;
		if(Panasonic_TV_Check(curr_device, setting_no))
		{
			//key_buf[0]=KEY_TVPOWER;
			PulseOut_Above_nFrame(5);
			key_buf_old[0]=backup_key;
			key_buf[0]=backup_key;
			backup_key=0xff;
		}
		else
		{
#if (USE_BATCH_SS_LG == TRUE)
			if(!batch_SS_LG_Common(curr_device, setting_no, IrKeyBuf))
#endif
			{
				PulseOut_nFrame(3);
			}
		}

		//t0_wait(3);
		Delayby10us(18050);


		curr_source=SRC_STB;
		setting_no=set_no.set_member.set_no.stb_src;
		curr_device=set_no.set_member.set_dev.stb_src;

		//key_buf[0]=KEY_STBPOWER;
		IrKeyBuf=KEY_STBPOWER;
		PulseOut_nFrame(2);

		
#if (USE_QUICKLY_KEY == TRUE)
		QuicklyKey_Delay();
#endif
	}
	else
		Other_Keys_Common();
}

void Key_TVPOWER(void)
{
	if(backup_key!=key_buf[0])
		power_toggle++;

	if(curr_state == NORMAL_MODE)
	{
		curr_source=SRC_TV;
		curr_device=*(set_no.set_reg+curr_source+(SRC_CNT*INC_PTR_SIZE));
	}

	Other_TV_Keys_Common();
}

void Key_TVVOLUP(void)
{
	if(curr_state == NORMAL_MODE)
	{
		curr_source=SRC_TV;
		curr_device=*(set_no.set_reg+curr_source+(SRC_CNT*INC_PTR_SIZE));
	}

	Other_TV_Keys_Common();
}
void Key_TVVOLDOWN(void)
{
	if(curr_state == NORMAL_MODE)
	{
		curr_source=SRC_TV;
		curr_device=*(set_no.set_reg+curr_source+(SRC_CNT*INC_PTR_SIZE));
	}

	Other_TV_Keys_Common();
}
void Key_INPUT(void)
{
	if(curr_state == NORMAL_MODE)
	{
		curr_source=SRC_TV;
		curr_device=*(set_no.set_reg+curr_source+(SRC_CNT*INC_PTR_SIZE));
	}

	Other_TV_Keys_Common();

#if (USE_AVSHIFT_MODE == TRUE)
	if(avShiftMode_active)
	{
		Timer0_avShiftMode_Update(AVSHIFT_TIMEOUT_S);
	}
	else
	{
		avShiftMode_active = 1;
		avShiftMode_StartTimeOut(AVSHIFT_TIMEOUT_S);
		device_led_on(IR_SENDING_LED);
	}
#endif
}

void Key_MENU(void)
{
	Other_Keys_Common();
}

void Key_NAVI_UP(void)
{
#if (USE_AVSHIFT_MODE == TRUE)
	avShiftMode_Keys_Common();
#else
	Other_Keys_Common();
#endif
}
void Key_NAVI_DOWN(void)
{
#if (USE_AVSHIFT_MODE == TRUE)
	avShiftMode_Keys_Common();
#else
	Other_Keys_Common();
#endif
}
void Key_NAVI_LEFT(void)
{
#if (USE_AVSHIFT_MODE == TRUE)
	avShiftMode_Keys_Common();
#else
	Other_Keys_Common();
#endif
}
void Key_NAVI_RIGHT(void)
{
#if (USE_AVSHIFT_MODE == TRUE)
	avShiftMode_Keys_Common();
#else
	Other_Keys_Common();
#endif
}

void Key_STBPOWER(void)
{
	Other_Keys_Common();
}
void Key_VOLUP(void)
{
	Other_Keys_Common();
}
void Key_VOLDOWN(void)
{
	Other_Keys_Common();
}
void Key_MUTE(void)
{
	Other_Keys_Common();
}
void Key_MOVIE(void)
{
	Other_Keys_Common();
}
void Key_REVIEW(void)
{
	Other_Keys_Common();
}
void Key_CHGUIDE(void)
{
	Other_Keys_Common();
}
void Key_OPTION(void)
{
	Other_Keys_Common();
}
void Key_BACK(void)
{
	Other_Keys_Common();
}
void Key_EXIT(void)
{
	Other_Keys_Common();
}
void Key_REW(void)
{
	Other_Keys_Common();
}
void Key_PLAYPAUSE(void)
{
	Other_Keys_Common();
}
void Key_STOP(void)
{
	Other_Keys_Common();
}
void Key_FF(void)
{
	Other_Keys_Common();
}
void Key_STAR(void)
{
	Other_Keys_Common();
}
void Key_SHARP(void)
{
	Other_Keys_Common();
}
void Key_DELETE(void)
{
	Other_Keys_Common();
}
void Key_HANENG(void)
{
	Other_Keys_Common();
}
void Key_RED(void)
{
	Other_Keys_Common();
}
void Key_GREEN(void)
{
	Other_Keys_Common();
}
void Key_YELLOW(void)
{
	Other_Keys_Common();
}
void Key_BLUE(void)
{
	Other_Keys_Common();
}
void Key_MYMENU(void)
{
	Other_Keys_Common();
}
void Key_SHOPPING(void)
{
	Other_Keys_Common();
}
void Key_WIDGET(void)
{
	Other_Keys_Common();
}
void Key_APPSTORE(void)
{
	Other_Keys_Common();
}
#endif	//__FUNCTION_KEY_C__

