/********************************** (C) COPYRIGHT *******************************
* File Name          : Debug.H
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/03
* Description        : This file contains all the functions prototypes for UART
*                      Printf , Delay functions.									 
*******************************************************************************/
#ifndef	__DEBUG_H__
#define __DEBUG_H__

#include <absacc.h>
#include <stdio.h>
#include <string.h>
#include <intrins.h>
#include "CH545.H"

//���庯������ֵ
#ifndef  SUCCESS
#define  SUCCESS    0
#endif
#ifndef  FAIL
#define  FAIL       0xFF
#endif


#define	 FREQ_SYS	                       24000000	          //ϵͳ��Ƶ24MHz
#define  OSC_EN_XT                       0                    //�ⲿ����ʹ�ܣ�Ĭ�Ͽ����ڲ�����
#ifndef  UART0_BUAD
#define  UART0_BUAD                      115200
#define  UART1_BUAD                      57600
#endif
#ifndef  DE_PRINTF                                            //����Pirntf�������
#define  DE_PRINTF                      1
#endif

void CfgFsys(void);                                           //CH545ʱ��ѡ�������
void mDelayuS(UINT16 n);                                 //��uSΪ��λ��ʱ
void mDelaymS(UINT16 n);                                 //��mSΪ��λ��ʱ
void mInitSTDIO(void);                                        //T1��Ϊ�����ʷ�����
void CH545UART0Alter(void);                                    //CH545����0����ӳ�䵽P0.2/P0.3
void CH545UART0SendByte(UINT8 SendDat);                    //CH545UART0����һ���ֽ�
UINT8 CH545UART0RcvByte(void);                                //CH545UART0��ѯ��ʽ����һ���ֽ�

void CH545WDTModeSelect(UINT8 mode);                       //CH545���Ź�ģʽ���� 
void CH545WDTFeed(UINT8 tim);                              //CH545���Ź�ι��
void CH545SoftReset(void);                                    //CH545����λ
  
#endif