/********************************** (C) COPYRIGHT *******************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH545 DataFlash字节读写演示示例                        
*******************************************************************************/
#include "Flash.H"

/* Global Variable */ 
#define Data_Size   128
UINT8X buf[Data_Size] _at_ 0x0000;

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Return         : None
*******************************************************************************/
void main(void) 
{
	UINT16 i;

	mDelaymS(50);
	CfgFsys();                                                                //CH545时钟选择配置       
	mInitSTDIO();                                                             //串口0初始化

	printf("CHIP_ID:%02x\n",(UINT16)CHIP_ID); 
	printf("EXAM DataFlash...\n");
	
	/* Erase dataflash */
	for(i=0;i<(Data_Size/64);i++)
			FlashErasePage(DATA_FLASH_ADDR+i*64);
	
	/* Read dataflash */
	memset(buf,0x00,Data_Size); 
	printf("Read DataFlash\n");    
	FlashReadBuf(DATA_FLASH_ADDR+0,buf,Data_Size);
	for(i=0;i<Data_Size;i++)
			printf("addr:%04x Data:%02x\n",i,(UINT16)buf[i]); 
	
	/* WriteByte dataflash */
	printf("WriteByte DataFlash:\n");
	for(i=0;i<Data_Size;i++){
			if( FlashProgByte(DATA_FLASH_ADDR+i,i&0xff)){
					printf("------------ FlashProgByte error\n");
			}
	}  

  /* Read dataflash */
	memset(buf,0x00,Data_Size);    
	printf("Read DataFlash\n");
	FlashReadBuf(DATA_FLASH_ADDR+0,buf,Data_Size);
	for(i=0;i<Data_Size;i++)
			printf("addr:%04x Data:%02x\n",i,(UINT16)buf[i]); 
	
	/* Erase dataflash */
	for(i=0;i<(Data_Size/64);i++)
			FlashErasePage(DATA_FLASH_ADDR+i*64);
	
	/* WritePage dataflash */
	printf("WritePage DataFlash:\n");
	memset(buf,0x5A,64);
	if(FlashProgPage(DATA_FLASH_ADDR,buf,64))
	{
			printf("------------ FlashProgByte error:%x\n");
	}
	
	/* Read dataflash */
	memset(buf,0x00,Data_Size);    
	printf("Read DataFlash\n");
	FlashReadBuf(DATA_FLASH_ADDR+0,buf,Data_Size);
	for(i=0;i<Data_Size;i++)
			printf("addr:%04x Data:%02x\n",i,(UINT16)buf[i]);

	/* Read Config Information */
	printf("Config Information Read...\n");
	i = FlashReadBuf(0xFFFE,buf,2);
	printf("real len:%d\n",(UINT16)i);
	printf("cfg:%02x %02x\n",(UINT16)buf[1],(UINT16)buf[0]);

	/* 读取设备唯一ID（只读区地址 10H~15H） 16h~17H是校验和 */
	printf("Unique ID...\n");
	printf("0x%lx ",FlashReadOTPword(0x14));                                 //17H,16H,15H,14H
	printf("0x%lx\n",FlashReadOTPword(0x10));   
	
	while(1);
}