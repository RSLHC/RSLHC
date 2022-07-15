
#ifndef __FLASH_H__
#define __FLASH_H__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "define.h"
#include "type_def.h"

/**************************************************************************************************
 *                                        FUNCTIONS - Declaration
 **************************************************************************************************/

#ifdef		__FLASH_C__
void 	flash_program_mode_entry();

#else

extern 	BOOL 	LearnDataRW(int8u rw, int8u device);
extern 	void 	Information_RW(int8u rw);
extern 	void 	Save_Information_All(void);
extern 	void 	flash_program_mode_entry();

#if (USE_IR_LEARN == TRUE)
extern 	void 	Erase_Learn_Key(int8u device);
extern	void	Erase_LearnDevice_Sub(void);
extern 	void 	Erase_LearnDevice(void);
extern 	void 	EraseLearnDataToAll(void);
#endif

#endif	//__FLASH_C__

#endif	//__FLASH_H__

