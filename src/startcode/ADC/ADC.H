#ifndef	__ADC_H__
#define __ADC_H__

#include "Debug.H"


#define ADC_INTERRUPT   1           //0:查询方式,1:中断方式

#define ADC_ENABLE()      ADC_CTRL |= bADC_EN
#define ADC_DISABLE()     ADC_CTRL &= ~bADC_EN

#define ADC_START()       ADC_CTRL |= bADC_START     
#define ADCIF_CLEAR()     ADC_CTRL |= bADC_IF

#define DCDC_FREQ(freq)   CMP_DCDC&=~MASK_DCDC_FREQ;CMP_DCDC|=MASK_DCDC_FREQ&freq
#define DCDC_PIN(b)       CMP_DCDC&=~bDCDC_PIN;CMP_DCDC|=bDCDC_PIN&b
//ADC时钟频率
#define ADC_FOSC512      0
#define ADC_FOSC256      1
#define ADC_FOSC128      2
#define ADC_FOSC64       3

//ADC通道
#define ADC_Channel0     0
#define ADC_Channel1     1
#define ADC_Channel2     2
#define ADC_Channel3     3
#define ADC_Channel4     4
#define ADC_Channel5     5
#define ADC_Channel6     6
#define ADC_Channel7     7
#define ADC_Channel8     8
#define ADC_Channel9     9
#define ADC_Channel10    10
#define ADC_Channel11    11
#define ADC_Channel12    12
#define ADC_Channel13    13
#define ADC_ChannelV33   14
#define ADC_ChannelV18   15

//正相输入
#define CMP_VDD          0
#define CMP_ADC          1

//反相输入
#define CMP_CLOSE        0
#define CMP_V12          1
#define CMP_V33          2
#define CMP_V50          3
#define CMP_V54          4
#define CMP_V58          5
#define CMP_V62          6
#define CMP_V66          7
/*******************************************************************************
* Function Name  : ADCInit(UINT8 div)
* Description    : ADC采样时钟设置,模块开启，中断开启
* Input          : UINT8 div 参考时钟设置 
                   3    64个Fosc
                   2    128个Fosc
                   1    256个Fosc                   								
                   0    512个Fosc									 
* Output         : None
* Return         : None
*******************************************************************************/
void ADCInit(UINT8 div);
/*******************************************************************************
* Function Name  : ADC_ChannelSelect(UINT8 ch)
* Description    : ADC采样启用
* Input          : UINT8 ch 采用通道
* Output         : None
* Return         : 成功 SUCCESS
                   失败 FAIL
*******************************************************************************/
UINT8 ADC_ChannelSelect(UINT8 ch);
/*******************************************************************************
* Function Name  :  VoltageCMPModeInit()
* Description    :  电压比较器模式初始化
* Input          :  UINT8 fo 正向端口 0:VDD 1:ADC_CHAN
                    UINT8 re 反向输入端
                    000-关闭比较器
                    001-内部1.2V
                    010-3.3V
                    011-5V
                    100-5.4V
                    101-5.8V
                    110-6.2V
                    111-6.6V
* Output         : None
* Return         : 成功 SUCCESS
                   失败 FAIL
*******************************************************************************/
void VoltageCMPModeInit(UINT8 fo,UINT8 re);
#endif