/********************************** (C) COPYRIGHT *******************************
* File Name          : I2C.H
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH545 I2C
*******************************************************************************/
#ifndef __I2C_H__
#define __I2C_H__

#include "Debug.H"

/****************** 外部调用子函数 ****************************/
extern void I2CM_Init( void );													/* I2C主机初始化 */
extern void I2CS_Init(UINT8 addr, PUINT8 pbuf );													/* I2C从机初始化 */
extern void I2CM_CMD( UINT8 cmd );												/* I2C主机发送命令 */
extern void I2CM_WaitACK( void );												/* I2C主机等待从机应答ACK */
extern UINT8 I2CM_RecvData( void );												/* I2C主机接收1个字节数据 */


#endif




