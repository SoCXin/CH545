/********************************** (C) COPYRIGHT *******************************
* File Name          : SPI.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH545 SPI
*******************************************************************************/
#ifndef __SPI_H__
#define __SPI_H__

#include "CH545.H"
#include "Debug.H"


//#define  SPI_INTERRUPT                                                      //中断使能（仅Slave模式）
#define  SPI_CK_SET(n) (SPI0_CK_SE = n)                                     //SPI时钟设置函数 


//Master相关函数
extern void  SPIMasterModeSet(UINT8 mode);                                    //SPI主机模式初始化
extern void  CH545SPIMasterWrite(UINT8 dat);                                  //CH545硬件SPI写数据，主机模式
extern UINT8 CH545SPIMasterRead(void);                                            //CH545硬件SPI0读数据，主机模式

//Slave相关函数
extern void  SPISlvModeSet(void);                                                //SPI从机模式初始化
extern void  CH545SPISlvWrite(UINT8 dat);                                     //CH545硬件SPI写数据，从机模式
extern UINT8 CH545SPISlvRead(void);                                               //CH545硬件SPI0读数据，从机模式

#endif
