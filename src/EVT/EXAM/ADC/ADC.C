/********************************** (C) COPYRIGHT *******************************
* File Name          : ADC.C
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/06
* Description        : CH545 ADC����ʱ�����ã�ADCͨ�����ú�������ѹ�Ƚ�ģʽ���� 
*******************************************************************************/
#include "ADC.H"

#pragma  NOAREGS

/*******************************************************************************
* Function Name  : ADCInit
* Description    : ADC����ʱ������,ģ�鿪�����жϿ���
* Input          : div���ο�ʱ������ 
*                  3-64��Fosc
*                  2-128��Fosc
*                  1-256��Fosc                   								
*                  0-512��Fosc									 
* Return         : None
*******************************************************************************/
void ADCInit(UINT8 div)
{
	ADC_CTRL &= ~MASK_ADC_CLK;
	ADC_CTRL |= bADC_EN | (div & MASK_ADC_CLK);                  //ADC��Դʹ��
	#if ADC_INTERRUPT
	ADCIF_CLEAR();                                               //��ADC�ж�
	IE_ADC = 1;                                                  //ʹ��ADC�ж�
	
#endif
}

/*******************************************************************************
* Function Name  : ADC_ChannelSelect
* Description    : ADC��������
* Input          : ch������ͨ��
* Return         : �ɹ� SUCCESS
*                  ʧ�� FAIL
*******************************************************************************/
UINT8 ADC_ChannelSelect(UINT8 ch)
{
	ADC_CHAN &= 0xf0;

	if(ch >= ADC_Channel0 && ch <= ADC_Channel7){                       //AIN0-AIN7,��������   
		P1_MOD_OC = P1_DIR_PU &= ~(1<<ch);       
	}
	else if(ch>=ADC_Channel8 && ch <=ADC_Channel13){                   //AIN8-AIN13,��������      
		P0_MOD_OC = P0_DIR_PU &= ~(1<<(ch-8));       
	}
	else if(ch == ADC_ChannelV33 || ch == ADC_ChannelV18){             //�ڲ�
			
	} 
	else return FAIL;

	ADC_CHAN |= ch;
	 
	return SUCCESS;
}

/*******************************************************************************
* Function Name  :  VoltageCMPModeInit
* Description    :  ��ѹ�Ƚ���ģʽ��ʼ��
* Input          :  fo������˿� 
*                    0-VDD 
*                    1-ADC_CHAN
*                   re�����������
*                    000-�رձȽ���
*                    001-�ڲ�1.2V
*                    010-3.3V
*                    011-5V
*                    100-5.4V
*                    101-5.8V
*                    110-6.2V
*                    111-6.6V
* Return         : �ɹ� SUCCESS
*                  ʧ�� FAIL
*******************************************************************************/
void VoltageCMPModeInit(UINT8 fo, UINT8 re)
{
	CMP_DCDC &= 0xf0;

	if(fo)  
		CMP_DCDC |= bCMP_PIN;
	if(re)  
		CMP_DCDC |= MASK_CMP_VREF & re;   
    
}