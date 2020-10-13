/* 调试 */
/* 提供printf子程序和延时函数 */

#ifndef	__DEBUG_H__
#define __DEBUG_H__

#include <absacc.h>
#include <stdio.h>
#include <string.h>
#include <intrins.h>

#include "CH545.H"
//定义函数返回值
#ifndef  SUCCESS
#define  SUCCESS    0
#endif
#ifndef  FAIL
#define  FAIL       0xFF
#endif


#define	 FREQ_SYS	                       24000000	          //系统主频24MHz
#define  OSC_EN_XT                       0                    //外部晶振使能，默认开启内部晶振
#ifndef  UART0_BUAD
#define  UART0_BUAD                      115200
#define  UART1_BUAD                      57600
#endif
#ifndef  DE_PRINTF                                            //程序Pirntf输出开启
#define  DE_PRINTF                      1
#endif

void CfgFsys( );                                           //CH545时钟选择和配置
void mDelayuS( UINT16 n );                                 // 以uS为单位延时
void mDelaymS( UINT16 n );                                 // 以mS为单位延时
void mInitSTDIO( );                                        //T1作为波特率发生器
void CH545UART0Alter();                                    //CH545串口0引脚映射到P0.2/P0.3
void CH545UART0SendByte(UINT8 SendDat);                    //CH545UART0发送一个字节
UINT8 CH545UART0RcvByte( );                                //CH545UART0查询方式接收一个字节

void CH545WDTModeSelect(UINT8 mode);                       //CH545看门狗模式设置 
void CH545WDTFeed(UINT8 tim);                              //CH545看门狗喂狗
void CH545SoftReset( );                                    //CH545软复位
void CH545USBDevWakeup( );                                 //CH545做USB设备唤醒主机
  
#endif
