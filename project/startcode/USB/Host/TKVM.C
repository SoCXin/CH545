/* KVM
****************************************
**  Copyright (C) wch.cn  1999-2019   **
**  Web:              http://wch.cn   **
****************************************
*/

/* C51   TEST.C */
/* LX51  TEST.OBJ */
/* OHX51 TEST */

#include "DEBUG.H"

#define UART_BPS   115200
// dual DPTR for memcpy...
#pragma	modp2
#pragma	NOAREGS


#define	TOTAL_USBX			4
#define	TOTAL_ROOT_HUB		4

#define	MAX_DESCR_SIZE		(256-UX_EP1_ADDR-40-sizeof(USB_DEV_DESCR))
#define	EP0_PACKET_SIZE		UX_EP0_SIZE_D4
#define	STR_DES_HUB_MANU	1
#define	STR_DES_HUB_PROD	2
#define	STR_DES_HUB_SN		0
#define	STR_DES_DEV_MANU	4
#define	STR_DES_DEV_PROD	5
#define	STR_DES_DEV_SN		0

// hub设备描述符
UINT8C	cHubDevDescr[] = { 0x12, USB_DESCR_TYP_DEVICE, 0x10, 0x01,
							USB_DEV_CLASS_HUB, 0x00, 0x00, EP0_PACKET_SIZE,
							0x48, 0x43, 0x48, 0x70,  // 厂商ID和产品ID
							0x00, 0x01, STR_DES_HUB_MANU, STR_DES_HUB_PROD,
							0x00, 0x01 };
// hub配置描述符
UINT8C	cHubCfgDescr[] = { 0x09, USB_DESCR_TYP_CONFIG, 0x09+0x09+0x07, 0x00, 0x01, 0x01, 0x00, 0x80|0x40, 0x32,
							0x09, USB_DESCR_TYP_INTERF, 0x00, 0x00, 0x01, USB_DEV_CLASS_HUB, 0x00, 0x00, 0x00,
							0x07, USB_DESCR_TYP_ENDP, 0x81, 0x03, 0x01, 0x00, 0xFF };
// hub类描述符
UINT8C	cHubClsDescr[] = { 0x09, USB_DESCR_TYP_HUB, 0x04, 0x00, 0x00, 0x05, 0x64, 0x00, 0xFF };
// 语言描述符
UINT8C	cHubLangDescr[] = { 0x04, USB_DESCR_TYP_STRING, 0x09, 0x04 };
// 厂家信息
UINT8C	cHubManuInfo[] = { 0x0E, USB_DESCR_TYP_STRING, 'w', 0, 'c', 0, 'h', 0, '.', 0, 'c', 0, 'n', 0 };
// hub产品信息
UINT8C	cHubProdInfo[] = { 0x0C, USB_DESCR_TYP_STRING, 'C', 0, 'H', 0, '5', 0, '4', 0, '5', 0 };
// device设备描述符,仅用于演示
UINT8C	cDevDevDescr[] = { 0x12, USB_DESCR_TYP_DEVICE, 0x10, 0x01,
							0x00, 0x00, 0x00, EP0_PACKET_SIZE,
							0x48, 0x43, 0x00, 0x70,  // 厂商ID和产品ID
							0x00, 0x01, STR_DES_DEV_MANU, STR_DES_DEV_PROD,
							0x00, 0x01 };
// keyboard描述符,不完整,仅用于演示
UINT8C	cKeyboardDescr[] = { 0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0xC0 };
// mouse描述符,不完整,仅用于演示
UINT8C	cMouseDescr[] = { 0x05, 0x01, 0x09, 0x02, 0xA1, 0x01, 0xC0 };
// device配置描述符,仅用于演示
UINT8C	cDevCfgDescrK[] = { 0x09, USB_DESCR_TYP_CONFIG, 0x09+0x09+0x09+0x07+0x07+0x07, 0x00, 0x01, 0x01, 0x00, 0x80|0x40, 0x32,
							0x09, USB_DESCR_TYP_INTERF, 0x00, 0x00, 0x03, USB_DEV_CLASS_HID, 0x01, 0x01, 0x00,
							0x09, USB_DESCR_TYP_HID, 0x10, 0x01, 0x00, 0x01, 0x22, sizeof( cKeyboardDescr ), 0x00,
							0x07, USB_DESCR_TYP_ENDP, 0x81, 0x03, 0x40, 0x00, 0x05,
							0x07, USB_DESCR_TYP_ENDP, 0x82, 0x03, 0x40, 0x00, 0x0A,
							0x07, USB_DESCR_TYP_ENDP, 0x03, 0x02, 0x10, 0x00, 0x00 };
UINT8C	cDevCfgDescrM[] = { 0x09, USB_DESCR_TYP_CONFIG, 0x09+0x09+0x09+0x07, 0x00, 0x01, 0x01, 0x00, 0x80|0x40, 0x32,
							0x09, USB_DESCR_TYP_INTERF, 0x00, 0x00, 0x01, USB_DEV_CLASS_HID, 0x01, 0x02, 0x00,
							0x09, USB_DESCR_TYP_HID, 0x10, 0x01, 0x00, 0x01, 0x22, sizeof( cMouseDescr ), 0x00,
							0x07, USB_DESCR_TYP_ENDP, 0x81, 0x03, 0x08, 0x00, 0x03 };
UINT8C	cDevCfgDescrD[] = { 0x09, USB_DESCR_TYP_CONFIG, 0x09+0x09+0x07+0x07, 0x00, 0x01, 0x01, 0x00, 0x80|0x40, 0x32,
							0x09, USB_DESCR_TYP_INTERF, 0x00, 0x00, 0x02, USB_DEV_CLASS_STORAGE, 0x06, 0x50, 0x00,
							0x07, USB_DESCR_TYP_ENDP, 0x82, 0x02, 0x40, 0x00, 0x00,
							0x07, USB_DESCR_TYP_ENDP, 0x03, 0x02, 0x40, 0x00, 0x00 };
// device产品信息,仅用于演示
UINT8C	cDevProdInfo[] = { 0x0A, USB_DESCR_TYP_STRING, 'K', 0, '+', 0, 'M', 0, '+', 0 };
// device配置描述符,仅用于演示
UINT8C	cDevReportDescr[] = { 0x04, USB_DESCR_TYP_REPORT, 0, 0 };
#define	Dev0ManuInfo		cHubManuInfo
#define	Dev1ManuInfo		cHubManuInfo
#define	Dev2ManuInfo		cHubManuInfo
#define	Dev0ProdInfo		cDevProdInfo
#define	Dev1ProdInfo		cDevProdInfo
#define	Dev2ProdInfo		cDevProdInfo
#define	Dev0ReportDescr		cDevReportDescr
#define	Dev1ReportDescr		cDevReportDescr
#define	Dev2ReportDescr		cDevReportDescr
#define	Dev0PhysicDescr		cDevReportDescr
#define	Dev1PhysicDescr		cDevReportDescr
#define	Dev2PhysicDescr		cDevReportDescr

/* 每个USBX的BUFFER占用1024字节XRAM，前3个DEVICE各256字节（看端点，可能未必用完），最后HUB一般只用64（EP0）+2（EP1）=66字节，故空出190字节，
   空出190*4的用途，首先用于各个USBX相关的自用变量（方便bUSBX_XRAM_OFS自动偏移），例如X0SetupLen~X3SetupLen，
   剩下的，USBX0的不方便使用（除非临时关闭bUSBX_XRAM_OFS自动偏移，才能避免自动偏移到USBX1~X3），USBX1~X3的剩余可用于全局的或者与各USBX无关的缓冲区或变量 */
/* 如果主程序/子程序中使用的变量，有可能被中断服务程序动态修改，建议加volatile标识提醒主程序 */

/* 以下定义只是形式上避免被其它变量占用，一般情况下并不会被占用，无需定义 */
UINT8X	X0D0Ep0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX0D0_EP0_BUF);	// USBX0 dev0 EP0 buffer，注意：会自动偏移到USBX1~X3
UINT8X	X1D0Ep0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX1D0_EP0_BUF);	// USBX1 dev0 EP0 buffer
UINT8X	X2D0Ep0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX2D0_EP0_BUF);	// USBX2 dev0 EP0 buffer
UINT8X	X3D0Ep0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX3D0_EP0_BUF);	// USBX3 dev0 EP0 buffer
UINT8X	X0D1Ep0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX0D1_EP0_BUF);	// USBX0 dev1 EP0 buffer，注意：会自动偏移到USBX1~X3
UINT8X	X1D1Ep0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX1D1_EP0_BUF);	// USBX1 dev1 EP0 buffer
UINT8X	X2D1Ep0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX2D1_EP0_BUF);	// USBX2 dev1 EP0 buffer
UINT8X	X3D1Ep0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX3D1_EP0_BUF);	// USBX3 dev1 EP0 buffer
UINT8X	X0D2Ep0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX0D2_EP0_BUF);	// USBX0 dev2 EP0 buffer，注意：会自动偏移到USBX1~X3
UINT8X	X1D2Ep0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX1D2_EP0_BUF);	// USBX1 dev2 EP0 buffer
UINT8X	X2D2Ep0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX2D2_EP0_BUF);	// USBX2 dev2 EP0 buffer
UINT8X	X3D2Ep0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX3D2_EP0_BUF);	// USBX3 dev2 EP0 buffer
UINT8X	X0HBEp0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX0HB_EP0_BUF);	// USBX0 hub EP0 buffer，注意：会自动偏移到USBX1~X3
UINT8X	X1HBEp0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX1HB_EP0_BUF);	// USBX1 hub EP0 buffer
UINT8X	X2HBEp0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX2HB_EP0_BUF);	// USBX2 hub EP0 buffer
UINT8X	X3HBEp0Buffer[UX_EP0_SIZE_D4]	_at_ ((UINT16)pX3HB_EP0_BUF);	// USBX3 hub EP0 buffer

UINT8X	X0D0Ep1Buffer[UX_EP1_SIZE_D5]	_at_ ((UINT16)(pUSBX0_BUF_DEV0+UX_EP1_ADDR));	// USBX0 dev0 EP1 buffer，注意：会自动偏移到USBX1~X3


/* 需要bUSBX_XRAM_OFS自动偏移，用于各个USBX相关变量 */
UINT8X	X0D0UsbConfig		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+2);	// USBX0 dev0配置标志，注意：会自动偏移到USBX1~X3
UINT8X	X0D1UsbConfig		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+3);	// USBX0 dev1配置标志，注意：会自动偏移到USBX1~X3
UINT8X	X0D2UsbConfig		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+4);	// USBX0 dev2配置标志，注意：会自动偏移到USBX1~X3
UINT8X	X0HBUsbConfig		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+5);	// USBX0 hub配置标志，注意：会自动偏移到USBX1~X3
UINT8X	X1D0UsbConfig		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+2);	// USBX1 dev0配置标志
UINT8X	X1D1UsbConfig		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+3);	// USBX1 dev1配置标志
UINT8X	X1D2UsbConfig		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+4);	// USBX1 dev2配置标志
UINT8X	X1HBUsbConfig		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+5);	// USBX1 hub配置标志
UINT8X	X2D0UsbConfig		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+2);	// USBX2 dev0配置标志
UINT8X	X2D1UsbConfig		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+3);	// USBX2 dev1配置标志
UINT8X	X2D2UsbConfig		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+4);	// USBX2 dev2配置标志
UINT8X	X2HBUsbConfig		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+5);	// USBX2 hub配置标志
UINT8X	X3D0UsbConfig		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+2);	// USBX3 dev0配置标志
UINT8X	X3D1UsbConfig		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+3);	// USBX3 dev1配置标志
UINT8X	X3D2UsbConfig		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+4);	// USBX3 dev2配置标志
UINT8X	X3HBUsbConfig		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+5);	// USBX3 hub配置标志
UINT8XV	X0DevBitFlag[4]		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+6);	// USBX0子设备各种位标志，注意：会自动偏移到USBX1~X3
UINT8XV	X1DevBitFlag[4]		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+6);	// USBX1子设备各种位标志
UINT8XV	X2DevBitFlag[4]		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+6);	// USBX2子设备各种位标志
UINT8XV	X3DevBitFlag[4]		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+6);	// USBX3子设备各种位标志
#define	BIT_PORT_CONNECT	0		// 端口连接状态(子设备有效标志),不受HUB初始化和复位的影响,注:bUX_DEV_EN为使能标志(正式工作)
#define	BIT_PORT_C_CONN		2		// 端口连接状态改变标志
#define	BIT_PORT_C_REST		3		// 端口复位完成标志

UINT8X	X0D0SetupReqCode	_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+10);	// setup request code for USBX0 dev0，注意：会自动偏移到USBX1~X3
UINT8X	X0D1SetupReqCode	_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+11);	// setup request code for USBX0 dev1，注意：会自动偏移到USBX1~X3
UINT8X	X0D2SetupReqCode	_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+12);	// setup request code for USBX0 dev2，注意：会自动偏移到USBX1~X3
UINT8X	X0HBSetupReqCode	_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+13);	// setup request code for USBX0 hub，注意：会自动偏移到USBX1~X3
UINT8X	X1D0SetupReqCode	_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+10);
UINT8X	X1D1SetupReqCode	_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+11);
UINT8X	X1D2SetupReqCode	_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+12);
UINT8X	X1HBSetupReqCode	_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+13);
UINT8X	X2D0SetupReqCode	_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+10);
UINT8X	X2D1SetupReqCode	_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+11);
UINT8X	X2D2SetupReqCode	_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+12);
UINT8X	X2HBSetupReqCode	_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+13);
UINT8X	X3D0SetupReqCode	_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+10);
UINT8X	X3D1SetupReqCode	_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+11);
UINT8X	X3D2SetupReqCode	_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+12);
UINT8X	X3HBSetupReqCode	_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+13);
UINT8X	X0D0SetupLen		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+14);	// setup length for USBX0 dev0，注意：会自动偏移到USBX1~X3
UINT8X	X0D1SetupLen		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+15);	// setup length for USBX0 dev1，注意：会自动偏移到USBX1~X3
UINT8X	X0D2SetupLen		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+16);	// setup length for USBX0 dev2，注意：会自动偏移到USBX1~X3
UINT8X	X0HBSetupLen		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+17);	// setup length for USBX0 hub，注意：会自动偏移到USBX1~X3
UINT8X	X1D0SetupLen		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+14);
UINT8X	X1D1SetupLen		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+15);
UINT8X	X1D2SetupLen		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+16);
UINT8X	X1HBSetupLen		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+17);
UINT8X	X2D0SetupLen		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+14);
UINT8X	X2D1SetupLen		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+15);
UINT8X	X2D2SetupLen		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+16);
UINT8X	X2HBSetupLen		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+17);
UINT8X	X3D0SetupLen		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+14);
UINT8X	X3D1SetupLen		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+15);
UINT8X	X3D2SetupLen		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+16);
UINT8X	X3HBSetupLen		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+17);
PUINT8	xdata X0D0pDescr	_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+18);	// point descriptor for USBX0 dev0，注意：会自动偏移到USBX1~X3
PUINT8	xdata X0D1pDescr	_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+21);	// point descriptor for USBX0 dev1，注意：会自动偏移到USBX1~X3
PUINT8	xdata X0D2pDescr	_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+24);	// point descriptor for USBX0 dev2，注意：会自动偏移到USBX1~X3
PUINT8	xdata X0HBpDescr	_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+27);	// point descriptor for USBX0 hub，注意：会自动偏移到USBX1~X3
PUINT8	xdata X1D0pDescr	_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+18);
PUINT8	xdata X1D1pDescr	_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+21);
PUINT8	xdata X1D2pDescr	_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+24);
PUINT8	xdata X1HBpDescr	_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+27);
PUINT8	xdata X2D0pDescr	_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+18);
PUINT8	xdata X2D1pDescr	_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+21);
PUINT8	xdata X2D2pDescr	_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+24);
PUINT8	xdata X2HBpDescr	_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+27);
PUINT8	xdata X3D0pDescr	_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+18);
PUINT8	xdata X3D1pDescr	_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+21);
PUINT8	xdata X3D2pDescr	_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+24);
PUINT8	xdata X3HBpDescr	_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+27);
UINT8X	X0Reserved[6]		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+30);	// reserved for USBX0 hub，注意：会自动偏移到USBX1~X3
UINT8X	X1Reserved[6]		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+30);
UINT8X	X2Reserved[6]		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+30);
UINT8X	X3Reserved[6]		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+30);
UINT8XV	X0D0_PROTOCOL		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+36);	// for USBX0 dev0，注意：会自动偏移到USBX1~X3
UINT8XV	X0D1_PROTOCOL		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+37);	// for USBX0 dev1，注意：会自动偏移到USBX1~X3
UINT8XV	X0D2_PROTOCOL		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+38);	// for USBX0 dev2，注意：会自动偏移到USBX1~X3
UINT8XV	X1D0_PROTOCOL		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+36);
UINT8XV	X1D1_PROTOCOL		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+37);
UINT8XV	X1D2_PROTOCOL		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+38);
UINT8XV	X2D0_PROTOCOL		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+36);
UINT8XV	X2D1_PROTOCOL		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+37);
UINT8XV	X2D2_PROTOCOL		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+38);
UINT8XV	X3D0_PROTOCOL		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+36);
UINT8XV	X3D1_PROTOCOL		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+37);
UINT8XV	X3D2_PROTOCOL		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+38);

/* 不方便使用的区域（除非临时关闭bUSBX_XRAM_OFS自动偏移，才能避免自动偏移到USBX1~X3） */
UINT8X	BufReserved[256-UX_EP1_ADDR-40]		_at_ ((UINT16)pUSBX0_BUF_HUB+UX_EP1_ADDR+40);	// 不方便使用（除非临时关闭bUSBX_XRAM_OFS自动偏移，才能避免自动偏移到USBX1~X3）

/* 禁止自动偏移的存储区域，充分利用空间，借用于保存，实无对应关系 */
UINT8X	Dev0DevDescr[sizeof(USB_DEV_DESCR)]	_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+40);	// 保存的子设备0的设备描述符
UINT8X	Dev0CfgDescr[MAX_DESCR_SIZE]		_at_ ((UINT16)pUSBX1_BUF_HUB+UX_EP1_ADDR+40+sizeof(USB_DEV_DESCR));	// 保存的子设备0的配置描述符
UINT8X	Dev1DevDescr[sizeof(USB_DEV_DESCR)]	_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+40);	// 保存的子设备1的设备描述符
UINT8X	Dev1CfgDescr[MAX_DESCR_SIZE]		_at_ ((UINT16)pUSBX2_BUF_HUB+UX_EP1_ADDR+40+sizeof(USB_DEV_DESCR));	// 保存的子设备1的配置描述符
UINT8X	Dev2DevDescr[sizeof(USB_DEV_DESCR)]	_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+40);	// 保存的子设备2的设备描述符
UINT8X	Dev2CfgDescr[MAX_DESCR_SIZE]		_at_ ((UINT16)pUSBX3_BUF_HUB+UX_EP1_ADDR+40+sizeof(USB_DEV_DESCR));	// 保存的子设备2的配置描述符

/* 全局 */
UINT8	Dev3FlagPC;					// 直通设备3#的PC侧系统位变量
#define	BIT_DEV3_IS_PASS	0x80	// 当前设备3#必须作为直通设备连接
#define	BIT_DEV3_ROOT_IDX	0x03	// 当前设备3#作为USB子设备的ROOT端口号
UINT8	ActionUSBX;					// 当前激活的USBX/PC编号
UINT8V	CurrentEnumRoot;			// 主机当前等待或者正在枚举的设备所在的ROOT端口号，为0xFF说明空闲，否则在最高位为1时（0x8X）说明此设备正在等待稳定,低4位是ROOT端口号
UINT8	CurrentWaitRoot;			// 主机当前等待稳定队列的设备所在的ROOT端口号，为0xFF说明等待稳定队列中空闲，否则是ROOT端口号
BOOL	TimerAct1mS;				// 1mS定时激活标志
UINT8V	TimerCount1mS;				// 1mS定时计数器
UINT8V	TaskReadyTimer;				// 计数到0触发设备插入稳定就绪任务
UINT8V	TaskResetTimer;				// 计数到0触发直通设备复位完成任务

#define	TASK_TIMER_READY	200		// 设备插入稳定就绪任务: 199~200mS
#define	TASK_TIMER_RESET	11		// 直通设备复位完成任务: 10~11mS
#define	RE_ATTACH_TIMEOUT	100		// 等待USB设备复位后重新连接,100mS超时
#define	ATTACH_WAIT_STAB	80		// 等待USB设备复位后连接稳定,已经稳定连接80mS
#define	BUS_RESET_TIME		10		/* USB总线复位时间10mS到20mS */

//UINT8X	RxBuffer[ MAX_PACKET_SIZE ];	// IN
//UINT8X	TxBuffer[ MAX_PACKET_SIZE ];	// OUT

struct _RootHubDev{
	UINT8	DeviceStatus;			// 设备状态,0-无设备,1-有设备但尚未初始化,2-有设备但初始化枚举失败,3-有设备且初始化枚举成功
//	UINT8	DeviceAddress;			// 设备被分配的USB地址,0x02~0x05
	UINT8	DeviceSpeed;			// 0为低速,非0为全速
//	UINT8	DeviceType;				// 设备类型
	UINT8	DeviceEp0Size;			// 端点0的长度
	UINT8	DeviceIndex;			// 作为USBX之HUB下子设备的序号，11对应于作为直通设备
	union {
		struct {
			UINT8	InterruptEndp;	// 鼠标中断端点号
			UINT8	IntEndpTog;		// 鼠标中断端点的同步标志
			UINT8	IntEndpTimer;	// 鼠标中断端点的查询定时
//			UINT8	IntEndpSize;	// 鼠标中断端点的长度
		} Mouse;
		struct {
			UINT8	InterruptEndp;	// 键盘中断端点号
			UINT8	IntEndpTog;		// 键盘中断端点的同步标志
			UINT8	IntEndpTimer;	// 键盘中断端点的查询定时
//			UINT8	IntEndpSize;	// 键盘中断端点的长度
		} Keyboard;
	} u;
//.....    struct  _Endp_Attr   Endp_Attr[4];	//端点的属性,最多支持4个端点
	UINT8	GpVar;					// 通用变量
} xdata RootHubDev[4];

#include <TKVMROOT.C>

sbit	USBD0_ACT	= P1^0;
sbit	USBD3_ACT	= P1^1;
sbit	TMR_ACT		= P1^2;
sbit	USBX_ACT	= P1^3;

void	InitUSBX0_Dev0( UINT8 mod )  // mod=0: clear, =1:reset&disable, =2:reset&enable
{
	pX0D0_EP1RES = bUEP_X_AUTO_TOG | UEP_X_RES_NAK;
	pX0D0_EP2RES = bUEP_X_AUTO_TOG | UEP_X_RES_NAK;
	pX0D0_EP3RES = bUEP_X_AUTO_TOG | UEP_X_RES_ACK;
	if ( mod ) {
		pX0D0_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
		pX0D0_EP_MOD = bUX_EP3O_EN | ( mod >=2 ? bUX_DEV_EN : 0 );  // EP1=IN,EP2=IN,EP3=OUT,bUX_DEV_EN=?
		pX0D0_ADDR = 0x00;
		X0DevBitFlag[0] = X0D0UsbConfig = 0x00;
//		X0D0SetupReqCode = 0xFF;
		pUSBX0_IF = bUX_IF_D0_TRANS;  // clear
	}
}

void	InitUSBX0_Dev1( UINT8 mod )  // mod=0: clear, =1:reset&disable, =2:reset&enable
{
	pX0D1_EP1RES = bUEP_X_AUTO_TOG | UEP_X_RES_NAK;
	pX0D1_EP2RES = bUEP_X_AUTO_TOG | UEP_X_RES_NAK;
	pX0D1_EP3RES = bUEP_X_AUTO_TOG | UEP_X_RES_ACK;
	if ( mod ) {
		pX0D1_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
		pX0D1_EP_MOD = bUX_EP3O_EN | ( mod >=2 ? bUX_DEV_EN : 0 );  // EP1=IN,EP2=IN,EP3=OUT,bUX_DEV_EN=?
		pX0D1_ADDR = 0x00;
		X0DevBitFlag[1] = X0D1UsbConfig = 0x00;
//		X0D1SetupReqCode = 0xFF;
		pUSBX0_IF = bUX_IF_D1_TRANS;  // clear
	}
}

void	InitUSBX0_Dev2( UINT8 mod )  // mod=0: clear, =1:reset&disable, =2:reset&enable
{
	pX0D2_EP1RES = bUEP_X_AUTO_TOG | UEP_X_RES_NAK;
	pX0D2_EP2RES = bUEP_X_AUTO_TOG | UEP_X_RES_NAK;
	pX0D2_EP3RES = bUEP_X_AUTO_TOG | UEP_X_RES_ACK;
	if ( mod ) {
		pX0D2_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
		pX0D2_EP_MOD = bUX_EP3O_EN | ( mod >=2 ? bUX_DEV_EN : 0 );  // EP1=IN,EP2=IN,EP3=OUT,bUX_DEV_EN=?
		pX0D2_ADDR = 0x00;
		X0DevBitFlag[2] = X0D2UsbConfig = 0x00;
//		X0D2SetupReqCode = 0xFF;
		pUSBX0_IF = bUX_IF_D2_TRANS;  // clear
	}
}

void	InitUSBX0_Hub( BOOL reset )  // reset=FALSE: clear, =TRUE:reset&start
{
	UINT8	mod;
	pUSBX0_IE = 0x00;
	pX0HB_EP1RES = bUEP_X_AUTO_TOG | UEP_X_RES_NAK;
	mod = reset ? 1 : 0;
	InitUSBX0_Dev0( mod );
	InitUSBX0_Dev1( mod );
	InitUSBX0_Dev2( mod );
	X0DevBitFlag[1] = X0DevBitFlag[0] = BIT_PORT_CONNECT;  // 默认有键盘和鼠标
	X0DevBitFlag[3] = X0DevBitFlag[2] = 0x00;  // 默认无3#子设备和直通设备
	if ( reset ) {
		pX0HB_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
		pX0HB_ADDR = 0x00;
		X0HBUsbConfig = 0x00;
//		X0HBSetupReqCode = 0xFF;
		pUSBX0_CTRL = bUX_DP_PU_EN;  // bUX_HUB_EP4_EN=0
		pUSBX0_IF = 0xFF;  // clear all interrupt flag
	}
	pUSBX0_IE = bUX_IE_TRANSFER | bUX_IE_BUS_RST | bUX_IE_SUSPEND;  // bUX_IE_NAK=0,bUX_IE_SOF=0
}

void	InitUSBX_Device( void )  // 初始化USBX设备
{
	IE_USBX = 0;
	IP_EX &= ~ bIP_USBX;
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	GLOBAL_CFG |= bXIR_XSFR;
	USBX_SEL = bUSBX_XSFR_OFS | bUSBX_XRAM_OFS | 0;  // USBX自动偏移,通过USBX0读写USBX0
	InitUSBX0_Hub( TRUE );
	USBX_SEL = bUSBX_XSFR_OFS | bUSBX_XRAM_OFS | 1;  // USBX自动偏移,通过USBX0读写USBX1
	InitUSBX0_Hub( TRUE );
	USBX_SEL = bUSBX_XSFR_OFS | bUSBX_XRAM_OFS | 2;  // USBX自动偏移,通过USBX0读写USBX2
	InitUSBX0_Hub( TRUE );
	USBX_SEL = bUSBX_XSFR_OFS | bUSBX_XRAM_OFS | 3;  // USBX自动偏移,通过USBX0读写USBX3
	InitUSBX0_Hub( TRUE );
//	USBX_SEL = 0x00;  // 解除自动偏移
	ActionUSBX = 0x00;
	Dev3FlagPC = 0x00;
	IE_USBX = 1;
}

void	ThisHubEP1IntUp( UINT8 PortBit )  // 为当前USBX上传HUB中断通知, 0#~3#子设备的位数据
{
	UINT8	i;
//	PortBit = 0x01 << ( PortIndex + 1 );
	if ( ( pX0HB_EP1RES & MASK_UEP_X_RES ) != UEP_X_RES_ACK ) {  // free
		pX0HB_EP1_BUF[0] = PortBit;
		pX0HB_EP1T_L = 0x01;
		pX0HB_EP1RES = pX0HB_EP1RES & ~ MASK_UEP_X_RES | UEP_X_RES_ACK;  // 上传
	}
	else {  // uploading
		for ( i = 255; i != 0; i -- ) if ( pUSBX0_IE & bUX_SIE_FREE ) break;  // wait USBX free
		pX0HB_EP1_BUF[0] |= PortBit;  // append
	}
}

void	AllHubEP1IntUp( UINT8 PortIndex, UINT8 NewStat )  // 在关闭中断后为所有USBX上传HUB中断通知, 0#~3#子设备
{
	UINT8	i, PortBit;
	PortBit = 0x01 << ( PortIndex + 1 );
	EA = 0;  // 关闭中断
	for ( i = 0; i != TOTAL_USBX; i ++ ) {
		USBX_SEL = bUSBX_XSFR_OFS | bUSBX_XRAM_OFS | i;
		X0DevBitFlag[PortIndex] = NewStat;
		ThisHubEP1IntUp( PortBit );
	}
	EA = 1;
}

void	InitTaskTimer( void )  // 初始化延时任务定时器
{
#define		TIMER_US	1000
	ET2 = 0;
	PT2 = 0;
	T2MOD = T2MOD | bTMR_CLK | bT2_CLK;
	T2COUNT = RCAP2 = 0x10000 - (UINT16)((SYS_FREQ/1000*TIMER_US+1000/2)/1000);  // 定时
	T2CON = 0;
	TR2 = 1;
	TaskReadyTimer = 0;
	TaskResetTimer = 0;
	TimerCount1mS = 0;
	TimerAct1mS = 0;
	ET2 = 1;
}

#pragma	nomodp2
void	Timer_Interrupt( void ) interrupt INT_NO_TMR2 using 1	/* 1000uS定时中断服务程序,使用寄存器组1 */
{
	TMR_ACT = 0;  // debug LED
	TF2 = 0;
	if ( TaskReadyTimer ) {  // 启用延时任务
		if ( -- TaskReadyTimer == 0 ) {  // 计数到0触发
			if ( CurrentEnumRoot != 0xFF && ( CurrentEnumRoot & 0x80 ) ) CurrentEnumRoot &= 0x0F;  // 空闲则处理下一个，主机当前正在枚举的设备所在的ROOT端口号
		}
	}
	if ( TaskResetTimer ) {  // 启用延时任务
		if ( -- TaskResetTimer == 0 ) { // 计数到0触发
			if ( Dev3FlagPC & BIT_DEV3_IS_PASS ) {
				ResetRootHubPort( Dev3FlagPC & BIT_DEV3_ROOT_IDX, 2 );  // 结束复位总线
//				EnableRootHubPort( Dev3FlagPC & BIT_DEV3_ROOT_IDX );  // 使能端口
				UH_THROUGH = ( ( Dev3FlagPC & BIT_DEV3_ROOT_IDX ) << 2 ) | ActionUSBX & MASK_UH_THR_UX;
				while ( ( USB_FREE & 0xE0 ) == 0 );  // 等待USB空闲, 680uS@12MHz, 170uS@48MHz
				USB_CTRL |= bUC_THROUGH;  // USB直通使能
//				sav = USBX_SEL;
				USBX_SEL = bUSBX_XSFR_OFS | bUSBX_XRAM_OFS | ActionUSBX;  // USBX自动偏移,通过USBX0读写USBX0-USBX3
				X0DevBitFlag[3] = BIT_PORT_CONNECT | BIT_PORT_C_REST;  //保持端口连接状态,复位完成
				ThisHubEP1IntUp( 0x01 << ( 3 + 1 ) );  // 为当前USBX上传HUB中断通知
//				USBX_SEL = sav;
			}
		}
	}
	TimerCount1mS ++;  // 1mS定时计数器
	TimerAct1mS = 1;  // 1mS定时激活标志
	TMR_ACT = 1;  // debug LED
}
#pragma	modp2

#pragma	nomodp2
void	USBX_DeviceInterrupt( void ) interrupt INT_NO_USBX using 1	/* USBX中断服务程序,使用寄存器组1 */
{
	UINT8	i, len, us;
	UINT8	rrd0, rrd1, rrd2, rrhb, rrhbc;
	UINT8	PortIndex;
	USBX_ACT = 0;  // debug LED
USBX_DevIntNext:
//	if ( ( USBX_INT & ( bUSBX0_INT_ACT | bUSBX1_INT_ACT | bUSBX2_INT_ACT | bUSBX3_INT_ACT ) ) == 0 ) return;
	us = USBX_SEL = bUSBX_XSFR_OFS | bUSBX_XRAM_OFS | USBX_INT & ( MASK_UX_PC_ID | MASK_UX_DEV_ID );  // USBX自动偏移,通过USBX0读写USBX0-USBX3
	switch ( us & MASK_UX_DEV_ID ) {  // which device
		case 0:  // sub-device 0#
//			if ( UIF_USBX_IF == 0 ) return;
//			USBX0_dev0_int( );
//			if ( pX0HB_STATUS & bUXS_IS_NAK ) {  // not enable for this example
//				switch ( pX0D0_STATUS & ( bUXS_SETUP_ACT | MASK_UXS_TOKEN | MASK_UXS_ENDP ) ) {  // 分析操作令牌和端点号
//				}
//				printf("NakInt,PrepareData\n");
//			} else
			switch ( pX0D0_STATUS & ( bUXS_SETUP_ACT | MASK_UXS_TOKEN | MASK_UXS_ENDP ) ) {  // 分析操作令牌和端点号
				case UXS_TOKEN_OUT | 3:  // endpoint 3# 端点下传
				case bUXS_SETUP_ACT | UXS_TOKEN_OUT | 3:
					if ( pX0D0_STATUS & bUXS_TOG_OK ) {  // 不同步的数据包将丢弃
//						pX0D0_EP3RES ^= bUEP_X_TOG;  // 已自动翻转
						len = pX0D0_RX_LEN;
						for ( i = 0; i < len; i ++ ) {
							(pUSBX0_BUF_DEV0+UX_EP2_ADDR)[i] = (pUSBX0_BUF_DEV0+UX_EP3_ADDR)[i] ^ 0xFF;  // OUT数据@3取反到IN@2由计算机验证
						}
						pX0D0_EP2T_L = len;
						pX0D0_EP2RES = pX0D0_EP2RES & ~ MASK_UEP_X_RES | UEP_X_RES_ACK;  // 允许上传
					}
					break;
				case UXS_TOKEN_IN | 2:  // endpoint 2# 端点上传
				case bUXS_SETUP_ACT | UXS_TOKEN_IN | 2:
//					pX0D0_EP2RES ^= bUEP_X_TOG;  // 已自动翻转
					pX0D0_EP2RES = pX0D0_EP2RES & ~ MASK_UEP_X_RES | UEP_X_RES_NAK;  // 暂停上传
					break;
				case UXS_TOKEN_IN | 1:  // endpoint 1# 端点上传
				case bUXS_SETUP_ACT | UXS_TOKEN_IN | 1:
//					pX0D0_EP1RES ^= bUEP_X_TOG;  // 已自动翻转
					pX0D0_EP1RES = pX0D0_EP1RES & ~ MASK_UEP_X_RES | UEP_X_RES_NAK;  // 暂停上传
					break;
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 0:  // endpoint 0# SETUP
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 1:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 2:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 3:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 4:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 5:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 6:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 7:
					X0D0SetupLen = pX0D0_SETUP_REQ->wLengthL;
					if ( pX0D0_SETUP_REQ->wLengthH || X0D0SetupLen > MAX_DESCR_SIZE ) X0D0SetupLen = MAX_DESCR_SIZE;  // 限制总长度
					len = 0;  // 默认为成功并且上传0长度
					if ( ( pX0D0_SETUP_REQ->bRequestType & USB_REQ_TYP_MASK ) == USB_REQ_TYP_STANDARD ) {  /* 标准请求 */
						X0D0SetupReqCode = pX0D0_SETUP_REQ->bRequest;
						switch( X0D0SetupReqCode ) {  // 请求码
							case USB_GET_DESCRIPTOR:
								switch( pX0D0_SETUP_REQ->wValueH ) {
									case USB_DESCR_TYP_DEVICE:  // 设备描述符
										X0D0pDescr = (PUINT8)( &Dev0DevDescr[0] );
										len = sizeof( Dev0DevDescr );
										break;
									case USB_DESCR_TYP_CONFIG:  // 配置描述符
										X0D0pDescr = (PUINT8)( &Dev0CfgDescr[0] );
										len = sizeof( Dev0CfgDescr );
										break;
									case USB_DESCR_TYP_STRING:  // 字符串描述符
										switch( pX0D0_SETUP_REQ->wValueL ) {
											case STR_DES_DEV_MANU:
												X0D0pDescr = (PUINT8)( &Dev0ManuInfo[0] );
												len = sizeof( Dev0ManuInfo );
												break;
											case STR_DES_DEV_PROD:
												X0D0pDescr = (PUINT8)( &Dev0ProdInfo[0] );
												len = sizeof( Dev0ProdInfo );
												break;
											case 0:
												X0D0pDescr = (PUINT8)( &cHubLangDescr[0] );
												len = sizeof( cHubLangDescr );
												break;
											default:
												len = 0xFF;  // 不支持的字符串描述符
												break;
										}
										break;
									case USB_DESCR_TYP_HID:  // HID描述符
										len = Dev0CfgDescr[0] + Dev0CfgDescr[ Dev0CfgDescr[0] ];
										X0D0pDescr = (PUINT8)( &Dev0CfgDescr[0] + len );
										len = Dev0CfgDescr[ len ];
										break;
									case USB_DESCR_TYP_REPORT:  // 报表描述符
										X0D0pDescr = (PUINT8)( &Dev0ReportDescr[0] );
										len = sizeof( Dev0ReportDescr );
										break;
									case USB_DESCR_TYP_PHYSIC:  // 物理描述符
										X0D0pDescr = (PUINT8)( &Dev0PhysicDescr[0] );  // 没准备
										len = sizeof( Dev0PhysicDescr );
										break;
									default:
										len = 0xFF;  // 不支持的描述符类型
										break;
								}
								if ( len != 0xFF ) {
									if ( X0D0SetupLen > len ) X0D0SetupLen = len;  // 限制总长度
									len = X0D0SetupLen >= EP0_PACKET_SIZE ? EP0_PACKET_SIZE : X0D0SetupLen;  // 本次传输长度
									X0D0SetupLen -= len;
									memcpy( pX0D0_EP0_BUF, X0D0pDescr, len );  /* 加载上传数据 */
									X0D0pDescr += len;
								}
								break;
							case USB_SET_ADDRESS:
								X0D0SetupLen = pX0D0_SETUP_REQ->wValueL;  // 暂存USB设备地址
								break;
							case USB_GET_CONFIGURATION:
								if ( X0D0SetupLen ) len = 1;  // 限制长度
								pX0D0_EP0_BUF[0] = X0D0UsbConfig;
								break;
							case USB_SET_CONFIGURATION:
								InitUSBX0_Dev0( 0 );
								X0D0UsbConfig = pX0D0_SETUP_REQ->wValueL;
								break;
							case USB_CLEAR_FEATURE:
								if ( pX0D0_SETUP_REQ->bRequestType == ( USB_REQ_TYP_STANDARD | USB_REQ_RECIP_ENDP ) ) {  // 目的类型:端点
									if ( pX0D0_SETUP_REQ->wValueL == 0 ) {  // ENDPOINT_HALT
										switch( pX0D0_SETUP_REQ->wIndexL ) {
											case 0x03:  // endpoint 3# 端点下传
												pX0D0_EP3RES = pX0D0_EP3RES & ~ ( bUEP_X_TOG | MASK_UEP_X_RES ) | UEP_X_RES_ACK;
												break;
											case 0x82:  // endpoint 2# 端点上传
												pX0D0_EP2RES = pX0D0_EP2RES & ~ ( bUEP_X_TOG | MASK_UEP_X_RES ) | UEP_X_RES_NAK;
												break;
											case 0x81:  // endpoint 1# 端点上传
												pX0D0_EP1RES = pX0D0_EP1RES & ~ ( bUEP_X_TOG | MASK_UEP_X_RES ) | UEP_X_RES_NAK;
												break;
											default:
												len = 0xFF;  // 不支持的端点
											break;
										}
									}
									else len = 0xFF;
								}
								else {  // 不是端点
//									len = 0xFF;
								}
								break;
							case USB_GET_INTERFACE:
								if ( X0D0SetupLen ) len = 1;  // 限制长度
								pX0D0_EP0_BUF[0] = 0x00;
								break;
							case USB_GET_STATUS:  // ???
								if ( X0D0SetupLen >= 2 ) len = 2;  // 限制长度
								else len = X0D0SetupLen;
								rrd0 = pX0D0_SETUP_REQ->bRequestType & USB_REQ_RECIP_MASK;
								if ( rrd0 == USB_REQ_RECIP_DEVICE ) pX0D0_EP0_BUF[0] = 0x01;
								else if ( rrd0 == USB_REQ_RECIP_INTERF ) pX0D0_EP0_BUF[0] = 0x00;
								else if ( rrd0 == USB_REQ_RECIP_ENDP ) pX0D0_EP0_BUF[0] = ( ( pX0D0_SETUP_REQ->wIndexL == 0x81 ? pX0D0_EP1RES
																							: ( pX0D0_SETUP_REQ->wIndexL == 0x82 ? pX0D0_EP2RES
																							: ( pX0D0_SETUP_REQ->wIndexL == 0x03 ? pX0D0_EP3RES : 0x00 ) ) ) & MASK_UEP_X_RES ) == UEP_X_RES_STALL ? 0x01 : 0x00;  // ???
								else pX0D0_EP0_BUF[0] = 0x00;
								pX0D0_EP0_BUF[1] = 0x00;
								break;
//							case USB_SET_FEATURE:  // ???
							default:
								len = 0xFF;  // 操作失败
//								printf("ErrEp0ReqCode=%02X\n",(UINT16)X0D0SetupReqCode);
								break;
						}
					}
					else if ( ( pX0D0_SETUP_REQ->bRequestType & USB_REQ_TYP_MASK ) == USB_REQ_TYP_CLASS ) {  /* 类请求 */
						X0D0SetupReqCode = pX0D0_SETUP_REQ->bRequest;
						switch( X0D0SetupReqCode ) {  // 请求码
							case HID_GET_REPORT:
								if ( X0D0SetupLen ) len = 1;  // 限制长度
								pX0D0_EP0_BUF[0] = 0x00;  // 设置报表请求的数据
								break;
							case HID_SET_REPORT:
//								= pX0D0_EP0_BUF[6];  // 报表长度
								break;
							case HID_GET_IDLE:
								if ( X0D0SetupLen ) len = 1;  // 限制长度
								pX0D0_EP0_BUF[0] = 0x00;  // 空闲时间
								break;
							case HID_SET_IDLE:
//								= pX0D0_EP0_BUF[3];  // 空闲时间
								break;
							case HID_GET_PROTOCOL:
								if ( X0D0SetupLen ) len = 1;  // 限制长度
								pX0D0_EP0_BUF[0] = X0D0_PROTOCOL;  // 协议标志
								break;
							case HID_SET_PROTOCOL:
								X0D0_PROTOCOL = pX0D0_EP0_BUF[2];  // 协议标志
								break;
							default:
								len = 0xFF;  // 操作失败
								break;
						}
					}
					else {  /* 只支持标准请求/类请求 */
						len = 0xFF;  // 操作失败
//						printf("ErrEp0ReqType=%02X\n",(UINT16)pX0D0_SETUP_REQ->bRequestType);
					}
					if ( len == 0xFF ) {  // 操作失败
						X0D0SetupReqCode = 0xFF;
						pX0D0_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;  // STALL
					}
					else if ( len <= EP0_PACKET_SIZE ) {  // 上传数据或者状态阶段返回0长度包
						pX0D0_EP0T_L = len;
						pX0D0_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // 默认数据包是DATA1
					}
					else {  // 下传数据或其它
						//????
						pX0D0_EP0T_L = 0;  // 虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
						pX0D0_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // 默认数据包是DATA1
					}
					break;
				case UXS_TOKEN_IN | 0:  // endpoint 0# IN
				case bUXS_SETUP_ACT | UXS_TOKEN_IN | 0:
					switch( X0D0SetupReqCode ) {
						case USB_GET_DESCRIPTOR:
							len = X0D0SetupLen >= EP0_PACKET_SIZE ? EP0_PACKET_SIZE : X0D0SetupLen;  // 本次传输长度
							X0D0SetupLen -= len;
							memcpy( pX0D0_EP0_BUF, X0D0pDescr, len );  /* 加载上传数据 */
							X0D0pDescr += len;
							pX0D0_EP0T_L = len;
							pX0D0_EP0RES ^= bUEP_T_TOG;  // 翻转
							break;
						case USB_SET_ADDRESS:
							pX0D0_ADDR = X0D0SetupLen;
							pX0D0_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
							break;
						default:
							pX0D0_EP0T_L = 0;  // 状态阶段完成中断或者是强制上传0长度数据包结束控制传输
							pX0D0_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
							break;
					}
					break;
				case UXS_TOKEN_OUT | 0:  // endpoint 0# OUT
				case bUXS_SETUP_ACT | UXS_TOKEN_OUT | 0:
					switch( X0D0SetupReqCode ) {
//						case download:
//							if ( pX0D0_STATUS & bUXS_TOG_OK ) {  // 不同步的数据包将丢弃
//								pX0D0_EP0RES ^= bUEP_R_TOG;  // 翻转
//								get_data;
//								//pX0D0_EP0RES = pX0D0_EP0RES & bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // 预置上传0长度数据包DATA1以防主机提前进入状态阶段
//							}
//							break;
						case USB_GET_DESCRIPTOR:
						default:
							if ( pX0D0_STATUS & bUXS_TOG_OK ) {  // 不同步的数据包将丢弃
//								if ( pX0D0_RX_LEN ) control_status_error;
//								else control_ok;  // 收到0长度包表示控制读操作/上传OK
							}
//							else control_status_error;
							pX0D0_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;  // 准备下一控制传输
							break;
					}
					break;
				default:
//					printf("ErrEndpInt\n");
					break;
			}
			UIF_USBX_IF = 0;  // 清中断标志
			break;
		case bUSBX_DEV_ID0:  // sub-device 1#
//			if ( UIF_USBX_IF == 0 ) return;
//			USBX0_dev1_int( );
//			if ( pX0HB_STATUS & bUXS_IS_NAK ) {  // not enable for this example
//				switch ( pX0D1_STATUS & ( bUXS_SETUP_ACT | MASK_UXS_TOKEN | MASK_UXS_ENDP ) ) {  // 分析操作令牌和端点号
//				}
//				printf("NakInt,PrepareData\n");
//			} else
			switch ( pX0D1_STATUS & ( bUXS_SETUP_ACT | MASK_UXS_TOKEN | MASK_UXS_ENDP ) ) {  // 分析操作令牌和端点号
				case UXS_TOKEN_OUT | 3:  // endpoint 3# 端点下传
				case bUXS_SETUP_ACT | UXS_TOKEN_OUT | 3:
					if ( pX0D1_STATUS & bUXS_TOG_OK ) {  // 不同步的数据包将丢弃
//						pX0D1_EP3RES ^= bUEP_X_TOG;  // 已自动翻转
						len = pX0D1_RX_LEN;
						for ( i = 0; i < len; i ++ ) {
							(pUSBX0_BUF_DEV1+UX_EP2_ADDR)[i] = (pUSBX0_BUF_DEV1+UX_EP3_ADDR)[i] ^ 0xFF;  // OUT数据@3取反到IN@2由计算机验证
						}
						pX0D1_EP2T_L = len;
						pX0D1_EP2RES = pX0D1_EP2RES & ~ MASK_UEP_X_RES | UEP_X_RES_ACK;  // 允许上传
					}
					break;
				case UXS_TOKEN_IN | 2:  // endpoint 2# 端点上传
				case bUXS_SETUP_ACT | UXS_TOKEN_IN | 2:
//					pX0D1_EP2RES ^= bUEP_X_TOG;  // 已自动翻转
					pX0D1_EP2RES = pX0D1_EP2RES & ~ MASK_UEP_X_RES | UEP_X_RES_NAK;  // 暂停上传
					break;
				case UXS_TOKEN_IN | 1:  // endpoint 1# 端点上传
				case bUXS_SETUP_ACT | UXS_TOKEN_IN | 1:
//					pX0D1_EP1RES ^= bUEP_X_TOG;  // 已自动翻转
					pX0D1_EP1RES = pX0D1_EP1RES & ~ MASK_UEP_X_RES | UEP_X_RES_NAK;  // 暂停上传
					break;
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 0:  // endpoint 0# SETUP
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 1:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 2:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 3:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 4:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 5:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 6:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 7:
					X0D1SetupLen = pX0D1_SETUP_REQ->wLengthL;
					if ( pX0D1_SETUP_REQ->wLengthH || X0D1SetupLen > MAX_DESCR_SIZE ) X0D1SetupLen = MAX_DESCR_SIZE;  // 限制总长度
					len = 0;  // 默认为成功并且上传0长度
					if ( ( pX0D1_SETUP_REQ->bRequestType & USB_REQ_TYP_MASK ) == USB_REQ_TYP_STANDARD ) {  /* 标准请求 */
						X0D1SetupReqCode = pX0D1_SETUP_REQ->bRequest;
						switch( X0D1SetupReqCode ) {  // 请求码
							case USB_GET_DESCRIPTOR:
								switch( pX0D1_SETUP_REQ->wValueH ) {
									case USB_DESCR_TYP_DEVICE:  // 设备描述符
										X0D1pDescr = (PUINT8)( &Dev1DevDescr[0] );
										len = sizeof( Dev1DevDescr );
										break;
									case USB_DESCR_TYP_CONFIG:  // 配置描述符
										X0D1pDescr = (PUINT8)( &Dev1CfgDescr[0] );
										len = sizeof( Dev1CfgDescr );
										break;
									case USB_DESCR_TYP_STRING:  // 字符串描述符
										switch( pX0D1_SETUP_REQ->wValueL ) {
											case STR_DES_DEV_MANU:
												X0D1pDescr = (PUINT8)( &Dev1ManuInfo[0] );
												len = sizeof( Dev1ManuInfo );
												break;
											case STR_DES_DEV_PROD:
												X0D1pDescr = (PUINT8)( &Dev1ProdInfo[0] );
												len = sizeof( Dev1ProdInfo );
												break;
											case 0:
												X0D1pDescr = (PUINT8)( &cHubLangDescr[0] );
												len = sizeof( cHubLangDescr );
												break;
											default:
												len = 0xFF;  // 不支持的字符串描述符
												break;
										}
										break;
									case USB_DESCR_TYP_HID:  // HID描述符
										len = Dev1CfgDescr[0] + Dev1CfgDescr[ Dev1CfgDescr[0] ];
										X0D1pDescr = (PUINT8)( &Dev1CfgDescr[0] + len );
										len = Dev1CfgDescr[ len ];
										break;
									case USB_DESCR_TYP_REPORT:  // 报表描述符
										X0D1pDescr = (PUINT8)( &Dev1ReportDescr[0] );
										len = sizeof( Dev1ReportDescr );
										break;
									case USB_DESCR_TYP_PHYSIC:  // 物理描述符
										X0D1pDescr = (PUINT8)( &Dev1PhysicDescr[0] );  // 没准备
										len = sizeof( Dev1PhysicDescr );
										break;
									default:
										len = 0xFF;  // 不支持的描述符类型
										break;
								}
								if ( len != 0xFF ) {
									if ( X0D1SetupLen > len ) X0D1SetupLen = len;  // 限制总长度
									len = X0D1SetupLen >= EP0_PACKET_SIZE ? EP0_PACKET_SIZE : X0D1SetupLen;  // 本次传输长度
									X0D1SetupLen -= len;
									memcpy( pX0D1_EP0_BUF, X0D1pDescr, len );  /* 加载上传数据 */
									X0D1pDescr += len;
								}
								break;
							case USB_SET_ADDRESS:
								X0D1SetupLen = pX0D1_SETUP_REQ->wValueL;  // 暂存USB设备地址
								break;
							case USB_GET_CONFIGURATION:
								if ( X0D1SetupLen ) len = 1;  // 限制长度
								pX0D1_EP0_BUF[0] = X0D1UsbConfig;
								break;
							case USB_SET_CONFIGURATION:
								InitUSBX0_Dev1( 0 );
								X0D1UsbConfig = pX0D1_SETUP_REQ->wValueL;
								break;
							case USB_CLEAR_FEATURE:
								if ( pX0D1_SETUP_REQ->bRequestType == ( USB_REQ_TYP_STANDARD | USB_REQ_RECIP_ENDP ) ) {  // 目的类型:端点
									if ( pX0D1_SETUP_REQ->wValueL == 0 ) {  // ENDPOINT_HALT
										switch( pX0D1_SETUP_REQ->wIndexL ) {
											case 0x03:  // endpoint 3# 端点下传
												pX0D1_EP3RES = pX0D1_EP3RES & ~ ( bUEP_X_TOG | MASK_UEP_X_RES ) | UEP_X_RES_ACK;
												break;
											case 0x82:  // endpoint 2# 端点上传
												pX0D1_EP2RES = pX0D1_EP2RES & ~ ( bUEP_X_TOG | MASK_UEP_X_RES ) | UEP_X_RES_NAK;
												break;
											case 0x81:  // endpoint 1# 端点上传
												pX0D1_EP1RES = pX0D1_EP1RES & ~ ( bUEP_X_TOG | MASK_UEP_X_RES ) | UEP_X_RES_NAK;
												break;
											default:
												len = 0xFF;  // 不支持的端点
											break;
										}
									}
									else len = 0xFF;
								}
								else {  // 不是端点
//									len = 0xFF;
								}
								break;
							case USB_GET_INTERFACE:
								if ( X0D1SetupLen ) len = 1;  // 限制长度
								pX0D1_EP0_BUF[0] = 0x00;
								break;
							case USB_GET_STATUS:  // ???
								if ( X0D1SetupLen >= 2 ) len = 2;  // 限制长度
								else len = X0D1SetupLen;
								rrd0 = pX0D1_SETUP_REQ->bRequestType & USB_REQ_RECIP_MASK;
								if ( rrd0 == USB_REQ_RECIP_DEVICE ) pX0D1_EP0_BUF[0] = 0x01;
								else if ( rrd0 == USB_REQ_RECIP_INTERF ) pX0D1_EP0_BUF[0] = 0x00;
								else if ( rrd0 == USB_REQ_RECIP_ENDP ) pX0D1_EP0_BUF[0] = ( ( pX0D1_SETUP_REQ->wIndexL == 0x81 ? pX0D1_EP1RES
																							: ( pX0D1_SETUP_REQ->wIndexL == 0x82 ? pX0D1_EP2RES
																							: ( pX0D1_SETUP_REQ->wIndexL == 0x03 ? pX0D1_EP3RES : 0x00 ) ) ) & MASK_UEP_X_RES ) == UEP_X_RES_STALL ? 0x01 : 0x00;  // ???
								else pX0D1_EP0_BUF[0] = 0x00;
								pX0D1_EP0_BUF[1] = 0x00;
								break;
//							case USB_SET_FEATURE:  // ???
							default:
								len = 0xFF;  // 操作失败
//								printf("ErrEp0ReqCode=%02X\n",(UINT16)X0D1SetupReqCode);
								break;
						}
					}
					else if ( ( pX0D1_SETUP_REQ->bRequestType & USB_REQ_TYP_MASK ) == USB_REQ_TYP_CLASS ) {  /* 类请求 */
						X0D1SetupReqCode = pX0D1_SETUP_REQ->bRequest;
						switch( X0D1SetupReqCode ) {  // 请求码
							case HID_GET_REPORT:
								if ( X0D1SetupLen ) len = 1;  // 限制长度
								pX0D1_EP0_BUF[0] = 0x00;  // 设置报表请求的数据
								break;
							case HID_SET_REPORT:
//								= pX0D1_EP0_BUF[6];  // 报表长度
								break;
							case HID_GET_IDLE:
								if ( X0D1SetupLen ) len = 1;  // 限制长度
								pX0D1_EP0_BUF[0] = 0x00;  // 空闲时间
								break;
							case HID_SET_IDLE:
//								= pX0D1_EP0_BUF[3];  // 空闲时间
								break;
							case HID_GET_PROTOCOL:
								if ( X0D1SetupLen ) len = 1;  // 限制长度
								pX0D1_EP0_BUF[0] = X0D1_PROTOCOL;  // 协议标志
								break;
							case HID_SET_PROTOCOL:
								X0D1_PROTOCOL = pX0D1_EP0_BUF[2];  // 协议标志
								break;
							default:
								len = 0xFF;  // 操作失败
								break;
						}
					}
					else {  /* 只支持标准请求/类请求 */
						len = 0xFF;  // 操作失败
//						printf("ErrEp0ReqType=%02X\n",(UINT16)pX0D1_SETUP_REQ->bRequestType);
					}
					if ( len == 0xFF ) {  // 操作失败
						X0D1SetupReqCode = 0xFF;
						pX0D1_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;  // STALL
					}
					else if ( len <= EP0_PACKET_SIZE ) {  // 上传数据或者状态阶段返回0长度包
						pX0D1_EP0T_L = len;
						pX0D1_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // 默认数据包是DATA1
					}
					else {  // 下传数据或其它
						//????
						pX0D1_EP0T_L = 0;  // 虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
						pX0D1_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // 默认数据包是DATA1
					}
					break;
				case UXS_TOKEN_IN | 0:  // endpoint 0# IN
				case bUXS_SETUP_ACT | UXS_TOKEN_IN | 0:
					switch( X0D1SetupReqCode ) {
						case USB_GET_DESCRIPTOR:
							len = X0D1SetupLen >= EP0_PACKET_SIZE ? EP0_PACKET_SIZE : X0D1SetupLen;  // 本次传输长度
							X0D1SetupLen -= len;
							memcpy( pX0D1_EP0_BUF, X0D1pDescr, len );  /* 加载上传数据 */
							X0D1pDescr += len;
							pX0D1_EP0T_L = len;
							pX0D1_EP0RES ^= bUEP_T_TOG;  // 翻转
							break;
						case USB_SET_ADDRESS:
							pX0D1_ADDR = X0D1SetupLen;
							pX0D1_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
							break;
						default:
							pX0D1_EP0T_L = 0;  // 状态阶段完成中断或者是强制上传0长度数据包结束控制传输
							pX0D1_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
							break;
					}
					break;
				case UXS_TOKEN_OUT | 0:  // endpoint 0# OUT
				case bUXS_SETUP_ACT | UXS_TOKEN_OUT | 0:
					switch( X0D1SetupReqCode ) {
//						case download:
//							if ( pX0D1_STATUS & bUXS_TOG_OK ) {  // 不同步的数据包将丢弃
//								pX0D1_EP0RES ^= bUEP_R_TOG;  // 翻转
//								get_data;
//								//pX0D1_EP0RES = pX0D1_EP0RES & bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // 预置上传0长度数据包DATA1以防主机提前进入状态阶段
//							}
//							break;
						case USB_GET_DESCRIPTOR:
						default:
							if ( pX0D1_STATUS & bUXS_TOG_OK ) {  // 不同步的数据包将丢弃
//								if ( pX0D1_RX_LEN ) control_status_error;
//								else control_ok;  // 收到0长度包表示控制读操作/上传OK
							}
//							else control_status_error;
							pX0D1_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;  // 准备下一控制传输
							break;
					}
					break;
				default:
//					printf("ErrEndpInt\n");
					break;
			}
			UIF_USBX_IF = 0;  // 清中断标志
			break;
		case bUSBX_DEV_ID1:  // sub-device 2#
//			if ( UIF_USBX_IF == 0 ) return;
//			USBX0_dev2_int( );
//			if ( pX0HB_STATUS & bUXS_IS_NAK ) {  // not enable for this example
//				switch ( pX0D2_STATUS & ( bUXS_SETUP_ACT | MASK_UXS_TOKEN | MASK_UXS_ENDP ) ) {  // 分析操作令牌和端点号
//				}
//				printf("NakInt,PrepareData\n");
//			} else
			switch ( pX0D2_STATUS & ( bUXS_SETUP_ACT | MASK_UXS_TOKEN | MASK_UXS_ENDP ) ) {  // 分析操作令牌和端点号
				case UXS_TOKEN_OUT | 3:  // endpoint 3# 端点下传
				case bUXS_SETUP_ACT | UXS_TOKEN_OUT | 3:
					if ( pX0D2_STATUS & bUXS_TOG_OK ) {  // 不同步的数据包将丢弃
//						pX0D2_EP3RES ^= bUEP_X_TOG;  // 已自动翻转
						len = pX0D2_RX_LEN;
						for ( i = 0; i < len; i ++ ) {
							(pUSBX0_BUF_DEV2+UX_EP2_ADDR)[i] = (pUSBX0_BUF_DEV2+UX_EP3_ADDR)[i] ^ 0xFF;  // OUT数据@3取反到IN@2由计算机验证
						}
						pX0D2_EP2T_L = len;
						pX0D2_EP2RES = pX0D2_EP2RES & ~ MASK_UEP_X_RES | UEP_X_RES_ACK;  // 允许上传
					}
					break;
				case UXS_TOKEN_IN | 2:  // endpoint 2# 端点上传
				case bUXS_SETUP_ACT | UXS_TOKEN_IN | 2:
//					pX0D2_EP2RES ^= bUEP_X_TOG;  // 已自动翻转
					pX0D2_EP2RES = pX0D2_EP2RES & ~ MASK_UEP_X_RES | UEP_X_RES_NAK;  // 暂停上传
					break;
				case UXS_TOKEN_IN | 1:  // endpoint 1# 端点上传
				case bUXS_SETUP_ACT | UXS_TOKEN_IN | 1:
//					pX0D2_EP1RES ^= bUEP_X_TOG;  // 已自动翻转
					pX0D2_EP1RES = pX0D2_EP1RES & ~ MASK_UEP_X_RES | UEP_X_RES_NAK;  // 暂停上传
					break;
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 0:  // endpoint 0# SETUP
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 1:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 2:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 3:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 4:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 5:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 6:
				case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 7:
					X0D2SetupLen = pX0D2_SETUP_REQ->wLengthL;
					if ( pX0D2_SETUP_REQ->wLengthH || X0D2SetupLen > MAX_DESCR_SIZE ) X0D2SetupLen = MAX_DESCR_SIZE;  // 限制总长度
					len = 0;  // 默认为成功并且上传0长度
					if ( ( pX0D2_SETUP_REQ->bRequestType & USB_REQ_TYP_MASK ) == USB_REQ_TYP_STANDARD ) {  /* 标准请求 */
						X0D2SetupReqCode = pX0D2_SETUP_REQ->bRequest;
						switch( X0D2SetupReqCode ) {  // 请求码
							case USB_GET_DESCRIPTOR:
								switch( pX0D2_SETUP_REQ->wValueH ) {
									case USB_DESCR_TYP_DEVICE:  // 设备描述符
										X0D2pDescr = (PUINT8)( &Dev2DevDescr[0] );
										len = sizeof( Dev2DevDescr );
										break;
									case USB_DESCR_TYP_CONFIG:  // 配置描述符
										X0D2pDescr = (PUINT8)( &Dev2CfgDescr[0] );
										len = sizeof( Dev2CfgDescr );
										break;
									case USB_DESCR_TYP_STRING:  // 字符串描述符
										switch( pX0D2_SETUP_REQ->wValueL ) {
											case STR_DES_DEV_MANU:
												X0D2pDescr = (PUINT8)( &Dev2ManuInfo[0] );
												len = sizeof( Dev2ManuInfo );
												break;
											case STR_DES_DEV_PROD:
												X0D2pDescr = (PUINT8)( &Dev2ProdInfo[0] );
												len = sizeof( Dev2ProdInfo );
												break;
											case 0:
												X0D2pDescr = (PUINT8)( &cHubLangDescr[0] );
												len = sizeof( cHubLangDescr );
												break;
											default:
												len = 0xFF;  // 不支持的字符串描述符
												break;
										}
										break;
									case USB_DESCR_TYP_HID:  // HID描述符
										len = Dev2CfgDescr[0] + Dev2CfgDescr[ Dev2CfgDescr[0] ];
										X0D2pDescr = (PUINT8)( &Dev2CfgDescr[0] + len );
										len = Dev2CfgDescr[ len ];
										break;
									case USB_DESCR_TYP_REPORT:  // 报表描述符
										X0D2pDescr = (PUINT8)( &Dev2ReportDescr[0] );
										len = sizeof( Dev2ReportDescr );
										break;
									case USB_DESCR_TYP_PHYSIC:  // 物理描述符
										X0D2pDescr = (PUINT8)( &Dev2PhysicDescr[0] );  // 没准备
										len = sizeof( Dev2PhysicDescr );
										break;
									default:
										len = 0xFF;  // 不支持的描述符类型
										break;
								}
								if ( len != 0xFF ) {
									if ( X0D2SetupLen > len ) X0D2SetupLen = len;  // 限制总长度
									len = X0D2SetupLen >= EP0_PACKET_SIZE ? EP0_PACKET_SIZE : X0D2SetupLen;  // 本次传输长度
									X0D2SetupLen -= len;
									memcpy( pX0D2_EP0_BUF, X0D2pDescr, len );  /* 加载上传数据 */
									X0D2pDescr += len;
								}
								break;
							case USB_SET_ADDRESS:
								X0D2SetupLen = pX0D2_SETUP_REQ->wValueL;  // 暂存USB设备地址
								break;
							case USB_GET_CONFIGURATION:
								if ( X0D2SetupLen ) len = 1;  // 限制长度
								pX0D2_EP0_BUF[0] = X0D2UsbConfig;
								break;
							case USB_SET_CONFIGURATION:
								InitUSBX0_Dev2( 0 );
								X0D2UsbConfig = pX0D2_SETUP_REQ->wValueL;
								break;
							case USB_CLEAR_FEATURE:
								if ( pX0D2_SETUP_REQ->bRequestType == ( USB_REQ_TYP_STANDARD | USB_REQ_RECIP_ENDP ) ) {  // 目的类型:端点
									if ( pX0D2_SETUP_REQ->wValueL == 0 ) {  // ENDPOINT_HALT
										switch( pX0D2_SETUP_REQ->wIndexL ) {
											case 0x03:  // endpoint 3# 端点下传
												pX0D2_EP3RES = pX0D2_EP3RES & ~ ( bUEP_X_TOG | MASK_UEP_X_RES ) | UEP_X_RES_ACK;
												break;
											case 0x82:  // endpoint 2# 端点上传
												pX0D2_EP2RES = pX0D2_EP2RES & ~ ( bUEP_X_TOG | MASK_UEP_X_RES ) | UEP_X_RES_NAK;
												break;
											case 0x81:  // endpoint 1# 端点上传
												pX0D2_EP1RES = pX0D2_EP1RES & ~ ( bUEP_X_TOG | MASK_UEP_X_RES ) | UEP_X_RES_NAK;
												break;
											default:
												len = 0xFF;  // 不支持的端点
											break;
										}
									}
									else len = 0xFF;
								}
								else {  // 不是端点
//									len = 0xFF;
								}
								break;
							case USB_GET_INTERFACE:
								if ( X0D2SetupLen ) len = 1;  // 限制长度
								pX0D2_EP0_BUF[0] = 0x00;
								break;
							case USB_GET_STATUS:  // ???
								if ( X0D2SetupLen >= 2 ) len = 2;  // 限制长度
								else len = X0D2SetupLen;
								rrd0 = pX0D2_SETUP_REQ->bRequestType & USB_REQ_RECIP_MASK;
								if ( rrd0 == USB_REQ_RECIP_DEVICE ) pX0D2_EP0_BUF[0] = 0x01;
								else if ( rrd0 == USB_REQ_RECIP_INTERF ) pX0D2_EP0_BUF[0] = 0x00;
								else if ( rrd0 == USB_REQ_RECIP_ENDP ) pX0D2_EP0_BUF[0] = ( ( pX0D2_SETUP_REQ->wIndexL == 0x81 ? pX0D2_EP1RES
																							: ( pX0D2_SETUP_REQ->wIndexL == 0x82 ? pX0D2_EP2RES
																							: ( pX0D2_SETUP_REQ->wIndexL == 0x03 ? pX0D2_EP3RES : 0x00 ) ) ) & MASK_UEP_X_RES ) == UEP_X_RES_STALL ? 0x01 : 0x00;  // ???
								else pX0D2_EP0_BUF[0] = 0x00;
								pX0D2_EP0_BUF[1] = 0x00;
								break;
//							case USB_SET_FEATURE:  // ???
							default:
								len = 0xFF;  // 操作失败
//								printf("ErrEp0ReqCode=%02X\n",(UINT16)X0D2SetupReqCode);
								break;
						}
					}
					else if ( ( pX0D2_SETUP_REQ->bRequestType & USB_REQ_TYP_MASK ) == USB_REQ_TYP_CLASS ) {  /* 类请求 */
						X0D2SetupReqCode = pX0D2_SETUP_REQ->bRequest;
						switch( X0D2SetupReqCode ) {  // 请求码
							case HID_GET_REPORT:
								if ( X0D2SetupLen ) len = 1;  // 限制长度
								pX0D2_EP0_BUF[0] = 0x00;  // 设置报表请求的数据
								break;
							case HID_SET_REPORT:
//								= pX0D2_EP0_BUF[6];  // 报表长度
								break;
							case HID_GET_IDLE:
								if ( X0D2SetupLen ) len = 1;  // 限制长度
								pX0D2_EP0_BUF[0] = 0x00;  // 空闲时间
								break;
							case HID_SET_IDLE:
//								= pX0D2_EP0_BUF[3];  // 空闲时间
								break;
							case HID_GET_PROTOCOL:
								if ( X0D2SetupLen ) len = 1;  // 限制长度
								pX0D2_EP0_BUF[0] = X0D2_PROTOCOL;  // 协议标志
								break;
							case HID_SET_PROTOCOL:
								X0D2_PROTOCOL = pX0D2_EP0_BUF[2];  // 协议标志
								break;
							default:
								len = 0xFF;  // 操作失败
								break;
						}
					}
					else {  /* 只支持标准请求/类请求 */
						len = 0xFF;  // 操作失败
//						printf("ErrEp0ReqType=%02X\n",(UINT16)pX0D2_SETUP_REQ->bRequestType);
					}
					if ( len == 0xFF ) {  // 操作失败
						X0D2SetupReqCode = 0xFF;
						pX0D2_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;  // STALL
					}
					else if ( len <= EP0_PACKET_SIZE ) {  // 上传数据或者状态阶段返回0长度包
						pX0D2_EP0T_L = len;
						pX0D2_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // 默认数据包是DATA1
					}
					else {  // 下传数据或其它
						//????
						pX0D2_EP0T_L = 0;  // 虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
						pX0D2_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // 默认数据包是DATA1
					}
					break;
				case UXS_TOKEN_IN | 0:  // endpoint 0# IN
				case bUXS_SETUP_ACT | UXS_TOKEN_IN | 0:
					switch( X0D2SetupReqCode ) {
						case USB_GET_DESCRIPTOR:
							len = X0D2SetupLen >= EP0_PACKET_SIZE ? EP0_PACKET_SIZE : X0D2SetupLen;  // 本次传输长度
							X0D2SetupLen -= len;
							memcpy( pX0D2_EP0_BUF, X0D2pDescr, len );  /* 加载上传数据 */
							X0D2pDescr += len;
							pX0D2_EP0T_L = len;
							pX0D2_EP0RES ^= bUEP_T_TOG;  // 翻转
							break;
						case USB_SET_ADDRESS:
							pX0D2_ADDR = X0D2SetupLen;
							pX0D2_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
							break;
						default:
							pX0D2_EP0T_L = 0;  // 状态阶段完成中断或者是强制上传0长度数据包结束控制传输
							pX0D2_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
							break;
					}
					break;
				case UXS_TOKEN_OUT | 0:  // endpoint 0# OUT
				case bUXS_SETUP_ACT | UXS_TOKEN_OUT | 0:
					switch( X0D2SetupReqCode ) {
//						case download:
//							if ( pX0D2_STATUS & bUXS_TOG_OK ) {  // 不同步的数据包将丢弃
//								pX0D2_EP0RES ^= bUEP_R_TOG;  // 翻转
//								get_data;
//								//pX0D2_EP0RES = pX0D2_EP0RES & bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // 预置上传0长度数据包DATA1以防主机提前进入状态阶段
//							}
//							break;
						case USB_GET_DESCRIPTOR:
						default:
							if ( pX0D2_STATUS & bUXS_TOG_OK ) {  // 不同步的数据包将丢弃
//								if ( pX0D2_RX_LEN ) control_status_error;
//								else control_ok;  // 收到0长度包表示控制读操作/上传OK
							}
//							else control_status_error;
							pX0D2_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;  // 准备下一控制传输
							break;
					}
					break;
				default:
//					printf("ErrEndpInt\n");
					break;
			}
			UIF_USBX_IF = 0;  // 清中断标志
			break;
//		case ( bUSBX_DEV_ID0 | bUSBX_DEV_ID1 ):  // hub
		default:
//			USBX0_hub_int( );
			if ( pUSBX0_IF & bUX_IF_HB_TRANS ) {  // USBX传输完成
//				if ( pX0HB_STATUS & bUXS_IS_NAK ) {  // not enable for this example
//					switch ( pX0HB_STATUS & ( bUXS_SETUP_ACT | MASK_UXS_TOKEN | MASK_UXS_ENDP ) ) {  // 分析操作令牌和端点号
//					}
//					printf("NakInt,PrepareData\n");
//				} else
				switch ( pX0HB_STATUS & ( bUXS_SETUP_ACT | MASK_UXS_TOKEN | MASK_UXS_ENDP ) ) {  // 分析操作令牌和端点号
					case UXS_TOKEN_IN | 1:  // endpoint 1# 中断端点上传
					case bUXS_SETUP_ACT | UXS_TOKEN_IN | 1:
//						pX0HB_EP1RES ^= bUEP_X_TOG;  // 已自动翻转
						pX0HB_EP1RES = pX0HB_EP1RES & ~ MASK_UEP_X_RES | UEP_X_RES_NAK;  // 暂停上传
						pX0HB_EP1_BUF[0] = 0x00;
						break;
					case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 0:  // endpoint 0# SETUP
					case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 1:
					case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 2:
					case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 3:
					case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 4:
					case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 5:
					case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 6:
					case bUXS_SETUP_ACT | UXS_TOKEN_FREE | 7:
						X0HBSetupLen = pX0HB_SETUP_REQ->wLengthL;
						if ( pX0HB_SETUP_REQ->wLengthH || X0HBSetupLen > MAX_DESCR_SIZE ) X0HBSetupLen = MAX_DESCR_SIZE;  // 限制总长度
						len = 0;  // 默认为成功并且上传0长度
						if ( ( pX0HB_SETUP_REQ->bRequestType & USB_REQ_TYP_MASK ) == USB_REQ_TYP_STANDARD ) {  /* 标准请求 */
							X0HBSetupReqCode = pX0HB_SETUP_REQ->bRequest;
							switch( X0HBSetupReqCode ) {  // 请求码
								case USB_GET_DESCRIPTOR:
									switch( pX0HB_SETUP_REQ->wValueH ) {
										case USB_DESCR_TYP_DEVICE:  // 设备描述符
											X0HBpDescr = (PUINT8)( &cHubDevDescr[0] );
											len = sizeof( cHubDevDescr );
											break;
										case USB_DESCR_TYP_CONFIG:  // 配置描述符
											X0HBpDescr = (PUINT8)( &cHubCfgDescr[0] );
											len = sizeof( cHubCfgDescr );
											break;
										case USB_DESCR_TYP_STRING:  // 字符串描述符
											switch( pX0HB_SETUP_REQ->wValueL ) {
												case STR_DES_HUB_MANU:
													X0HBpDescr = (PUINT8)( &cHubManuInfo[0] );
													len = sizeof( cHubManuInfo );
													break;
												case STR_DES_HUB_PROD:
													X0HBpDescr = (PUINT8)( &cHubProdInfo[0] );
													len = sizeof( cHubProdInfo );
													break;
												case 0:
													X0HBpDescr = (PUINT8)( &cHubLangDescr[0] );
													len = sizeof( cHubLangDescr );
													break;
												default:
													len = 0xFF;  // 不支持的字符串描述符
													break;
											}
											break;
										default:
											len = 0xFF;  // 不支持的描述符类型
											break;
									}
									if ( len != 0xFF ) {
										if ( X0HBSetupLen > len ) X0HBSetupLen = len;  // 限制总长度
										len = X0HBSetupLen >= EP0_PACKET_SIZE ? EP0_PACKET_SIZE : X0HBSetupLen;  // 本次传输长度
										X0HBSetupLen -= len;
										memcpy( pX0HB_EP0_BUF, X0HBpDescr, len );  /* 加载上传数据 */
										X0HBpDescr += len;
									}
									break;
								case USB_SET_ADDRESS:
									X0HBSetupLen = pX0HB_SETUP_REQ->wValueL;  // 暂存USB设备地址
									break;
								case USB_GET_CONFIGURATION:
									if ( X0HBSetupLen ) len = 1;  // 限制长度
									pX0HB_EP0_BUF[0] = X0HBUsbConfig;
									break;
								case USB_SET_CONFIGURATION:
									InitUSBX0_Hub( FALSE );
									X0HBUsbConfig = pX0HB_SETUP_REQ->wValueL;
									if ( X0HBUsbConfig ) {  // 重新配置
										ThisHubEP1IntUp( (X0DevBitFlag[0]?0x02:0)|(X0DevBitFlag[1]?0x04:0)|(X0DevBitFlag[2]?0x08:0)|(X0DevBitFlag[3]?0x10:0) );  // 为当前USBX上传HUB中断通知
									}
									break;
								case USB_CLEAR_FEATURE:
									if ( pX0HB_SETUP_REQ->bRequestType == ( USB_REQ_TYP_STANDARD | USB_REQ_RECIP_ENDP ) ) {  // 目的类型:端点
										if ( pX0HB_SETUP_REQ->wValueL == 0 ) {  // ENDPOINT_HALT
											switch( pX0HB_SETUP_REQ->wIndexL ) {
												case 0x81:  // endpoint 1# 端点上传
													pX0HB_EP1RES = pX0HB_EP1RES & ~ ( bUEP_X_TOG | MASK_UEP_X_RES ) | UEP_X_RES_NAK;
													break;
												default:
													len = 0xFF;  // 不支持的端点
													break;
											}
										}
										else len = 0xFF;
									}
									else {  // 不是端点
//										len = 0xFF;
									}
									break;
								case USB_GET_INTERFACE:
									if ( X0HBSetupLen ) len = 1;  // 限制长度
									pX0HB_EP0_BUF[0] = 0x00;
									break;
								case USB_GET_STATUS:  // ???
									if ( X0HBSetupLen >= 2 ) len = 2;  // 限制长度
									else len = X0HBSetupLen;
									rrhb = pX0HB_SETUP_REQ->bRequestType & USB_REQ_RECIP_MASK;
									if ( rrhb == USB_REQ_RECIP_DEVICE ) pX0HB_EP0_BUF[0] = 0x01;
									else if ( rrhb == USB_REQ_RECIP_INTERF ) pX0HB_EP0_BUF[0] = 0x00;
									else if ( rrhb == USB_REQ_RECIP_ENDP ) pX0HB_EP0_BUF[0] = ( ( pX0HB_SETUP_REQ->wIndexL == 0x81 ? pX0HB_EP1RES : 0x00 ) & MASK_UEP_X_RES ) == UEP_X_RES_STALL ? 0x01 : 0x00;  // ???
									else pX0HB_EP0_BUF[0] = 0x00;
									pX0HB_EP0_BUF[1] = 0x00;
									break;
//								case USB_SET_FEATURE:  // ???
								default:
									len = 0xFF;  // 操作失败
//									printf("ErrEp0ReqCode=%02X\n",(UINT16)X0HBSetupReqCode);
									break;
							}
						}
						else if ( ( pX0HB_SETUP_REQ->bRequestType & USB_REQ_TYP_MASK ) == USB_REQ_TYP_CLASS ) {  /* 类请求 */
							X0HBSetupReqCode = pX0HB_SETUP_REQ->bRequest;
							switch( X0HBSetupReqCode ) {  // 请求码
								case HUB_GET_DESCRIPTOR:  // HUB描述符
									if ( X0HBSetupLen > sizeof( cHubDevDescr ) ) X0HBSetupLen = sizeof( cHubDevDescr );  // 限制总长度
									len = X0HBSetupLen >= EP0_PACKET_SIZE ? EP0_PACKET_SIZE : X0HBSetupLen;  // 本次传输长度
									X0HBSetupLen -= len;
									X0HBpDescr = (PUINT8)( &cHubDevDescr[0] );
									memcpy( pX0HB_EP0_BUF, X0HBpDescr, len );  /* 加载上传数据 */
									X0HBpDescr += len;
									break;
								case USB_SET_FEATURE:
									if ( pX0HB_SETUP_REQ->bRequestType == ( USB_REQ_TYP_CLASS | USB_REQ_RECIP_OTHER ) ) {  // 目的类型
										PortIndex = pX0HB_SETUP_REQ->wIndexL - 1;  // 端口
										if ( PortIndex <= 3 ) {
											switch( pX0HB_SETUP_REQ->wValueL ) {  // 特性选择符
												case HUB_PORT_RESET:  // 端口复位
													if ( PortIndex != 3 ) {
														if ( PortIndex == 0 ) InitUSBX0_Dev0( 2 );
														else if ( PortIndex == 1 ) InitUSBX0_Dev1( 2 );
														else InitUSBX0_Dev2( 2 );
														X0DevBitFlag[PortIndex] = BIT_PORT_CONNECT | BIT_PORT_C_REST;  //保持端口连接状态,复位完成
														ThisHubEP1IntUp( 0x01 << ( PortIndex + 1 ) );  // 为当前USBX上传HUB中断通知
													}
													else {
														X0DevBitFlag[3] = 0x00;  // 端口空闲
														if ( ( USBX_SEL & MASK_UX_PC_SEL ) == ActionUSBX && ( Dev3FlagPC & BIT_DEV3_IS_PASS ) ) {  // 当前PC下要求直连
															USB_CTRL &= ~ bUC_THROUGH;  // USB直通禁止
															ResetRootHubPort( Dev3FlagPC & BIT_DEV3_ROOT_IDX, 1 );  // 开始复位总线
															TaskResetTimer = TASK_TIMER_RESET;  // 启用延时任务
														}
														else {
															len = 0xFF;  // 操作失败
														}
													}
													break;
												case HUB_PORT_POWER:  // 端口上电
													break;
												case HUB_PORT_SUSPEND:  // 端口挂起
													break;
												default:
//													len = 0xFF;  // 操作失败
													break;
											}
										}
										else {
											len = 0xFF;  // 操作失败
										}
									}
//									else if ( pX0HB_SETUP_REQ->bRequestType == ( USB_REQ_TYP_CLASS | USB_REQ_RECIP_DEVICE ) ) {
//										len = 0x00;
//									}
									else {
//										len = 0xFF;  // 操作失败
									}
									break;
								case HUB_CLEAR_FEATURE:
									if ( pX0HB_SETUP_REQ->bRequestType == ( USB_REQ_TYP_CLASS | USB_REQ_RECIP_OTHER ) ) {  // 目的类型
										PortIndex = pX0HB_SETUP_REQ->wIndexL - 1;  // 端口
										if ( PortIndex <= 3 ) {
											switch( pX0HB_SETUP_REQ->wValueL ) {  // 特性选择符
												case HUB_PORT_ENABLE:  // 端口禁止
													if ( PortIndex == 0 ) pX0D0_EP_MOD &= ~ bUX_DEV_EN;  // 端口禁止
													else if ( PortIndex == 1 ) pX0D1_EP_MOD &= ~ bUX_DEV_EN;  // 端口禁止
													else if ( PortIndex == 2 ) pX0D2_EP_MOD &= ~ bUX_DEV_EN;  // 端口禁止
													else {
														USB_CTRL &= ~ bUC_THROUGH;  // USB直通禁止
													}
													break;
												case HUB_PORT_POWER:  // 端口下电
													break;
												case HUB_PORT_SUSPEND:  // 端口唤醒
													break;
												case HUB_C_PORT_CONNECTION:  // 清端口连接改变
													X0DevBitFlag[PortIndex] &= ~ BIT_PORT_C_CONN;
													break;
												case HUB_C_PORT_RESET:  // 清端口复位完成
													X0DevBitFlag[PortIndex] &= ~ BIT_PORT_C_REST;
													break;
												default:
//													len = 0xFF;  // 操作失败
													break;
											}
										}
										else {
											len = 0xFF;  // 操作失败
										}
									}
//									else if ( pX0HB_SETUP_REQ->bRequestType == ( USB_REQ_TYP_CLASS | USB_REQ_RECIP_DEVICE ) ) {
//										len = 0x00;
//									}
									else {
//										len = 0xFF;  // 操作失败
									}
									break;
								case HUB_GET_STATUS:
									if ( X0HBSetupLen >= 4 ) len = 4;  // 限制长度
									else len = X0HBSetupLen;
									PortIndex = pX0HB_SETUP_REQ->wIndexL - 1;  // 端口
									rrhbc = pX0HB_SETUP_REQ->bRequestType & USB_REQ_RECIP_MASK;
									pX0HB_EP0_BUF[3] = pX0HB_EP0_BUF[2] = pX0HB_EP0_BUF[1] = pX0HB_EP0_BUF[0] = 0x00;  // default
									if ( rrhbc == USB_REQ_RECIP_OTHER ) {  // 目的类型
										if ( PortIndex <= 3 ) {
											pX0HB_EP0_BUF[1] |= 0x01 << ( HUB_PORT_POWER - 8 );  // 已经上电
											if ( X0DevBitFlag[PortIndex] & BIT_PORT_C_CONN ) pX0HB_EP0_BUF[2] |= 0x01 << HUB_PORT_CONNECTION;  // 连接状态改变
											if ( X0DevBitFlag[PortIndex] & BIT_PORT_CONNECT ) {  // 已经连接
												if ( PortIndex != 3 ) {
													if ( ( PortIndex == 0 ? pX0D0_EP_MOD : ( PortIndex == 1 ? pX0D1_EP_MOD : pX0D2_EP_MOD ) ) & bUX_DEV_EN ) pX0HB_EP0_BUF[0] |= 0x01 << HUB_PORT_ENABLE;  // 端口使能
X0HB_HUB_GET_STATUS:
													pX0HB_EP0_BUF[0] |= 0x01 << HUB_PORT_CONNECTION;  // 子设备存在
													if ( X0DevBitFlag[PortIndex] & BIT_PORT_C_REST ) pX0HB_EP0_BUF[2] |= 0x01 << HUB_PORT_RESET;  // 复位完成
												}
												else {  // 直通设备
													if ( ( UH_THROUGH & MASK_UH_THR_UX ) == ( USBX_SEL & MASK_UX_PC_SEL ) ) {  // 当前直通设备
														if ( USB_CTRL & bUC_THROUGH ) pX0HB_EP0_BUF[0] |= 0x01 << HUB_PORT_ENABLE;  // 端口使能
														else if ( TaskResetTimer ) pX0HB_EP0_BUF[0] |= 0x01 << HUB_PORT_RESET;  // 设备在总线复位
														goto X0HB_HUB_GET_STATUS;
													}
													else X0DevBitFlag[3] = 0x00;  // 当前已被切换掉
												}
											}
										}
									}
//									else if ( rrhbc == USB_REQ_RECIP_DEVICE ) {
//									}
									else {
//										len = 0xFF;  // 操作失败
									}
									break;
								default:
									len = 0xFF;  // 操作失败
//									printf("ErrEp0ReqCode=%02X\n",(UINT16)X0HBSetupReqCode);
									break;
							}
						}
						else {  /* 只支持标准请求/类请求 */
							len = 0xFF;  // 操作失败
//							printf("ErrEp0ReqType=%02X\n",(UINT16)pX0HB_SETUP_REQ->bRequestType);
						}
						if ( len == 0xFF ) {  // 操作失败
							X0HBSetupReqCode = 0xFF;
							pX0HB_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;  // STALL
						}
						else if ( len <= EP0_PACKET_SIZE ) {  // 上传数据或者状态阶段返回0长度包
							pX0HB_EP0T_L = len;
							pX0HB_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // 默认数据包是DATA1
						}
						else {  // 下传数据或其它
							//????
							pX0HB_EP0T_L = 0;  // 虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
							pX0HB_EP0RES = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // 默认数据包是DATA1
						}
						break;
					case UXS_TOKEN_IN | 0:  // endpoint 0# IN
					case bUXS_SETUP_ACT | UXS_TOKEN_IN | 0:
						switch( X0HBSetupReqCode ) {
							case USB_GET_DESCRIPTOR:
								len = X0HBSetupLen >= EP0_PACKET_SIZE ? EP0_PACKET_SIZE : X0HBSetupLen;  // 本次传输长度
								X0HBSetupLen -= len;
								memcpy( pX0HB_EP0_BUF, X0HBpDescr, len );  /* 加载上传数据 */
								X0HBpDescr += len;
								pX0HB_EP0T_L = len;
								pX0HB_EP0RES ^= bUEP_T_TOG;  // 翻转
								break;
							case USB_SET_ADDRESS:
								pX0HB_ADDR = X0HBSetupLen;
								pX0HB_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
								break;
							default:
								pX0HB_EP0T_L = 0;  // 状态阶段完成中断或者是强制上传0长度数据包结束控制传输
								pX0HB_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
								break;
						}
						break;
					case UXS_TOKEN_OUT | 0:  // endpoint 0# OUT
					case bUXS_SETUP_ACT | UXS_TOKEN_OUT | 0:
						switch( X0HBSetupReqCode ) {
//							case download:
//								if ( pX0HB_STATUS & bUXS_TOG_OK ) {  // 不同步的数据包将丢弃
//									pX0HB_EP0RES ^= bUEP_R_TOG;  // 翻转
//									get_data;
//									//pX0HB_EP0RES = pX0HB_EP0RES & bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // 预置上传0长度数据包DATA1以防主机提前进入状态阶段
//								}
//								break;
							case USB_GET_DESCRIPTOR:
							default:
								if ( pX0HB_STATUS & bUXS_TOG_OK ) {  // 不同步的数据包将丢弃
//									if ( pX0HB_RX_LEN ) control_status_error;
//									else control_ok;  // 收到0长度包表示控制读操作/上传OK
								}
//								else control_status_error;
								pX0HB_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;  // 准备下一控制传输
								break;
						}
						break;
					default:
//						printf("ErrEndpInt\n");
						break;
				}
				UIF_USBX_IF = 0;  // 清中断标志
			}
			if ( pUSBX0_IF & bUX_IF_BUS_RST ) {  // USBX总线复位
//				pX0HB_ADDR = pX0D2_ADDR = pX0D1_ADDR = pX0D0_ADDR = 0x00;
//				pX0HB_EP0RES = pX0D2_EP0RES = pX0D1_EP0RES = pX0D0_EP0RES = UEP_R_RES_ACK | UEP_T_RES_NAK;
//				pUSBX0_IF = 0xFF;  // clear all interrupt flag
				InitUSBX0_Hub( TRUE );
			}
			if ( pUSBX0_IF & bUX_IF_SUSPEND ) {  // USBX总线挂起/唤醒完成
				pUSBX0_IF = bUX_IF_SUSPEND;  // clear
//				USBX_SEL &= ~ ( bUSBX_XSFR_OFS | bUSBX_XRAM_OFS );  // 解除自动偏移
				USBX_SEL = 0x00;  // 解除自动偏移
				if ( ( ( pUSBX0_IF & pUSBX1_IF & pUSBX2_IF & pUSBX3_IF ) & bUX_SUSPEND ) && TaskReadyTimer == 0 && TaskResetTimer == 0 ) {  // all suspend
					printf( "sleep" );  // 睡眠状态
					while ( XBUS_AUX & bUART0_TX );  // 等待发送完成
					while ( ( USB_MIS_ST & ( bUMS_SOF_PRES | bUMS_SOF_ACT | bUMS_SIE_FREE ) ) != bUMS_SIE_FREE );
					SAFE_MOD = 0x55;
					SAFE_MOD = 0xAA;
					WAKE_CTRL = bWAK_BY_USB | bWAK_BY_USBX | bWAK_RXD0_LO;  // USB或者USBX或者RXD0有信号时可被唤醒
					PCON |= PD;  // 睡眠
					SAFE_MOD = 0x55;
					SAFE_MOD = 0xAA;
					WAKE_CTRL = 0x00;
					printf( "wakeup" );  // 睡眠状态
				}
//				USBX_SEL |= bUSBX_XSFR_OFS | bUSBX_XRAM_OFS;  // USBX自动偏移,通过USBX0读写USBX0-USBX3
			}
			break;
	}
	if ( USBX_INT & ( bUSBX0_INT_ACT | bUSBX1_INT_ACT | bUSBX2_INT_ACT | bUSBX3_INT_ACT ) ) goto USBX_DevIntNext;
//	USBX_SEL = 0x00;
	USBX_ACT = 1;  // debug LED
}
#pragma	modp2


void	SwitchUSBX( UINT8 newid )
{
	UINT8	PortBit;
	if ( Dev3FlagPC & BIT_DEV3_IS_PASS ) {
		if ( IsRootHubEnabled( Dev3FlagPC & BIT_DEV3_ROOT_IDX ) == 0x00 ) {
			Dev3FlagPC = 0x00;  // 直通设备已移除
			goto SwitchUSBX_Change;
		}
	}
	if ( newid != ActionUSBX ) {
SwitchUSBX_Change:
		EA = 0;  // 关闭中断
//		sav = USBX_SEL;
		USB_CTRL &= ~ bUC_THROUGH;  // USB直通禁止
		USBX_SEL = bUSBX_XSFR_OFS | bUSBX_XRAM_OFS | ActionUSBX;  // USBX自动偏移,通过USBX0读写USBX0-USBX3
		PortBit = X0DevBitFlag[3] & BIT_PORT_CONNECT ? 0x01 << ( ActionUSBX + 1 ) : 0x00;  // 如果是断开就需要上传通知
		X0DevBitFlag[3] = X0DevBitFlag[3] & BIT_PORT_CONNECT ? BIT_PORT_C_CONN : 0x00;  // 断开或者一直未连
		ActionUSBX = newid;
		USBX_SEL = bUSBX_XSFR_OFS | bUSBX_XRAM_OFS | ActionUSBX;  // USBX自动偏移,通过USBX0读写USBX0-USBX3
		PortBit |= Dev3FlagPC & BIT_DEV3_IS_PASS ? 0x01 << ( ActionUSBX + 1 ) : 0x00;  // 如果有需求就上传连接通知
		X0DevBitFlag[3] = Dev3FlagPC & BIT_DEV3_IS_PASS ? BIT_PORT_CONNECT | BIT_PORT_C_CONN : 0x00;  // 有连接需求
		ThisHubEP1IntUp( PortBit );  // 为当前USBX上传HUB中断通知
//		USBX_SEL = sav;
		EA = 1;
	}
}

void	ScanRootHub( UINT8 RootIndex )  // 扫描ROOT-HUB状态,处理ROOT-HUB端口的设备插拔事件,参数为0xFF则从头扫描,否则指定扫描的起始ROOT端口
{
	UINT8	i, s;
	CurrentWaitRoot = 0xFF;  // 等待稳定队列中空闲
	if ( RootIndex == 0xFF ) {  // 空闲/从头扫描
		UIF_DETECT = 0;  // 清中断标志
		i = 0;  // 从头扫描
	}
	else i = RootIndex & 0x0F;  // 指定扫描的起始ROOT端口
	for ( ; i != TOTAL_ROOT_HUB; i ++ ) {
		s = RootHubDev[i].DeviceStatus;
		if ( USB_HUB_ST & ( bUHS_H0_ATTACH << i ) ) {  // 设备存在 bUHS_H?_ATTACH
			if ( s == ROOT_DEV_DISCONNECT || s != ROOT_DEV_FAILED && IsRootHubEnabled( i ) == 0x00 ) {  // 检测到有设备插入,但尚未允许,说明是刚插入，排除因枚举失败而主动关闭的设备
				DisableRootHubPort( i );  // 关闭端口
				if ( CurrentEnumRoot == 0xFF ) {  // 延时定时器空闲时处理
					CurrentEnumRoot = i | 0x80;  // 主机当前正在识别的设备所在的ROOT端口号,等待稳定
//					RootHubDev[i].DeviceSpeed = USB_HUB_ST & ( bUHS_HM0_LEVEL << i ) ? 0 : 1;  // bUHS_H?M_LEVEL
					RootHubDev[i].DeviceStatus = ROOT_DEV_CONNECTED;  // 置连接标志
					TaskReadyTimer = i == RootIndex ? TASK_TIMER_READY - BUS_RESET_TIME - ATTACH_WAIT_STAB : TASK_TIMER_READY;  // 启用延时任务,如果是加过队列的,则去掉前一设备的总线复位和等待稳定时间
					printf( "HI%d\n", (UINT16)i );
//					break;  // 一次只能处理一个插入事件
				}
				else {  // 加入等待稳定队列,唯一目的就是减少等待时间
					if ( CurrentWaitRoot == 0xFF ) {  // 等待稳定队列中空闲
						CurrentWaitRoot = i;  // 设备的ROOT端口号
					}
					else break;  // 一次只能处理两个插入事件（一个在处理，一个入队列）
				}
			}
		}
		else if ( s >= ROOT_DEV_CONNECTED ) {  //检测到设备拔出
			if ( s >= ROOT_DEV_SUCCESS ) {
				if ( RootHubDev[i].DeviceIndex == 3 ) {  // 作为直通设备
					EA = 0;  // 关闭中断
					USBX_SEL = bUSBX_XSFR_OFS | bUSBX_XRAM_OFS | ActionUSBX;  // USBX自动偏移,通过USBX0读写USBX0-USBX3
					Dev3FlagPC = 0x00;  // 直通设备已移除
					X0DevBitFlag[3] = 0x00;  // 端口空闲
					if ( TaskResetTimer || ( USB_CTRL & bUC_THROUGH ) ) {  // 正在复位或者已经连接
						TaskResetTimer = 0;
						USB_CTRL &= ~ bUC_THROUGH;  // USB直通禁止
						ThisHubEP1IntUp( 0x01 << ( 3 + 1 ) );  // 为当前USBX上传HUB中断通知
					}
					EA = 1;
				}
				else AllHubEP1IntUp( RootHubDev[i].DeviceIndex, 0x00 );  // 在关闭中断后为所有USBX上传HUB中断通知
			}
			DisableRootHubPort( i );  // 关闭端口
			printf( "HO%d\n", (UINT16)i );
		}
	}
}

UINT8	InitRootDevice( UINT8 RootIndex )  // 初始化指定ROOT端口的USB设备
{
#define	pCfgDescrBuf	( (PXUSB_CFG_DESCR_LONG)TxBuffer )
	UINT8	i, s, epaddr, dv_cls, if_prot, if_cls, ofs;
	printf( "ResetRoot%d#\n", (UINT16)RootIndex );
	ResetRootHubPort( RootIndex, 0 );  // 检测到设备后,复位相应端口的USB总线
	for ( i = 0, s = 0; i < RE_ATTACH_TIMEOUT; i ++ ) {  // 等待USB设备复位后重新连接,检查超时
		mDelaymS( 1 );
		if ( EnableRootHubPort( RootIndex ) == ERR_SUCCESS ) {  // 使能ROOT-HUB端口
			i = 0;
			s ++;  // 计时等待USB设备连接后稳定
			if ( s > ATTACH_WAIT_STAB ) break;  // 已经稳定连接若干时间
		}
	}
	if ( i ) {  // 复位后设备没有连接
        InitRootDevDiscon:
		DisableRootHubPort( RootIndex );
		printf( "DisableRoot%d#\n", (UINT16)RootIndex );
		return( ERR_USB_DISCON );
	}
	SelectHubPort( RootIndex );  // 选择操作指定的ROOT-HUB端口
//	printf( "GetDevDescr: " );
	s = CtrlGetDeviceDescr( );  // 获取设备描述符
	if ( s == ERR_SUCCESS ) {
		RootHubDev[RootIndex].DeviceEp0Size = UsbDevEndp0Size;  // 端点0的长度
//		for ( i = 0; i < ( (PUSB_SETUP_REQ)SetupGetDevDescr ) -> wLengthL; i ++ ) printf( "x%02X ", (UINT16)( TxBuffer[i] ) );
//		printf( "\n" ); // 显示出描述符
		dv_cls = ( (PXUSB_DEV_DESCR)TxBuffer ) -> bDeviceClass;  // 设备类代码
		s = CtrlSetUsbAddress( RootIndex + ( (PUSB_SETUP_REQ)SetupSetUsbAddr ) -> wValueL );  // 设置USB设备地址,加上RootIndex可以保证各HUB端口分配不同的地址
		if ( s == ERR_SUCCESS ) {
//			RootHubDev[RootIndex].DeviceAddress = RootIndex + ( (PUSB_SETUP_REQ)SetupSetUsbAddr ) -> wValueL;  // 保存USB地址
//			printf( "GetCfgDescr: " );
			s = CtrlGetConfigDescr( );  // 获取配置描述符
			if ( s == ERR_SUCCESS ) {
//				for ( i = 0; i < ( (PXUSB_CFG_DESCR)TxBuffer ) -> wTotalLengthL; i ++ ) printf( "x%02X ", (UINT16)( TxBuffer[i] ) );
//				printf("\n");
/* 分析配置描述符,获取端点数据/各端点地址/各端点大小等,更新变量endp_addr和endp_size等 */
				if ( dv_cls == 0x00 && pCfgDescrBuf -> itf_descr.bInterfaceClass == USB_DEV_CLASS_HID && pCfgDescrBuf -> itf_descr.bInterfaceSubClass <= 0x01 ) {  // 是HID类设备,键盘/鼠标等
					if_prot = pCfgDescrBuf -> itf_descr.bInterfaceProtocol;
					ofs = AnalyzeHidIntEndp( TxBuffer );  // 从描述符中分析出HID中断端点的信息,返回偏移
					epaddr = ( (PXUSB_ENDP_DESCR)(TxBuffer+ofs) ) -> bEndpointAddress;  // 中断端点的地址
					if ( epaddr ) {
						if ( epaddr & USB_ENDP_DIR_MASK ) epaddr = epaddr & USB_ENDP_ADDR_MASK | USB_PID_IN << 4;
						else epaddr = epaddr & USB_ENDP_ADDR_MASK | USB_PID_OUT << 4;
					}
					RootHubDev[RootIndex].u.Mouse.InterruptEndp = epaddr;
					RootHubDev[RootIndex].u.Mouse.IntEndpTimer = ( (PXUSB_ENDP_DESCR)(TxBuffer+ofs) ) -> bInterval;  // 间隔
					RootHubDev[RootIndex].u.Mouse.IntEndpTog = 0;  // 同步标志位,清0
// 需保存端点信息以便主程序进行USB传输
					s = CtrlSetUsbConfig( ( (PXUSB_CFG_DESCR)TxBuffer ) -> bConfigurationValue );  // 设置USB设备配置
					if ( s == ERR_SUCCESS ) {
						if ( if_prot == 1 ) {
							RootHubDev[RootIndex].DeviceIndex = 0;  // 作为USBX之HUB下0#设备
// 进一步初始化,例如设备键盘指示灯LED等
							AllHubEP1IntUp( 0, BIT_PORT_CONNECT | BIT_PORT_C_CONN );  // 在关闭中断后为所有USBX上传HUB中断通知
							printf( "KeyboardReady\n" );
InitRootDevSuccess:
							RootHubDev[RootIndex].DeviceStatus = ROOT_DEV_SUCCESS;
							SetUsbSpeed( 1 );  // 默认为全速
							return( ERR_SUCCESS );
						}
						else if ( if_prot == 2 ) {
							RootHubDev[RootIndex].DeviceIndex = 1;  // 作为USBX之HUB下1#设备
// 为了以后查询鼠标状态,应该分析描述符,取得中断端口的地址,长度等信息
							AllHubEP1IntUp( 1, BIT_PORT_CONNECT | BIT_PORT_C_CONN );  // 在关闭中断后为所有USBX上传HUB中断通知
							printf( "MouseReady\n" );
							goto InitRootDevSuccess;
						}
					}
				}
				if_cls = pCfgDescrBuf -> itf_descr.bInterfaceClass;  // 接口类代码
				if ( ( Dev3FlagPC & BIT_DEV3_IS_PASS ) == 0 || ( Dev3FlagPC & BIT_DEV3_ROOT_IDX ) == RootIndex ) {  // 直通设备通道可用
					if ( dv_cls == 0x00 && if_cls == USB_DEV_CLASS_STORAGE  // 是USB存储类设备(基本上是U盘)
						|| dv_cls == 0x00 && if_cls == USB_DEV_CLASS_PRINTER  // 是打印机类设备
						|| dv_cls == 0x00 && if_cls == USB_DEV_CLASS_AUDIO ) {  // 是Audio类设备
						mDelaymS( 1 );
						ResetRootHubPort( RootIndex, 0 );  // 复位相应端口的USB总线
						EA = 0;  // 关闭中断
						Dev3FlagPC = BIT_DEV3_IS_PASS | RootIndex;  // 作为直通设备连接
						USB_CTRL &= ~ bUC_THROUGH;  // USB直通禁止
						USBX_SEL = bUSBX_XSFR_OFS | bUSBX_XRAM_OFS | ActionUSBX;  // USBX自动偏移,通过USBX0读写USBX0-USBX3
						X0DevBitFlag[3] = BIT_PORT_CONNECT | BIT_PORT_C_CONN;  // 有连接需求
						ThisHubEP1IntUp( 0x01 << ( ActionUSBX + 1 ) );  // 为当前USBX上传HUB中断通知
						EA = 1;
						printf( "WaitPassThro\n" );
						goto InitRootDevSuccess;
					}
				}
			}
		}
	}
	SetUsbSpeed( 1 );  // 默认为全速
	mDelayuS( 8 );
	if ( ( USB_HUB_ST & ( bUHS_H0_ATTACH << RootIndex ) ) == 0 ) goto InitRootDevDiscon;  // 没有设备
	DisableRootHubPort( RootIndex );
	RootHubDev[RootIndex].DeviceStatus = ROOT_DEV_FAILED;  // 不支持的设备
	printf( "Unsupported\n" );
	return( ERR_USB_CONNECT );  // 不支持的设备
}

void	QueryInterrupt( void )  // 查询端点数据
{
	UINT8	i, j, s, tmr, devidx, freecnt;
	for ( i = 0; i != TOTAL_ROOT_HUB; i ++ ) {
		if ( RootHubDev[i].DeviceStatus >= ROOT_DEV_SUCCESS && RootHubDev[i].DeviceIndex < 3 ) {  // 非直通设备
			tmr = RootHubDev[i].u.Mouse.IntEndpTimer;
			if ( tmr > 8 ) tmr = 7;
			else if ( tmr > 4 ) tmr = 3;
			else if ( tmr > 2 ) tmr = 1;
			else tmr = 0;
			if ( tmr == 0 || ( TimerCount1mS & tmr ) == 0 ) {  // 查询间隔到
				if ( RootHubDev[i].u.Mouse.InterruptEndp ) {  // 端点有效
					SelectHubPort( i );  // 选择操作指定的ROOT-HUB端口
					s = USBHostTransact( RootHubDev[i].u.Mouse.InterruptEndp, RootHubDev[i].u.Mouse.IntEndpTog, 0 );  // CH545传输事务,获取数据,NAK不重试
					SetUsbSpeed( 1 );  // 默认为全速
					if ( s == ERR_SUCCESS ) {
						RootHubDev[i].u.Mouse.IntEndpTog ^= bUH_R_TOG | bUH_T_TOG;  // 同步标志翻转
						if ( USB_RX_LEN ) {  // 接收到的数据长度
//							printf("Interrupt data: ");
//							for ( i = 0; i < USB_RX_LEN; i ++ ) printf("x%02X ",(UINT16)(RxBuffer[i]) );
//							printf("\n");
						}
						devidx = RootHubDev[i].DeviceIndex;
						for ( j = 255, freecnt = 0; j != 0; j -- ) {  // 等待上传空闲，约1mS忙超时后强行上传
							EA = 0;  // 关闭中断
							USBX_SEL = bUSBX_XSFR_OFS | bUSBX_XRAM_OFS | ActionUSBX;  // USBX自动偏移,通过USBX0读写USBX0-USBX3
//							if ( ( ( devidx == 0 ? pX0D0_EP1RES : ( devidx == 1 ? pX0D1_EP1RES : pX0D2_EP1RES ) ) & MASK_UEP_X_RES ) != UEP_X_RES_ACK ) goto QueryInterCopy;  // 对应的端点已经空闲
							if ( ( pXiDj_EP1RES(0,devidx) & MASK_UEP_X_RES ) != UEP_X_RES_ACK ) goto QueryInterCopy;  // 对应的端点已经空闲
							if ( pUSBX0_IE & bUX_SIE_FREE ) {
								if ( ++ freecnt > 12 ) goto QueryInterCopy;  // 设备空闲计数，较长时间（约50uS）空闲则强行上传
							}
							else freecnt = 0;
							EA = 1;
							mDelayuS( 3 );
						}
						EA = 0;  // 关闭中断
						USBX_SEL = bUSBX_XSFR_OFS | bUSBX_XRAM_OFS | ActionUSBX;  // USBX自动偏移,通过USBX0读写USBX0-USBX3
QueryInterCopy:
						memcpy( pUSBXi_BUF_DEVj(0,devidx)+UX_EP1_ADDR, RxBuffer, USB_RX_LEN );  // 复制数据
						pXiDj_EP1RES(0,devidx) = pXiDj_EP1RES(0,devidx) & ~ MASK_UEP_X_RES | UEP_X_RES_ACK;  // 允许上传，自动翻转
						EA = 1;
					}
//					else {
//						if ( s == ERR_USB_DISCON || IsRootHubEnabled( i ) == 0 ) RootHubDev[i].DeviceStatus = ROOT_DEV_DISCONNECT;  // 断开
//					}
				}
			}
		}
	}
//	return( 0 );
}

main( ) {
	UINT8	key, s;
    CfgFsys( );
	mInitSTDIO( );  /* 为了让计算机通过串口监控演示过程 */
	printf( "ID=%02X\n", (UINT16)CHIP_ID );
//$$???仅用于演示
	memcpy( Dev0DevDescr, cDevDevDescr, sizeof( cDevDevDescr ) );
	((PXUSB_DEV_DESCR)Dev0DevDescr)->idProductL = 0x4B;  // Keyboard
	memcpy( Dev1DevDescr, cDevDevDescr, sizeof( cDevDevDescr ) );
	((PXUSB_DEV_DESCR)Dev1DevDescr)->idProductL = 0x4D;  // Mouse
	memcpy( Dev2DevDescr, cDevDevDescr, sizeof( cDevDevDescr ) );
	((PXUSB_DEV_DESCR)Dev2DevDescr)->idProductL = 0x44;  // Disk
	memcpy( Dev0CfgDescr, cDevCfgDescrK, sizeof( cDevCfgDescrK ) );
	memcpy( Dev1CfgDescr, cDevCfgDescrM, sizeof( cDevCfgDescrM ) );
	memcpy( Dev2CfgDescr, cDevCfgDescrD, sizeof( cDevCfgDescrD ) );
//
	InitUSB_Host( );
	CurrentWaitRoot = 0xFF;
	CurrentEnumRoot = 0xFF;
	InitUSBX_Device( );
	InitTaskTimer( );
	EA = 1;
	while ( 1 ) {
		if ( RI ) {
			key = getkey( );
			printf( "%c$", key );
			if ( key >= '0' && key <= '3' ) {  // switch
				key -= '0';
				SwitchUSBX( key );
				printf( "!\n" );
			}
		}
		if ( UIF_DETECT || CurrentEnumRoot == 0xFF && CurrentWaitRoot != 0xFF ) {  // 如果有USB主机检测中断则处理，如果在空闲时仍有另一个设备在等待则处理
			ScanRootHub( CurrentWaitRoot );  // 扫描ROOT-HUB状态,处理ROOT-HUB端口的设备插拔事件
		}
		if ( CurrentEnumRoot < TOTAL_ROOT_HUB ) {  // 在等待主机枚举/识别
			s = InitRootDevice( CurrentEnumRoot );  // 初始化指定ROOT端口的USB设备
			TaskReadyTimer = 0;  // 确保清0
			CurrentEnumRoot = 0xFF;  // 空闲
			if ( CurrentWaitRoot == 0xFF ) CurrentWaitRoot = 0x80;  // 强制扫描一次,防止初始化前后丢失插拔中断信息
//			if ( s == ERR_USB_CONNECT ) retry!
		}
		if ( TimerAct1mS ) {  // 1mS
			TimerAct1mS = 0;
			QueryInterrupt( );  // 查询端点数据
		}
	}
}
