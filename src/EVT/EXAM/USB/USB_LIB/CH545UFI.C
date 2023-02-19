/* 2014.09.09
*****************************************
**   Copyright  (C)  W.ch  1999-2015   **
**   Web:      http://wch.cn           **
*****************************************
**  USB-flash File Interface for CH545@CH549LIB **
**  KC7.0@MCS51                        **
*****************************************
*/
/* CH545 U�������ļ�ϵͳ�ӿ�, ֧��: FAT12/FAT16/FAT32 */

#include "CH545UFI.H"

CMD_PARAM_I	mCmdParam;						/* ������� */
#if		DISK_BASE_BUF_LEN > 0
UINT8X	DISK_BASE_BUF[ DISK_BASE_BUF_LEN ] _at_ 0x0080;	/* �ⲿRAM�Ĵ������ݻ�����,����������Ϊһ�������ĳ��� */
#endif

/* ���³�����Ը�����Ҫ�޸� */

#ifndef	NO_DEFAULT_ACCESS_SECTOR		/* ��Ӧ�ó����ж���NO_DEFAULT_ACCESS_SECTOR���Խ�ֹĬ�ϵĴ���������д�ӳ���,Ȼ�������б�д�ĳ�������� */
//if ( use_external_interface ) {  // �滻U�������ײ��д�ӳ���
//    CH549vSectorSizeH=512>>8;  // ����ʵ�ʵ�������С,������512�ı���,��ֵ�Ǵ��̵�������С�ĸ��ֽ�,���ֽ�����0x00,��ֵ2����0x200
//    CH549vSectorSizeB=9;   // ����ʵ�ʵ�������С��λ����,512���Ӧ9,1024��Ӧ10,2048��Ӧ11
//    CH549DiskStatus=DISK_MOUNTED;  // ǿ�ƿ��豸���ӳɹ�(ֻ������ļ�ϵͳ)
//}

UINT8	CH549ReadSector( UINT8 SectCount, PUINT8X DataBuf )  /* �Ӵ��̶�ȡ������������ݵ��������� */
{
	UINT8	retry;
//	if ( use_external_interface ) return( extReadSector( CH549vLbaCurrent, SectCount, DataBuf ) );  /* �ⲿ�ӿ� */
	for( retry = 0; retry < 3; retry ++ ) {  /* �������� */
		pCBW -> mCBW_DataLen0 = 0;  /* ���ݴ��䳤�� */
		pCBW -> mCBW_DataLen1 = SectCount << CH549vSectorSizeB - 8;
		pCBW -> mCBW_DataLen2 = 0;
		pCBW -> mCBW_DataLen3 = 0;
		pCBW -> mCBW_Flag = 0x80;
		pCBW -> mCBW_LUN = CH549vCurrentLun;
		pCBW -> mCBW_CB_Len = 10;
		pCBW -> mCBW_CB_Buf[ 0 ] = SPC_CMD_READ10;
		pCBW -> mCBW_CB_Buf[ 1 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 2 ] = *(PUINT8)&CH549vLbaCurrent;
		pCBW -> mCBW_CB_Buf[ 3 ] = *( (PUINT8)&CH549vLbaCurrent + 1 );
		pCBW -> mCBW_CB_Buf[ 4 ] = *( (PUINT8)&CH549vLbaCurrent + 2 );
		pCBW -> mCBW_CB_Buf[ 5 ] = *( (PUINT8)&CH549vLbaCurrent + 3 );
		pCBW -> mCBW_CB_Buf[ 6 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 7 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 8 ] = SectCount;
		pCBW -> mCBW_CB_Buf[ 9 ] = 0x00;
		CH549BulkOnlyCmd( DataBuf );  /* ִ�л���BulkOnlyЭ������� */
		if ( CH549IntStatus == ERR_SUCCESS ) {
			return( ERR_SUCCESS );
		}
		CH549IntStatus = CH549AnalyzeError( retry );
		if ( CH549IntStatus != ERR_SUCCESS ) {
			return( CH549IntStatus );
		}
	}
	return( CH549IntStatus = ERR_USB_DISK_ERR );  /* ���̲������� */
}

#ifdef	EN_DISK_WRITE
UINT8	CH549WriteSector( UINT8 SectCount, PUINT8X DataBuf )  /* ���������еĶ�����������ݿ�д����� */
{
	UINT8	retry;
//	if ( use_external_interface ) return( extWriteSector( CH549vLbaCurrent, SectCount, DataBuf ) );  /* �ⲿ�ӿ� */
	for( retry = 0; retry < 3; retry ++ ) {  /* �������� */
		pCBW -> mCBW_DataLen0 = 0;  /* ���ݴ��䳤�� */
		pCBW -> mCBW_DataLen1 = SectCount << CH549vSectorSizeB - 8;
		pCBW -> mCBW_DataLen2 = 0;
		pCBW -> mCBW_DataLen3 = 0;
		pCBW -> mCBW_Flag = 0x00;
		pCBW -> mCBW_LUN = CH549vCurrentLun;
		pCBW -> mCBW_CB_Len = 10;
		pCBW -> mCBW_CB_Buf[ 0 ] = SPC_CMD_WRITE10;
		pCBW -> mCBW_CB_Buf[ 1 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 2 ] = *(PUINT8)&CH549vLbaCurrent;
		pCBW -> mCBW_CB_Buf[ 3 ] = *( (PUINT8)&CH549vLbaCurrent + 1 );
		pCBW -> mCBW_CB_Buf[ 4 ] = *( (PUINT8)&CH549vLbaCurrent + 2 );
		pCBW -> mCBW_CB_Buf[ 5 ] = *( (PUINT8)&CH549vLbaCurrent + 3 );
		pCBW -> mCBW_CB_Buf[ 6 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 7 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 8 ] = SectCount;
		pCBW -> mCBW_CB_Buf[ 9 ] = 0x00;
		CH549BulkOnlyCmd( DataBuf );  /* ִ�л���BulkOnlyЭ������� */
		if ( CH549IntStatus == ERR_SUCCESS ) {
			mDelayuS( 200 );  /* д��������ʱ */
			return( ERR_SUCCESS );
		}
		CH549IntStatus = CH549AnalyzeError( retry );
		if ( CH549IntStatus != ERR_SUCCESS ) {
			return( CH549IntStatus );
		}
	}
	return( CH549IntStatus = ERR_USB_DISK_ERR );  /* ���̲������� */
}
#endif
#endif  // NO_DEFAULT_ACCESS_SECTOR

#ifndef	NO_DEFAULT_DISK_CONNECT			/* ��Ӧ�ó����ж���NO_DEFAULT_DISK_CONNECT���Խ�ֹĬ�ϵļ����������ӳ���,Ȼ�������б�д�ĳ�������� */
/* �������Ƿ����� */
UINT8	CH549DiskConnect( void )
{
	UINT8	rhub_idx1, rhub_idx2;
	USB_DEV_AD &= 0x7F;
	rhub_idx2 = ( USB_DEV_AD >> 4 ) - 1;  // Root-HUB index with external HUB
	rhub_idx1 = USB_DEV_AD - USB_DEVICE_ADDR;  // Root-HUB index without external HUB
	if ( rhub_idx1 < 4 ) {  /* ����Root-HUB�µ�USB�豸 */
		if ( ( rhub_idx1 & 0x02 ? UHUB23_CTRL : UHUB01_CTRL ) & ( rhub_idx1 & 0x01 ? bUH1_PORT_EN : bUH0_PORT_EN ) ) {  /* ����Root-HUB�µ�USB�豸������δ��� */
			return( ERR_SUCCESS );  /* USB�豸�Ѿ�������δ��� */
		}
		else if ( USB_HUB_ST & (UINT8)( (UINT8)bUHS_H0_ATTACH << rhub_idx1 ) ) {  /* USB�豸���� */
mDiskConnect:
			CH549DiskStatus = DISK_CONNECT;  /* �����Ͽ��� */
			return( ERR_SUCCESS );  /* �ⲿHUB��USB�豸�Ѿ����ӻ��߶Ͽ����������� */
		}
		else {  /* USB�豸�Ͽ� */
mDiskDisconn:
			CH549DiskStatus = DISK_DISCONNECT;
			return( ERR_USB_DISCON );
		}
	}
#ifndef	FOR_ROOT_UDISK_ONLY
	else if ( rhub_idx2 < 4 && (UINT8)( ( USB_DEV_AD & 0x0F ) - 1 ) < 4 ) {  /* �ⲿHUB�Ķ˿��µ�USB�豸 */
		if ( ( rhub_idx2 & 0x02 ? UHUB23_CTRL : UHUB01_CTRL ) & ( rhub_idx2 & 0x01 ? bUH1_PORT_EN : bUH0_PORT_EN ) ) {  /* ����Root-HUB�µ��ⲿHUB������δ��� */
			TxBuffer[ MAX_PACKET_SIZE - 1 ] = USB_DEV_AD;  /* ���� */
			USB_DEV_AD = USB_DEVICE_ADDR + rhub_idx2;  /* ����USB�����˵�USB��ַָ��HUB */
			CH549IntStatus = HubGetPortStatus( TxBuffer[ MAX_PACKET_SIZE - 1 ] & 0x0F );  /* ��ѯHUB�˿�״̬,������TxBuffer�� */
			if ( CH549IntStatus == ERR_SUCCESS ) {
				if ( TxBuffer[2] & (1<<(HUB_C_PORT_CONNECTION-0x10)) ) {  /* ��⵽HUB�˿��ϵĲ���¼� */
					CH549DiskStatus = DISK_DISCONNECT;  /* �ٶ�ΪHUB�˿��ϵ�USB�豸�Ͽ� */
					HubClearPortFeature( TxBuffer[ MAX_PACKET_SIZE - 1 ] & 0x0F, HUB_C_PORT_CONNECTION );  /* ���HUB�˿������¼�״̬ */
				}
				USB_DEV_AD = TxBuffer[ MAX_PACKET_SIZE - 1 ];  /* ����USB�����˵�USB��ַָ��USB�豸 */
				if ( TxBuffer[0] & (1<<HUB_PORT_CONNECTION) ) {  /* ����״̬ */
					if ( CH549DiskStatus < DISK_CONNECT ) {
						CH549DiskStatus = DISK_CONNECT;  /* �����Ͽ��� */
					}
					return( ERR_SUCCESS );  /* USB�豸�Ѿ����ӻ��߶Ͽ����������� */
				}
				else {
//					CH549DiskStatus = DISK_DISCONNECT;
//					return( ERR_USB_DISCON );
					CH549DiskStatus = DISK_CONNECT;
					return( ERR_HUB_PORT_FREE );  /* HUB�Ѿ����ӵ���HUB�˿���δ���Ӵ��� */
				}
			}
			else {
				USB_DEV_AD = TxBuffer[ MAX_PACKET_SIZE - 1 ];  /* ����USB�����˵�USB��ַָ��USB�豸 */
				if ( CH549IntStatus == ERR_USB_DISCON ) {
//					CH549DiskStatus = DISK_DISCONNECT;
//					return( ERR_USB_DISCON );
					goto mDiskDisconn;
				}
				else {
					CH549DiskStatus = DISK_CONNECT;  /* HUB����ʧ�� */
					return( CH549IntStatus );
				}
			}
		}
		else if ( USB_HUB_ST & (UINT8)( (UINT8)bUHS_H0_ATTACH << rhub_idx2 ) ) {  /* USB�豸����,�ⲿHUB��USB�豸�Ѿ����ӻ��߶Ͽ����������� */
//			CH549DiskStatus = DISK_CONNECT;  /* �����Ͽ��� */
//			return( ERR_SUCCESS );  /* �ⲿHUB��USB�豸�Ѿ����ӻ��߶Ͽ����������� */
			goto mDiskConnect;
		}
		else {  /* �ⲿHUB�Ͽ� */
			CH549DiskStatus = DISK_DISCONNECT;
			return( ERR_USB_DISCON );
		}
	}
#endif
	else {
//		CH549DiskStatus = DISK_DISCONNECT;
//		return( ERR_USB_DISCON );
		goto mDiskDisconn;
	}
}
#endif  // NO_DEFAULT_DISK_CONNECT

#ifndef	NO_DEFAULT_FILE_ENUMER			/* ��Ӧ�ó����ж���NO_DEFAULT_FILE_ENUMER���Խ�ֹĬ�ϵ��ļ���ö�ٻص�����,Ȼ�������б�д�ĳ�������� */
void xFileNameEnumer( void )			/* �ļ���ö�ٻص��ӳ��� */
{
/* ���ָ��ö�����CH549vFileSizeΪ0xFFFFFFFF�����FileOpen����ôÿ������һ���ļ�FileOpen������ñ��ص�����
   �ص�����xFileNameEnumer���غ�FileOpen�ݼ�CH549vFileSize������ö��ֱ�����������ļ�����Ŀ¼�����������ǣ�
   �ڵ���FileOpen֮ǰ����һ��ȫ�ֱ���Ϊ0����FileOpen�ص�������󣬱�������CH549vFdtOffset�õ��ṹFAT_DIR_INFO��
   �����ṹ�е�DIR_Attr�Լ�DIR_Name�ж��Ƿ�Ϊ�����ļ�������Ŀ¼������¼�����Ϣ������ȫ�ֱ�������������
   ��FileOpen���غ��жϷ���ֵ�����ERR_MISS_FILE��ERR_FOUND_NAME����Ϊ�����ɹ���ȫ�ֱ���Ϊ����������Ч�ļ�����
   ����ڱ��ص�����xFileNameEnumer�н�CH549vFileSize��Ϊ1����ô����֪ͨFileOpen��ǰ���������������ǻص��������� */
#if		0
	UINT8			i;
	UINT16			FileCount;
	PX_FAT_DIR_INFO	pFileDir;
	PUINT8			NameBuf;
	pFileDir = (PX_FAT_DIR_INFO)( pDISK_BASE_BUF + CH549vFdtOffset );  /* ��ǰFDT����ʼ��ַ */
	FileCount = (UINT16)( 0xFFFFFFFF - CH549vFileSize );  /* ��ǰ�ļ�����ö�����,CH549vFileSize��ֵ��0xFFFFFFFF,�ҵ��ļ�����ݼ� */
	if ( FileCount < sizeof( FILE_DATA_BUF ) / 12 ) {  /* ��黺�����Ƿ��㹻���,�ٶ�ÿ���ļ�����ռ��12���ֽڴ�� */
		NameBuf = & FILE_DATA_BUF[ FileCount * 12 ];  /* ���㱣�浱ǰ�ļ����Ļ�������ַ */
		for ( i = 0; i < 11; i ++ ) NameBuf[ i ] = pFileDir -> DIR_Name[ i ];  /* �����ļ���,����Ϊ11���ַ�,δ�����ո� */
//		if ( pFileDir -> DIR_Attr & ATTR_DIRECTORY ) NameBuf[ i ] = 1;  /* �ж���Ŀ¼�� */
		NameBuf[ i ] = 0;  /* �ļ��������� */
	}
#endif
}
#endif  // NO_DEFAULT_FILE_ENUMER

UINT8	CH549LibInit( void )  /* ��ʼ��CH549�����,�����ɹ�����0 */
{
	if ( CH549GetVer( ) < CH549_LIB_VER ) return( 0xFF );  /* ��ȡ��ǰ�ӳ����İ汾��,�汾̫���򷵻ش��� */
#if		DISK_BASE_BUF_LEN > 0
	pDISK_BASE_BUF = & DISK_BASE_BUF[0];  /* ָ���ⲿRAM�Ĵ������ݻ����� */
	pDISK_FAT_BUF = & DISK_BASE_BUF[0];  /* ָ���ⲿRAM�Ĵ���FAT���ݻ�����,������pDISK_BASE_BUF�����Խ�ԼRAM */
/* ���ϣ������ļ���ȡ�ٶ�,��ô�������������е���CH549LibInit֮��,��pDISK_FAT_BUF����ָ����һ�������������pDISK_BASE_BUFͬ����С�Ļ����� */
#endif
	CH549DiskStatus = DISK_UNKNOWN;  /* δ֪״̬ */
	CH549vSectorSizeB = 9;  /* Ĭ�ϵ��������̵�������512B */
	CH549vSectorSizeH = 512 >> 8;  // Ĭ�ϵ��������̵�������512B,��ֵ�Ǵ��̵�������С�ĸ��ֽ�,���ֽ�����0x00,��ֵ2����0x200
	CH549vStartLba = 0;  /* Ĭ��Ϊ�Զ�����FDD��HDD */
	return( ERR_SUCCESS );
}