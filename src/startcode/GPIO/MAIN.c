/********************************** (C) COPYRIGHT *******************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : GPIO设置与使用和GPIO中断使用示例   
*******************************************************************************/                                                  
#include "Debug.H"
#include "GPIO.H"

#pragma  NOAREGS
sbit LED = P2^2;

void main( ) 
{
    mDelaymS(50);
    CfgFsys( );                                                                //CH545时钟选择配置       
    mInitSTDIO( );                                                             //串口0初始化
    CH545UART0Alter();
    printf("CHIP_ID:%02x\r\n",(UINT16)CHIP_ID); 

    /* 配置GPIO */
    GPIO_Init( PORT1,PIN0,MODE3);                                              //P1.0上拉输入
	                                         
    /* 配置外部中断 */
    GPIO_Init( PORT0,PIN3,MODE3);                                              //P03上拉输入
	GPIO_Init( PORT1,PIN5,MODE3);                                              //P15上拉输入
    GPIO_Init( PORT1,PIN4,MODE3);                                              //P14上拉输入
    GPIO_Init( PORT3,PIN2,MODE3);                                              //P32(INT0)上拉输入
    GPIO_Init( PORT3,PIN3,MODE3);                                              //P33(INT1)上拉输入
	GPIO_Init( PORT4,PIN0,MODE3);                                              //P40上拉输入
	GPIO_Init( PORT4,PIN1,MODE3);                                              //P41上拉输入
	
	GPIO_INT_Init( (INT_P03_L|INT_P15_L|INT_P14_L|INT_P4x_L),INT_EDGE,Enable); //外部中断配置03\15\14\INT0\INT1\40\41

	while(1){
	     LED = ~LED;
		 mDelaymS(100);
	}
}