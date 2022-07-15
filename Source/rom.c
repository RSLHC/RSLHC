
#ifndef	__ROM_C__
#define	__ROM_C__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "main.h"
#include "define.h"
#include "type_def.h"
#include "function_key.h"
#include "common.h"
#include "special.h"

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

code const unsigned char X_CODECVT[] = {
	KEY_ALLPOWER,		//01
	KEY_TVPOWER,		//02
	KEY_STBPOWER,  		//03
	KEY_MOVIE,    		//04
	KEY_REVIEW,			//05
	KEY_OPTION,			//06
	KEY_MENU,			//07
	KEY_CHGUIDE,		//08
	KEY_VOLUP,			//09
	KEY_NAVI_UP,		//10
	KEY_CHUP,			//11
	KEY_NAVI_LEFT,		//12
	KEY_OK,				//13
	KEY_NAVI_RIGHT,		//14
	KEY_VOLDOWN,		//15
	KEY_NAVI_DOWN,		//16
	KEY_CHDOWN,			//17
	KEY_BACK,			//18
	KEY_MUTE,			//19
	KEY_EXIT,			//20
	KEY_REW,			//21
	KEY_PLAYPAUSE,		//22
	KEY_STOP,			//23
	KEY_FF,				//24
	KEY_1,				//25
	KEY_2,				//26
	KEY_3,				//27
	KEY_4,				//28
	KEY_5,				//29
	KEY_6,				//30
	KEY_7,				//31
	KEY_8,				//32
	KEY_9,				//33
	KEY_STAR,			//34
	KEY_0,				//35
	KEY_SHARP,			//36
	KEY_SEARCH,			//37
	KEY_DELETE,			//38
	KEY_HANENG,			//39
	KEY_RED,			//40
	KEY_GREEN,			//41
	KEY_YELLOW,			//42
	KEY_BLUE,			//43
	KEY_MYMENU,			//44
	KEY_SHOPPING,		//45
	KEY_WIDGET,			//46
	KEY_APPSTORE,  		//47
	KEY_INPUT,  		//48
	KEY_TVVOLUP,  		//49
	KEY_TVVOLDOWN  		//50

};

//;---------------------------------------;
//;           TABLE                       ;
//;---------------------------------------;

code const unsigned char X_DATADUMMY[]={
	0xed,		//DEV_STB
	0xed,		//DEV_TV
	0xed,		//DEV_TV1
	0xed,		//DEV_TESTMODE
};

code const unsigned char X_MAPDUMMY[]={
	90
};

code const unsigned char X_TOGGKEY[]={
	KEY_TVPOWER
};


code const unsigned char X_TOGGPOS[]={
	0,			//DEV_STB
	1,			//DEV_TV
	1,			//DEV_TV1
	0,			//DEV_TESTMODE
};

code const func_ptr pfunc_ptr[]={
//func_ptr xdata pfunc_ptr[]={
			&Key_NULL,

			&Key_TVPOWER,
			&Key_TVVOLUP,
			&Key_TVVOLDOWN,
			&Key_INPUT,
			&Key_ALLPOWER,
			&Key_STBPOWER,
			&Key_VOLUP,
			&Key_VOLDOWN,
			&Key_MUTE,
			&Key_CHUP,
			&Key_CHDOWN,
			&Key_NAVI_UP,
			&Key_NAVI_DOWN,
			&Key_NAVI_LEFT,
			&Key_NAVI_RIGHT,
			&Key_OK,
			&Key_1,
			&Key_2,
			&Key_3,
			&Key_4,
			&Key_5,
			&Key_6,
			&Key_7,
			&Key_8,
			&Key_9,
			&Key_0,
			&Key_MOVIE,
			&Key_REVIEW,
			&Key_CHGUIDE,
			&Key_MENU,
			&Key_OPTION,
			&Key_BACK,
			&Key_EXIT,
			&Key_REW,
			&Key_PLAYPAUSE,
			&Key_STOP,
			&Key_FF,
			&Key_STAR,
			&Key_SHARP,
			&Key_SEARCH,
			&Key_DELETE,
			&Key_HANENG,
			&Key_RED,
			&Key_GREEN,
			&Key_YELLOW,
			&Key_BLUE,
			&Key_MYMENU,
			&Key_SHOPPING,
			&Key_WIDGET,
			&Key_APPSTORE  
};


code const  func_special pfunc_special[]={
	&func_SPECIAL_F0,
	&func_SPECIAL_F1,
	&func_SPECIAL_F2,
	&func_SPECIAL_F3,
	&func_SPECIAL_F4,
	&func_SPECIAL_F5,
	&func_SPECIAL_F6,
	&func_SPECIAL_F7,
	&func_SPECIAL_F8,
	&func_SPECIAL_F9,
	&func_SPECIAL_FA,
	&func_SPECIAL_FB,
	&func_SPECIAL_FC,
	&func_SPECIAL_FD,
	&func_SPECIAL_FE										
};

code const int8u  constBitMask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
code const int8u  pbMaskIndex[] = {0x01,0x03,0x7,0x0f,0x1f,0x3f};

code const unsigned short  RC6[]={
//unsigned short xdata RC6[]={
	2666,			//high 	2.666ms		0
	889,			//low  	889us		1
	444,			//high 	444us		2
	889,			//low	889us		3
	444,			//high	444us		4
	444,			//low	444us		5
	444,			//high	444us		6
	444				//low	444us		7
	};

code const unsigned char FACTORY_TEST_KEY[5]={
	KEY_SEARCH,
	KEY_HANENG,
	KEY_DELETE,
	KEY_DELETE,
	KEY_APPSTORE
};

code const unsigned char SW_VERSION[]={
	APP_FW_VERSION | 0x80					//Software Revision Number
};

#endif	//__ROM_C__

