
#ifndef __HAL_H__
#define __HAL_H__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "type_def.h"
#include "define.h"

/**************************************************************************************************
 *                                        FUNCTIONS - Declaration
 **************************************************************************************************/

void FactoryDefault(void);

extern	void SystemInit(void);
extern	void ExtInt_init(void);
extern	void reset_define(void);
extern	void stop_define(void);
extern	void InitPort(void);
extern	void TickTimer_65msStart(int16u time);
extern  void BatchTimer_500usStart(int16u time);
extern	void FactoryDefault(void);
extern	void StuckKeyTimer_Start(int8u time);
extern	void Keyscan_Interrupt_On(void);
extern	void Keyscan_Interrupt_Off(void);
extern	void Timer0_65msStart(void);
extern  void Timer1_1secStart(void);
extern  void Timer2_Start(void);
extern  void SetDeltaV(void);
extern  void crt_init_IR(void);
extern  void ClrWDT();

//extern  void WDT_clear();


#endif	// __HAL_H__

