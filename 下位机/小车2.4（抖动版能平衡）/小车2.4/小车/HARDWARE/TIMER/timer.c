#include "timer.h"
#include "led.h"
#include "usart.h"
#include "sys.h"
#include "pid.h"
#include "pwm.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "math.h"
u32 time=0;
u32 js_tim2;
u8 Init_Flag=0;
u32 pitch_count=0;
float pitch_offset=0;
float yaw_offset=0;
extern float mv_distance,mv_theta;
#define FOWORD_SPEED  4000
int foword_speed=0;
int seesaw_flag=0;
u8 ban_flag=0;
int ban_time=0;
int run_flag=0,stop_time=0,fb_flag=1;
int speed_offset=0;
float y_threshold=0;
int max_speed=0;
float seesaw_threshold=0;

#define VAL_LIMIT(val, min, max)\
if(val<=min)\
{\
	val = min;\
}\
else if(val>=max)\
{\
	val = max;\
}\


void TIM2_Int_Init(u16 arr,u16 psc)   //自动重装载和预分频系数
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);   //使能定时器2
	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;      //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;            //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;         //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);         //使能定时器2的更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;   //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;          //响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);                               //使能定时器2
}

void ShakeSet(int count_time,int period,int run_time,float pitch_threshold,int lock_speed)
{
	if(count_time%period>run_time||(pitch>-pitch_threshold&&pitch<pitch_threshold))  //10.2稳 	400 100 	9.5  			/500 80 4550最快
		{
			if(pitch>-7.5&&pitch<7.5)set_motor_forward_back_speed(0,0);	
			else set_motor_forward_back_speed(lock_speed,lock_speed);		  
			stop_time++;
			if(stop_time>205)
			seesaw_flag=0;
		}
		else if(count_time%period<=run_time&&!(pitch>-pitch_threshold&&pitch<pitch_threshold))
		{
			if(pitch>5)	set_speed(foword_speed,0,0,foword_speed);
			else if(pitch<-5)set_speed(foword_speed,0,0,foword_speed);
			stop_time=0;
		}
		else
			set_motor_forward_back_speed(lock_speed,lock_speed);	
}

void TIM2_IRQHandler(void)   //TIM2中断
{

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		  TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
			js_tim2++;
		
		if(pitch>5)fb_flag=0;		
		else if(pitch<-5)fb_flag=1;
		
		if(seesaw_flag)ban_time++;//上跷跷板冲刺
      if(Init_Flag)
			{
				time++;//比赛计时

				if(js_tim2%50==0)//跷跷板以及寻迹控制
				{
	
					if(seesaw_flag==0)
					{
							foword_speed=4100;
							PID_Control(mv_distance,44,&Openmv_Position);//利用OPENMV与中线间距  位置式
							VAL_LIMIT(Openmv_Position.pid_out,-foword_speed,foword_speed);

							if(mv_distance<14)
									set_motor_forward_back_speed(3000,-3000);	
							else
								set_motor_forward_back_speed(foword_speed-Openmv_Position.pid_out,foword_speed+Openmv_Position.pid_out);	//位置式
					}
					else if(seesaw_flag)
					{
							PID_Control(pitch,pitch_offset,&CrossSeesaw_Pitch_Position);//利用OPENMV与中线间距  位置式
							PID_Control(y_gyro,CrossSeesaw_Pitch_Position.pid_out,&CrossSeesaw_Pitch_Speed);//利用OPENMV与中线间距  位置式
							foword_speed=CrossSeesaw_Pitch_Speed.pid_out;
							VAL_LIMIT(foword_speed,-6600,6600);
							/****************************/
							PID_Control(mv_distance,32,&Openmv_Position);//利用OPENMV与中线间距  位置式
							VAL_LIMIT(Openmv_Position.pid_out,-4200,4200);

						if(ban_time>525)
						{
							y_threshold=9;
							max_speed=10000;
							seesaw_threshold=9.97;
							VAL_LIMIT(pitch,-10,10);
								if(fb_flag)//向前
								{
									if(y_gyro<y_threshold&&y_gyro>-y_threshold)
									{
									  ShakeSet(js_tim2,400,180,seesaw_threshold,1200);
									}
//									else if(y_gyro>=y_threshold)
//									{
//											speed_offset=y_gyro*max_speed;
//										  VAL_LIMIT(speed_offset,-4100,4100);
//											set_speed(-speed_offset,0,0,-speed_offset);
//									}
//									else
//									{
//											speed_offset=y_gyro*max_speed;
//										  VAL_LIMIT(speed_offset,-4100,4100);
//											set_speed(speed_offset,0,0,speed_offset);
//									}
									else
									{
											speed_offset=y_gyro*max_speed;
										  VAL_LIMIT(speed_offset,-4200,4200);
										
										  set_speed(-speed_offset,0,0,-speed_offset);
									}
										
								}
								else
								{
									if(y_gyro<y_threshold&&y_gyro>-y_threshold)
									{
										ShakeSet(js_tim2,400,180,seesaw_threshold,-1200);
									}									
//									else if(y_gyro>=y_threshold)
//									{
//											speed_offset=y_gyro*max_speed;
//										  VAL_LIMIT(speed_offset,-4100,4100);
//											set_speed(-speed_offset,0,0,-speed_offset);
//									}
//									else
//									{
//											speed_offset=y_gyro*max_speed;
//										  VAL_LIMIT(speed_offset,-4100,4100);
//											set_speed(speed_offset,0,0,speed_offset);
//									}
									else
									{
											speed_offset=y_gyro*max_speed;
										  VAL_LIMIT(speed_offset,-4100,4100);
										
										  set_speed(-speed_offset,0,0,-speed_offset);
									}
								}						
						}
						else
							set_motor_forward_back_speed(foword_speed-Openmv_Position.pid_out,foword_speed+Openmv_Position.pid_out);	//位置式		
					}
				}
	}
			else
     		set_motor_forward_back_speed(0,0);	
			
	}
}


