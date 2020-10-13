/*
****************************************
**  Copyright  (C)  W.ch  1999-2019   **
**  Web:              http://wch.cn   **
****************************************
*/
/* USB host example for CH545, enum all USB device under root-hub */
/* USB主机,有关Root-HUB的应用例子,初始化和枚举当前根集线器连接的USB设备,同时支持最多4个USB设备,支持一级外部HUB */

/* C51   TEST.C */
/* LX51  TEST.OBJ */
/* OHX51 TEST */

#include <CH545.H>
#include <stdio.h>
#include <string.h>

#ifndef	ERR_SUCCESS
// 各子程序返回状态码
#define	ERR_SUCCESS			0x00	// 操作成功
#define	ERR_USB_CONNECT		0x15	/* 检测到USB设备连接事件,已经连接 */
#define	ERR_USB_DISCON		0x16	/* 检测到USB设备断开事件,已经断开 */
#define	ERR_USB_BUF_OVER	0x17	/* USB传输的数据有误或者数据太多缓冲区溢出 */
#define	ERR_USB_DISK_ERR	0x1F	/* USB存储器操作失败,在初始化时可能是USB存储器不支持,在读写操作中可能是磁盘损坏或者已经断开 */
#define	ERR_USB_TRANSFER	0x20	/* NAK/STALL等更多错误码在0x20~0x2F */
#define	ERR_USB_UNSUPPORT	0xFB
#define	ERR_USB_UNKNOWN		0xFE
#endif

#ifndef	BUS_RESET_TIME
#define	BUS_RESET_TIME		11		/* USB总线复位时间10mS到20mS */
#endif

// 获取设备描述符
UINT8C	SetupGetDevDescr[] = { USB_REQ_TYP_IN, USB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_DEVICE, 0x00, 0x00, sizeof( USB_DEV_DESCR ), 0x00 };
// 获取配置描述符
UINT8C	SetupGetCfgDescr[] = { USB_REQ_TYP_IN, USB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_CONFIG, 0x00, 0x00, 0x04, 0x00 };
// 设置USB地址
UINT8C	SetupSetUsbAddr[] = { USB_REQ_TYP_OUT, USB_SET_ADDRESS, USB_DEVICE_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00 };
// 设置USB配置
UINT8C	SetupSetUsbConfig[] = { USB_REQ_TYP_OUT, USB_SET_CONFIGURATION, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// 清除端点STALL
UINT8C	SetupClrEndpStall[] = { USB_REQ_TYP_OUT | USB_REQ_RECIP_ENDP, USB_CLEAR_FEATURE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// 获取HUB描述符
UINT8C	SetupGetHubDescr[] = { HUB_GET_HUB_DESCRIPTOR, HUB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_HUB, 0x00, 0x00, sizeof( USB_HUB_DESCR ), 0x00 };

UINT8X	UsbDevEndp0Size;	/* USB设备的端点0的最大包尺寸 */

//USB设备相关信息表,CH545最多支持4个设备
#define	ROOT_DEV_DISCONNECT		0
#define	ROOT_DEV_CONNECTED		1
#define	ROOT_DEV_FAILED			2
#define	ROOT_DEV_SUCCESS		3

/*
约定: USB设备地址分配规则(参考USB_DEVICE_ADDR)
地址值  设备位置
0x02    内置Root-HUB0下的USB设备或外部HUB
0x03    内置Root-HUB1下的USB设备或外部HUB
0x04    内置Root-HUB2下的USB设备或外部HUB
0x05    内置Root-HUB3下的USB设备或外部HUB
0x1x    内置Root-HUB0下的外部HUB的端口x下的USB设备,x为1~n
0x2x    内置Root-HUB1下的外部HUB的端口x下的USB设备,x为1~n
0x3x    内置Root-HUB2下的外部HUB的端口x下的USB设备,x为1~n
0x4x    内置Root-HUB3下的外部HUB的端口x下的USB设备,x为1~n
*/

UINT8X	RxBuffer[ MAX_PACKET_SIZE ] ;  // IN, must even address
UINT8X	TxBuffer[ MAX_PACKET_SIZE ] ;  // OUT, must even address
#define	pSetupReq	((PXUSB_SETUP_REQ)TxBuffer)
UINT8	RootHubId;		// 当前正在操作的root-hub端口号:0=HUB0,1=HUB1,2=HUB2,3=HUB3

#pragma NOAREGS


void	DisableRootHubPort( UINT8 RootHubIndex );  // 关闭指定的ROOT-HUB端口,实际上硬件已经自动关闭,此处只是清除一些结构状态

void	SetHostUsbAddr( UINT8 addr );  // 设置USB主机当前操作的USB设备地址

void	SetUsbSpeed( UINT8 FullSpeed );  // 设置当前USB速度

void	ResetRootHubPort( UINT8 RootHubIndex, UINT8 mod );  // 检测到设备后,复位相应端口的总线,为枚举设备准备,设置为默认为全速
// mod: 0=reset and wait end, 1=begin reset, 2=end reset

UINT8	EnableRootHubPort( UINT8 RootHubIndex );  // 使能ROOT-HUB端口,相应的bUH_PORT_EN置1开启端口,设备断开可能导致返回失败

void	SelectHubPort( UINT8 RootHubIndex );  // 选择操作指定的ROOT-HUB端口

#define	WAIT_USB_TOUT_200US		255  // 等待USB中断超时时间200uS@Fsys=24MHz
UINT8	WaitUSB_Interrupt( void );  // 等待USB中断

// CH545传输事务,输入目的端点地址/PID令牌,同步标志,以20uS为单位的NAK重试总时间(0则不重试,0xFFFF无限重试),返回0成功,超时/出错重试
UINT8	USBHostTransact( UINT8 endp_pid, UINT8 tog, UINT16 timeout );  // endp_pid: 高4位是token_pid令牌, 低4位是端点地址

UINT8	HostCtrlTransfer( PUINT8X DataBuf, PUINT8I RetLen );  // 执行控制传输,8字节请求码在pSetupReq中,DataBuf为可选的收发缓冲区
// 如果需要接收和发送数据,那么DataBuf需指向有效缓冲区用于存放后续数据,实际成功收发的总长度返回保存在ReqLen指向的字节变量中

void	CopySetupReqPkg( PUINT8C pReqPkt );  // 复制控制传输的请求包

UINT8	CtrlGetDeviceDescr( void );  // 获取设备描述符,返回在TxBuffer中

UINT8	CtrlGetConfigDescr( void );  // 获取配置描述符,返回在TxBuffer中

UINT8	CtrlSetUsbAddress( UINT8 addr );  // 设置USB设备地址

UINT8	CtrlSetUsbConfig( UINT8 cfg );  // 设置USB设备配置

UINT8	CtrlClearEndpStall( UINT8 endp );  // 清除端点STALL

UINT8	HubGetPortStatus( UINT8 HubPortIndex );  // 查询HUB端口状态,返回在TxBuffer中

UINT8	HubSetPortFeature( UINT8 HubPortIndex, UINT8 FeatureSelt );  // 设置HUB端口特性

UINT8	HubClearPortFeature( UINT8 HubPortIndex, UINT8 FeatureSelt );  // 清除HUB端口特性

UINT8	AnalyzeHidIntEndp( PUINT8X buf );  // 从描述符中分析出HID中断端点的信息,返回偏移

void	InitUSB_Host( void );  // 初始化USB主机


void	DisableRootHubPort( UINT8 RootHubIndex )  // 关闭指定的ROOT-HUB端口,实际上硬件已经自动关闭,此处只是清除一些结构状态
{
	RootHubDev[ RootHubIndex ].DeviceStatus = ROOT_DEV_DISCONNECT;
//	RootHubDev[ RootHubIndex ].DeviceAddress = 0x00;
	switch( RootHubIndex ) {
		case 0: UHUB01_CTRL = UHUB01_CTRL & 0xF0 | bUH0_PD_EN; break;  // 清除有关HUB0的控制数据,实际上不需要清除
		case 1: UHUB01_CTRL = UHUB01_CTRL & 0x0F | bUH1_PD_EN; break;  // 清除有关HUB1的控制数据,实际上不需要清除
		case 2: UHUB23_CTRL = UHUB23_CTRL & 0xF0 | bUH2_PD_EN; break;  // 清除有关HUB2的控制数据,实际上不需要清除
		case 3: UHUB23_CTRL = UHUB23_CTRL & 0x0F | bUH3_PD_EN; break;  // 清除有关HUB3的控制数据,实际上不需要清除
		default: break;
	}
//	printf( "HUB %01x close\n",(UINT16)RootHubIndex );
}

UINT8	IsRootHubEnabled( UINT8 RootHubIndex )  // 检查指定的ROOT-HUB端口是否使能
{
	return( ( RootHubIndex < 2 ? UHUB01_CTRL : UHUB23_CTRL ) & ( RootHubIndex & 1 ? bUH1_PORT_EN : bUH0_PORT_EN ) );
}

void	SetHostUsbAddr( UINT8 addr )  // 设置USB主机当前操作的USB设备地址
{
	USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | addr & 0x7F;
}

void	SetUsbSpeed( UINT8 FullSpeed )  // 设置当前USB速度
{
	if ( FullSpeed ) {  // 全速
		USB_CTRL &= ~ bUC_LOW_SPEED;  // 全速
		UH_SETUP &= ~ bUH_PRE_PID_EN;  // 禁止PRE PID
	}
	else {
		USB_CTRL |= bUC_LOW_SPEED;  // 低速
	}
}

void	ResetRootHubPort( UINT8 RootHubIndex, UINT8 mod )  // 检测到设备后,复位相应端口的总线,为枚举设备准备,设置为默认为全速
// mod: 0=reset and wait end, 1=begin reset, 2=end reset
{
	UsbDevEndp0Size = DEFAULT_ENDP0_SIZE;  /* USB设备的端点0的最大包尺寸 */
	SetHostUsbAddr( 0x00 );
	SetUsbSpeed( 1 );  // 默认为全速
	if ( mod <= 1 ) switch( RootHubIndex ) {
		case 0: UHUB01_CTRL = UHUB01_CTRL & 0xF0 | bUH0_PD_EN | bUH0_BUS_RESET; break;  // 默认为全速,开始复位
		case 1: UHUB01_CTRL = UHUB01_CTRL & 0x0F | bUH1_PD_EN | bUH1_BUS_RESET; break;  // 默认为全速,开始复位
		case 2: UHUB23_CTRL = UHUB23_CTRL & 0xF0 | bUH2_PD_EN | bUH2_BUS_RESET; break;  // 默认为全速,开始复位
		case 3: UHUB23_CTRL = UHUB23_CTRL & 0x0F | bUH3_PD_EN | bUH3_BUS_RESET; break;  // 默认为全速,开始复位
		default: break;
	}
	if ( mod == 0 ) mDelaymS( BUS_RESET_TIME );  // 复位时间10mS到20mS
	if ( mod != 1 ) switch( RootHubIndex ) {
		case 0: UHUB01_CTRL = UHUB01_CTRL & ~ bUH0_BUS_RESET; break;  // 结束复位
		case 1: UHUB01_CTRL = UHUB01_CTRL & ~ bUH1_BUS_RESET; break;  // 结束复位
		case 2: UHUB23_CTRL = UHUB23_CTRL & ~ bUH2_BUS_RESET; break;  // 结束复位
		case 3: UHUB23_CTRL = UHUB23_CTRL & ~ bUH3_BUS_RESET; break;  // 结束复位
		default: break;
	}
	mDelayuS( 2 );
	UIF_DETECT = 0;  // 清中断标志
}

UINT8	EnableRootHubPort( UINT8 RootHubIndex )  // 使能ROOT-HUB端口,相应的bUH_PORT_EN置1开启端口,设备断开可能导致返回失败
{
	if ( RootHubIndex < 4 ) {
		if ( RootHubDev[ RootHubIndex ].DeviceStatus < ROOT_DEV_CONNECTED ) RootHubDev[ RootHubIndex ].DeviceStatus = ROOT_DEV_CONNECTED;
		if ( USB_HUB_ST & ( bUHS_H0_ATTACH << RootHubIndex ) ) {  // 有设备 bUHS_H?_ATTACH
			if ( IsRootHubEnabled( RootHubIndex ) == 0x00 ) {  // 尚未使能
				RootHubDev[ RootHubIndex ].DeviceSpeed = USB_HUB_ST & ( bUHS_HM0_LEVEL << RootHubIndex ) ? 0 : 1;
				if ( RootHubDev[ RootHubIndex ].DeviceSpeed == 0 ) {
					if ( RootHubIndex < 2 ) UHUB01_CTRL |= RootHubIndex & 1 ? bUH1_LOW_SPEED : bUH0_LOW_SPEED;  // 低速
					else UHUB23_CTRL |= RootHubIndex & 1 ? bUH3_LOW_SPEED : bUH2_LOW_SPEED;  // 低速
				}
			}
			if ( RootHubIndex < 2 ) UHUB01_CTRL |= RootHubIndex & 1 ? bUH1_PORT_EN : bUH0_PORT_EN;  // 使能HUB端口
			else UHUB23_CTRL |= RootHubIndex & 1 ? bUH3_PORT_EN : bUH2_PORT_EN;  // 使能HUB端口
			return( ERR_SUCCESS );
		}
	}
	return( ERR_USB_DISCON );
}

void	SelectHubPort( UINT8 RootHubIndex )  // 选择操作指定的ROOT-HUB端口
{
		UsbDevEndp0Size = RootHubDev[RootHubIndex].DeviceEp0Size;  // 端点0的长度
//		SetHostUsbAddr( RootHubDev[RootHubIndex].DeviceAddress );  // 设置USB主机当前操作的USB设备地址
		SetHostUsbAddr( RootHubDev[RootHubIndex].DeviceStatus >= ROOT_DEV_SUCCESS ? RootHubIndex + ( (PUSB_SETUP_REQ)SetupSetUsbAddr ) -> wValueL : 0 );
		SetUsbSpeed( RootHubDev[RootHubIndex].DeviceSpeed );  // 设置当前USB速度
	RootHubId = RootHubIndex;
}

UINT8	WaitUSB_Interrupt( void )  // 等待USB中断
{
	UINT16	i;
	for ( i = WAIT_USB_TOUT_200US; i != 0 && UIF_TRANSFER == 0; i -- ) CHIP_ID++;
	return( UIF_TRANSFER ? ERR_SUCCESS : ERR_USB_UNKNOWN );
}

// CH545传输事务,输入目的端点地址/PID令牌,同步标志,以20uS为单位的NAK重试总时间(0则不重试,0xFFFF无限重试),返回0成功,超时/出错重试
UINT8	USBHostTransact( UINT8 endp_pid, UINT8 tog, UINT16 timeout )  // endp_pid: 高4位是token_pid令牌, 低4位是端点地址
{  // 本子程序着重于易理解,而在实际应用中,为了提供运行速度,应该对本子程序代码进行优化
//	UINT8	TransRetry;
#define	TransRetry	UEP0_T_LEN	// 节约内存
	UINT8	r;
	UINT16	i;
	UH_RX_CTRL = UH_TX_CTRL = tog;
	TransRetry = 0;
	do {
//		LED_TMP = 0;
		UH_EP_PID = endp_pid;  // 指定令牌PID和目的端点号
		UIF_TRANSFER = 0;  // 允许传输
//		s = WaitUSB_Interrupt( );
		for ( i = WAIT_USB_TOUT_200US; i != 0 && UIF_TRANSFER == 0; i -- ) CHIP_ID++;
		UH_EP_PID = 0x00;  // 停止USB传输
//		LED_TMP = 1;
//		if ( s != ERR_SUCCESS ) return( s );  // 中断超时,可能是硬件异常
		if ( UIF_TRANSFER == 0 ) return( ERR_USB_UNKNOWN );
//		if ( UIF_DETECT ) {  // USB设备插拔事件
//			mDelayuS( 200 );  // 等待传输完成
//			UIF_DETECT = 0;  // 清中断标志
//			s = AnalyzeRootHub( );   // 分析ROOT-HUB状态
//			if ( s == ERR_USB_CONNECT ) FoundNewDev = 1;
//			if ( RootHubDev[RootHubId].DeviceStatus == ROOT_DEV_DISCONNECT ) return( ERR_USB_DISCON );  // USB设备断开事件
//			if ( RootHubDev[RootHubId].DeviceStatus == ROOT_DEV_CONNECTED ) return( ERR_USB_CONNECT );  // USB设备连接事件
//			if ( ( USB_HUB_ST & ( bUHS_H0_ATTACH << RootHubId ) ) == 0x00 ) return( ERR_USB_DISCON );  // USB设备断开事件
//			mDelayuS( 200 );  // 等待传输完成
//		}
//		if ( UIF_TRANSFER ) {  // 传输完成
		else {  // 传输完成
			if ( U_TOG_OK ) return( ERR_SUCCESS );
#ifdef DEBUG_NOW
printf("endp_pid=%02X\n",(UINT16)endp_pid);
printf("USB_INT_FG=%02X\n",(UINT16)USB_INT_FG);
printf("USB_INT_ST=%02X\n",(UINT16)USB_INT_ST);
printf("USB_MIS_ST=%02X\n",(UINT16)USB_MIS_ST);
printf("USB_RX_LEN=%02X\n",(UINT16)USB_RX_LEN);
printf("UH_TX_LEN=%02X\n",(UINT16)UH_TX_LEN);
printf("UH_RX_CTRL=%02X\n",(UINT16)UH_RX_CTRL);
printf("UH_TX_CTRL=%02X\n",(UINT16)UH_TX_CTRL);
printf("UHUB01_CTRL=%02X\n",(UINT16)UHUB01_CTRL);
printf("UHUB23_CTRL=%02X\n",(UINT16)UHUB23_CTRL);
#endif
			r = USB_INT_ST & MASK_UIS_H_RES;  // USB设备应答状态
			if ( r == USB_PID_STALL ) return( r | ERR_USB_TRANSFER );
			if ( r == USB_PID_NAK ) {
				if ( timeout == 0 ) return( r | ERR_USB_TRANSFER );
				if ( timeout < 0xFFFF ) timeout --;
				-- TransRetry;
			}
			else switch ( endp_pid >> 4 ) {
				case USB_PID_SETUP:
				case USB_PID_OUT:
//					if ( U_TOG_OK ) return( ERR_SUCCESS );
//					if ( r == USB_PID_ACK ) return( ERR_SUCCESS );
//					if ( r == USB_PID_STALL || r == USB_PID_NAK ) return( r | ERR_USB_TRANSFER );
					if ( r ) return( r | ERR_USB_TRANSFER );  // 不是超时/出错,意外应答
					break;  // 超时重试
				case USB_PID_IN:
//					if ( U_TOG_OK ) return( ERR_SUCCESS );
//					if ( tog ? r == USB_PID_DATA1 : r == USB_PID_DATA0 ) return( ERR_SUCCESS );
//					if ( r == USB_PID_STALL || r == USB_PID_NAK ) return( r | ERR_USB_TRANSFER );
					if ( r == USB_PID_DATA0 && r == USB_PID_DATA1 ) {  // 不同步则需丢弃后重试
					}  // 不同步重试
					else if ( r ) return( r | ERR_USB_TRANSFER );  // 不是超时/出错,意外应答
					break;  // 超时重试
				default:
					return( ERR_USB_UNKNOWN );  // 不可能的情况
					break;
			}
		}
//		else {  // 其它中断,不应该发生的情况
//			USB_INT_FG = 0xFF;  /* 清中断标志 */
//		}
		mDelayuS( 15 );
		if ( UIF_DETECT ) {  // USB设备插拔事件
			if ( IsRootHubEnabled( RootHubId ) == 0 ) return( ERR_USB_DISCON );  // USB设备断开事件
		}
	} while ( ++ TransRetry < 3 );
	return( ERR_USB_TRANSFER );  // 应答超时
}

UINT8	HostCtrlTransfer( PUINT8X DataBuf, PUINT8I RetLen )  // 执行控制传输,8字节请求码在pSetupReq中,DataBuf为可选的收发缓冲区
// 如果需要接收和发送数据,那么DataBuf需指向有效缓冲区用于存放后续数据,实际成功收发的总长度保存在ReqLen指向的字节变量中
{
	UINT8	s, RemLen, RxLen, RxCnt, TxCnt;
	PUINT8X	xdata	pBuf;
	PUINT8I	xdata	pLen;
	pBuf = DataBuf;
	pLen = RetLen;
	mDelayuS( 64 );
	if ( pLen ) *pLen = 0;  // 实际成功收发的总长度
	UH_TX_LEN = sizeof( USB_SETUP_REQ );
	s = USBHostTransact( USB_PID_SETUP << 4 | 0x00, 0x00, 200000/20 );  // SETUP阶段,200mS超时
	if ( s != ERR_SUCCESS ) return( s );
	UH_RX_CTRL = UH_TX_CTRL = bUH_R_TOG | bUH_R_AUTO_TOG | bUH_T_TOG | bUH_T_AUTO_TOG;  // 默认DATA1
	UH_TX_LEN = 0x01;  // 默认无数据故状态阶段为IN
	RemLen = pSetupReq -> wLengthH ? 0xFF : pSetupReq -> wLengthL;
	if ( RemLen && pBuf ) {  // 需要收发数据
		if ( pSetupReq -> bRequestType & USB_REQ_TYP_IN ) {  // 收
			while ( RemLen ) {
				mDelayuS( 64 );
				s = USBHostTransact( USB_PID_IN << 4 | 0x00, UH_RX_CTRL, 200000/20 );  // IN数据
				if ( s != ERR_SUCCESS ) return( s );
				RxLen = USB_RX_LEN < RemLen ? USB_RX_LEN : RemLen;
				RemLen -= RxLen;
				if ( pLen ) *pLen += RxLen;  // 实际成功收发的总长度
//				memcpy( pBuf, RxBuffer, RxLen );
//				pBuf += RxLen;
				for ( RxCnt = 0; RxCnt != RxLen; RxCnt ++ ) {
					*pBuf = RxBuffer[ RxCnt ];
					pBuf ++;
				}
				if ( USB_RX_LEN == 0 || ( USB_RX_LEN & ( UsbDevEndp0Size - 1 ) ) ) break;  // 短包
			}
			UH_TX_LEN = 0x00;  // 状态阶段为OUT
		}
		else {  // 发
			while ( RemLen ) {
				mDelayuS( 64 );
				UH_TX_LEN = RemLen >= UsbDevEndp0Size ? UsbDevEndp0Size : RemLen;
//				memcpy( TxBuffer, pBuf, UH_TX_LEN );
//				pBuf += UH_TX_LEN;
				for ( TxCnt = 0; TxCnt != UH_TX_LEN; TxCnt ++ ) {
					TxBuffer[ TxCnt ] = *pBuf;
					pBuf ++;
				}
				s = USBHostTransact( USB_PID_OUT << 4 | 0x00, UH_TX_CTRL, 200000/20 );  // OUT数据
				if ( s != ERR_SUCCESS ) return( s );
				RemLen -= UH_TX_LEN;
				if ( pLen ) *pLen += UH_TX_LEN;  // 实际成功收发的总长度
			}
//			UH_TX_LEN = 0x01;  // 状态阶段为IN
		}
	}
	mDelayuS( 64 );
	s = USBHostTransact( ( UH_TX_LEN ? USB_PID_IN << 4 | 0x00: USB_PID_OUT << 4 | 0x00 ), bUH_R_TOG | bUH_T_TOG, 200000/20 );  // STATUS阶段
	if ( s != ERR_SUCCESS ) return( s );
	if ( UH_TX_LEN == 0 ) return( ERR_SUCCESS );  // 状态OUT
	if ( USB_RX_LEN == 0 ) return( ERR_SUCCESS );  // 状态IN,检查IN状态返回数据长度
	return( ERR_USB_BUF_OVER );  // IN状态阶段错误
}

void	CopySetupReqPkg( PUINT8C pReqPkt )  // 复制控制传输的请求包
{
	UINT8	i;
	for ( i = 0; i != sizeof( USB_SETUP_REQ ); i ++ ) {
		((PUINT8X)pSetupReq)[ i ] = *pReqPkt;
		pReqPkt ++;
	}
}

UINT8	CtrlGetDeviceDescr( void )  // 获取设备描述符,返回在TxBuffer中
{
	UINT8	s;
	UINT8D	len;
	UsbDevEndp0Size = DEFAULT_ENDP0_SIZE;
	CopySetupReqPkg( SetupGetDevDescr );
	s = HostCtrlTransfer( TxBuffer, &len );  // 执行控制传输
	if ( s != ERR_SUCCESS ) return( s );
	UsbDevEndp0Size = ( (PXUSB_DEV_DESCR)TxBuffer ) -> bMaxPacketSize0;  // 端点0最大包长度,这是简化处理,正常应该先获取前8字节后立即更新UsbDevEndp0Size再继续
	if ( len < ( (PUSB_SETUP_REQ)SetupGetDevDescr ) -> wLengthL ) return( ERR_USB_BUF_OVER );  // 描述符长度错误
	return( ERR_SUCCESS );
}

UINT8	CtrlGetConfigDescr( void )  // 获取配置描述符,返回在TxBuffer中
{
	UINT8	s;
	UINT8D	len;
	CopySetupReqPkg( SetupGetCfgDescr );
	s = HostCtrlTransfer( TxBuffer, &len );  // 执行控制传输
	if ( s != ERR_SUCCESS ) return( s );
	if ( len < ( (PUSB_SETUP_REQ)SetupGetCfgDescr ) -> wLengthL ) return( ERR_USB_BUF_OVER );  // 返回长度错误
	len = ( (PXUSB_CFG_DESCR)TxBuffer ) -> wTotalLengthL;
	if ( len > MAX_PACKET_SIZE ) return( ERR_USB_BUF_OVER );  // 返回长度错误
	CopySetupReqPkg( SetupGetCfgDescr );
	pSetupReq -> wLengthL = len;  // 完整配置描述符的总长度
	s = HostCtrlTransfer( TxBuffer, &len );  // 执行控制传输
	if ( s != ERR_SUCCESS ) return( s );
	if ( len < ( (PUSB_SETUP_REQ)SetupGetCfgDescr ) -> wLengthL || len < ( (PXUSB_CFG_DESCR)TxBuffer ) -> wTotalLengthL ) return( ERR_USB_BUF_OVER );  // 描述符长度错误
	return( ERR_SUCCESS );
}

UINT8	CtrlSetUsbAddress( UINT8 addr )  // 设置USB设备地址
{
	UINT8	s;
	CopySetupReqPkg( SetupSetUsbAddr );
	pSetupReq -> wValueL = addr;  // USB设备地址
	s = HostCtrlTransfer( NULL, NULL );  // 执行控制传输
	if ( s != ERR_SUCCESS ) return( s );
	SetHostUsbAddr( addr );  // 设置USB主机当前操作的USB设备地址
	mDelaymS( BUS_RESET_TIME >> 1 );  // 等待USB设备完成操作
	return( ERR_SUCCESS );
}

UINT8	CtrlSetUsbConfig( UINT8 cfg )  // 设置USB设备配置
{
	CopySetupReqPkg( SetupSetUsbConfig );
	pSetupReq -> wValueL = cfg;  // USB设备配置
	return( HostCtrlTransfer( NULL, NULL ) );  // 执行控制传输
}

UINT8	CtrlClearEndpStall( UINT8 endp )  // 清除端点STALL
{
	CopySetupReqPkg( SetupClrEndpStall );  // 清除端点的错误
	pSetupReq -> wIndexL = endp;  // 端点地址
	return( HostCtrlTransfer( NULL, NULL ) );  /* 执行控制传输 */
}

UINT8	HubGetPortStatus( UINT8 HubPortIndex )  // 查询HUB端口状态,返回在TxBuffer中
{
	UINT8	s;
	UINT8D	len;
	pSetupReq -> bRequestType = HUB_GET_PORT_STATUS;
	pSetupReq -> bRequest = HUB_GET_STATUS;
	pSetupReq -> wValueL = 0x00;
	pSetupReq -> wValueH = 0x00;
	pSetupReq -> wIndexL = HubPortIndex;
	pSetupReq -> wIndexH = 0x00;
	pSetupReq -> wLengthL = 0x04;
	pSetupReq -> wLengthH = 0x00;
	s = HostCtrlTransfer( TxBuffer, &len );  // 执行控制传输
	if ( s != ERR_SUCCESS ) return( s );
	if ( len < 4 ) return( ERR_USB_BUF_OVER );  // 描述符长度错误
	return( ERR_SUCCESS );
}

UINT8	HubSetPortFeature( UINT8 HubPortIndex, UINT8 FeatureSelt )  // 设置HUB端口特性
{
	pSetupReq -> bRequestType = HUB_SET_PORT_FEATURE;
	pSetupReq -> bRequest = HUB_SET_FEATURE;
	pSetupReq -> wValueL = FeatureSelt;
	pSetupReq -> wValueH = 0x00;
	pSetupReq -> wIndexL = HubPortIndex;
	pSetupReq -> wIndexH = 0x00;
	pSetupReq -> wLengthL = 0x00;
	pSetupReq -> wLengthH = 0x00;
	return( HostCtrlTransfer( NULL, NULL ) );  // 执行控制传输
}

UINT8	HubClearPortFeature( UINT8 HubPortIndex, UINT8 FeatureSelt )  // 清除HUB端口特性
{
	pSetupReq -> bRequestType = HUB_CLEAR_PORT_FEATURE;
	pSetupReq -> bRequest = HUB_CLEAR_FEATURE;
	pSetupReq -> wValueL = FeatureSelt;
	pSetupReq -> wValueH = 0x00;
	pSetupReq -> wIndexL = HubPortIndex;
	pSetupReq -> wIndexH = 0x00;
	pSetupReq -> wLengthL = 0x00;
	pSetupReq -> wLengthH = 0x00;
	return( HostCtrlTransfer( NULL, NULL ) );  // 执行控制传输
}

/* ************************************************************************************** */

UINT8	AnalyzeHidIntEndp( PUINT8X buf )  // 从描述符中分析出HID中断端点的信息,返回偏移
{
	UINT8	i, s, l;
	s = 0;
	for ( i = 0; i < ( (PXUSB_CFG_DESCR)buf ) -> wTotalLengthL; i += l ) {  // 搜索中断端点描述符,跳过配置描述符和接口描述符
		if ( ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bDescriptorType == USB_DESCR_TYP_ENDP  // 是端点描述符
			&& ( ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bmAttributes & USB_ENDP_TYPE_MASK ) == USB_ENDP_TYPE_INTER  // 是中断端点
			&& ( ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bEndpointAddress & USB_ENDP_DIR_MASK ) ) {  // 是IN端点
//				s = ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bEndpointAddress;  // 中断端点的地址
//				break;  // 可以根据需要保存wMaxPacketSize和bInterval
				s = i;  // 返回偏移
				break;
		}
		l = ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bLength;  // 当前描述符长度,跳过
		if ( l > 16 ) break;
	}
	return( s );
}

void	InitUSB_Host( void )  // 初始化USB主机
{
	UINT8	i;
	IE_USB = 0;
//	LED_CFG = 1;
//	LED_RUN = 0;
	USB_CTRL = bUC_HOST_MODE;  // 先设定模式
	USB_DEV_AD = 0x00;
	UH_EP_MOD = bUH_EP_TX_EN | bUH_EP_RX_EN ;
	UH_RX_DMA = RxBuffer;
	UH_TX_DMA = TxBuffer;
	UH_RX_CTRL = 0x00;
	UH_TX_CTRL = 0x00;
	UHUB23_CTRL = UHUB01_CTRL = bUH1_PD_EN | bUH0_PD_EN;
	USB_CTRL = bUC_HOST_MODE | bUC_INT_BUSY;  // 启动USB主机及DMA,在中断标志未清除前自动暂停
	UH_SETUP = bUH_SOF_EN;
	USB_INT_FG = 0xFF;  // 清中断标志
	for ( i = 0; i != 4; i ++ ) {
		DisableRootHubPort( i );  // 清空
		RootHubDev[i].DeviceSpeed = 1;
		RootHubDev[i].DeviceEp0Size = 8;
	}
	USB_INT_EN = bUIE_TRANSFER | bUIE_DETECT;
//	IE_USB = 1;  // 查询方式
}
