#ifndef	__DATAFLASH_H__
#define __DATAFLASH_H__

#include "Debug.H"
/*********************** Flash空间划分 *****************************
*   0xFFFD  ---
*            |                   Code Flash(BOOT区域3K)
*   0xF400  ---
*   0xF3FF  ---
*            |                   Data Flash（1K）
*   0xF000  ---
*   0xEFFF  ---
*            |                   Code Flash（用户代码区60K）
*   0x0000  ---
******************************************************************/
/* Code Flash和Data Flash操作 */
/*******************************************************************************
* Function Name  : ErasePage( UINT16 Addr )
* Description    : 用于页擦除，每64字节为一页。将页内所有数据变为0x00
* Input          : Addr:擦除地址所在页
* Output         : None
* Return         : 返回操作状态，0x00:成功  0x01:地址无效  0x02:未知命令或超时
*******************************************************************************/
UINT8 FlashErasePage( UINT16 Addr );
/*******************************************************************************
* Function Name  : FlashProgByte(UINT16 Addr,UINT8 Data)
* Description    : Flash 字节编程
* Input          : Addr：写入地址
*                  Data：字节编程的数据
* Output         : None
* Return         : 编程状态返回 0x00:成功  0x01:地址无效  0x02:未知命令或超时
*******************************************************************************/
UINT8 FlashProgByte( UINT16 Addr,UINT8 Data );
/*******************************************************************************
* Function Name  : FlashProgPage( UINT16 Addr, PUINT8X Buf,UINT8 len )
* Description    : 页编程,仅编程当前Addr所在页。
* Input          : Addr：写入地址
*                  Buf：Buf地址的低6位要与Addr地址低6位相等，也就是（Buf地址%64）与页内偏移地址要相同
*                  len: 写入长度，最大64
* Output         : None
* Return         : 编程状态返回 0x00:成功  0x01:地址无效  0x02:未知命令或超时
*******************************************************************************/
UINT8 FlashProgPage( UINT16 Addr, PUINT8X Buf,UINT8 len );
/*******************************************************************************
* Function Name  : FlashReadBuf(UINT16 Addr,PUINT8 buf,UINT16 len)
* Description    : 读Flash（包含data和code）
* Input          : UINT16 Addr,PUINT8 buf,UINT16 len
* Output         : None
* Return         : 返回实际读出长度
*******************************************************************************/
UINT8 FlashReadBuf(UINT16 Addr,PUINT8 buf,UINT16 len);
/* OTP区与ReadOnly区操作 */
/*******************************************************************************
* Function Name  : FlashProgOTPbyte( UINT8 Addr, UINT8 Data )
* Description    : 单字节写OTP区域，只能0变成1,且不可擦除
* Input          : Addr：0x20~0x3F
*                  Data:
* Output         : None
* Return         : 操作状态 0x00:成功  0x02:未知命令或超时
*******************************************************************************/
UINT8  FlashProgOTPbyte( UINT8 Addr, UINT8 Data );
/*******************************************************************************
* Function Name  : FlashReadOTPword( UINT8 Addr )
* Description    : 4字节为单位读取ReadOnly区或者OTP区
* Input          : Addr：0x00~0x3F
* Output         : None
* Return         : 读取的四字节数据
*******************************************************************************/
UINT32 FlashReadOTPword( UINT8 Addr );

#endif