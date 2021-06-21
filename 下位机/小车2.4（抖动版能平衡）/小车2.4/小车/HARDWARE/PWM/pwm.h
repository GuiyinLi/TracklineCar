#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

#define BLACK 1      //下方是黑色胶带
#define WHITE 0      //下方是地板

#define FORWARD             LEFT==WHITE&&MIDDLE==BLACK&&RIGHT==WHITE||(LEFT==WHITE&&MIDDLE==WHITE&&RIGHT==WHITE)     //前进
#define LEFT_BIGGER_TURN    LEFT==BLACK&&MIDDLE==BLACK&&RIGHT==WHITE      //左小转
#define LEFT_LITTLE_TURN    LEFT==BLACK&&MIDDLE==WHITE&&RIGHT==WHITE      //左大转
#define RIGHT_BIGGER_TURN   LEFT==WHITE&&MIDDLE==BLACK&&RIGHT==BLACK      //右小转
#define RIGHT_LITTLE_TURN   LEFT==WHITE&&MIDDLE==WHITE&&RIGHT==BLACK      //右大转
#define STOP                LEFT==BLACK&&MIDDLE==BLACK&&RIGHT==BLACK      //停止

#define MAX_SPEED                 4000       //最大速度   10000       
#define SLOW_SPEED                1000       //最大速度   10000   
#define FORWARD_SPEED             2000       //前进速度
#define LITTLE_TURN_SPEED         6000       //小转速度
#define BIGGER_TURN_SPEED         8000       //大转速度

typedef enum
{
	READY=0,
	NORMAL=1,
	SPECIAL=2,
}WALK_STATUS;
void TIM3_PWM_Init(void);
void TIM4_PWM_Init(void);
void set_motor_forward_back_speed(int lspeed,int rspeed);
void set_motor_left_right_speed(int lspeed,int rspeed);
void choose_fbspeed(int fspeed,int bspeed);
void set_speed(int speed1,int speed2,int speed3,int speed4);
void TRAC_Control(void);
extern float pitch,roll,yaw,x_gyro,y_gyro,z_gyro;  
extern u8 yaw_switch;
extern WALK_STATUS walk_status;
#endif
