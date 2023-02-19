#ifndef __USBHOST_H__
#define __USBHOST_H__


#include "DEBUG.H"


//���ӳ��򷵻�״̬��
#define ERR_SUCCESS         0x00    // �����ɹ�
#define ERR_USB_CONNECT     0x15    /* ��⵽USB�豸�����¼�,�Ѿ����� */
#define ERR_USB_DISCON      0x16    /* ��⵽USB�豸�Ͽ��¼�,�Ѿ��Ͽ� */
#define ERR_USB_BUF_OVER    0x17    /* USB��������������������̫�໺������� */
#define ERR_USB_DISK_ERR    0x1F    /* USB�洢������ʧ��,�ڳ�ʼ��ʱ������USB�洢����֧��,�ڶ�д�����п����Ǵ����𻵻����Ѿ��Ͽ� */
#define ERR_USB_TRANSFER    0x20    /* NAK/STALL�ȸ����������0x20~0x2F */
#define ERR_USB_UNSUPPORT   0xFB    /*��֧�ֵ�USB�豸*/
#define ERR_USB_UNKNOWN     0xFE    /*�豸��������*/ 
#define ERR_AOA_PROTOCOL    0x41    /*Э��汾���� */ 

/*USB�豸�����Ϣ��,CH545���֧��1���豸*/
#define ROOT_DEV_DISCONNECT  0
#define ROOT_DEV_CONNECTED   1
#define ROOT_DEV_FAILED      2
#define ROOT_DEV_SUCCESS     3
#define DEV_TYPE_KEYBOARD   ( USB_DEV_CLASS_HID | 0x20 )
#define DEV_TYPE_MOUSE      ( USB_DEV_CLASS_HID | 0x30 )
#define DEF_AOA_DEVICE       0xF0
/*
Լ��: USB�豸��ַ�������(�ο�USB_DEVICE_ADDR)
��ֵַ  �豸λ��
0x02    ����Root-HUB�µ�USB�豸���ⲿHUB
0x1x    ����Root-HUB�µ��ⲿHUB�Ķ˿�x�µ�USB�豸,xΪ1~n
*/
#define HUB_MAX_PORTS       4
#define WAIT_USB_TOUT_200US     400   // �ȴ�USB�жϳ�ʱʱ��200uS@Fsys=12MHz


/* �����С���� */
#define COM_BUF_SIZE            128   // �ɸ��������������С����̬�޸��Խ�ʡ�ڴ档

extern UINT8X  UsbDevEndp0Size;       //* USB�豸�Ķ˵�0�������ߴ� */
#ifndef DISK_BASE_BUF_LEN
typedef struct
{
    UINT8   DeviceStatus;              // �豸״̬,0-���豸,1-���豸����δ��ʼ��,2-���豸����ʼ��ö��ʧ��,3-���豸�ҳ�ʼ��ö�ٳɹ�
    UINT8   DeviceAddress;             // �豸�������USB��ַ
    UINT8   DeviceSpeed;               // 0Ϊ����,��0Ϊȫ��
    UINT8   DeviceType;                // �豸����
		UINT16  DeviceVID;
		UINT16  DevicePID;
    UINT8   GpVar[4];                    // ͨ�ñ�������Ŷ˵�
    UINT8   GpHUBPortNum;                // ͨ�ñ���,�����HUB����ʾHUB�˿���
} _RootHubDev;

typedef struct 
{
    UINT8   DeviceStatus;             // �豸״̬,0-���豸,1-���豸����δ��ʼ��,2-���豸����ʼ��ö��ʧ��,3-���豸�ҳ�ʼ��ö�ٳɹ�
    UINT8   DeviceAddress;            // �豸�������USB��ַ
    UINT8   DeviceSpeed;              // 0Ϊ����,��0Ϊȫ��
    UINT8   DeviceType;               // �豸����
		UINT16  DeviceVID;
		UINT16  DevicePID;
    UINT8   GpVar[4];                    // ͨ�ñ���
} _DevOnHubPort;                      // �ٶ�:������1���ⲿHUB,ÿ���ⲿHUB������HUB_MAX_PORTS���˿�(���˲���)

extern _RootHubDev xdata ThisUsbDev;
extern _DevOnHubPort xdata DevOnHubPort[HUB_MAX_PORTS];// �ٶ�:������1���ⲿHUB,ÿ���ⲿHUB������HUB_MAX_PORTS���˿�(���˲���)
extern UINT8 Set_Port;
#endif

extern UINT8X  RxBuffer[];                   // IN, must even address
extern UINT8X  TxBuffer[];                   // OUT, must even address
extern UINT8X  Com_Buffer[];
extern bit     FoundNewDev;
////////////////�����ӿ�����//////////////////////////////////////
UINT8   InitRootDevice( void );                        // ��ʼ��ָ��ROOT-HUB�˿ڵ�USB�豸
void    InitUSB_Host( void );                          // ��ʼ��USB����
void    DisableRootHubPort( );                        // �ر�ROOT-HUB�˿�,ʵ����Ӳ���Ѿ��Զ��ر�,�˴�ֻ�����һЩ�ṹ״̬
UINT8   AnalyzeRootHub( void );                       // ����ROOT-HUB״̬,����ROOT-HUB�˿ڵ��豸����¼�                     
// ����ERR_SUCCESSΪû�����,����ERR_USB_CONNECTΪ��⵽������,����ERR_USB_DISCONΪ��⵽�Ͽ�
void    ResetRootHubPort( );                          // ��⵽�豸��,��λ��Ӧ�˿ڵ�����,Ϊö���豸׼��,����ΪĬ��Ϊȫ��
void    SetHostUsbAddr( UINT8 addr );                 // ����USB������ǰ������USB�豸��ַ
void    SetUsbSpeed( UINT8 FullSpeed );               // ���õ�ǰUSB�ٶ�
UINT8   EnableRootHubPort( );                         // ʹ��ROOT-HUB�˿�,��Ӧ��bUH_PORT_EN��1�����˿�,�豸�Ͽ����ܵ��·���ʧ��
void    SelectHubPort( UINT8 HubPortIndex );// HubPortIndex=0ѡ�����ָ����ROOT-HUB�˿�,����ѡ�����ָ����ROOT-HUB�˿ڵ��ⲿHUB��ָ���˿�
UINT8   WaitUSB_Interrupt( void );                    // �ȴ�USB�ж�
// CH554��������,����Ŀ�Ķ˵��ַ/PID����,ͬ����־,��20uSΪ��λ��NAK������ʱ��(0������,0xFFFF��������),����0�ɹ�,��ʱ/��������
UINT8   USBHostTransact( UINT8 endp_pid, UINT8 tog, UINT16 timeout );  // endp_pid: ��4λ��token_pid����, ��4λ�Ƕ˵��ַ
UINT8   HostCtrlTransfer( PUINT8X DataBuf, PUINT8I RetLen );  // ִ�п��ƴ���,8�ֽ���������pSetupReq��,DataBufΪ��ѡ���շ�������
// �����Ҫ���պͷ�������,��ôDataBuf��ָ����Ч���������ڴ�ź�������,ʵ�ʳɹ��շ����ܳ��ȷ��ر�����ReqLenָ����ֽڱ�����
void    CopySetupReqPkg( PUINT8C pReqPkt );            // ���ƿ��ƴ���������
UINT8   CtrlGetDeviceDescr( void );                    // ��ȡ�豸������,������TxBuffer��
UINT8   CtrlGetConfigDescr( void );                    // ��ȡ����������,������TxBuffer��
UINT8   CtrlSetUsbAddress( UINT8 addr );               // ����USB�豸��ַ
UINT8   CtrlSetUsbConfig( UINT8 cfg );                 // ����USB�豸����
UINT8   CtrlClearEndpStall( UINT8 endp );              // ����˵�STALL

#ifndef DISK_BASE_BUF_LEN
UINT8   CtrlSetUsbIntercace( UINT8 cfg );              // ����USB�豸�ӿ� 
UINT8   CtrlGetHIDDeviceReport( UINT8 infc );          // HID�����SET_IDLE��GET_REPORT 
UINT8   CtrlGetHubDescr( void );                       // ��ȡHUB������,������TxBuffer��
UINT8   HubGetPortStatus( UINT8 HubPortIndex );        // ��ѯHUB�˿�״̬,������TxBuffer��
UINT8   HubSetPortFeature( UINT8 HubPortIndex, UINT8 FeatureSelt );  // ����HUB�˿�����
UINT8   HubClearPortFeature( UINT8 HubPortIndex, UINT8 FeatureSelt );  // ���HUB�˿�����
UINT8   CtrlGetXPrinterReport1( void ) ;               //��ӡ��������
UINT8   AnalyzeHidIntEndp( PUINT8X buf, UINT8 HubPortIndex);           // ���������з�����HID�ж϶˵�ĵ�ַ
UINT8   AnalyzeBulkEndp( PUINT8X buf, UINT8 HubPortIndex ) ;           //�����������˵�
UINT8   TouchStartAOA( void );                         // ����AOA����
UINT8   EnumAllRootDevice( void );                     // ö������ROOT-HUB�˿ڵ�USB�豸
UINT8   InitDevOnHub(UINT8 HubPortIndex );             // ��ʼ��ö���ⲿHUB��Ķ���USB�豸
UINT8   EnumHubPort( );                                // ö��ָ��ROOT-HUB�˿��ϵ��ⲿHUB�������ĸ����˿�,�����˿��������ӻ��Ƴ��¼�����ʼ������USB�豸
UINT8   EnumAllHubPort( void );                        // ö������ROOT-HUB�˿����ⲿHUB��Ķ���USB�豸
UINT16  SearchTypeDevice( UINT8 type );                // ��ROOT-HUB�Լ��ⲿHUB���˿�������ָ�����͵��豸���ڵĶ˿ں�,����˿ں�Ϊ0xFFFF��δ������
													   // �����8λΪROOT-HUB�˿ں�,��8λΪ�ⲿHUB�Ķ˿ں�,��8λΪ0���豸ֱ����ROOT-HUB�˿���
UINT8 SETorOFFNumLock(PUINT8 buf);
#endif




#endif