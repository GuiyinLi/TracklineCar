#include "led.h"

//电机  : TIM3:PA^6 PA^7 PB^0 PB^1   TIM4:PB^6 PB^7 PB^8 PB^9
//OLED :SCL->PB11 SDA->PB10
//IMU  ： AO->PA^3 SDA->PA^4 SCL->PA^5
//红外对管：
//超声波：TX->PA9 RX->PA10

//PC13
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);	  //上电开灯
}

void WHEEL_GPIO_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PA,PD端口时钟
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;				  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);				
	GPIO_SetBits(GPIOA,GPIO_Pin_6|GPIO_Pin_7);							

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;				
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOB, &GPIO_InitStructure);					
  GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);						 

}

void MPU_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void TRAC_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 /*| GPIO_Pin_2*/;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}







