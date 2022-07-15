
#ifndef __TYPE_DEF_H__
#define __TYPE_DEF_H__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "define.h"

/**************************************************************************************************
 *                                            TYPEDEFS
 **************************************************************************************************/

typedef		unsigned	char	__uint8__;

typedef		unsigned 	char	__byte__;
typedef		signed 		char 	__int8__;
typedef		unsigned 	short	__uint16__;
typedef		signed 		short	__int16__;
typedef		unsigned 	long 	__uint32__;
typedef		signed 		long 	__int32__;
typedef		unsigned 	char	byte;
typedef		unsigned 	int 	word;

typedef		unsigned 	char   	BYTE;
typedef		unsigned 	char   	UBYTE;
typedef		unsigned 	char   	BOOL;
typedef		unsigned 	short  	UWORD;
typedef		unsigned 	short  	WORD;
typedef		unsigned 	long   	uint32;
typedef		unsigned 	long   	ULONG;
typedef		unsigned 	int    	UINT;

typedef 	unsigned 	char  	boolean;
typedef 	unsigned 	char  	int8u;
typedef 	signed   	char  	int8s;
typedef 	unsigned 	short 	int16u;
typedef 	signed   	short 	int16s;
typedef 	unsigned 	int   	int32u;
typedef 	signed   	int   	int32s;
typedef 	unsigned 	short 	PointerType;


#if (USE_2BYTE_SETTING_NO == TRUE)
typedef unsigned short         set_number_t;
#else
typedef unsigned char          set_number_t;
#endif

typedef union
{
	byte        byteArray[38];
	struct
	{
		byte    formno;
		struct
		{
			byte    time  :4;
			byte    outp  :4;
		}size;

		struct
		{
			byte    non_word  :1;
			byte    repeat    :1;
			byte    single    :1;
			byte    flash     :1;
			byte    end_com   :1;
			byte    multi_cust:1;
			byte    multi_form:1;
			byte    spare     :1;
		}option1;

		struct
		{
			byte    repeat  :2;
			byte    lead    :2;
			byte    logic0  :2;
			byte    logic1  :2;
		}option2;

		struct
		{
			byte    data1 :6;
			byte    index1:2;
		}datasize;

		struct
		{
			byte    custom:6;
			byte    index2:2;
		}customsize;

		byte    outorder[8];
		byte    logic1[3];
		byte    logic0[3];
		byte    lead[3];
		byte    repeat[3];
		byte    end_pulse;
		byte    timeval[7];

		UWORD   word1;
		UWORD   word2;
	} strMember;
} TYPE_FORMAT;

typedef struct                                  // ^ : 실제로 pulse sending전에 setting되어야함.
{
	BYTE    bCarrier;                           // ^
	ULONG   Data;
	ULONG   Custom;
}TYPE_CODE;

///////////////////////////////////////
// function pointer
///////////////////////////////////////
typedef void (*func_ptr) (void);

typedef int16u (*func_special) (void);


//----------------------------------
// setting no
//----------------------------------
//#define	BACKUP_INFO		0xFFC0
//#define	BACKUP_INFO		0x4000
#define	BACKUP_INFO		0xFC00

#if (USE_2BYTE_SETTING_NO == TRUE)
#define	BACKUP_SIZE		8
#else
#define	BACKUP_SIZE		6
#endif

typedef union
{
	unsigned char set_reg[BACKUP_SIZE];
	struct
	{
		struct
		{
			set_number_t stb_src;
			set_number_t tv_src;
		}set_no;  

		struct
		{
			unsigned char stb_src;
			unsigned char tv_src;
		}set_dev;

		struct
		{
			unsigned char sum;
			unsigned char com;
		}Chksum;

	}set_member;
}TYPE_SETNO;

//typedef void  (*FuncFieldProc) (Toutputdata  *pOutputdata, byte Key);

typedef struct
{
  unsigned short  HIGH;
  unsigned short  LOW;
} IR_PULSE_HIGHLOW;

#define BITSTREAMLENGTH		272
#define MAX_BITSTREAM   	(BITSTREAMLENGTH-1)

typedef struct IR_STREAM_TAG
{
	union
	{
		struct
		{
			unsigned short   Carrier;
			unsigned short   bidxEndWord1;              //  ^ 1'st Word의 Wait구간 즉, "low구간"만을 point.
			unsigned short   bidxEndWord2;              //  ^ 2'nd Word의 Wait구간 즉, "low구간"만을 point.
			unsigned short   bidxRepeatWord;            //  ^ 2'nd Word의 Start 즉, "high구간"만을 point.

			union
			{
				unsigned short   BitStream[BITSTREAMLENGTH];
				IR_PULSE_HIGHLOW Envelope[BITSTREAMLENGTH/2];
			} pulse;

		} Tstruct;

		int8u   data1[(BITSTREAMLENGTH*2)+8];

	}Tunion;
}IR_STREAM_ST;


typedef struct
{
	int8u	Envelop[3];
}TinputEnvelop;

#if 0
#define iEnvelop_SIZE    		(BITSTREAMLENGTH+1)
#define iEnvelop_TimeSIZE  		(iEnvelop_SIZE*3)

typedef struct
{
	union
	{
		struct
		{
			TinputEnvelop	iEnvelop[iEnvelop_SIZE];
		}Tstruct;

		int8u data1[iEnvelop_TimeSIZE];

	}Tunion;

}Input_Carrier;
#endif

#define IRZIP_BUFFER_SIZE    		38
#if (IRZIP_CARR_2BYTES == TRUE)
#define	FULL_IRZIP_BUFFER_SIZE		(51+1)	//(51+3)
#else
#define	FULL_IRZIP_BUFFER_SIZE		51		//(51+3)
#endif

typedef struct IR_ZIP_TAG
{
	union
	{
		struct
		{
#if (IRZIP_CARR_2BYTES == TRUE)
			unsigned short	Carrier;
#else
			unsigned char	Carrier;
#endif
			unsigned char   bidxEndWord1;              //  ^ 1'st Word의 Wait구간 즉, "low구간"만을 point.
			unsigned char   bidxEndWord2;              //  ^ 2'nd Word의 Wait구간 즉, "low구간"만을 point.
			unsigned char   bidxRepeatWord;            //  ^ 2'nd Word의 Start 즉, "high구간"만을 point.

			unsigned char   bType;                      // >> warrPattern9_F의 크기.(D5~D0)
      		//unsigned char   bDummy_32bit[3];
			unsigned char   barrPatHig[2];              // >> 완성.
			unsigned char   barrPatLow[2];              // >> 완성.
			unsigned short	bidxEndWord1_time;
			unsigned short  bidxEndWord2_time;

			union
			{
				unsigned short   warrPattern9_F[IRZIP_BUFFER_SIZE/2];  // >> 완성.
				unsigned char    barrNibbStream[IRZIP_BUFFER_SIZE];    // +++
				unsigned char    barrBitData[IRZIP_BUFFER_SIZE];       // +++
			}uniData;
		} Tstruct;
		unsigned char   data1[FULL_IRZIP_BUFFER_SIZE];                              // 80바이트단위로 6번 전송함.
	} Tunion;
} IR_ZIP_ST;    //ZigBee

typedef struct
{
	union
	{
		struct
		{
			unsigned short   	Carrier;
			unsigned short   	bidxEndWord1;              //  ^ 1'st Word의 Wait구간 즉, "low구간"만을 point.
			unsigned short   	bidxEndWord2;              //  ^ 2'nd Word의 Wait구간 즉, "low구간"만을 point.
			unsigned short   	bidxRepeatWord;            //  ^ 2'nd Word의 Start 즉, "high구간"만을 point.

			unsigned short  	BitStream[30];
			unsigned char			bCarrierArray[30];
		} Tstruct;

		int8u   data1[90];
	}Tunion;
}TYPE_CODE_SPECIALF4F8;

#endif	//__TYPE_DEF_H__

