
#ifndef __FLASH_C__
#define __FLASH_C__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include 	"SC51F33XX.h"
#include 	"type_def.h"
#include  	"main.h"
#include	"common.h"
#include	"rom.h"
#include	"ram.h"
#include	"define.h"
#include	"function_key.h"
#include	"data.h"
#include	"hal.h"
#include	"flash.h"

#if (USE_IR_LEARN == TRUE)
#include	"learn.h"
#endif

#define	coderom	((volatile unsigned char code *)0x0000)
//unsigned char code coderom[1024] _at_ 0xFC00;

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/
//=============================================================
// Function：Flash page erase
// Input：
//       addr: Page address
// Note：1024 Bytes is 1 Page
//       FSHTIM : Use Flash Erase or Write
//				  f=MCLK/(FSHTIM+1)=1MHz
void flash_page_erase(int32u addr)
{
	int16u ii;
	int16u addr0 = addr & ~0x3ff;	  //1024byte

	eeFAIL=0;
    eeFAILAddress=0;

	//address precheck
	if((addr0<EEPROM_START_ADDR)||(addr0>EEPROM_END_ADDR))	//precheck avoiding against malfunction like jump error 
	{
		eeFAIL=0xff;
		eeFAILAddress=addr;
		return;
	}

	//blank check 
	for(ii = 0; ii < EEPROM_PBUFF_SIZE; ii++) 
	{
        if(coderom[addr0+ii] != 0x00) {
			break;
		}
	}
	if(ii==EEPROM_PBUFF_SIZE)	//skip if blank 
	{
		return;	
	}

					// MCLK = 16MHz
	FSHTIM = 0x0F; 	// FSHTIM = 15 => MCLK/(FSHTIM+1) = 16MHz/(15+1) = 1MHz 
	FSHADRH = (addr>>8)&0xFF;
	FSHADRL = addr&0xFF;
	FSHCON |= 0x80;
	FSHKEY = 0xA5;
	FSHKEY = 0xF1;
	FSHCON &= ~0x01;
	FSHCON |= 0x20; //start flash page erasing
	//return (FSHCON&0x01);	
	return;	
}


//=============================================================
// Function：Flash byte write
// Input：
//       addr:Address， uc_data:Data
// Note: FSHTIM : Use Flash Erase or Write
//		 f=MCLK/(FSHTIM+1)=1MHz

void flash_page_write(int16u addr, int8u *uc_data)
{
	int16u ii;
    int16u addr0 = addr & ~0x3ff; //1024byte  

	eeFAIL=0x00;
	eeFAILAddress=0x00;

	//address precheck
	if((addr<EEPROM_START_ADDR)||(addr>EEPROM_END_ADDR))	//precheck avoiding against malfunction like jump error 
	{
		eeFAIL=0xff;
		eeFAILAddress=addr;
		return;
	}

					// MCLK = 16MHz
	FSHTIM = 0x0F;	// FSHTIM = 15 => MCLK/(FSHTIM+1) = 16MHz/(15+1) = 1MHz 
	
	for(ii = 0; ii < EEPROM_PBUFF_SIZE; ii ++) {
		FSHADRH = (addr>>8)&0xFF;
		FSHADRL = addr&0xFF;
		FSHDAT = uc_data[ii];
		FSHCON = 0x80;
		FSHKEY = 0xA5;
		FSHKEY = 0xF1;
		FSHCON &= ~0x01;    
		FSHCON |= 0x10; // start flash write
		addr ++;
	}
	
	//verify
    for(ii = 0; ii < EEPROM_PBUFF_SIZE; ii ++) {
        if(coderom[addr0+ii] != uc_data[ii]) {
			_nop_();
			eeFAIL=0x02;
			eeFAILAddress=addr0+ii;
			break;
        }
    }

	return;
}

void ReadFromFlash(WORD bAddr, byte *pByte, int size)
{
	byte	i;

	for(i = 0; i < size; i++) 
	{
		*pByte = coderom[bAddr+(unsigned int)i];
		pByte++;
  }
}


void WriteToFlash(int16u bAddr, int8u *pByte)
{
	// Note: Usually Byte write after page erage
	flash_page_erase(bAddr);

	if(eeFAIL)
	{	
		_nop_(); 
		return; 
	}

	flash_page_write(bAddr, pByte);

}

//----------------------------------------------------
//flash_rw=0 : read
//				=1 : write
void Information_RW(int8u rw)
{
	pflashCode = STARTADDR_INFO;

	if(rw)
	{
		WriteToFlash(STARTADDR_INFO, set_no.set_reg);
	}
	else
	{
		ReadFromFlash(STARTADDR_INFO, set_no.set_reg, BACKUP_SIZE);
	}
}


void Save_Information_All(void)
{
	chksum_cal();
	set_no.set_member.Chksum.sum=chksum;
	set_no.set_member.Chksum.com=~chksum;
	Information_RW(WRITE);
}

#endif	//__FLASH_C__

