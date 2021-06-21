#include "matrix.h"

void Matrix_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=bootOUT;  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  
	GPIO_Init(GPIOA,&GPIO_InitStructure);

   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=bootIN;  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	RestA;

}

u8 KEY_Scan (u8 mode)
{
	static u8 key = 1;
	if(key==1&&(K_4==0||K_5==0||K_6==0))//||K_7==0
		{
			delay_ms(15);
			key=0;
			if(K_4==0){
				SetK_0 = 1;
				SetK_1 = 1;
				SetK_2 = 1;
				SetK_3 = 0;
				if(K_4==0) return KEY_1;				
				SetK_0 = 1;
				SetK_1 = 1;
				SetK_2 = 0;
				SetK_3 = 1;
				if(K_4==0) return KEY_2;			
				SetK_0 = 1;
				SetK_1 = 0;
				SetK_2 = 1;
				SetK_3 = 1;
				if(K_4==0) return KEY_3;				
				SetK_0 = 0;
				SetK_1 = 1;
				SetK_2 = 1;
				SetK_3 = 1;
				if(K_4==0) return KEY_4;				
			}
			
			if(K_5==0){
				SetK_0 = 1;
				SetK_1 = 1;
				SetK_2 = 1;
				SetK_3 = 0;
				if(K_5==0) return KEY_5;
				SetK_0 = 1;
				SetK_1 = 1;
				SetK_2 = 0;
				SetK_3 = 1;
				if(K_5==0) return KEY_6;
				SetK_0 = 1;
				SetK_1 = 0;
				SetK_2 = 1;
				SetK_3 = 1;
				if(K_5==0) return KEY_7;
				SetK_0 = 0;
				SetK_1 = 1;
				SetK_2 = 1;
				SetK_3 = 1;
				if(K_5==0) return KEY_8;
			}
			
			if(K_6==0){
				SetK_0 = 1;
				SetK_1 = 1;
				SetK_2 = 1;
				SetK_3 = 0;
				if(K_6==0) return KEY_9;
				SetK_0 = 1;
				SetK_1 = 1;
				SetK_2 = 0;
				SetK_3 = 1;
				if(K_6==0) return KEY_10;
				SetK_0 = 1;
				SetK_1 = 0;
				SetK_2 = 1;
				SetK_3 = 1;
				if(K_6==0) return KEY_11;
				SetK_0 = 0;
				SetK_1 = 1;
				SetK_2 = 1;
				SetK_3 = 1;
				if(K_6==0) return KEY_12;
			}
			
//			if(K_7==0){
//				SetK_0 = 1;
//				SetK_1 = 1;
//				SetK_2 = 1;
//				SetK_3 = 0;
//				if(K_7==0) return KEY_13;
//				SetK_0 = 1;
//				SetK_1 = 1;
//				SetK_2 = 0;
//				SetK_3 = 1;
//				if(K_7==0) return KEY_14;
//				SetK_0 = 1;
//				SetK_1 = 0;
//				SetK_2 = 1;
//				SetK_3 = 1;
//				if(K_7==0) return KEY_15;
//				SetK_0 = 0;
//				SetK_1 = 1;
//				SetK_2 = 1;
//				SetK_3 = 1;
//				if(K_7==0) return KEY_16;
//			}
		}else if(K_4==1&&K_5==1&&K_6==1) key=1;	//&&K_7==1	
		if(mode == 1) key = 1;
		return 0;

}
