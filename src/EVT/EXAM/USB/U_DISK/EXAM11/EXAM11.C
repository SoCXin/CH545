/********************************** (C) COPYRIGHT *******************************
* File Name          : EXAM11.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/03
* Description        : CH545 C���Ե�U��Ŀ¼�ļ�ö�ٳ���
 ֧��: FAT12/FAT16/FAT32
 ע����� CH545UFI.LIB/CH545_USB.C/DEBUG.C
 ע��ʹ��CH545UFI.LIBע��(�������ڴ�ģʽ���ó�:small)
*******************************************************************************/
#include "absacc.h"
#include "CH545UFI.C"
#include "DEBUG.H"
#include "USBHOST.H"

#pragma NOAREGS

/*******************************************************************************
* Function Name  : mStopIfError
* Description    : ������״̬,�����������ʾ������벢ͣ��
* Input          : iError����������
* Return         : None
*******************************************************************************/
void mStopIfError(UINT8 iError)
{
	if(iError == ERR_SUCCESS)
	{
		return;    /* �����ɹ� */
	}
	printf("Error: %02X\n", (UINT16)iError);  /* ��ʾ���� */
	/* ���������,Ӧ�÷����������Լ�CH545DiskStatus״̬,�������CH545DiskReady��ѯ��ǰU���Ƿ�����,���U���ѶϿ���ô�����µȴ�U�̲����ٲ���,
		 ���������Ĵ�������:
		 1������һ��CH545DiskReady,�ɹ����������,����Open,Read/Write��
		 2�����CH545DiskReady���ɹ�,��ôǿ�н���ͷ��ʼ����(�ȴ�U�����ӣ�CH545DiskReady��) */
	while(1)
	{
	}
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Return         : None
*******************************************************************************/
void main(void)
{
	UINT8 s,i;
	UINT8 *pCodeStr;
	UINT16 j;
	
	CfgFsys();
	mDelaymS(5);                                                              //�޸���Ƶ�Լ���ʱ�ȴ���Ƶ�ȶ�
	mInitSTDIO();                                                            //��ʼ������0Ϊ���ü����ͨ�����ڼ����ʾ���� */
	InitUSB_Host();
	CH549LibInit();                                                          //��ʼ��CH545�������֧��U���ļ�
	FoundNewDev = 0;
	printf( "Wait Device In:%02x\n" ,(UINT16)CH549GetVer());
	while(1)
	{
		s = ERR_SUCCESS;
		if(UIF_DETECT)                                                     // �����USB��������ж�����
		{
			UIF_DETECT = 0;                                                     // ���жϱ�־
			s = AnalyzeRootHub();                                              // ����ROOT-HUB״̬
			if(s == ERR_USB_CONNECT)
			{
				FoundNewDev = 1;
			}
		}
		if(FoundNewDev || s == ERR_USB_CONNECT)
		{
			// ���µ�USB�豸����
			FoundNewDev = 0;
			mDelaymS(200);                                                    // ����USB�豸�ղ�����δ�ȶ�,�ʵȴ�USB�豸���ٺ���,������ζ���
			s = InitRootDevice();                                              // ��ʼ��USB�豸
			if(s == ERR_SUCCESS)
			{
				printf("Start UDISK_demo @CH549UFI library\n");
				// U�̲������̣�USB���߸�λ��U�����ӡ���ȡ�豸������������USB��ַ����ѡ�Ļ�ȡ������������֮�󵽴�˴�����CH549�ӳ���������ɺ�������
				CH549DiskStatus = DISK_USB_ADDR;
				for(i = 0; i != 10; i ++)
				{
					printf("Wait DiskReady\n");
					s = CH549DiskReady();
					if(s == ERR_SUCCESS)
					{
						break;
					}
					mDelaymS(50);
				}
				if(CH549DiskStatus >= DISK_MOUNTED)                           //U��׼����
				{
					/* ���ļ� */ 
					strcpy(mCmdParam.Open.mPathName, "/C51/CH549HFT.C");         //����Ҫ�������ļ�����·��
					s = CH549FileOpen();                                          //���ļ�
					printf("Open:%02x\n",(UINT16)s);
					if(s == ERR_MISS_DIR)
					{
						printf("�����ڸ��ļ������г���Ŀ¼�����ļ�\n");
						pCodeStr = "/*";
					}
					else
					{
						pCodeStr = "/C51/*";    //�г�\C51��Ŀ¼�µĵ��ļ�
					}
					printf("List file %s\n", pCodeStr);
					for(j = 0; j < 10000; j ++)                                 //�޶�10000���ļ�,ʵ����û������
					{
						strcpy(mCmdParam.Open.mPathName, pCodeStr);              //�����ļ���,*Ϊͨ���,�����������ļ�������Ŀ¼
						i = strlen(mCmdParam.Open.mPathName);
						mCmdParam.Open.mPathName[i] = 0xFF;                      //�����ַ������Ƚ��������滻Ϊ���������,��0��254,�����0xFF��255��˵�����������CH549vFileSize������
						CH549vFileSize = j;                                        //ָ������/ö�ٵ����
						i = CH549FileOpen();                                      //���ļ�,����ļ����к���ͨ���*,��Ϊ�����ļ�������
						/* CH549FileEnum �� CH549FileOpen ��Ψһ�����ǵ����߷���ERR_FOUND_NAMEʱ��ô��Ӧ��ǰ�߷���ERR_SUCCESS */
						if(i == ERR_MISS_FILE)
						{
							break;    //��Ҳ��������ƥ����ļ�,�Ѿ�û��ƥ����ļ���
						}
						if(i == ERR_FOUND_NAME)                                 //��������ͨ�����ƥ����ļ���,�ļ�����������·�������������
						{
							printf("match file %04d#: %s\n", (unsigned int)j, mCmdParam.Open.mPathName);  /* ��ʾ��ź���������ƥ���ļ���������Ŀ¼�� */
							continue;                                                /* ����������һ��ƥ����ļ���,�´�����ʱ��Ż��1 */
						}
						else                                                       //����
						{
							mStopIfError(i);
							break;
						}
					}
					i = CH549FileClose();                                          //�ر��ļ�
					printf("U����ʾ���\n");
				}
				else
				{
					printf("U��û��׼���� ERR =%02X\n", (UINT16)s);
				}
			}
			else
			{
				printf("��ʼ��U��ʧ�ܣ������U������\n");
			}
		}
		mDelaymS(100);  // ģ�ⵥƬ����������
		SetUsbSpeed(1);  // Ĭ��Ϊȫ��
	}
}