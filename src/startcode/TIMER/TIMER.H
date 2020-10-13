#ifndef __TIMER_H__
#define __TIMER_H__

#include "Debug.H"


#define Timer0_INTERRUPT        1                                              //选择Timer0中断方式
#define Timer1_INTERRUPT        0                                              //选择Timer1中断方式
#define Timer2_INTERRUPT        0                                              //选择Timer2中断方式
#define T2_CAP                  1                                              //选择Timer2捕捉模式

/******************* CH545 Timer0相关 ************************/
//bTMR_CLK同时影响Timer0&1&2,使用时要注意 (除定时使用标准时钟)
#define mTimer0Clk12DivFsys( ) (T2MOD &= ~bT0_CLK)                             //定时器,时钟=Fsys/12 T0标准时钟
#define mTimer0ClkFsys( )      (T2MOD |= bTMR_CLK | bT0_CLK)                   //定时器,时钟=Fsys
#define mTimer0Clk4DivFsys( )  (T2MOD &= ~bTMR_CLK;T2MOD |=  bT0_CLK)          //定时器,时钟=Fsys/4（此时需同步调整DEBUG.C的mInitSTDIO）
#define mTimer0CountClk( )     (TMOD |= bT0_CT)                                //选择工作在计数器模式（T0引脚（P34）的下降沿有效）
/***** CH555 Timer1相关(DEBUG.C的mInitSTDIO使用T1模式2) *****/
#define mTimer1Clk12DivFsys( ) (T2MOD &= ~bT1_CLK)                             //定时器,时钟=Fsys/12  T1标准时钟
#define mTimer1ClkFsys( )      (T2MOD |= bTMR_CLK | bT1_CLK)                   //定时器,时钟=Fsys
#define mTimer1Clk4DivFsys( )  (T2MOD &= ~bTMR_CLK;T2MOD |=  bT1_CLK)          //定时器,时钟=Fsys/4
#define mTimer1CountClk( )     (TMOD |= bT1_CT)                                //选择工作在计数器模式（T1引脚（P35）的下降沿有效）
/******************* CH555 Timer2相关 ************************/
#define mTimer2Clk12DivFsys( ) {T2MOD &= ~ bT2_CLK;C_T2 = 0;}                  //定时器,时钟=Fsys/12 T2标准时钟
#define mTimer2ClkFsys( )      {T2MOD |= (bTMR_CLK | bT2_CLK);C_T2=0;}         //定时器,时钟=Fsys
#define mTimer2Clk4DivFsys( )  {T2MOD &= ~bTMR_CLK;T2MOD |=  bT2_CLK;C_T2 = 0;}//定时器,时钟=Fsys/4（此时需同步调整DEBUG.C的mInitSTDIO）
#define mTimer2CountClk( )     {C_T2 = 1;}                                     //选择工作在计数器模式（T2引脚（P10）的下降沿有效）
#define mTimer2OutCTL( )       (T2MOD |= T2OE)                                 //T2输出  频率TF2/2   


/*******************************************************************************
* Function Name  : mTimer_x_ModInit(UINT8 x ,UINT8 mode)
* Description    : CH545定时计数器x模式设置
* Input          : UINT8 mode,Timer模式选择
                   0：模式0，13位定时器，TLn的高3位无效
                   1：模式1，16位定时器
                   2：模式2，8位自动重装定时器
                   3：模式3，两个8位定时器  Timer0
                   3：模式3，Timer1停止
                   UINT8 x 定时器  0 1 2
* Output         : None
* Return         : 成功  SUCCESS
                   失败  FAIL
*******************************************************************************/
UINT8 mTimer_x_ModInit(UINT8 x ,UINT8 mode);
/*******************************************************************************
* Function Name  : mTimer_x_SetData(UINT8 x,UINT16 dat)
* Description    : CH545Timer
* Input          : UINT16 dat;定时器赋值
                   UINT8 x 定时器  0 1 2
* Output         : None
* Return         : None
*******************************************************************************/
void mTimer_x_SetData(UINT8 x,UINT16 dat);
/*******************************************************************************
* Function Name  : CAP2Init(UINT8 mode)
* Description    : CH545定时计数器2 T2EX引脚捕捉功能初始化（CAP2 P11）
                   UINT8 mode,边沿捕捉模式选择
                   0:T2ex从下降沿到下一个下降沿
                   1:T2ex任意边沿之间
                   3:T2ex从上升沿到下一个上升沿
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAP2Init(UINT8 mode);
/*******************************************************************************
* Function Name  : CAP1Init(UINT8 mode)
* Description    : CH545定时计数器2 T2引脚捕捉功能初始化T2(CAP1 P10)
                   UINT8 mode,边沿捕捉模式选择
                   0:T2ex从下降沿到下一个下降沿
                   1:T2ex任意边沿之间
                   3:T2ex从上升沿到下一个上升沿
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAP1Init(UINT8 mode);

#endif

