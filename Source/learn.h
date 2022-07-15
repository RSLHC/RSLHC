
#ifndef __IR_LEARN_H__
#define __IR_LEARN_H__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "define.h"
#include "type_def.h"

#if (USE_IR_LEARN == TRUE)

/**************************************************************************************************
 *                                        FUNCTIONS - Declaration
 **************************************************************************************************/
#ifdef		__IR_LEARN_C__
	void 	FindPattern();
	void 	SetPattern();
	void 	CompressBitPattern();
	void 	Bit_streamIdx_Setting();
	void 	learn_toggle_check_RC5();
	void 	learn_toggle_check_RC6();

#else

	extern	void 	Enter_Learn_Mode(void);
	extern	void	FailToLearn(void);
	extern	void	Enter_Delete_Mode(void);
	extern	void	MeltBitPattern(IR_ZIP_ST  *pzipdata);
	extern	void	Clear_Learn_Receive_Buffer(int8u fillData);
	extern	byte	Check_LearnEnableKey(int8u* key);
#endif	//__IR_LEARN_C__

#endif	//USE_IR_LEARN

#endif	//__IR_LEARN_H__

