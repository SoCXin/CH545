/********************************** (C) COPYRIGHT *******************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.4
* Date               : 2021/12/15
* Description        : GPIO������ʹ�ú�GPIO�ж�ʹ��ʾ��   
*******************************************************************************/                                                  
#include "Debug.H"
#include "GPIO.H"

#pragma  NOAREGS

/* Global Variable */ 
sbit LED = P2^2;

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Return         : None
*******************************************************************************/
void main(void) 
{
	mDelaymS(50);
	CfgFsys();                                                                //CH545ʱ��ѡ������       
	mInitSTDIO();                                                             //����0��ʼ��
	printf("CHIP_ID:%02x\n",(UINT16)CHIP_ID); 
  printf("EXAM GPIO...\n"); 
	
	/* ����GPIO */
	GPIO_Init(PORT2,PIN2,MODE1);                                              //P2.2�������                                        
  GPIO_Init(PORT0,PIN3,MODE3);                                              //P03��������
	GPIO_Init(PORT1,PIN5,MODE3);                                              //P15��������
  GPIO_Init(PORT3,PIN2,MODE3);                                              //P32(INT0)��������
  GPIO_Init(PORT3,PIN3,MODE3);                                              //P33(INT1)��������
	GPIO_Init(PORT4,PIN0,MODE3);                                              //P40��������
	GPIO_Init(PORT4,PIN1,MODE3);                                              //P41��������
	
  /* �����ⲿ�ж� */	
	GPIO_INT_Init((INT_P03_L|INT_P15_L|INT_P4x_L|INT_INT0_L|INT_INT1_L),INT_EDGE,Enable); //�ⲿ�ж�����03\15\INT0\INT1\40\41
	
	while(1){
		LED = ~LED;
		mDelaymS(100);
	}
}