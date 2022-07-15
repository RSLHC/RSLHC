
#ifndef	__ROM_H__
#define	__ROM_H__

extern 	 code const unsigned char 	X_CODECVT[];
extern	 code const unsigned char 	X_MAPDUMMY[];
extern	 code const unsigned char 	X_DATADUMMY[];
extern	 code const unsigned char 	X_TOGGKEY[];
extern	 code const unsigned char 	X_TOGGPOS[];
extern	 code const func_ptr		pfunc_ptr[];		
extern	 code const func_special	pfunc_special[];
extern   code const int8u			constBitMask[8];
extern	 code const int8u			pbMaskIndex[];
extern	 code const unsigned short 	RC6[];
extern	 code const unsigned char 	FACTORY_TEST_KEY[5];
extern	 code const unsigned char 	SW_VERSION[];

//extern   code unsigned char coderom[1024];

#endif	//__ROM_H__

