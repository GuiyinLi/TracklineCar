#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "usart.h"
#include "oled.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"   
#include "public.h"
#include "stm32f10x.h"
#include "math.h"
#include "timer.h"
#include "pid.h"
#include "timer.h"
//SCL A5引脚//SDA A4引脚//ADO A3引脚
//四路PWM输出，属于TIM3的四个通道 其中A6A7为电机PWM输出B0B1为矢量舵机输出

u8 left_flag=0;
u8 right_flag=0;
u8 middle_flag=0;
unsigned char speed_ge,speed_shi,speed_bai;           //速度个十百
unsigned char distance_ge,distance_shi,distance_bai;  //路程个十百  
int count=0;
int flag=0;
extern int seesaw_flag,stop_time;

int main(void)
{	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	LED_Init();
  WHEEL_GPIO_Init();
  MPU_GPIO_Init();
	TRAC_GPIO_Init();
	OLED_GPIO_Init();
	
	delay_init();
	uart1_init(115200);
	uart2_init(9600);    //单片机 手机通信
	OLED_Init();
	OLED_P8x16Str(30,0,"Init...");  
	TIM3_PWM_Init();
	TIM4_PWM_Init();
	TIM2_Int_Init(9,7199); //周期为1ms
	
	MPU_Init();
	PID_Init();
	

	while(mpu_dmp_init());
	delay_ms(1000);
	
	mpu_dmp_get_data(&pitch,&roll,&yaw,&x_gyro,&y_gyro,&z_gyro);
	pitch_offset=pitch;
	yaw_offset=yaw;
	yaw_switch=1;
	OLED_Display_Init();
	led=1;                //标志外设初始化完成
	while(!Init_Flag){};  //等待手机发送启动指令
//  Init_Flag=1;
  while(1)
	{	
		if(mpu_dmp_get_data(&pitch,&roll,&yaw,&x_gyro,&y_gyro,&z_gyro)==0&&Init_Flag)	 //初始化完成且获取到陀螺仪数据
		{
			left_flag= PAin(0);
			right_flag= PAin(2);
			middle_flag= PAin(1);
			OLED_Display();			
			if((pitch>5||pitch<-5)&&stop_time<=205)   //判断是否过跷跷板
			{
				seesaw_flag=1;
			}
			if(RIGHT==BLACK&&LEFT==WHITE&&flag==0)
			{
				flag=1;
			}
			if(flag==1)
			{ 
				if(RIGHT==WHITE) 
					 {
						 flag=0;
						 Init_Flag=1;
					 }
				else if(RIGHT==BLACK&&LEFT==BLACK&&seesaw_flag==0)
					 {
						flag=2;
						Init_Flag=0;
						time=0;
						USART_SendData(USART2,'S');   // 串口2 发送给手机 手机停止计时
						led=1;
						set_motor_forward_back_speed(0,0);	
					 }			
			}				
		}		
	}
}
