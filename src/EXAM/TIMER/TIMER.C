/********************************** (C) COPYRIGHT *******************************
* File Name          : TIMER.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH545 Time ��ʼ������ʱ������������ֵ��T2��׽���ܿ���������
                       ��ʱ���жϺ���
*******************************************************************************/
#include "Timer.H"
#pragma  NOAREGS

/*******************************************************************************
* Function Name  : mTimer_x_ModInit
* Description    : CH545��ʱ������ѡ���ģʽ����
* Input          : x��ѡ��ʱ������
*                   0-TIM0
*                   1-TIM1
*                   2-TIM2
*                  mode��Timerģʽѡ��
*                   0-ģʽ0��13λ��ʱ����TLn�ĸ�3λ��Ч
*                   1-ģʽ1��16λ��ʱ��
*                   2-ģʽ2��8λ�Զ���װ��ʱ��
*                   3-ģʽ3��Timer0�ֳ�����8λ��ʱ����Timer1ֹͣ
* Return         : �ɹ�  SUCCESS
*                  ʧ��  FAIL
*******************************************************************************/
UINT8 mTimer_x_ModInit(UINT8 x ,UINT8 mode)
{
	if(x == 0)
	{
		TMOD = TMOD & 0xf0 | mode;
	}
	else if(x == 1)
	{
		TMOD = TMOD & 0x0f | (mode<<4);
	}
	else if(x == 2)
	{
		RCLK = 0;    //16λ�Զ����ض�ʱ��
		TCLK = 0;
		CP_RL2 = 0;
	}
	else
	{
		return FAIL;
	}
	
	return SUCCESS;
}

/*******************************************************************************
* Function Name  : mTimer_x_SetData
* Description    : CH545Timer ��������ֵ
* Input          : x��ѡ��ʱ������
*                   0-TIM0
*                   1-TIM1
*                   2-TIM2
*                  dat����������ֵ
* Return         : None
*******************************************************************************/
void mTimer_x_SetData(UINT8 x,UINT16 dat)
{
	UINT16 tmp;
	
	tmp = 65536 - dat;
	if(x == 0)
	{
		TL0 = tmp & 0xff;
		TH0 = (tmp>>8) & 0xff;
	}
	else if(x == 1)
	{
		TL1 = tmp & 0xff;
		TH1 = (tmp>>8) & 0xff;
	}
	else if(x == 2)
	{
		RCAP2L = TL2 = tmp & 0xff;                                               //16λ�Զ����ض�ʱ��
		RCAP2H = TH2 = (tmp>>8) & 0xff;
	}
}

/*******************************************************************************
* Function Name  : CAP2Init
* Description    : CH545��ʱ������2 T2EX���Ų�׽���ܳ�ʼ����CAP2 P11��
* Input          : mode�����ز�׽ģʽѡ��
*                   0��2��T2ex���½��ص���һ���½���
*                   1��T2ex�������֮��
*                   3��T2ex�������ص���һ��������
* Return         : None
*******************************************************************************/
void CAP2Init(UINT8 mode)
{
	RCLK = 0;
	TCLK = 0;
	C_T2  = 0;
	EXEN2 = 1;
	CP_RL2 = 1;                                                                //����T2ex�Ĳ�׽����
	T2MOD |= mode << 2;                                                        //���ز�׽ģʽѡ��
}

/*******************************************************************************
* Function Name  : CAP1Init
* Description    : CH545��ʱ������2 T2���Ų�׽���ܳ�ʼ��T2(CAP1 P10)
* Input          : mode,���ز�׽ģʽѡ��
*                   0��2:T2ex���½��ص���һ���½���
*                   1:T2ex�������֮��
*                   3:T2ex�������ص���һ��������
* Return         : None
*******************************************************************************/
void CAP1Init(UINT8 mode)
{
	RCLK = 0;
	TCLK = 0;
	CP_RL2 = 1;
	EXEN2 = 1;
	C_T2 = 0;
	T2MOD = T2MOD & ~T2OE | (mode << 2) | bT2_CAP1_EN;                         //ʹ��T2���Ų�׽����,���ز�׽ģʽѡ��
}
