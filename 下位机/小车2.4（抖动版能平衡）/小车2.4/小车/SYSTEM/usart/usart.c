#include "sys.h"
#include "usart.h"	  
#include "string.h"
#include "timer.h"
#include "led.h"
#include "stm32f10x_dma.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{   
//  USART_SendData(USART1,(u8)ch);
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//	return ch;
//	
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 

 
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 openmv_buff[USART_REC_LEN];
float mv_distance=0,mv_theta=0;
char timecount_flag[2];

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart1_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
//	DMA_InitTypeDef DMA_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//	//DMA1通道5配置
//	DMA_DeInit(DMA1_Channel5);
//	//外设地址
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
//	//内存地址
//	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)openmv_buff;
//	//dma传输方向单向
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//	//设置DMA在传输时缓冲区的长度
//	DMA_InitStructure.DMA_BufferSize = USART_REC_LEN;
//	//设置DMA的外设递增模式，一个外设
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	//设置DMA的内存递增模式
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	//外设数据字长
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	//内存数据字长
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	//设置DMA的传输模式
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//	//设置DMA的优先级别
//	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//	//设置DMA的2个memory中的变量互相访问
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//	DMA_Init(DMA1_Channel5,&DMA_InitStructure);
// 
//	//使能通道5
//	DMA_Cmd(DMA1_Channel5,ENABLE);

	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
 //中断配置
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	//采用DMA方式接收
//	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

void uart2_init(u32 bound){
  //GPIO端口设置

	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
	
	// 外设使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_DeInit(USART2);  //复位串口2 -> 可以没有
	
	// 初始化 串口对应IO口  TX-PA2  RX-PA3
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;
  GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	// 初始化 串口模式状态
	USART_InitStrue.USART_BaudRate=bound; // 波特率
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // 硬件流控制
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; // 发送 接收 模式都使用
	USART_InitStrue.USART_Parity=USART_Parity_No; // 没有奇偶校验
	USART_InitStrue.USART_StopBits=USART_StopBits_1; // 一位停止位
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; // 每次发送数据宽度为8位
	USART_Init(USART2,&USART_InitStrue);
	
	USART_Cmd(USART2,ENABLE);//使能串口
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启接收中断
	
	// 初始化 中断优先级
	NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrue);

}

int i = 0;
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
				mv_distance=USART_ReceiveData(USART1);  // 串口2 接收
			
			USART_ClearITPendingBit(USART1,USART_IT_RXNE );

		}
//	u8 Res;
//	uint32_t temp = 0;
//	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//	{
////		Res= USART_ReceiveData(USART1);  // 串口2 接收
////		openmv_buff[i]=Res;
////		i++;
////		if(i==4)
////			i=0;
//		DMA_ClearFlag( DMA1_FLAG_TC5 );  
//		temp = USART1->SR;
//		temp = USART1->DR;                  //清USART_IT_IDLE标志
//		DMA_Cmd(DMA1_Channel5,DISABLE);
//		if(openmv_buff[0]==0xAA&&openmv_buff[3]==0x55)
//		{
//			mv_distance = (int16_t)openmv_buff[1];
//			mv_theta = (int16_t)openmv_buff[2];
//		}
//		DMA_SetCurrDataCounter(DMA1_Channel5,USART_REC_LEN);
//		//打开DMA
//		DMA_Cmd(DMA1_Channel5,ENABLE);
//		//USART_ClearITPendingBit(USART1,USART_IT_RXNE );
//  } 
}


void USART2_IRQHandler(void) // 串口2中断服务函数
{
	char res=' ';
	if(USART_GetITStatus(USART2,USART_IT_RXNE)) // 中断标志
	{
		res= USART_ReceiveData(USART2);  // 串口2 接收
		if(res=='B')  //手机发出启动指令
		{
			USART_SendData(USART2,'B');   // 串口2 发送给手机 手机开始计时
			led=0;
			Init_Flag=1;
			res=' ';
		}
		else if(res=='S')
		{
			USART_SendData(USART2,'S');   // 串口2 发送给手机 手机停止计时
			time=0;
			led=1;
			Init_Flag=0;
			res=' ';
		}
	  USART_ClearITPendingBit(USART2,USART_IT_RXNE );
	}
}

#endif	
