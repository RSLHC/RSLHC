
#ifndef __COMMON_H__
#define __COMMON_H__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "define.h"
#include "type_def.h"

/**************************************************************************************************
 *                                        FUNCTIONS - Declaration
 **************************************************************************************************/
#ifdef		__COMMON_C__
	void Delayby10us(int16u nCnt);
	void wBitStream_Clear(void);
	void getkey(unsigned char* p);
	unsigned char getkey_simple(void);
	void wait_getkey_simple(void);
	void codeCvt(BYTE* pKey_buf);
	int8u get_map_pointer(void);
	void my_sub(void);
	void my_newkeyincond(void);
	void t0_wait(int16u i);
	//unsigned char	t0_wait_keyoff(unsigned char i);
	BYTE t0_wait_keyin_chk(int16u i);
	void t1_wait(int16u i);
	//BYTE t0_wait_keyoff_chk(unsigned char i);
	void carrier_set();
	volatile void wait_10us(void);
	void pulse_stop(void);
	void send_ur(void);
	BYTE Send_UR_Ready(void);
	WORD* SetBitStreamFromNibble(WORD * npWaddr,BYTE *	npbAddr, int16u  size,UWORD * npWord12);
	WORD* FillBitStreamFromOutOrder(int16u start, WORD* npWaddr, UWORD* npWord12);
	BYTE LoadFormat(BYTE formatno);
	BYTE* FormatNextAddrCal(BYTE code* pAddr);
	BOOL LoadCustDataWithFormat(BYTE  * pCodeaddr);
	void Convert_cd(unsigned long* a);
	BYTE const *GetCodeAddress();
	BYTE const *GetCodeAddress_set(set_number_t bSetNo);
	void Setting_Mode(void);
	byte Get1CodeFrom3Buf(BYTE* p);
	void next_searchNo_find_inAutoSearch(void);
	void prev_searchNo_find_inAutoSearch(void);
	void device_Led_blink(int8u type, int8u repeat, int16u on_ms, int16u off_ms);
	void device_led_off(BYTE i);
	void device_led_on(BYTE i);
	void SetMode_StartTimeOut(int16u time);
	void Timer0_50ms16_Update(int16u time);
	BOOL device_Led_blink_key_chk(int8u type, int8u repeat, int16u on_ms, int16u off_ms);
	void IrZipData_Clear(void);
	void device_Led_InSetUp(int8u result, int8u type);
	int8u Check_Key_2keyNoStop(int8u* key1, int8u* key2);
	int8u Check_Key_3keyNoStop(int8u* key1, int8u* key2, int8u* key3);
	int8u Check_Key_4keyNoStop(int8u* key1, int8u* key2, int8u* key3, int8u* key4);
//	int8u Check_Key_5keyNoStop(int8u* key1, int8u* key2, int8u* key3, int8u* key4, int8u* key5);
	void Low_Voltage_Check(byte lvi_key);
	void Code_Read_Out(void);
	void BackLight_Dim_On(void);
	void BackLight_Dim_Off(void);
	//void getkey_wait(void);
	void auto_search_1(int8u direction);
	//void get2key_wait(void);
	void Check_Key_InSetmode(int8u* key1, int8u* key2);
	void EnvelopCarrierOff(void);
	int8u Panasonic_TV_Check(int8u device, set_number_t set_no);
	void Timer0_65ms16_2_Update(int16u time);
	void SW_SystemReset(void);
	void Envelop_wait(int16u i);

#else
	extern void	Delayby10us(int16u nCnt);
	extern void	pulse_stop(void);
	extern void	my_sub(void);
	extern void	device_led_off(BYTE i);
	extern void	device_led_on(BYTE i);
	extern void	getkey(unsigned char* p);
	extern unsigned char getkey_simple(void);
	extern void	wait_getkey_simple(void);
	extern volatile void wait_10us(void);
	extern WORD* SetBitStreamFromNibble(WORD * npWaddr,BYTE *	npbAddr, int16u  size,UWORD * npWord12);
	extern WORD* FillBitStreamFromOutOrder(int16u start, WORD* npWaddr, UWORD* npWord12);
	extern void	my_newkeyincond(void);
	extern void	send_ur(void);
	extern BYTE	Send_UR_Ready(void);
	extern void device_Led_blink(int8u type, int8u repeat, int16u on_ms, int16u off_ms);
	extern int8u Check_Key_2keyNoStop(int8u* key1, int8u* key2);
	extern int8u Check_Key_3keyNoStop(int8u* key1, int8u* key2, int8u* key3);
	extern int8u Check_Key_4keyNoStop(int8u* key1, int8u* key2, int8u* key3, int8u* key4);
//	extern int8u Check_Key_5keyNoStop(int8u* key1, int8u* key2, int8u* key3, int8u* key4, int8u* key5);
	extern void	Convert_cd(unsigned long* a);
	extern void Check_Key_InSetmode(int8u* key1, int8u* key2);
	extern void Setting_Mode(void);
	extern byte Get1CodeFrom3Buf(BYTE* p);
	extern void	SetMode_StartTimeOut(int16u time);
	extern void	Timer0_50ms16_Update(int16u time);
	extern void device_Led_InSetUp(int8u result, int8u type);
	extern void auto_search_1(int8u direction);
	extern void Code_Read_Out(void);
	extern int8u Panasonic_TV_Check(int8u device, set_number_t set_no);
	extern void	key_buf_old_clear(void);
	extern void	wBitStream_Clear(void);
	extern void Low_Voltage_Check(byte lvi_key);
	extern void Timer0_avShiftMode_Update(int16u time);
	extern void avShiftMode_StartTimeOut(int16u time);
	extern void t0_wait(int16u i);
	extern BYTE t0_wait_keyin_chk(int16u i);
	extern void t1_wait(int16u i);
	extern BYTE t1_wait_keyin_chk(int16u i);
	extern BOOL device_Led_blink_key_chk(int8u type, int8u repeat, int16u on_ms, int16u off_ms);
	extern void WDG_SystemReset(void);
	extern void Timer0_65ms16_2_Update(int16u time);
	extern void SW_SystemReset(void);
	extern void Envelop_wait(int16u i);
#endif

#endif

