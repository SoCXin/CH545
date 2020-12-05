/********************************** (C) COPYRIGHT *******************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH545 RGB演示                     
*******************************************************************************/
#include "DEBUG.H"
#include "RGB.H"
extern UINT16D	 BufferAddr;
extern UINT8X	RGBdataBuffer[128][4]; 
extern UINT8X	RGBdataBuffer1[128][4]; 
void main(){

    UINT8 i,RGB_stauts;
    signed char data incre=0;
    CfgFsys( );
    mDelaymS(20);
//    mInitSTDIO( );                       /* 为了让计算机通过串口监控演示过程 */
//    CH545UART0Alter();                   //串口0引脚映射
//	printf("ID=%02X\n", (UINT16)CHIP_ID );
//    printf("RGB demo start ...\n");
    InitRGBLED(  );
    LED_RED_ADJ = 0;
    LED_GRE_ADJ = 0;
    LED_BLU_ADJ = 0;
    EA = 1;
    /*变量初始化*/     
    for(i=0;i<128;i++){
        RGBdataBuffer[i][0]=127;
        RGBdataBuffer[i][1]=255;//R
        RGBdataBuffer[i][2]=127;//G
        RGBdataBuffer[i][3]=127;//B
    }
    for(i=0;i<128;i++){
        RGBdataBuffer1[i][0]=0;
        RGBdataBuffer1[i][1]=0;//R
        RGBdataBuffer1[i][2]=0;//G
        RGBdataBuffer1[i][3]=0;//B
    }    
    mDelaymS(200);
    BufferAddr = (UINT16)( &RGBdataBuffer1[0] );  
    mDelaymS(200);
    BufferAddr = (UINT16)( &RGBdataBuffer[0] ); 
    mDelaymS(200); 
    BufferAddr = (UINT16)( &RGBdataBuffer1[0] );  
    mDelaymS(200);  
    BufferAddr = (UINT16)( &RGBdataBuffer[0] ); 
    mDelaymS(200); 
    BufferAddr = (UINT16)( &RGBdataBuffer1[0] );  
    mDelaymS(200); 
    
    while(1){
        BufferAddr = (UINT16)( &RGBdataBuffer[0] );            
        if(incre >= 127)  RGB_stauts = 1;
        if(incre <= -127) RGB_stauts = 0;
        LED_GRE_ADJ=RGB_stauts?(incre--):(incre++);  
        mDelaymS(10);
    }
}