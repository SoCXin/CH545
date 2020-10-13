/********************************** (C) COPYRIGHT *******************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : GPIO������ʹ�ú�GPIO�ж�ʹ��ʾ��   
*******************************************************************************/                                                  
#include "Debug.H"
#include "GPIO.H"

#pragma  NOAREGS
sbit LED = P2^2;

void main( ) 
{
    mDelaymS(50);
    CfgFsys( );                                                                //CH545ʱ��ѡ������       
    mInitSTDIO( );                                                             //����0��ʼ��
    CH545UART0Alter();
    printf("CHIP_ID:%02x\r\n",(UINT16)CHIP_ID); 

    /* ����GPIO */
    GPIO_Init( PORT1,PIN0,MODE3);                                              //P1.0��������
	                                         
    /* �����ⲿ�ж� */
    GPIO_Init( PORT0,PIN3,MODE3);                                              //P03��������
	GPIO_Init( PORT1,PIN5,MODE3);                                              //P15��������
    GPIO_Init( PORT1,PIN4,MODE3);                                              //P14��������
    GPIO_Init( PORT3,PIN2,MODE3);                                              //P32(INT0)��������
    GPIO_Init( PORT3,PIN3,MODE3);                                              //P33(INT1)��������
	GPIO_Init( PORT4,PIN0,MODE3);                                              //P40��������
	GPIO_Init( PORT4,PIN1,MODE3);                                              //P41��������
	
	GPIO_INT_Init( (INT_P03_L|INT_P15_L|INT_P14_L|INT_P4x_L),INT_EDGE,Enable); //�ⲿ�ж�����03\15\14\INT0\INT1\40\41

	while(1){
	     LED = ~LED;
		 mDelaymS(100);
	}
}