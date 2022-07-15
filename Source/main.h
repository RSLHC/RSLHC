
#ifndef __MAIN_H__
#define __MAIN_H__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include    <INTRINS.H>
#include    "SC51F33XX.h"
#include	"type_def.h"
#include 	"define.h"
#include 	"rom.h"
#include	"ram.h"
#include	"data.h"

/**************************************************************************************************
 *                                        FUNCTIONS - Declaration
 **************************************************************************************************/
#ifdef		__MAIN_C__
#else
extern	void Stop_Mode(void);
extern	void Sleep_Mode(void);
extern	void Run_Mode(void);
extern  void main(void);
void Special_F4_isr(void);
void Special_F0_carrier_isr(void);
void Special_F0_isr(void);
#endif	//__MAIN_C__

#endif	//__MAIN_H__
