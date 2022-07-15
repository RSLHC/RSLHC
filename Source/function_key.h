#ifndef __FUNCTION_KEY_H__
#define __FUNCTION_KEY_H__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "type_def.h"
#include "define.h"

/**************************************************************************************************
 *                                        FUNCTIONS - Declaration
 **************************************************************************************************/
#ifdef		__FUNCTION_KEY_C__
	void 	irout_n(unsigned char i);
	void 	ReturnNormal(void);

#else		//__FUNCTION_KEY_C__
	extern	void	Key_NULL(void);

	extern	void 	Key_TVPOWER(void);
	extern	void 	Key_TVVOLUP(void);
	extern	void 	Key_TVVOLDOWN(void);
	extern	void 	Key_INPUT(void);
	extern	void 	Key_ALLPOWER(void);
	extern	void 	Key_STBPOWER(void);
	extern	void 	Key_VOLUP(void);
	extern	void 	Key_VOLDOWN(void);
	extern	void 	Key_MUTE(void);
	extern	void 	Key_CHUP(void);
	extern	void 	Key_CHDOWN(void);
	extern	void 	Key_NAVI_UP(void);
	extern	void 	Key_NAVI_DOWN(void);
	extern	void 	Key_NAVI_LEFT(void);
	extern	void 	Key_NAVI_RIGHT(void);
	extern	void 	Key_OK(void);
	extern	void 	Key_1(void);
	extern	void 	Key_2(void);
	extern	void 	Key_3(void);
	extern	void 	Key_4(void);
	extern	void 	Key_5(void);
	extern	void 	Key_6(void);
	extern	void 	Key_7(void);
	extern	void 	Key_8(void);
	extern	void 	Key_9(void);
	extern	void 	Key_0(void);
	extern	void 	Key_MOVIE(void);
	extern	void 	Key_REVIEW(void);
	extern	void 	Key_CHGUIDE(void);
	extern	void 	Key_MENU(void);
	extern	void 	Key_OPTION(void);
	extern	void 	Key_BACK(void);
	extern	void 	Key_EXIT(void);
	extern	void 	Key_REW(void);
	extern	void 	Key_PLAYPAUSE(void);
	extern	void 	Key_STOP(void);
	extern	void 	Key_FF(void);
	extern	void 	Key_STAR(void);
	extern	void 	Key_SHARP(void);
	extern	void 	Key_SEARCH(void);
	extern	void 	Key_DELETE(void);
	extern	void 	Key_HANENG(void);
	extern	void 	Key_RED(void);
	extern	void 	Key_GREEN(void);
	extern	void 	Key_YELLOW(void);
	extern	void 	Key_BLUE(void);
	extern	void 	Key_MYMENU(void);
	extern	void 	Key_SHOPPING(void);
	extern	void 	Key_WIDGET(void);
	extern	void 	Key_APPSTORE(void);

	extern	void 	irout_n(unsigned char i);
//	extern	void 	Volume_keys(void);
	extern	void 	ReturnNormal(void);
	extern	void 	rsAppMain(void);
	extern 	void 	chksum_cal(void);	
	extern	void 	getSameKey_wait(void);
	extern	BOOL 	batch_SS_LG_Common(int8u device, set_number_t set_no, int8u key);
	extern	void 	STB_IRoutCommon(void);
	extern	void 	STB_SettingCommon(void);
	extern	void 	TestMode_IRoutCommon(void);
	extern	void 	UR_SettingCommon(int8u set_source);
	
#endif		//__FUNCTION_KEY_C__

#endif	// __FUNCTION_KEY_H__

