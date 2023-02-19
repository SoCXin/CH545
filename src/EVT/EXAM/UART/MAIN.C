/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH545����0~1 ��ѯ���ж��շ���ʾ��ʵ�����ݻػ�                     
*******************************************************************************/
#include "DEBUG.H"
#include "UART.H"

/* Global Variable */ 
UINT8 dat;

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Return         : None
*******************************************************************************/
void main(void)
{
	CfgFsys();
	mDelaymS(20);
	mInitSTDIO();                                                             /* Ϊ���ü����ͨ�����ڼ����ʾ���� */
	CH545UART0Alter();                                                         //����0����ӳ��
	printf("ID=%02X\n", (UINT16)CHIP_ID);
	printf("UART demo start ...\n");

	CH545UART1Init();                                                          //����1��ʼ��
	CH545UART1Alter();                                                         //����1����ӳ��   

	dat = 0;
	
	while(1){
#ifndef UART1_INTERRUPT
		dat = CH545UART1RcvByte();
		CH545UART1SendByte(dat);
		
#endif
	}
}