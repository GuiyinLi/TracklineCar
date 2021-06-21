#include "pid.h"

extern int seesaw_flag;

PID CrossSeesaw_Pitch_Position,CrossSeesaw_Pitch_Speed;
PID CrossSeesaw_Yaw_Position,CrossSeesaw_Yaw_Speed;
PID Openmv_Position,Openmv_theta;
PID_ADD Openmv_Increment;
PID_CONTROL_MODE pid_control_mode=NONE;
void PID_Init(void)    //定时器的发送周期不能过小
{
	//底盘电机  3.9 0.06 125     ||   3.5 0 2    ||3.2 0.015 2.1    //3.5 0 2
	CrossSeesaw_Pitch_Position.error_last=0;
	CrossSeesaw_Pitch_Position.error_now=0;
	CrossSeesaw_Pitch_Position.error_inter=0;
	CrossSeesaw_Pitch_Position.Kp=200;      
	CrossSeesaw_Pitch_Position.Ki=0;  
	CrossSeesaw_Pitch_Position.Kd=90;
	
	CrossSeesaw_Pitch_Speed.error_last=0;
	CrossSeesaw_Pitch_Speed.error_now=0;
	CrossSeesaw_Pitch_Speed.error_inter=0;
	CrossSeesaw_Pitch_Speed.Kp=10000;      
	CrossSeesaw_Pitch_Speed.Ki=0;  
	CrossSeesaw_Pitch_Speed.Kd=10;
	
	
	CrossSeesaw_Yaw_Position.error_last=0;
	CrossSeesaw_Yaw_Position.error_now=0;
	CrossSeesaw_Yaw_Position.error_inter=0;
	CrossSeesaw_Yaw_Position.Kp=5;      
	CrossSeesaw_Yaw_Position.Ki=0.01;  
	CrossSeesaw_Yaw_Position.Kd=1;
	
	CrossSeesaw_Yaw_Speed.error_last=0;
	CrossSeesaw_Yaw_Speed.error_now=0;
	CrossSeesaw_Yaw_Speed.error_inter=0;
	CrossSeesaw_Yaw_Speed.Kp=20;      
	CrossSeesaw_Yaw_Speed.Ki=0.01;  
	CrossSeesaw_Yaw_Speed.Kd=2;
	
/*******************OPENMV寻迹*************************/
if(seesaw_flag)
{
	Openmv_Position.error_last=0; //利用距离中线的距离 4000稳 85 0 30  /110 0 60
	Openmv_Position.error_now=0;
	Openmv_Position.error_inter=0;
	Openmv_Position.Kp=300;      
	Openmv_Position.Ki=0;  
	Openmv_Position.Kd=60;
}
else
{
	Openmv_Position.error_last=0; //利用距离中线的距离 4000稳 85 0 30  /110 0 60
	Openmv_Position.error_now=0;
	Openmv_Position.error_inter=0;
	Openmv_Position.Kp=90;      
	Openmv_Position.Ki=0;  
	Openmv_Position.Kd=60;
}
//	Openmv_theta.error_last=0;  //利用黑线弯曲的角度
//	Openmv_theta.error_now=0;
//	Openmv_theta.error_inter=0;
//	Openmv_theta.Kp=6.5;      
//	Openmv_theta.Ki=0;  
//	Openmv_theta.Kd=1.5;
	
}
void PID_Control(float current, float expected,PID* motor_type)  //&motor_type
{
	motor_type->error_last=motor_type->error_now;
	motor_type->error_now=expected-current;
	motor_type->error_inter+=motor_type->error_now;
	
	if(motor_type->error_inter>10000)            //积分超限
		  motor_type->error_inter = 10000;
	if(motor_type->error_inter<-10000)
		  motor_type->error_inter = -10000;
	
	motor_type->pid_out=motor_type->Kp*motor_type->error_now+motor_type->Ki*motor_type->error_inter+ \
	                    motor_type->Kd*(motor_type->error_now-motor_type->error_last);
	
}
float PID_Increment(float current,float expect,PID_ADD* motor)//功率环控制的PID代码  增量式PID
{
	   motor->error_now=expect-current;
	   motor->increament=motor->Kp*(motor->error_now-motor->error_next)+motor->Ki*(motor->error_now)+
	                          motor->Kd*(motor->error_now-2*motor->error_next+motor->error_last);
     motor->error_last=motor->error_next;
	   motor->error_next=motor->error_now;
	   current+=motor->increament;
	   return current;
}

//PID控制方式检测
void PID_Control_Status_Check(void)
{
}


