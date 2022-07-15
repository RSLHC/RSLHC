/**
  ******************************************************************************
  * @file      SC51F33XX.H
  * @author    MCU Application Team 
  * @version   V1.0.0
  * @date      12-Oct-2016
  * @brief     This file contains all HW registers definitions and memory mapping.
  *            History:
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Silan Microelectronics </center></h2>
  *
  * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
  * AS A RESULT, SLMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
  * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
  * CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
  * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED.
  * 
  ******************************************************************************
  */
#include "define.h"
#include "type_def.h"

#ifndef __SC51F33XX_H
#define __SC51F33XX_H


/** The prefix character "x" means the register is in x-ram */

/******************************************************************************/

/** @addtogroup IP_Registers
  * @{
  */
/** @addtogroup MCU_Core_Reg
  * @{
  */
sfr SP     = 0x81;    /*!< stack pointer                         */
sfr DPL    = 0x82;    /*!< data pointer low                      */
sfr DPH    = 0x83;    /*!< data pointer high                     */
sfr PSW    = 0xD0;    /*!< program status                        */
sfr ACC    = 0xE0;    /*!< accumulator                           */
sfr B      = 0xF0;    /*!< B                                     */
sfr MOVXRH = 0xA1;    /*!< Ri indirect adressing high byte       */
sfr AUXR1  = 0xA2;    /*!< DPTR switch of the dual DPTR register */

/*  PSW   */
sbit CY   = 0xD7;     /*!< carry flag bit                        */
sbit AC   = 0xD6;     /*!< auxiliary carry flag bit              */
sbit F0   = 0xD5;     /*!< user status flags                     */
sbit RS1  = 0xD4;     /*!< RS[1:0] 4 register groups option      */
sbit RS0  = 0xD3;     /*!< RS[1:0] 4 register groups option      */
sbit OV   = 0xD2;     /*!< over flow flag                        */
sbit F1   = 0xD1;     /*!< reserved                              */
sbit P    = 0xD0;     /*!< parity bit                            */

/*  ACC bit   */
sbit ACC0 = ACC^0;
sbit ACC1 = ACC^1;
sbit ACC2 = ACC^2;
sbit ACC3 = ACC^3;
sbit ACC4 = ACC^4;
sbit ACC5 = ACC^5;
sbit ACC6 = ACC^6;
sbit ACC7 = ACC^7;
/**
  * @}
  */
  
/******************************************************************************/

/** @addtogroup Interrupt_Reg
  * @{
  */
sfr IE      = 0xA8;    /*!< interrupt enable config register                  */
sfr IP      = 0xB8;    /*!< interrupt priority config register                */
sfr EIE1    = 0xA9;    /*!< extend interrupt enable config register 1         */
sfr EIP1    = 0xB9;    /*!< extend interrupt priority config register 1       */
sfr EIE2    = 0xAA;    /*!< extend interrupt enable config register 2         */
sfr EIP2    = 0xBA;    /*!< extend interrupt priority config register 2       */
sfr EINTCS0 = 0xAC;    /*!< external interrupt control and status register 0  */
sfr EINTCS1 = 0xBC;    /*!< external interrupt control and status register 1  */

/*  IE bit   */
sbit EA   = 0xAF;    /*!< interrupt enable global control bit                 */
sbit ET2  = 0xAD;    /*!< Timer2 interrupt enable bit                         */
sbit ES   = 0xAC;    /*!< UART0 interrupt enable bit                          */
sbit ET1  = 0xAB;    /*!< Timer1 interrupt enable bit                         */
sbit EX1  = 0xAA;    /*!< External interrupt (INT1) enable bit                */
sbit ET0  = 0xA9;    /*!< Timer0 overflow interrupt enable bit                */
sbit EX0  = 0xA8;    /*!< External interrupt (INT0) enable bit                */

/*  IP bit   */ 
sbit PT2  = 0xBD;    /*!< Timer2 interrupt priority control bit               */
sbit PT1  = 0xBB;    /*!< Timer1 interrupt priority control bit               */
sbit PX1  = 0xBA;    /*!< INT1 interrupt priority control bit                 */
sbit PT0  = 0xB9;    /*!< Timer0 interrupt priority control bit               */
sbit PX0  = 0xB8;    /*!< INT0 interrupt priority control bit                 */
/**
  * @}
  */
/******************************************************************************/
/** @addtogroup Power_Management_Reg
  * @{
  */
/** @brief
  * @note PCON\n
      Bit1 STOP: stop mode control bit.\n
                 When the value is 1, all the high frequency clocks are off and system is in deep sleep. Reading this bit\n
                 to get value of SSREG[1], which reflects that whether the circuit has been entered STOP mode.\n
      Bit0 IDLE: idle mode control bit.\n
                 When the value is 1, MCU enters IDLE mode. Reading this bit to get value of SSREG0, which reflects\n
                 that whether IC has been entered IDLE mode.
  */
sfr  PCON     = 0x87;    /*!< power (MCU mode) control.  {-,-,-,-,-,-,STOP(SSREG1),IDLE(SSREG0)}   */
sfr  SPMC0    = 0xA6;    /*!< low voltage reset config.  {LVREN,LVRS,BGEN,CVFLG,CVMEN,BGOE,-,DROE} */
sfr  LVRCON   = 0xA6;    /*!< low voltage reset config.  {LVREN,LVRS,BGEN,CVFLG,CVMEN,BGOE,-,DROE} */
sfr  VMC0     = 0xA5;    /*!< low voltage detection config.  {LVDEN,-,LVFLG,LVES,LVLS[3:0]}        */
sfr  LVDCON   = 0xA5;    /*!< low voltage detection config.  {LVDEN,-,LVFLG,LVES,LVLS[3:0]}        */


#define  xSTOPCFG  (*(volatile unsigned char xdata *)0xFFA0)    /*!< MCU stop mode config.  {-,-,-,-,PWRCFG,-,-,-} */

/**
  * @}
  */
/******************************************************************************/
/** @addtogroup Reset_Reg
  * @{
  */
sfr  RSTSRC = 0xA7;    /*!< reset flags.  {-,-,-,SOFTF,WDTF,LVRF,PORF,nRSTF} */
/**
  * @}
  */
/******************************************************************************/
/** @addtogroup WatchDog_Reg
  * @{
  */
sfr WDTCON   = 0xBD;    /*!< WDT counter starter value and WDT clock pre-divider.  WDTRL[4:0],-,WDTPR[1:0] */
sfr WDTCLR0  = 0x84;    /*!< WDT clear */
sfr WDTCLR1  = 0xEB;    /*!< WDT clear */
/**
  * @}
  */
/******************************************************************************/
/** @addtogroup Clock_System_Reg
  * @{
  */
sfr MCKSET   = 0x91;    /*!< Option of MCLK clock source.  {OSTS[1:0],-,CRYEN,CRYIOEN,MCKS,MDIV[1:0]} */
sfr	MCKDIV   = 0x91;    /*!< MCLK clock divider.  {OSTS[1:0],-,CRYEN,CRYIOEN,MCKS,MDIV[1:0]}          */
sfr WPKEY    = 0x95;    /*!< secret key register for clock source changing                            */
sfr RCHTRIM  = 0x97;    /*!< RCH calibration register                                                 */

#define  xSYSCFG   (*(volatile unsigned char xdata *)0xFFA2)    /*!< clock config.  {RSTCFG,TREN,TRDS,WDTCFG,CFG[1:0],-,RCLCFG} */
#define  xSYSCFG2  (*(volatile unsigned char xdata *)0xFFA3)    /*!< clock config2.  {-,-,-,-,-,-,-,DREN}                       */
/**
  * @}
  */
/******************************************************************************/
/** @addtogroup General_IO_Reg
  * @{
  */
sfr  P0 = 0x80;  /*!< P0 port */
sfr  P1 = 0x90;  /*!< P1 port */
sfr  P2 = 0xA0;  /*!< P2 port */
sfr  P3 = 0xB0;  /*!< P3 port */

/*  P0 bit  */
sbit  P00 = P0^0;
sbit  P01 = P0^1;
sbit  P02 = P0^2;
sbit  P03 = P0^3;
sbit  P04 = P0^4;
sbit  P05 = P0^5;
sbit  P06 = P0^6;
sbit  P07 = P0^7;

/*  P1 bit  */
sbit  P10 = P1^0;
sbit  P11 = P1^1;
sbit  P12 = P1^2;
sbit  P13 = P1^3;
sbit  P14 = P1^4;
sbit  P15 = P1^5;
sbit  P16 = P1^6;
sbit  P17 = P1^7;

/*  P2 bit  */
sbit  P20 = P2^0;
sbit  P21 = P2^1;
sbit  P22 = P2^2;
sbit  P23 = P2^3;
sbit  P24 = P2^4;
sbit  P25 = P2^5;
sbit  P26 = P2^6;
sbit  P27 = P2^7;

/*  P3 bit  */
sbit  P30 = P3^0;
sbit  P31 = P3^1;
sbit  P32 = P3^2;
sbit  P33 = P3^3;
sbit  P34 = P3^4;
sbit  P35 = P3^5;
sbit  P36 = P3^6;
sbit  P37 = P3^7;

/* IO mode config 00:input,float 01:input,pull-up 10:push-pull out 11:open-drain out */
sfr  P0MDL = 0xEC;    /*!< P0.0~P0.3 */
sfr  P0MDH = 0xFC;    /*!< P0.4~P0.7 */			
sfr  P1MDL = 0xED;    /*!< P1.0~P1.3 */
sfr  P1MDH = 0xFD;    /*!< P1.4~P1.7 */
sfr  P2MDL = 0xEE;    /*!< P2.0~P2.3 */
sfr  P2MDH = 0xFE;    /*!< P2.4~P2.7 */
sfr  P3MDL = 0xEF;    /*!< P3.0~P3.3 */
sfr  P3MDH = 0xFF;    /*!< P3.4~P3.7 */


#define  xP10        (*(volatile unsigned char xdata *)0xFF9A)    /*!< P10 data register                    */
#define  xP10MDL     (*(volatile unsigned char xdata *)0xFFEA)    /*!< P10 mode config register (low byte)  */

#define  xP1DCFG     (*(volatile unsigned char xdata *)0xFFBF)    /*!< P1drive capability  0:weak?1:strong */
#define  xTRRCFG     (*(volatile unsigned char xdata *)0xFFEF)    /*!< P1 pull-up resistor  RPU=40+RPUS[2:0]*10(K?).  {-,-,-,-,-,RPUS[2:0]} */

#define  xP3AEN      (*(volatile unsigned char xdata *)0xFFC3)    /*!< P3 analog enable control.  {-,-,-,-,P3AEN3,P3AEN2,P3AEN1,P3AEN0}     */

#define  xIOMUX0     (*(volatile unsigned char xdata *)0xFFE0)    /*!< IO multiplex control0.  {IICEN,REMEN,CPOE,-,-,-,PWM0OE,UART0EN}      */
#define  xIOMUX1     (*(volatile unsigned char xdata *)0xFFE1)    /*!< IO multiplex control1.  {T2EXS[2:0],SPIEN,-,-,-,-}                   */
#define  xIOMUX2     (*(volatile unsigned char xdata *)0xFFE2)    /*!< IO multiplex control2.  {-,CRTOE,INT1S,INT0S,PWM3OE,PWM2OE,PWM1OE,-} */
#define  xIOMUX3     (*(volatile unsigned char xdata *)0xFFE3)    /*!< IO multiplex control3.  {-,-,-,-,-,ENVOE,T1OE,T0OE}                  */
/**
  * @}
  */
/******************************************************************************/
/** @addtogroup UART_Reg
  * @{
  */
sfr  SBUF0  = 0x99;    /*!< UART0 data buf register                                          */
sfr  SCON0  = 0x98;    /*!< UART0 control.  {SM0,SM1,SM2,REN,TB8,RB8,TI,RI}                  */
sfr  BRCON0 = 0x8E;    /*!< UART0 baudrate register.  {SMOD,BRTEN,-,-,HPREN,-,-,-}           */
sfr  BRTIM0 = 0x8F;    /*!< starter value of buad generator                                  */
/*  SCON0 bit  */
sbit S0M0   = 0x9F;    /*!< SM0~SM1: UART operating mode selection bit                       */
sbit S0M1   = 0x9E;    /*!< SM0~SM1: UART operating mode selection bit                       */
sbit S0M2   = 0x9D;    /*!< Multi-computer communication control bit, active when S0REN=1    */
sbit S0REN  = 0x9C;    /*!< Serial receive enable bit                                        */
sbit S0TB8  = 0x9B;    /*!< In mode 2 or 3, TB8 is loaded the 9th bit data to be transmitted */
sbit S0RB8  = 0x9A;    /*!< In mode 2 or 3, RB8 is the 9th bit data received. In mode 1, RB8 */
                       /*!< is the stop bit received                                         */
sbit S0TI   = 0x99;    /*!< Transmit interrupt flag bit                                      */
sbit S0RI   = 0x98;    /*!< Receive interrupt flag bit                                       */
/**
  * @}
  */
/******************************************************************************/
/** @addtogroup KBI_Interrupt_Reg
  * @{
  */
sfr  KBIFLG  = 0x9E;    /*!< KBI interrupt flag */


#define  xP0KBICON  (*(volatile unsigned char xdata *)0xFFAA)    /*!< P0 KBI interrupt control */
#define  xP0KBIWKP  (*(volatile unsigned char xdata *)0xFFAB)    /*!< P0 KBI polarity config   */
#define  xP1KBICON  (*(volatile unsigned char xdata *)0xFFDC)    /*!< P1 KBI interrupt control */
#define  xP1KBIWKP  (*(volatile unsigned char xdata *)0xFFDD)    /*!< P1 KBI polarity config   */
#define  xP2KBICON  (*(volatile unsigned char xdata *)0xFFDE)    /*!< P2 KBI interrupt control */
#define  xP2KBIWKP  (*(volatile unsigned char xdata *)0xFFDF)    /*!< P2 KBI polarity config   */

/**
  * @}
  */
/******************************************************************************/
/** @addtogroup Timer_Reg
  * @{
  */
sfr TCON  = 0x88;  /*!< {TF1,TR1,TF0,TR0,IE1,IT1,IE0,IT0}         */
sfr TMOD  = 0x89;  /*!< {GATE1,C/T1,T1M[1:0],GATE0,C/T0,T0M[1:0]} */
sfr TL0   = 0x8A;  /*!< Timer0 lower byte                         */
sfr TL1   = 0x8B;  /*!< Timer1 lower byte                         */
sfr TH0   = 0x8C;  /*!< Timer0 high byte                          */
sfr TH1   = 0x8D;  /*!< Timer1 high byte                          */

/*  TCON bit  */
sbit TF1  = 0x8F;    /*!< Timer1 overflow flag bit                */
sbit TR1  = 0x8E;    /*!< Timer1 enable bit                       */
sbit TF0  = 0x8D;    /*!< Timer0 overflow flag bit                */
sbit TR0  = 0x8C;    /*!< Timer0 enable bit                       */
sbit IE1  = 0x8B;    /*!< INT1 interrupt flag bit                 */
sbit IT1  = 0x8A;    /*!< INT0 trigger mode selection bit         */
sbit IE0  = 0x89;    /*!< INT0 interrupt flag bit                 */
sbit IT0  = 0x88;    /*!< INT0 trigger mode selection bit         */

#define xTIMPRS    (*(volatile unsigned char xdata *)0xFFCA)    /*!< timer clock pre-divider.  {-,TIMPRS1[2:0],-,TIMPRS0[2:0]} */
#define xTIMCFG    (*(volatile unsigned char xdata *)0xFFCD)    /*!< timer PWM config.  {-,-,-,-,-,-,T1PWMEN,T0PWMEN}          */

/******************************************************************************/

/******  TIMER2 ******/

sfr T2CON1    = 0xC8;    /*!< timer2 control1.  {TF2,EXF2,-,-,FCPEN,TR2,C/T2,CP/RL2} */
sfr RCAP2L    = 0xCA;    /*!< timer2 capture low byte  */
sfr RCAP2H    = 0xCB;    /*!< timer2 capture high byte */
sfr TL2       = 0xCC;    /*!< timer2 low byte          */
sfr TH2       = 0xCD;    /*!< timer2 high byte         */

/*  T2CON1 bit */
sbit TF2      = 0xCF;    /*!< Timer2 overflow flag bit */
sbit EXF2     = 0xCE;    /*!< T2EX edge detect flag    */
sbit RCPFEN2  = 0xCB;    /*!< The falling edge of T2EX capture enable bit */
sbit TR2      = 0xCA;    /*!< Timer2 enable bit                           */
sbit T2CNT    = 0xC9;    /*!< Timer2 counter/timer mode selection bit     */
sbit T2CPNRL  = 0xC8;    /*!< Capture/reload mode selection bit           */

  #define xT2CON2  (*(volatile unsigned char xdata *) 0xFFCB)    /*!< timer2 control2.  {-,-,-,T2EXIN,RCPEN,T2CK[2:0]} */
/**
  * @}
  */
/******************************************************************************/
/** @addtogroup PWM_Reg
  * @{
  */

#define   xPWMPSC    (*(volatile unsigned char xdata *)0xFF79)    /*!< clock pre-divider of PWM modulate */
#define   xPWMCN     (*(volatile unsigned char xdata *)0xFF7A)    /*!< PWM modulate control.  {PWM3EN,PWM2EN,PWM1EN,PWM0EN,PWM0RS,-,-,-} */
#define   xPWMP      (*(volatile unsigned char xdata *)0xFF7B)    /*!< PWM period       */
#define   xPWM0D     (*(volatile unsigned char xdata *)0xFF7C)    /*!< PWM width config */
#define   xPWM1D     (*(volatile unsigned char xdata *)0xFF7D)    /*!< PWM width config */
#define   xPWM2D     (*(volatile unsigned char xdata *)0xFF7E)    /*!< PWM width config */
#define   xPWM3D     (*(volatile unsigned char xdata *)0xFF7F)    /*!< PWM width config */
/**
  * @}
  */
/******************************************************************************/
/** @addtogroup Carrier_Timer_Reg
  * @{
  */
sfr  CRTCON1   = 0xE1;    /*!< carrier timer control1.  {CRTEN,CRTMODE,CRTEXEN,CRTOVEN,CRTIS,REMOPS,CRTEXF,CRTOVF} */
sfr  CRTCON2   = 0xE2;    /*!< carrier timer control2.  {-,-,-,-,CRTCKD[3:0]}                                      */
sfr  CRTCNT    = 0xE3;    /*!< carrier counter timer                                                               */
sfr  CRTDATA0  = 0xE4;    /*!< carrier timer initial value register0                                               */
sfr  CRTDATA1  = 0xE5;    /*!< carrier timer initial value register1                                               */
/**
  * @}
  */
/******************************************************************************/
/** @addtogroup Infrared InvelopeBuffer(T168)_Reg
  * @{
  */
sfr  T168CON    = 0xE8;    /*!< T168 Control register.  {T168EN,START,CKL[1:0],EPINTE,REMEND,FULL,EMPTY} */
sfr  T168DATA0  = 0xEA;    /*!< write data interface of FIFO (high 8bits)                                */
sfr  T168DATA1  = 0xE9;    /*!< write data interface of FIFO (low 8bits)                                 */

/*  T168CON bit */
sbit  T168EN        = T168CON^7;    /*!< T168 Enable                                                     */
sbit  T168START     = T168CON^6;    /*!< T168 Start                                                      */
sbit  T168EPINTE    = T168CON^3;    /*!< T168 interrupt of code transmitting end enable bit              */
sbit  T168REMEND    = T168CON^2;    /*!< T168 transmitting end flag bit                                  */
sbit  T168FULL      = T168CON^1;    /*!< T168 data buffer full flag bit                                  */
sbit  T168EMPTY     = T168CON^0;    /*!< T168 data buffer empty flag bit                                 */
/**
  * @}
  */
/******************************************************************************/
/** @addtogroup SPI_Reg
  * @{
  */
sfr  SPICON0 = 0xF8;    /*!< SPI control0.  {SPIE,MSTEN,LSBF,SPC,BIDIROE,MODFEN,SPTIE,SPRIE} */
sfr  SPICON  = 0xF8;    /*!< SPI control0                                                    */
sfr  SPICON1 = 0xF9;    /*!< SPI control1.  {CPOL,CPHA,-,-,-,SPCR[2:0]}                      */
sfr  SPIFLG  = 0xFA;    /*!< SPI flags register.  {-,-,SPRF,SPTEF,RXOV,WCOL,MODF,SLVSEL}     */
sfr  SPIBUF  = 0xFB;    /*!< SPI send data buf register                                      */
sfr  SPIDAT  = 0xFB;    /*!< SPI send data register                                          */


/**
  * @}
  */
/******************************************************************************/
/** @addtogroup Analog_Comparator_Reg
  * @{
  */
sfr  CP0CON     = 0xAE;     /*!< analog comparator control.  {CPO,CPFS,CPOPS,CPEN,CPPS,CPNS,CP0RIF,CP0FIF}              */
sfr  CP0VRCON   = 0xAF;     /*!< reference config of the internal analog comparator.  {RDS[3:0],VRRH,VRRL,VRHVS,VREFEN} */
/**
  * @}
  */
/******************************************************************************/
/** @addtogroup IIC_Reg
  * @{
  */
/******  IIC control  ******/
sfr  IIC0CON  = 0xC0;    /*!< IIC control register.  {CR2,ENS1,STA,STO,SI,AA,CR1,CR0}            */
sfr  IIC0STA  = 0xC1;    /*!< IIC status register.  {IICSTA[4:0],TMOUT,TSOUT,TOUT}               */
sfr  IIC0DAT  = 0xC2;    /*!< IIC send or receive data register                                  */
sfr  IIC0ADDR = 0xC3;    /*!< slave address                                                      */
sfr	 SMBUSSEL = 0xC4;    /*!< SMBUS selection register                                           */
sfr	 SMBUSDAT = 0xC5;    /*!< SMBUS data register                                                */

/*  IIC0CON  */
sbit  CR2   = IIC0CON^7;    /*!< CR[2:0]: transmission/receiving rate control bit in master mode */
sbit  ENS1  = IIC0CON^6;    /*!< IIC enable bit                                                  */
sbit  STA   = IIC0CON^5;    /*!< IIC start flag bit                                              */
sbit  STO   = IIC0CON^4;    /*!< IIC stop flag bit                                               */
sbit  SI    = IIC0CON^3;    /*!< IIC interrupt flag bit                                          */
sbit  AA    = IIC0CON^2;    /*!< IIC answer flag bit                                             */
sbit  CR1   = IIC0CON^1;    /*!< CR[2:0]: transmission/receiving rate control bit in master mode */
sbit  CR0   = IIC0CON^0;    /*!< CR[2:0]: transmission/receiving rate control bit in master mode */
/**
  * @}
  */
/******************************************************************************/
/** @addtogroup FlashIAP_Reg
  * @{
  */
sfr16 FSHADDR  = 0xD1;    /*!< program address low byte                                                  */
sfr   FSHADRH  = 0xD1;    /*!< program address low byte                                                  */
sfr   FSHADRL  = 0xD2;    /*!< program address high byte                                                 */
sfr   FSHDAT   = 0xD3;    /*!< program data byte                                                         */
sfr   FSHCON   = 0xD4;    /*!< program control.  {FshEn[1:0],FshERSt,FshWRSt,FshLVCFG,FshCKCFG,-,FshErr} */
sfr   FSHKEY   = 0xD5;    /*!< program secure register                                                   */
sfr	  FSHTIM   = 0xD6;    /*!< program , erase clock divider                                             */

#define xTESTCON  (*(volatile unsigned char xdata *)0xFFAF)    /*!< Test control register              */
#define xTESTCON2 (*(volatile unsigned char xdata *)0xFF9F)    /*!< Test control register2             */
/**
  * @}
  */
/******************************************************************************/
	/**
  * @}
  */

#endif /* __SC51F33XX_H */

/******************** (C) COPYRIGHT Silan Microelectronics *****END OF FILE****/