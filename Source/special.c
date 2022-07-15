
#ifndef __SPECIAL_C__
#define __SPECIAL_C__

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "main.h"
#include "define.h"
#include "type_def.h"
#include "common.h"
#include "special.h"

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/
#if (USE_SPECIAL_0F0H == TRUE)
// option 0 : M708
// option 1 : SAA1250
// option 2 : SAA1250 BAR
// option 3 : M708 CARRIER
// option 4 : SAA1250 BAR2
BOOL func_SPECIAL_F0(void)
{
	int16u	i, j;
	ULONG   ulTemp;
	UWORD   uWord12;

	ramFormat.strMember.option1.flash = 1;
	uWord12 = ramFormat.strMember.word1;                          	// word계산.

	*npWaddr_1 &= 0x7fff;
	*npWaddr_1 += ramFormat.strMember.lead[0];                      // lead

	uWord12 -= (*npWaddr_1)&0x7fff;
	npWaddr_1++;

	if(ramFormat.strMember.timeval[0] == 1)                       	// saa1250
	{
		ulTemp = ramCode.Custom;                                  		// system

		for(i=0; i<ramFormat.strMember.customsize.custom; i++)
		{
			if(ulTemp & 0x01)
			{
				*npWaddr_1 = ramFormat.strMember.logic1[0];
			}
			else
			{
				*npWaddr_1 = ramFormat.strMember.logic0[0];
			}

			uWord12 -= (*npWaddr_1)&0x7fff;
			npWaddr_1++;
			ulTemp >>= 1;
		}

		ulTemp = ramCode.Data;                                    		// data
		
		for(i=0; i<ramFormat.strMember.datasize.data1; i++)
		{
			if(ulTemp&0x01)
			{
				*npWaddr_1 = ramFormat.strMember.logic1[0];
			}
			else
			{
				*npWaddr_1 = ramFormat.strMember.logic0[0];
			}

			uWord12 -= (*npWaddr_1)&0x7fff;
			npWaddr_1++;
			ulTemp >>= 1;
		}
	}
	else if(ramFormat.strMember.timeval[0] == 2)                       	// saa1250 bar
	{
		*npWaddr_1 += ramFormat.strMember.logic0[0];                      // lead2
		uWord12 -= (*npWaddr_1)&0x7fff;
		npWaddr_1++;

		ulTemp = ramCode.Custom;                                  		// system
		
		for(i=0; i<ramFormat.strMember.customsize.custom; i++)
		{
			if(ulTemp&0x01)
			{
				*npWaddr_1 = ramFormat.strMember.logic1[0];
			}
			else
			{
				*npWaddr_1 = ramFormat.strMember.logic0[0];
			}

			uWord12 -= (*npWaddr_1)&0x7fff;
			npWaddr_1++;
			ulTemp >>= 1;
		}

		ulTemp = ramCode.Data;                                    		// data
		for(i=0;i<ramFormat.strMember.datasize.data1;i++)
		{
			if(ulTemp&0x01)
			{
				*npWaddr_1 = ramFormat.strMember.logic1[0];
			}
			else
			{
				*npWaddr_1 = ramFormat.strMember.logic0[0];
			}

			uWord12 -= (*npWaddr_1)&0x7fff;
			npWaddr_1++;
			ulTemp >>= 1;
		}

		*npWaddr_1=ramFormat.strMember.end_pulse;              // end
		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;

		*npWaddr_1=uWord12;                                    // end + 5(0를 주지 않기위해)

		// the wait POINTER of 1'st word(word 단위)
		ir_stream.Tunion.Tstruct.bidxEndWord1 = npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
		// the wait POINTER of 1'st word
		ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;

		npWaddr_1++;
		*npWaddr_1 += ramFormat.strMember.lead[0];                      // lead1
		ir_stream.Tunion.Tstruct.bidxRepeatWord = (npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);

		uWord12 -= (*npWaddr_1)&0x7fff;
		npWaddr_1++;

		*npWaddr_1 += ramFormat.strMember.logic0[0];                      // lead2

		uWord12 -= (*npWaddr_1)&0x7fff;
		npWaddr_1++;

		ulTemp=ramCode.Custom;                                  		// system

		for(i=0; i<ramFormat.strMember.customsize.custom; i++)
		{
			if(ulTemp & 0x01)
			{
				*npWaddr_1 = ramFormat.strMember.logic0[0];
			}
			else
			{
				*npWaddr_1 = ramFormat.strMember.logic1[0];
			}

			uWord12 -= (*npWaddr_1)&0x7fff;
			npWaddr_1++;
			ulTemp >>= 1;
		}

		ulTemp=ramCode.Data;                                    		// data

		for(i=0;i<ramFormat.strMember.datasize.data1;i++)
		{
			if(ulTemp&0x01)
			{
				*npWaddr_1=ramFormat.strMember.logic1[0];
			}
			else
			{
				*npWaddr_1=ramFormat.strMember.logic0[0];
			}

			uWord12 -= (*npWaddr_1)&0x7fff;
			npWaddr_1++;
			ulTemp >>= 1;
		}

		*npWaddr_1=ramFormat.strMember.end_pulse;              // end
		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;

		*npWaddr_1=uWord12;                                    // end + 5(0를 주지 않기위해)

		// the wait POINTER of 1'st word(word 단위)
		ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
		// the wait POINTER of 1'st word
		ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;

		return TRUE;

	}
	else if(ramFormat.strMember.timeval[0]==4)                         	// SAA1250 BAR2
	{
		*npWaddr_1+=ramFormat.strMember.logic0[0];                      // lead2

		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;

		ulTemp=ramCode.Custom;                                  		// system
		for(i=0;i<ramFormat.strMember.customsize.custom;i++)
		{
			if(ulTemp&0x01)
			{
				*npWaddr_1=ramFormat.strMember.logic1[0];
			}
			else
			{
				*npWaddr_1=ramFormat.strMember.logic0[0];
			}

			uWord12-=(*npWaddr_1)&0x7fff;
			npWaddr_1++;
			ulTemp>>=1;
		}

		ulTemp=ramCode.Data;                                    		// data
		for(i=0;i<ramFormat.strMember.datasize.data1;i++)
		{
			if(ulTemp&0x01)
			{
				*npWaddr_1=ramFormat.strMember.logic1[0];
			}
			else
			{
				*npWaddr_1=ramFormat.strMember.logic0[0];
			}

			uWord12-=(*npWaddr_1)&0x7fff;
			npWaddr_1++;
			ulTemp>>=1;
		}

		*npWaddr_1+=ramFormat.strMember.lead[0];                      // lead

		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;

		*npWaddr_1+=ramFormat.strMember.logic0[0];                      // lead2

		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;

		*npWaddr_1=uWord12;                                    // end + 5(0를 주지 않기위해)


		// the wait POINTER of 1'st word(word 단위)
		ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
		//ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
		// the wait POINTER of 1'st word
		ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;

		npWaddr_1++;

		*npWaddr_1+=ramFormat.strMember.lead[0];                      // lead1
		//ir_stream.Tunion.Tstruct.bidxRepeatWord=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);

		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;

		*npWaddr_1+=ramFormat.strMember.logic0[0];                      // lead2

		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;

		ulTemp=ramCode.Custom;                                  		// system

		for(i=0;i<ramFormat.strMember.customsize.custom;i++)
		{
			if(ulTemp&0x01)
			{
				*npWaddr_1=ramFormat.strMember.logic0[0];
			}
			else
			{
				*npWaddr_1=ramFormat.strMember.logic1[0];
			}

			uWord12-=(*npWaddr_1)&0x7fff;
			npWaddr_1++;
			ulTemp>>=1;
		}

		ulTemp=ramCode.Data;                                    		// data
		for(i=0;i<ramFormat.strMember.datasize.data1;i++)
		{
			if(ulTemp&0x01)
			{
				*npWaddr_1=ramFormat.strMember.logic1[0];
			}
			else
			{
				*npWaddr_1=ramFormat.strMember.logic0[0];
			}

			uWord12-=(*npWaddr_1)&0x7fff;
			npWaddr_1++;
			ulTemp>>=1;
		}

		*npWaddr_1+=ramFormat.strMember.lead[0];                      // lead

		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;

		*npWaddr_1+=ramFormat.strMember.logic0[0];                      // lead2

		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;

		*npWaddr_1=uWord12;                                    // end + 5(0를 주지 않기위해)


		// the wait POINTER of 1'st word(word 단위)
		//ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
		//ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
		// the wait POINTER of 1'st word
		//ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;
		ir_stream.Tunion.Tstruct.bidxEndWord2=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];

		return TRUE;
	}
	else                         										// m708, m708 carrier
	{
		j=0;
		ulTemp=ramCode.Custom;                                 			// system
		for(i=0;i<ramFormat.strMember.customsize.custom;i++)
		{
			if(ulTemp&0x01)
			{
				if(j%2)
				{
					*npWaddr_1=ramFormat.strMember.logic1[0];
				}
				else
				{
					*npWaddr_1=ramFormat.strMember.logic1[2];
				}
			}
			else
			{
				*npWaddr_1=ramFormat.strMember.logic0[0];
			}

			uWord12-=(*npWaddr_1)&0x7fff;
			npWaddr_1++;
			ulTemp>>=1;
			j++;
		}
		ulTemp=ramCode.Data;                                    		// data
		for(i=0;i<ramFormat.strMember.datasize.data1;i++)
		{
			if(ulTemp&0x01)
			{
				if(j%2)
				{
					*npWaddr_1=ramFormat.strMember.logic1[0];
				}
				else
				{
					*npWaddr_1=ramFormat.strMember.logic1[2];
				}
			}
			else
			{
				*npWaddr_1=ramFormat.strMember.logic0[0];
			}

			uWord12-=(*npWaddr_1)&0x7fff;
			npWaddr_1++;
			ulTemp>>=1;
			j++;
		}
	}

	*npWaddr_1 = ramFormat.strMember.end_pulse;              // end
	uWord12 -= (*npWaddr_1)&0x7fff;
	npWaddr_1++;

	*npWaddr_1=uWord12;                                    // end + 5(0를 주지 않기위해)


	// the wait POINTER of 1'st word(word 단위)
	ir_stream.Tunion.Tstruct.bidxEndWord1 = npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
	ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
	// the wait POINTER of 1'st word
	ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;

	return TRUE;
}
#else
BOOL  func_SPECIAL_F0(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0F1H == TRUE)
// SAA1350
BOOL func_SPECIAL_F1(void)
{
	int     i;		//,j;
	ULONG   ulTemp;
	UWORD   uWord12;

	ramFormat.strMember.option1.flash=1;
	uWord12=ramFormat.strMember.word1;                          // word계산.

	*npWaddr_1&=0x7fff;

	*npWaddr_1+=ramFormat.strMember.lead[0];                      // lead
	uWord12-=(*npWaddr_1)&0x7fff;
	npWaddr_1++;

	ulTemp=ramCode.Data;                                        // data
	for(i=0;i<ramFormat.strMember.datasize.data1;i++)
	{
		if(ulTemp&0x01)
		{
			*npWaddr_1=ramFormat.strMember.logic1[0];
		}
		else
		{
			*npWaddr_1=ramFormat.strMember.logic0[0];
		}

		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;
		ulTemp>>=1;
	}

	*npWaddr_1=ramFormat.strMember.end_pulse;              // end
	uWord12-=(*npWaddr_1)&0x7fff;
	npWaddr_1++;

	//*npWaddr_1=0x8000+100;                                        // end + 100(0를 주지 않기위해)
	//uWord12-=(*npWaddr_1)&0x7fff;
	//npWaddr_1++;

	*npWaddr_1=uWord12;                                           // 남은 word

	ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
	ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
	ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;

	ramFormat.strMember.outorder[0]=0xf0;
	return TRUE;
}
#else
BOOL  func_SPECIAL_F1(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0F2H == TRUE)
// CX7947
BOOL  func_SPECIAL_F2(void)
{
    int16u	i,j;
    UWORD   uWord12;
    ULONG   ulTemp;

    uWord12=ramFormat.strMember.word1;                          // word계산.

                                                                // lead
    npWaddr_1=SetBitStreamFromNibble(npWaddr_1,&ramFormat.strMember.lead[0],3,(WORD*)&uWord12);

    ulTemp=ramCode.Data;                                        // data(12 or 15)

    if(ramCode.Data&0x8000) j=15;                               // 15 bit
    else                    j=12;                               // 12 bit

    for(i=0;i<j;i++)
    {
        if(ulTemp&0x01) npWaddr_1=SetBitStreamFromNibble(npWaddr_1,&ramFormat.strMember.logic1[0],3,(WORD*)&uWord12);
        else            npWaddr_1=SetBitStreamFromNibble(npWaddr_1,&ramFormat.strMember.logic0[0],3,(WORD*)&uWord12);
        ulTemp>>=1;
    }
    if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
    {
        if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
        *npWaddr_1=0;
    }
    //*npWaddr_1+=(uWord12+17);                                          // 남은 word
    *npWaddr_1+=(uWord12);                                          // 남은 word

    ir_stream.Tunion.Tstruct.bidxEndWord1=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);         // the wait POINTER of 1'st word(word 단위)
    ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
    ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;                  // the wait POINTER of 1'st word

    return TRUE;
}
#else
BOOL  func_SPECIAL_F2(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0F3H == TRUE)
//SONY OLD
BOOL  func_SPECIAL_F3(void)
{
    int16u	i;
    UWORD   uWord12;
    //UINT    xRef;
    unsigned short	xRef;

    uWord12=ramFormat.strMember.word1;                          // word계산.

                                                                // lead
    npWaddr_1=SetBitStreamFromNibble(npWaddr_1,&ramFormat.strMember.lead[0],3,(WORD*)&uWord12);
    if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
    {
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
		*npWaddr_1=0;
    }

    i=ramFormat.strMember.logic0[0];
    i>>=4;
    xRef=ramFormat.strMember.timeval[i-1];

    i=ramCode.Data&0x0ff;                                       // data - 1
    *npWaddr_1=xRef*i;
    uWord12-=(xRef*i);

                                                               // HIGH
    npWaddr_1=SetBitStreamFromNibble(npWaddr_1,&ramFormat.strMember.logic1[0],3,(WORD*)&uWord12);
    if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
    {
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
		*npWaddr_1=0;
    }

    i=ramCode.Data>>8;                                          // data - 2
    i&=0x0ff;
    *npWaddr_1=xRef*i;
    uWord12-=(xRef*i);

    if(ramFormat.strMember.option1.non_word)
    {
		uWord12=ramFormat.strMember.word1;  // an interval of WAIT
    }
    //*npWaddr_1+=(uWord12+17);                                          // + word - non word
    *npWaddr_1+=(uWord12);                                          // + word - non word

    ir_stream.Tunion.Tstruct.bidxEndWord1=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);         // the wait POINTER of 1'st word(word 단위)
    ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
    ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;                  // the wait POINTER of 1'st word

    return TRUE;

}
#else
BOOL  func_SPECIAL_F3(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0F4H == TRUE)
//U327
BOOL  func_SPECIAL_F4(void)
{
	int     i;
	UWORD   uWord12;
	ULONG   ulTemp,ulTempPrv;
	BYTE    bBit1,bBit0;
	int     idxCarrier;
	int     first=0;
	BYTE    bBit1Carrier,bBit0Carrier;
	int     iCntBit0=0;                                         // 비교.

	bBit1Carrier=CARR_38400_Hz;   	//38.4KHz
	bBit0Carrier=CARR_34700_Hz;		//34.7KHz

	ir_stream.Tunion.Tstruct.Carrier=bBit1Carrier;

	pramCode=(TYPE_CODE_SPECIALF4F8 *)&ir_stream;

	i=ramFormat.strMember.lead[0];                              // bit 1
	i>>=4;
	i&=0x7;
	bBit1=ramFormat.strMember.timeval[i-1];

	i=ramFormat.strMember.repeat[0];                            // bit 0
	i>>=4;
	i&=0x7;
	bBit0=ramFormat.strMember.timeval[i-1];

	uWord12=ramFormat.strMember.word1;                          // word계산.

	ulTemp=ramCode.Data;
	ulTemp&=0xff;                                               // 8bit data

	ulTempPrv=ramCode.Custom<<8;
	ulTempPrv&=0x1f00;                                          // 5bit custom

	ulTemp|=ulTempPrv;

	ulTempPrv=0xff;
	for(i=0;i<13;i++)                                           // total size = 13
	{
		if(ulTempPrv!=(ulTemp&0x01))
		{
			if(ulTempPrv!=0xff)
			{
				uWord12-=(*npWaddr_1)&0x7fff;
				npWaddr_1++;
			}
			*npWaddr_1=0x8000;
			first=0;
		}
		else
		{
			if(++first==1)
			{
				uWord12-=(*npWaddr_1)&0x7fff;
				npWaddr_1++;
			}
		}

		idxCarrier=npWaddr_1-&pramCode->Tunion.Tstruct.BitStream[0];
		if(ulTemp&0x01)
		{
			*npWaddr_1+=bBit1;

			pramCode->Tunion.Tstruct.bCarrierArray[idxCarrier]=bBit1Carrier;
		}
		else
		{
			*npWaddr_1+=bBit0;

			pramCode->Tunion.Tstruct.bCarrierArray[idxCarrier]=bBit0Carrier;
			iCntBit0+=bBit0;                                    // bit0 count
		}
		ulTempPrv=(ulTemp&0x01);

		ulTemp>>=1;
	}
	uWord12-=(*npWaddr_1)&0x7fff;                                 // 마지막 data고려.

	iCntBit0*=(bBit0-bBit1);                                    // frequency compensation
	iCntBit0/=bBit1;
	uWord12+=iCntBit0;

	npWaddr_1++;                                                  // 마지막은 기준 carrier적용.
	idxCarrier=npWaddr_1-&pramCode->Tunion.Tstruct.BitStream[0];
	pramCode->Tunion.Tstruct.bCarrierArray[idxCarrier]=bBit1Carrier;           // 기준.
	*npWaddr_1=uWord12;                                           // END word

	ir_stream.Tunion.Tstruct.bidxEndWord1=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);	// the wait POINTER of 1'st word(word 단위)
	ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
	ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;                  // the wait POINTER of 1'st word

	//ramFormat.strMember.option1.flash=1;
    return TRUE;
}
#else
BOOL  func_SPECIAL_F4(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0F5H == TRUE)
//PIONEER
BOOL  func_SPECIAL_F5(void)
{
    UWORD   uWord12;
    //UINT    cust,data1;
    //UINT    NormalTransport;
    ULONG    cust,data1;
    unsigned short    NormalTransport;

    NormalTransport=(ramCode.Data>>16)&0x00ff;

    cust=ramCode.Custom;
    data1=ramCode.Data;

	if(ramFormat.strMember.customsize.custom == 16)
	{
	    ramFormat.strMember.outorder[0]=0x01;
	    ramFormat.strMember.outorder[1]=0x23;
	    ramFormat.strMember.outorder[2]=0x45;
	    ramFormat.strMember.outorder[3]=0xff;

	    uWord12=ramFormat.strMember.word1;                          // word계산.

	    ramCode.Custom=(cust&0xff);
	    ramCode.Data=(data1&0xff);

	    ramFormat.strMember.customsize.custom=8;                    // size 조정 8 <- 16
	    ramFormat.strMember.datasize.data1=8;
	}
	else if(ramFormat.strMember.customsize.custom == 32)
	{
    	ramFormat.strMember.outorder[0]=0x01;
    	ramFormat.strMember.outorder[1]=0x34;
    	ramFormat.strMember.outorder[2]=0x5f;

	    uWord12=ramFormat.strMember.word1;                          // word계산.

	    ramCode.Custom=(cust&0xffff);
	    ramCode.Data=(data1&0xff);

	    ramFormat.strMember.customsize.custom=16;                    // size 조정 16 <- 32
	    ramFormat.strMember.datasize.data1=8;

	}

    npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);
    if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
    {
        if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
        *npWaddr_1=0;
    }
    //*npWaddr_1+=(uWord12+17);                                          // END 1 word
    *npWaddr_1+=(uWord12);                                          // END 1 word

//	if(!(NormalTransport&1))									// normal 1-2-1-2
	if(NormalTransport==0)									// normal 1-2-1-2
	{
    	//*npWaddr_1-=78;                                     //13.07.19 1st word adjust in 2nd word 

		npWaddr_1++;
		*npWaddr_1=0x8000;

		uWord12=ramFormat.strMember.word2;                          // word계산.
		if(ramFormat.strMember.customsize.custom == 8)
		{
			ramCode.Custom=cust>>8;
			ramCode.Custom&=0xff;
		}
		else if(ramFormat.strMember.customsize.custom == 16)
		{
			ramCode.Custom=cust>>16;
			ramCode.Custom&=0xffff;
		}
		ramCode.Data=data1>>8;
		ramCode.Data&=0xff;
		npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);
		if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
		{
	        if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
			*npWaddr_1=0;
		}
		//*npWaddr_1+=(uWord12+17);                                          // END 2 word
		*npWaddr_1+=(uWord12);                                          // END 2 word

    	ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
		ir_stream.Tunion.Tstruct.bidxEndWord2=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);			// the wait POINTER of 1'st word(word 단위)
    	ir_stream.Tunion.Tstruct.bidxEndWord1=ir_stream.Tunion.Tstruct.bidxEndWord2;        				// the wait POINTER of 1'st word(word 단위)

	}

	else if(NormalTransport==1)	//1-1-1-1
	{
	//1-1-1-1
    	ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
		ir_stream.Tunion.Tstruct.bidxEndWord2=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);			// the wait POINTER of 1'st word(word 단위)
    	ir_stream.Tunion.Tstruct.bidxEndWord1=ir_stream.Tunion.Tstruct.bidxEndWord2;        				// the wait POINTER of 1'st word(word 단위)
	}

	else if(NormalTransport==2)	//2-2-2-2
	{
		npWaddr_1=(WORD*) &ir_stream.Tunion.Tstruct.pulse.BitStream[0];
    	*npWaddr_1=0x8000;

    	uWord12=ramFormat.strMember.word1;

		if(ramFormat.strMember.customsize.custom == 8)
		{
			ramCode.Custom=cust>>8;
			ramCode.Custom&=0xff;
		}
		else if(ramFormat.strMember.customsize.custom == 16)
		{
			ramCode.Custom=cust>>16;
			ramCode.Custom&=0xffff;
		}
		ramCode.Data=data1>>8;
		ramCode.Data&=0xff;
		npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);
		if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
		{
	        if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
			*npWaddr_1=0;
		}
		//*npWaddr_1+=(uWord12+17);                 // ??                        // END 2 word
		*npWaddr_1+=(uWord12);                 // ??                        // END 2 word

    	ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
		ir_stream.Tunion.Tstruct.bidxEndWord2=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);			// the wait POINTER of 1'st word(word 단위)
    	ir_stream.Tunion.Tstruct.bidxEndWord1=ir_stream.Tunion.Tstruct.bidxEndWord2;        				// the wait POINTER of 1'st word(word 단위)

	}

	else if(NormalTransport==3)									// normal 1-2-2-2
	{
		ir_stream.Tunion.Tstruct.bidxRepeatWord=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];

    	//*npWaddr_1-=78;                                     //13.07.19 1st word adjust in 2nd word 

		npWaddr_1++;
		*npWaddr_1=0x8000;

		uWord12=ramFormat.strMember.word2;                          // word계산.
		if(ramFormat.strMember.customsize.custom == 8)
		{
			ramCode.Custom=cust>>8;
			ramCode.Custom&=0xff;
		}
		else if(ramFormat.strMember.customsize.custom == 16)
		{
			ramCode.Custom=cust>>16;
			ramCode.Custom&=0xffff;
		}
		ramCode.Data=data1>>8;
		ramCode.Data&=0xff;
		npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);
		if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
		{
	        if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
			*npWaddr_1=0;
		}
		//*npWaddr_1+=(uWord12+17);                                          // END 2 word
		*npWaddr_1+=(uWord12);                                          // END 2 word

		ir_stream.Tunion.Tstruct.bidxEndWord2=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);			// the wait POINTER of 1'st word(word 단위)
    	ir_stream.Tunion.Tstruct.bidxEndWord1=ir_stream.Tunion.Tstruct.bidxEndWord2;        				// the wait POINTER of 1'st word(word 단위)
	}

    ramCode.Custom=cust;
    ramCode.Data=data1;
    ramFormat.strMember.outorder[0]=0xf5;

    return TRUE;
}
#else
BOOL  func_SPECIAL_F5(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0F6H == TRUE)
//ANYCARRIER
BOOL  func_SPECIAL_F6(void)
{
	ir_stream.Tunion.Tstruct.Carrier=(int8u)ramCode.Data;       	// bCarrier

	*npWaddr_1=0x8000;
	*npWaddr_1+=2500;                        // High구간.
	npWaddr_1++;

	*npWaddr_1=ramFormat.strMember.word2;                         // low 구간.

	ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];        // the wait POINTER of 1'st word(word 단위)
	ir_stream.Tunion.Tstruct.bidxEndWord1--;                                     // HIGH 구간만.
	ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
	ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;  // the wait POINTER of 1'st word

	return TRUE;
}
#else
BOOL  func_SPECIAL_F6(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0F7H == TRUE)
void	func_SPECIAL_F7_Fill_Logic(void)
{
	int16u  data1;
	int8u	i;
	int8u	BitData;

	data1=(int16u)ramCode.Data;

	for(i=0; i<16; i++)
	{
		BitData=(int8u)(data1&0x01);
		data1>>=1;

		if(i%2)														//low envelop		(1,3,5,...)
		{
			if(BitData)											//bit 1
				*npWaddr_1=(ramFormat.strMember.logic1[0]);
			else														//bit 0
				*npWaddr_1=(ramFormat.strMember.logic0[0]);
		}
		else															//high envelop 	(0,2,4,...)
		{
			if(BitData)											//bit 1
				*npWaddr_1=(ramFormat.strMember.logic1[0]+0x8000);
			else														//bit 0
				*npWaddr_1=(ramFormat.strMember.logic0[0]+0x8000);
		}
		npWaddr_1++;
	}
}

//PANSONIC ZAP
BOOL func_SPECIAL_F7(void)
{
	//lead
	*npWaddr_1=(ramFormat.strMember.lead[0]+0x8000);
	npWaddr_1++;
	*npWaddr_1=ramFormat.strMember.lead[1];
	npWaddr_1++;

	//1'st frame
	func_SPECIAL_F7_Fill_Logic();

	//end stream
	*npWaddr_1=(ramFormat.strMember.end_pulse+0x8000);
	npWaddr_1++;
	*npWaddr_1=ramFormat.strMember.word1;
	ir_stream.Tunion.Tstruct.bidxEndWord1=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);

	npWaddr_1++;
	ir_stream.Tunion.Tstruct.bidxRepeatWord=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);

	//lead
	*npWaddr_1=(ramFormat.strMember.lead[2]+0x8000);
	npWaddr_1++;
	*npWaddr_1=ramFormat.strMember.lead[1];
	npWaddr_1++;

	//2'nd frame
	func_SPECIAL_F7_Fill_Logic();

	//end stream
	*npWaddr_1=(ramFormat.strMember.end_pulse+0x8000);
	npWaddr_1++;

	*npWaddr_1=ramFormat.strMember.word2;
	ir_stream.Tunion.Tstruct.bidxEndWord2=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);

	return TRUE;
}
#else
BOOL  func_SPECIAL_F7(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0F8H == TRUE)
//two carrier
BOOL func_SPECIAL_F8(void)
{
    UWORD	uWord12;
    ULONG	ulIndex;

    ulIndex=ramCode.Data;
    ulIndex>>=(ramFormat.strMember.datasize.data1-8);
	ramFormat.strMember.datasize.data1 = ulIndex;

    ramFormat.strMember.outorder[0]=0x13;                       // S() +D()

    uWord12=ramFormat.strMember.word1;                          // word계산.

    npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);

    if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
    {
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
		*npWaddr_1=0;
    }

	uWord12=ramFormat.strMember.word1;
	*npWaddr_1+=uWord12;

    ir_stream.Tunion.Tstruct.bidxEndWord1=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);         // the wait POINTER of 1'st word(word 단위)
    ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
    ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;                  // the wait POINTER of 1'st word

	ramFormat.strMember.option1.flash=1;
	ramFormat.strMember.outorder[0]=0xf8;

	return	TRUE;
}
#else
BOOL func_SPECIAL_F8(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0F9H == TRUE)
//ONKYO FORMAT(MUTICUSTOM & REPEAT,CONTINUOUS)
BOOL func_SPECIAL_F9(void)
{
	UWORD   uWord12;
	ULONG   ulIndex;

	ulIndex=ramCode.Data;
	ulIndex>>=ramFormat.strMember.datasize.data1;
	ulIndex&=(ULONG)pbMaskIndex[ramFormat.strMember.datasize.index1+ramFormat.strMember.customsize.index2-1];

	ramFormat.strMember.outorder[2]=0xff;

	uWord12=ramFormat.strMember.word1;                          // word계산.

	ramFormat.strMember.outorder[0]=0x01;                       // L + S(8) +D(24) + E
	ramFormat.strMember.outorder[1]=0x35;

	npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);

	if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
	{
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
		*npWaddr_1=0;
	}
	*npWaddr_1+=uWord12;                                          // END 1 word

	ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];        // the wait POINTER of 1'st word(word 단위)

	if(ulIndex==0)                                              // repeat pulse
	{
		npWaddr_1++;
		*npWaddr_1=0x8000;

		ir_stream.Tunion.Tstruct.bidxRepeatWord=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
		uWord12=ramFormat.strMember.word2;                          // word계산.

		ramFormat.strMember.outorder[0]=0xaf;                   // A
		npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);
		if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
		{
			if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
			*npWaddr_1=0;
		}
		*npWaddr_1+=uWord12;                                          // END 2 word

		ir_stream.Tunion.Tstruct.bidxEndWord2=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
	}
	else
	{
		ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
		ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;                  // the wait POINTER of 1'st word
	}

	ramFormat.strMember.outorder[0]=0xf9;

	return TRUE;
}
#else
BOOL  func_SPECIAL_F9(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0FAH == TRUE)
//nec 1word - 2word - repeat
BOOL func_SPECIAL_FA(void)
{
	UWORD	uWord12;

	ramFormat.strMember.outorder[0]=0x01;                       // L + S(32) + E
	ramFormat.strMember.outorder[1]=0x5f;

	uWord12=ramFormat.strMember.word1;                          // word계산.

	npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);

	if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
	{
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
		*npWaddr_1=0;
	}
	*npWaddr_1+=uWord12;                                          // END 0.5 word

	npWaddr_1++;
	*npWaddr_1=0x8000;

	uWord12=ramFormat.strMember.word2;                          // word계산.
	ramFormat.strMember.outorder[0]=0x03;                       // L + D(32) + E
	ramFormat.strMember.outorder[1]=0x5f;
	npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);
	if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
	{
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
		*npWaddr_1=0;
	}
	*npWaddr_1+=uWord12;                                          // END 1 word

	ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];        // the wait POINTER of 1'st word(word 단위)

	npWaddr_1++;
	*npWaddr_1=0x8000;

	ir_stream.Tunion.Tstruct.bidxRepeatWord=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];

	uWord12=ramFormat.strMember.word2;                          // word계산.
	ramFormat.strMember.outorder[0]=0x0af;                      // R
	npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);
	if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
	{
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
		*npWaddr_1=0;
	}
	*npWaddr_1+=uWord12;                                          // END 2 word

	ir_stream.Tunion.Tstruct.bidxEndWord2=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];        // the wait POINTER of 2'nd word

	ramFormat.strMember.outorder[0]=0xfa;

  return TRUE;
}
#else
BOOL  func_SPECIAL_FA(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0FBH == TRUE)
//nec repeat & continue
BOOL func_SPECIAL_FB(void)
{
    UWORD	uWord12;
    ULONG	ulIndex;

    ulIndex=ramCode.Data;
    ulIndex>>=ramFormat.strMember.datasize.data1;
    ulIndex&=(ULONG)pbMaskIndex[ramFormat.strMember.datasize.index1+ramFormat.strMember.customsize.index2-1];

    ramFormat.strMember.outorder[0]=0x01;                       // L + S(16) +D(16) + E
    ramFormat.strMember.outorder[1]=0x35;
    ramFormat.strMember.outorder[2]=0xff;

    uWord12=ramFormat.strMember.word1;                          // word계산.

    npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);

    if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
    {
        if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
        *npWaddr_1=0;
    }
    *npWaddr_1+=uWord12;                                          // END 1 word

    ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];        // the wait POINTER of 1'st word(word 단위)

    if(ulIndex==0)                                              // repeat pulse
    {
        npWaddr_1++;
        *npWaddr_1=0x8000;

        ir_stream.Tunion.Tstruct.bidxRepeatWord=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
        uWord12=ramFormat.strMember.word2;                          // word계산.

        ramFormat.strMember.outorder[0]=0xaf;                   // A
        npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);
        if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
        {
            if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
            *npWaddr_1=0;
        }
        *npWaddr_1+=uWord12;                                          // END 2 word

        ir_stream.Tunion.Tstruct.bidxEndWord2=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
    }
    else
    {
        ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
        ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;                  // the wait POINTER of 1'st word
    }

    ramFormat.strMember.outorder[0]=0xfb;

    return TRUE;
}
#else
BOOL  func_SPECIAL_FB(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0FCH == TRUE)
//PATTERN112_ 1word - 1word - repeat
BOOL func_SPECIAL_FC(void)
{
	UWORD   uWord12;

	ramFormat.strMember.outorder[0]=0x03;                       // L + D(24) + E
	ramFormat.strMember.outorder[1]=0x5f;

	uWord12=ramFormat.strMember.word1;                          // word계산.

	npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);

	if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
	{
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
		*npWaddr_1=0;
	}
	*npWaddr_1+=uWord12;                                          // END 0.5 word

	npWaddr_1++;
	*npWaddr_1=0x8000;

	uWord12=ramFormat.strMember.word2;                          // word계산.
																															// L + D(24) + E
	npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);
	if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
	{
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
		*npWaddr_1=0;
	}
	*npWaddr_1+=uWord12;                                          // END 1 word

	ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];        // the wait POINTER of 1'st word(word 단위)

	npWaddr_1++;
	*npWaddr_1=0x8000;

	ir_stream.Tunion.Tstruct.bidxRepeatWord=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];

	uWord12=ramFormat.strMember.word2;                          // word계산.
	ramFormat.strMember.outorder[0]=0xaf;                      // R
	npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);
	if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
	{
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
		*npWaddr_1=0;
	}
	*npWaddr_1+=uWord12;                                          // END 1 word

	ir_stream.Tunion.Tstruct.bidxEndWord2=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];        // the wait POINTER of 2'nd word

	ramFormat.strMember.outorder[0]=0xfc;

  return TRUE;
}
#else
BOOL  func_SPECIAL_FC(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0FDH == TRUE)
//nec repeat & continue
//16A(L + D + D_bar + C + C') REPEAT & CONTINUOUS
BOOL func_SPECIAL_FD(void)
{
    UWORD	uWord12;
    ULONG	ulIndex;

    ulIndex=ramCode.Data;
    ulIndex>>=ramFormat.strMember.datasize.data1;
    ulIndex&=(ULONG)pbMaskIndex[ramFormat.strMember.datasize.index1+ramFormat.strMember.customsize.index2-1];

    ramFormat.strMember.outorder[0]=0x03;                       // L + S(16) +D(16) + E
    ramFormat.strMember.outorder[1]=0x41;
    ramFormat.strMember.outorder[2]=0x5f;

    uWord12=ramFormat.strMember.word1;                          // word계산.

    npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);

    if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
    {
        if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
        *npWaddr_1=0;
    }
    *npWaddr_1+=uWord12;                                          // END 1 word

    ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];        // the wait POINTER of 1'st word(word 단위)

    if(ulIndex==0)                                              // repeat pulse
    {
        npWaddr_1++;
        *npWaddr_1=0x8000;

        ir_stream.Tunion.Tstruct.bidxRepeatWord=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
        uWord12=ramFormat.strMember.word2;                          // word계산.

        ramFormat.strMember.outorder[0]=0xaf;                   // A
        npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);
        if(*npWaddr_1&0x8000)                                         // Ending the 1'st Word
        {
            if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;
            *npWaddr_1=0;
        }
        *npWaddr_1+=uWord12;                                          // END 2 word

        ir_stream.Tunion.Tstruct.bidxEndWord2=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
    }
    else
    {
        ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
        ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;                  // the wait POINTER of 1'st word
    }

    ramFormat.strMember.outorder[0]=0xfd;

    return TRUE;
}
#else
BOOL  func_SPECIAL_FD(void)
{
	return	FALSE;
}
#endif

#if (USE_SPECIAL_0FEH_3 == TRUE)
//Metz Three low..
BOOL func_SPECIAL_E3(void)
{
	int16u   		i;
	unsigned long	ulTemp;
	unsigned short  uWord12;

	//Order : L + S + D + E
	uWord12=ramFormat.strMember.word1;                      			// word계산.
	//LEAD
	*npWaddr_1+=ramFormat.strMember.end_pulse;          			// high
	uWord12-=(*npWaddr_1)&0x7fff;
	npWaddr_1++;

	*npWaddr_1=ramFormat.strMember.lead[0];                  			// lead
	uWord12-=(*npWaddr_1)&0x7fff;
	npWaddr_1++;

	*npWaddr_1+=0x8000+ramFormat.strMember.end_pulse;          			// high
	uWord12-=(*npWaddr_1)&0x7fff;
	npWaddr_1++;

	*npWaddr_1=ramFormat.strMember.lead[1];                  			// lead
	uWord12-=(*npWaddr_1)&0x7fff;
	npWaddr_1++;

	*npWaddr_1+=0x8000+ramFormat.strMember.end_pulse;          			// high
	uWord12-=(*npWaddr_1)&0x7fff;
	npWaddr_1++;

	*npWaddr_1=ramFormat.strMember.lead[2];                  			// lead
	uWord12-=(*npWaddr_1)&0x7fff;
	npWaddr_1++;

	//CUSTOM
	ulTemp=ramCode.Custom;
	for(i=0;i<ramFormat.strMember.customsize.custom;i+=2)
	{
		*npWaddr_1=0x8000+ramFormat.strMember.end_pulse+1;     			// high
		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;

#if 0
		*npWaddr_1=ramFormat.strMember.timeval[1+(ulTemp&0x03)];  	//Three Low
#else
		switch(ulTemp&0x03)
		{
			case	0:
				*npWaddr_1=ramFormat.strMember.timeval[1];  	//Three Low
				break;
			case	1:
				*npWaddr_1=ramFormat.strMember.timeval[3];  	//Three Low
				break;
			case	2:
				*npWaddr_1=ramFormat.strMember.timeval[2];  	//Three Low
				break;
			case	3:
				*npWaddr_1=ramFormat.strMember.timeval[4];  	//Three Low
				break;
		}
#endif

		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;
		ulTemp>>=2;
	}

	//DATA
	ulTemp=ramCode.Data;                                    			// data

#if 1
	switch(ramFormat.strMember.timeval[0])					//option
	{
		case	0:											//no toggle
			break;
		case	1:											//2ea toggle
			ulTemp=ulTemp&(ULONG)~0xf0000000;
			switch(gl_Toggle[curr_source]&0x01)
			{
				case	0:
					ulTemp=ulTemp|0x80000000;
					break;
				case	1:
					ulTemp=ulTemp|0xd0000000;
					break;
			}
			break;
		case	2:											//4ea toggle
			ulTemp=ulTemp&(ULONG)~0xff000000;
			switch(gl_Toggle[curr_source]&0x03)
			{
				case	0:
					ulTemp=ulTemp|0x88000000;
					break;
				case	1:
					ulTemp=ulTemp|0xdd000000;
					break;
				case	2:
					ulTemp=ulTemp|0x8d000000;
					break;
				case	3:
					ulTemp=ulTemp|0xd8000000;
					break;
			}
			break;
		case	3:											//4ea toggle
			ulTemp=ulTemp&(ULONG)~0xff000000;
			switch(gl_Toggle[curr_source]&0x03)
			{
				case	0:
					ulTemp=ulTemp|0x44000000;
					break;
				case	1:
					ulTemp=ulTemp|0xee000000;
					break;
				case	2:
					ulTemp=ulTemp|0x4e000000;
					break;
				case	3:
					ulTemp=ulTemp|0xe4000000;
					break;
			}
			break;
	}
#endif

	for(i=0;i<ramFormat.strMember.datasize.data1;i+=2)
	{
		*npWaddr_1=0x8000+ramFormat.strMember.end_pulse+1;     			// high
		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;

#if 0
		*npWaddr_1=ramFormat.strMember.timeval[1+(ulTemp&0x03)];  	//Three Low
#else
		switch(ulTemp&0x03)
		{
			case	0:
				*npWaddr_1=ramFormat.strMember.timeval[1];  	//Three Low
				break;
			case	1:
				*npWaddr_1=ramFormat.strMember.timeval[3];  	//Three Low
				break;
			case	2:
				*npWaddr_1=ramFormat.strMember.timeval[2];  	//Three Low
				break;
			case	3:
				*npWaddr_1=ramFormat.strMember.timeval[4];  	//Three Low
				break;
		}
#endif

		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;
		ulTemp>>=2;
	}
	//end..
	*npWaddr_1=0x8000+ramFormat.strMember.end_pulse+1;     				// high
	uWord12-=(*npWaddr_1)&0x7fff;
	npWaddr_1++;

	*npWaddr_1+=uWord12;                                          // END 1 word

	ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]; // the wait POint16uER of 1'st word(word 단위)
	ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
	ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;                  // the wait POint16uER of 1'st word

	return TRUE;
}

#else

BOOL func_SPECIAL_E3()
{
	return FALSE;
}

#endif


#if (USE_SPECIAL_0FEH_4 == TRUE)
WORD* func_SPECIAL_BnO_SubBitStream(int16u data1, int8u size, int8u	type)
{
	int8u	prev_bit=0xff;
	int8u	i;

	if(type)																									//data
	{
		int16u	cust=(int16u)ramCode.Custom;

		cust>>=9;
		if(cust&0x01)
			prev_bit=1;
		else
			prev_bit=0;
	}

	for(i=0; i<size; i++)
	{
		if(data1&0x01)
		{
			*npWaddr_1++=(ramFormat.strMember.end_pulse+0x8000);	//high 30us

			if(prev_bit==1)																				//same bit
			{
				*npWaddr_1=ramFormat.strMember.repeat[0];
				*npWaddr_1<<=8;
				*npWaddr_1|=ramFormat.strMember.repeat[1];
			}
			else
			{
				*npWaddr_1=ramFormat.strMember.logic1[0];
				*npWaddr_1<<=8;
				*npWaddr_1|=ramFormat.strMember.logic1[1];
			}
			npWaddr_1++;
			prev_bit=1;
		}
		else
		{
			*npWaddr_1++=(ramFormat.strMember.end_pulse+0x8000);	//high 30us

			if(!prev_bit)																					//same bit
			{
				*npWaddr_1=ramFormat.strMember.repeat[0];
				*npWaddr_1<<=8;
				*npWaddr_1|=ramFormat.strMember.repeat[1];
			}
			else
			{
				*npWaddr_1=ramFormat.strMember.logic0[0];
				*npWaddr_1<<=8;
				*npWaddr_1|=ramFormat.strMember.logic0[1];
			}
			npWaddr_1++;
			prev_bit=0;
		}
		data1>>=1;
	}
	return	npWaddr_1;
}

WORD* func_Special_BnO_1stFrameFillStream()
{
	int16u	data1;

	*npWaddr_1++=(ramFormat.strMember.end_pulse+0x8000);			//high 30us
	*npWaddr_1=ramFormat.strMember.logic0[0];
	*npWaddr_1<<=8;
	*npWaddr_1|=ramFormat.strMember.logic0[1];								//lead 1'st 3ms
	npWaddr_1++;

	*npWaddr_1++=(ramFormat.strMember.end_pulse+0x8000);			//high 30us
	*npWaddr_1=ramFormat.strMember.logic0[0];
	*npWaddr_1<<=8;
	*npWaddr_1|=ramFormat.strMember.logic0[1];								//lead 2'nd 3ms
	npWaddr_1++;

	*npWaddr_1++=(ramFormat.strMember.end_pulse+0x8000);			//high 30us
	*npWaddr_1=ramFormat.strMember.lead[0];
	*npWaddr_1<<=8;
	*npWaddr_1|=ramFormat.strMember.lead[1];									//lead 3'rd 15.4ms
	npWaddr_1++;

	data1=(int16u)ramCode.Custom;															//custom 9bit
	npWaddr_1=func_SPECIAL_BnO_SubBitStream(data1, 9, 0);

	data1=(int16u)ramCode.Data;																//data 8bit
	npWaddr_1=func_SPECIAL_BnO_SubBitStream(data1, 8, 1);

	*npWaddr_1++=(ramFormat.strMember.end_pulse+0x8000);			//high 30us
	*npWaddr_1=ramFormat.strMember.timeval[0];
	*npWaddr_1<<=8;
	*npWaddr_1|=ramFormat.strMember.timeval[1];								//end bit 12.4ms
	npWaddr_1++;

	*npWaddr_1++=(ramFormat.strMember.end_pulse+0x8000);			//high 30us

	return	npWaddr_1;
}

//Bang & Olufsen 455KHz
BOOL func_SPECIAL_E4()
{
	int16u	data1;

	npWaddr_1=func_Special_BnO_1stFrameFillStream();

	data1=(int16u)ramCode.Data;
	data1>>=8;

	if(data1&0xff)																		//index 1	-> continous ~~
	{
		*npWaddr_1++=ramFormat.strMember.word1;				//1'st end
		*npWaddr_1++=ramFormat.strMember.word1;				//1'st end
		*npWaddr_1=ramFormat.strMember.word2;					//1'st end

		ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];

		npWaddr_1++;
		ir_stream.Tunion.Tstruct.bidxRepeatWord=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];

		npWaddr_1=func_Special_BnO_1stFrameFillStream();
		npWaddr_1--;

		npWaddr_1--;		//add 2012

		ir_stream.Tunion.Tstruct.bidxEndWord2=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
	}
	else																						//index 0	-> single 1frame
	{
		*npWaddr_1++=ramFormat.strMember.word1;				//1'st end (0x7fff)
		*npWaddr_1++=ramFormat.strMember.word1;				//1'st end (0x7fff)
		*npWaddr_1=ramFormat.strMember.word2;					//1'st end (0x451e)

		ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0];
		ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;
		ir_stream.Tunion.Tstruct.bidxRepeatWord=ir_stream.Tunion.Tstruct.bidxEndWord1;
	}
	return TRUE;
}

#else

BOOL func_SPECIAL_E4()
{
	return FALSE;
}
#endif


#if (USE_SPECIAL_0FEH_5 == TRUE)
//Three low..
BOOL func_SPECIAL_E5(void)
{
	int16u   		i;
	unsigned long	ulTemp;
	unsigned short  uWord12;

	Three_low_keyoff=0;

	//Order : L + S + D + E
	uWord12=ramFormat.strMember.word1;                      			// word계산.
	//LEAD
	*npWaddr_1+=ramFormat.strMember.lead[0]+1;               			// high
	uWord12-=(*npWaddr_1)&0x7fff;
	npWaddr_1++;

	*npWaddr_1=ramFormat.strMember.lead[1];                  			// lead
	uWord12-=(*npWaddr_1)&0x7fff;
	npWaddr_1++;

#if 0
	if((!key_cnt)&&(ramFormat.strMember.timeval[0]==0))
	{
    	ramCode.Custom=(ramCode.Custom|0x00650000);
	}
#endif

	//CUSTOM
	ulTemp=ramCode.Custom;
	for(i=0;i<ramFormat.strMember.customsize.custom;i+=2)
	{
		*npWaddr_1=0x8000+ramFormat.strMember.end_pulse+1;     			// high
		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;

#if 0
		*npWaddr_1=ramFormat.strMember.timeval[1+(ulTemp&0x03)];  	//Three Low
#else
		switch(ulTemp&0x03)
		{
			case	0:
				*npWaddr_1=ramFormat.strMember.timeval[1];  	//Three Low
				break;
			case	1:
				*npWaddr_1=ramFormat.strMember.timeval[3];  	//Three Low
				break;
			case	2:
				*npWaddr_1=ramFormat.strMember.timeval[2];  	//Three Low
				break;
			case	3:
				*npWaddr_1=ramFormat.strMember.timeval[4];  	//Three Low
				break;
		}
#endif

		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;
		ulTemp>>=2;
	}

#if 0
	switch(ramFormat.strMember.timeval[0])					//option
	{
		case	0:
			Three_low_keyoff=1;
			break;
		case	1:
			*npWaddr_1=0x8000+ramFormat.strMember.end_pulse+1;     			// high
			uWord12-=(*npWaddr_1)&0x7fff;
			npWaddr_1++;

			switch(gl_Toggle[curr_source]&0x01)
			{
				case	0:
					*npWaddr_1=ramFormat.strMember.timeval[1];  	//Three Low
					break;
				case	1:
					*npWaddr_1=ramFormat.strMember.timeval[3];  	//Three Low
					break;
			}
			npWaddr_1++;
			break;
		case	2:
			*npWaddr_1=0x8000+ramFormat.strMember.end_pulse+1;     			// high
			uWord12-=(*npWaddr_1)&0x7fff;
			npWaddr_1++;

			switch(gl_Toggle[curr_source]&0x03)
			{
				case	0:
					*npWaddr_1=ramFormat.strMember.timeval[1];  	//Three Low
					break;
				case	1:
					*npWaddr_1=ramFormat.strMember.timeval[3];  	//Three Low
					break;
				case	2:
					*npWaddr_1=ramFormat.strMember.timeval[2];  	//Three Low
					break;
				case	3:
					*npWaddr_1=ramFormat.strMember.timeval[4];  	//Three Low
					break;
			}
			npWaddr_1++;
			break;
	}

	DATA
	ulTemp=ramCode.Data;                                    			// data

#else
	//DATA
	ulTemp=ramCode.Data;                                    			// data

	switch(ramFormat.strMember.timeval[0])					//option
	{
		case	0:											//key off toggle
			if(!key_cnt)
			{
				switch(ulTemp&0x01)
				{
					case	0:
						ulTemp=ulTemp|0x01;
						break;
					case	1:
						ulTemp=ulTemp&~0x01;
						break;
				}
				//for(i=0;i<3000;i++) wait_10us();
			}
			else
				Three_low_keyoff=1;
			break;
		case	1:											//2ea toggle
			if(gl_Toggle[curr_source]&0x01)
			{
				switch(ulTemp&0x01)
				{
					case	0:
						ulTemp=ulTemp|0x01;
						break;
					case	1:
						ulTemp=ulTemp&~0x01;
						break;
				}
			}
			break;
		case	2:											//4ea toggle
			ulTemp=ulTemp&~0x03;
			switch(gl_Toggle[curr_source]&0x03)
			{
				case	0:
					break;
				case	1:
					ulTemp=ulTemp|0x01;
					break;
				case	2:
					ulTemp=ulTemp|0x02;
					break;
				case	3:
					ulTemp=ulTemp|0x03;
					break;
			}
			break;
		case	3:											//no toggle
			break;
	}

#endif

	for(i=0;i<ramFormat.strMember.datasize.data1;i+=2)
	{
		*npWaddr_1=0x8000+ramFormat.strMember.end_pulse+1;     			// high
		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;

#if 0
		*npWaddr_1=ramFormat.strMember.timeval[1+(ulTemp&0x03)];  	//Three Low
#else
		switch(ulTemp&0x03)
		{
			case	0:
				*npWaddr_1=ramFormat.strMember.timeval[1];  	//Three Low
				break;
			case	1:
				*npWaddr_1=ramFormat.strMember.timeval[3];  	//Three Low
				break;
			case	2:
				*npWaddr_1=ramFormat.strMember.timeval[2];  	//Three Low
				break;
			case	3:
				*npWaddr_1=ramFormat.strMember.timeval[4];  	//Three Low
				break;
		}
#endif

		uWord12-=(*npWaddr_1)&0x7fff;
		npWaddr_1++;
		ulTemp>>=2;
	}
	//end..
	*npWaddr_1=0x8000+ramFormat.strMember.end_pulse+1;     				// high
	uWord12-=(*npWaddr_1)&0x7fff;
	npWaddr_1++;

	*npWaddr_1+=uWord12;                                          // END 1 word

	ir_stream.Tunion.Tstruct.bidxEndWord1=npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]; // the wait POint16uER of 1'st word(word 단위)
	ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
	ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;                  // the wait POint16uER of 1'st word

	return TRUE;
}

#else

BOOL func_SPECIAL_E5()
{
	return FALSE;
}
#endif


#if (USE_SPECIAL_0FEH_8 == TRUE)
#if 0
//하위 4bit return
int8u func_SPECIAL_E8_CHKSUM(ULONG *  pudata)
{
	//ULONG *		pu=0;
	int8u		i,j;
	int8u		sum;

	//pu = pdata;
	sum = 0;

	for(i = 0 ; i < 6 ; i++)
	{
		j = (int8u)((ULONG)(*pudata>>(i*4)) & 0x0000000f);
		sum += (int8u)j;
	}

	sum =(int8u)(~sum);		//subtract 256

	sum &=0x0f;		//16 mod

	return sum;

}
#endif

enum SPECIAL_FORMAT_E8_POINT_TAG
{
	FMT_HIGH_PT =0,
	FMT_LOW0_PT =1,
	FMT_LOW1_PT =2,
	FMT_LOW2_PT =3,
	FMT_LOW3_PT =4,
	FMT_LOW4_PT =5,
	FMT_LOW5_PT =6,
	FMT_LOW6_PT =7,
	FMT_LOW7_PT =8,
	FMT_LOW8_PT =9,
	FMT_LOW9_PT =10,
	FMT_LOW10_PT =11,
	FMT_LOW11_PT =12,
	FMT_LOW12_PT =13,
	FMT_LOW13_PT =14,
	FMT_LOW14_PT =15,
	FMT_LOW15_PT =16,
	FMT_CUST_PAUSE_PT =17,
	FMT_WORD1_PT =18,
	FMT_WORD2_PT =20
} SPECIAL_FORMAT_E8_POINT;



//format : XMP-1 -2010. 7. 6.
BOOL func_SPECIAL_E8()
{
	//int8u		repeat_f=0;
	int8u *		pfmt;
	int8u		i;
	int8u		j;
	WORD		w_cal_wordtime;
	//ULONG		utmp;
	WORD *		pStart1;
	WORD *		pEnd1;
	WORD *		pStart2;
	WORD *		pEnd2;
	WORD *		p;

	//convert LSB first
	Convert_cd(&ramCode.Data);
	Convert_cd(&ramCode.Custom);

#if 0
	//set repeat_F
	j = (int8u)(ramCode.Data>>28 & 0x0f);
	if(j == 0xf)
	{
		ramCode.Data &= ~0xF0000000;
		ramCode.Data |= 0x10000000;
		repeat_f = 1;
	}
	else
	{
		ramCode.Data &= ~0xF0000000;
		ramCode.Data |= 0x10000000;
		repeat_f = 0;
	}
#endif

	//load carrier
	ir_stream.Tunion.Tstruct.Carrier = (WORD)ramCode.bCarrier;

	//init point.
	pStart1 = &ir_stream.Tunion.Tstruct.pulse.BitStream[0];
	p=npWaddr_1;

	//load custom bitstream
	//pfmt = (byte*)&ramFormat.strMember.outorder[0];
	pfmt = (byte*)&ramFormat.strMember.logic1[0];
	for(i = 0 ; i < 8 ; i++)
	{
		*p = pfmt[FMT_HIGH_PT];
		*p +=  0x8000;	//high
		p++;

		j = (ramCode.Custom>>(28 - (4*i))) & 0x0F;			//상위 4bit low.
		*p=((WORD)pfmt[FMT_LOW0_PT + j]);
		p++;
	}

	//end bit
	*p=((WORD)pfmt[FMT_HIGH_PT]+0x8000);	//high
	p++;

	//load custom pause
	//p--;

#if 0
	*p = ((WORD)pfmt[FMT_CUST_PAUSE_PT] + 400); //pause time : 12.897ms, 38khz.
#else
	*p = ((WORD)pfmt[FMT_CUST_PAUSE_PT]*5); //pause time : 12.897ms, 38khz.
#endif
	p++;

	//load data bitstream
	for(i = 0 ; i < 8 ; i++)
	{
		*p=((WORD)pfmt[FMT_HIGH_PT]+0x8000);	//high
		p++;

		j = (ramCode.Data>>(28 - (4*i))) & 0x0F;			//상위 4bit low.
		*p=((WORD)pfmt[FMT_LOW0_PT + j]);
		p++;
	}

	//end bit
	*p=((WORD)pfmt[FMT_HIGH_PT]+0x8000);	//high
	p++;

	//calculate end_word time.
	//pw = (WORD*)pfmt[FMT_WORD1_PT];
	//w_cal_wordtime = *pw;		//1word
	w_cal_wordtime = ramFormat.strMember.word1;

	j = p-pStart1;

	for(i = 0 ; i < j ; i++)
	{
		w_cal_wordtime -= (pStart1[i]&0x7fff) ;

	}
	//p--;
	*p = w_cal_wordtime;
	p++;

#if 0
	if(repeat_f == 0)
	{
		ir_stream.Tunion.Tstruct.bidxEndWord1 = p-pStart1-1;
		ir_stream.Tunion.Tstruct.bidxEndWord2 = p-pStart1-1;
		ir_stream.Tunion.Tstruct.bidxRepeatWord = p-pStart1-1;
		*p=0;
		return TRUE	;
	}
#endif

//2dn word
	pStart2 = p;
	pEnd1 = p-1;

	//load custom bitstream
	for(i = 0 ; i < 8 ; i++)
	{
		*p=((WORD)pfmt[FMT_HIGH_PT]+0x8000);	//high
		p++;

		j = (ramCode.Custom>>(28-(i*4)))&0x0F;			//상위 4bit low.
		*p=((WORD)pfmt[FMT_LOW0_PT + j]);
		p++;

	}

	//end bit
	*p=((WORD)pfmt[FMT_HIGH_PT]+0x8000);	//high
	p++;

	//custom pause
	//p--;
#if 0
	*p = ((WORD)pfmt[FMT_CUST_PAUSE_PT] + 400); //pause time : 12.897ms, 38khz.
#else
	*p = ((WORD)pfmt[FMT_CUST_PAUSE_PT]*5); //pause time : 12.897ms, 38khz.
#endif
	p++;

#if 0
	//load data bitstream
	for(i = 0 ; i < 8 ; i++)
	{
		if(i != 1 && i !=2)
		{
			*p=((WORD)pfmt[FMT_HIGH_PT]+0x8000);	//high
			p++;

			j = (ramCode.Data>>(28-(i*4))) & 0x0F;			//상위 4bit low.
			*p=((WORD)pfmt[FMT_LOW0_PT + j]);
			p++;

		}
		else if(i == 1)		//checksum.
		{
			*p=((WORD)pfmt[FMT_HIGH_PT]+0x8000);	//high
			p++;

			//세번째 nibble 8입력.
			ramCode.Data &=~0x00f00000;
			ramCode.Data |=0x00800000;
			j = func_SPECIAL_E8_CHKSUM((ULONG *)&ramCode.Data);	//상위 4bit low.

			//두번째 nibble checksum입력.
			ramCode.Data &=~0x0f000000;
			utmp = (ULONG)j;
			ramCode.Data |=utmp<<24;

			*p=((WORD)pfmt[FMT_LOW0_PT + j]);
			p++;
		}
		else if(i == 2)		//toggle
		{
			*p=((WORD)pfmt[FMT_HIGH_PT]+0x8000);	//high
			p++;

			j = 8;			//toggle 8.
			*p=((WORD)pfmt[FMT_LOW0_PT + j]);
			p++;
		}

	}
#else
	//ramCode.Data = ramCode.Data;
	if(ramCode.Data&0x00800000)
		ramCode.Data &=~0x00800000;
	else
		ramCode.Data |=0x00800000;
	if(ramCode.Data&0x08000000)
		ramCode.Data &=~0x08000000;
	else
		ramCode.Data |=0x08000000;

	//load data bitstream
	for(i = 0 ; i < 8 ; i++)
	{
		*p=((WORD)pfmt[FMT_HIGH_PT]+0x8000);	//high
		p++;

		j = (ramCode.Data>>(28 - (4*i))) & 0x0F;			//상위 4bit low.
		*p=((WORD)pfmt[FMT_LOW0_PT + j]);
		p++;
	}

#endif

	//end bit
	*p=((WORD)pfmt[FMT_HIGH_PT]+0x8000);	//high
	p++;

	//calculate end_word time.
	//pw = (WORD*)pfmt[FMT_WORD2_PT];
	//w_cal_wordtime = *pw;		//1word
	w_cal_wordtime = ramFormat.strMember.word2;

	j = p-pStart2;

	for(i = 0 ; i < j ; i++)
	{
		w_cal_wordtime -= (pStart2[i]&0x7fff) ;
	}
	//p--;

	*p = w_cal_wordtime;

	pEnd2 = p;

	ir_stream.Tunion.Tstruct.bidxEndWord1 = pEnd1-pStart1;
	ir_stream.Tunion.Tstruct.bidxEndWord2 = pEnd2-pStart1;
	ir_stream.Tunion.Tstruct.bidxRepeatWord = pStart2-pStart1;
	p++;
	*p=0;


	return TRUE;
}

#else

BOOL func_SPECIAL_E8()
{
	return FALSE;
}
#endif


#if (USE_SPECIAL_0FEH_9 == TRUE)
//Canal toggle
// 1st frame : S(14) + Logic1 + D(8)
// next frame : S(14) + Logic0 + D(8)
BOOL func_SPECIAL_E9()
{
	unsigned short  uWord12;

    ramFormat.strMember.outorder[0]=0x18;                       // 1st frame : S(14) + Logic1 + D(8)
    ramFormat.strMember.outorder[1]=0x3f;

	uWord12=ramFormat.strMember.word1;                          			// word계산.
	npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);

	if(*npWaddr_1&0x8000)                                         		// Ending the 1'st Word
	{
		if(++npWaddr_1 >= (WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM] )
			npWaddr_1--;
		*npWaddr_1=0;
	}

	*npWaddr_1+=uWord12;
	ir_stream.Tunion.Tstruct.bidxEndWord1=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);         // the wait POINTER of 1'st word(word 단위)

	if(ramFormat.strMember.option1.repeat)
	{
	    ramFormat.strMember.outorder[0]=0x19;                       // next frame : S(14) + Logic0 + D(8)
	    ramFormat.strMember.outorder[1]=0x3f;

		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM]) npWaddr_1--;

		*npWaddr_1=0x8000;                                        			// repeat start //!!! 반드시 처음 pulse는 HIGH부터 !!!///
		ir_stream.Tunion.Tstruct.bidxRepeatWord=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);  // the wait POINTER of 1'st word

		uWord12=ramFormat.strMember.word2;                      				// word계산.
		npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);

		if(*npWaddr_1&0x8000)                                     			// Ending the 2'nd Word
		{
			if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM])
				npWaddr_1--;
			*npWaddr_1=0;
		}
		*npWaddr_1+=uWord12;
		ir_stream.Tunion.Tstruct.bidxEndWord2=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);
	}
	else
	{
		ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
		ir_stream.Tunion.Tstruct.bidxEndWord2=ir_stream.Tunion.Tstruct.bidxEndWord1;              // the wait POINTER of 1'st word
	}

	return TRUE;
}

#else

BOOL func_SPECIAL_E9()
{
	return FALSE;
}
#endif

#if (USE_SPECIAL_0FEH_A == TRUE)
//AMINO_AROSA
int16u  func_SPECIAL_EA(void)
{
    UWORD   uWord12;
    unsigned short    cust,data1,data2;

    cust=ramCode.Custom;
    data1=ramCode.Data;
    data2=ramCode.Data;
	data2&=0xff00;

    ramFormat.strMember.outorder[0]=0x01;
    ramFormat.strMember.outorder[1]=0x3f;

    uWord12=ramFormat.strMember.word1;                          // word계산.

    ramFormat.strMember.customsize.custom=16;                    //
    ramFormat.strMember.datasize.data1=16;

    npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);
	if(*npWaddr_1&0x8000)         // Ending the 2'nd Word
	{
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM])
			npWaddr_1--;
		*npWaddr_1=0;
	}
    *npWaddr_1+=(uWord12);                                          // END 1 word

	npWaddr_1++;
	*npWaddr_1=0x8000;

	ir_stream.Tunion.Tstruct.bidxRepeatWord=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);

	uWord12=ramFormat.strMember.word2;                          // word계산.
	ramCode.Custom=cust;
	ramCode.Custom|=0x0020;

	ramCode.Data=data1;

	switch(data2)
	{
		case	0x0000:
		case	0x1000:
		case	0x4000:
		case	0x5000:
		case	0x8000:
		case	0x9000:
		case	0xc000:
		case	0xd000:
			ramCode.Data|=0x2000;
			break;
		case	0x3000:
		case	0x7000:
		case	0xb000:
		case	0xf000:
			ramCode.Data&=~0x3000;
			break;
		case	0x2000:
		case	0x6000:
		case	0xa000:
		case	0xe000:
			ramCode.Data&=~0x2000;
			ramCode.Data|=0x1000;
			break;
		default:	
			break;
	}


	npWaddr_1=FillBitStreamFromOutOrder(0,(WORD*)npWaddr_1,(WORD*)&uWord12);
	if(*npWaddr_1&0x8000)         // Ending the 2'nd Word
	{
		if(++npWaddr_1>=(WORD*)&ir_stream.Tunion.Tstruct.pulse.BitStream[MAX_BITSTREAM])
			npWaddr_1--;
		*npWaddr_1=0;
	}
	*npWaddr_1+=(uWord12);                                          // END 2 word

    //ir_stream.Tunion.Tstruct.bidxRepeatWord=0;
	ir_stream.Tunion.Tstruct.bidxEndWord2=(npWaddr_1-&ir_stream.Tunion.Tstruct.pulse.BitStream[0]);			// the wait POINTER of 1'st word(word 단위)
    ir_stream.Tunion.Tstruct.bidxEndWord1=ir_stream.Tunion.Tstruct.bidxEndWord2;        				// the wait POINTER of 1'st word(word 단위)

    ramCode.Custom=cust;
    ramCode.Data=data1;

    return TRUE;
}
#else

BOOL func_SPECIAL_EA()
{
	return FALSE;
}
#endif

#if (USE_SPECIAL_0FEH_2 == TRUE)
BOOL func_SPECIAL_E2()
{
	return FALSE;
}
#else

BOOL func_SPECIAL_E2()
{
	return FALSE;
}
#endif

#if (USE_SPECIAL_0FEH_6 == TRUE)
BOOL func_SPECIAL_E6()
{
	return FALSE;
}
#else

BOOL func_SPECIAL_E6()
{
	return FALSE;
}
#endif

unsigned char function_special_Ext_call(void)
{
	unsigned char rtn = FALSE;
	
	switch(ramFormat.strMember.outorder[1]&0x0f)
	{
		case 0x2:
			rtn = func_SPECIAL_E2();	//DATA2_PREIOD
			break;

		case 0x3:
			rtn = func_SPECIAL_E3();	//Metz_Three_Low
			break;

		case 0x4:
			rtn = func_SPECIAL_E4();	//Bang & Olufsen 455KHz
			break;

		case 0x5:
			rtn = func_SPECIAL_E5();	//Three low..
			break;

		case 0x6:
			rtn = func_SPECIAL_E6();	//Shift_Bit
			break;

		case 0x8:
			rtn = func_SPECIAL_E8();	//xmp
			break;

		case 0x9:
			rtn = func_SPECIAL_E9();	//Canal
			break;

		case 0x0a:
			rtn = func_SPECIAL_EA();	////AMINO_AROSA
			break;

		default:
			rtn = FALSE;
			break;
	}

	return rtn;
}

BOOL func_SPECIAL_FE(void)
{
	return function_special_Ext_call();
}

#endif	//__SPECIAL_C__
