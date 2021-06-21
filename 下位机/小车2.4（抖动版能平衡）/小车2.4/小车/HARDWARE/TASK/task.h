#ifndef __task_H
#define __task_H

#include <stm32f10x.h>
#include "sys.h"
#include "delay.h"
#include "pwm.h"
#include "led.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "oled.h"

extern float yaw,pitch,roll;
extern int	speed1,speed2;
extern double dis_x,dis_y;
extern int p,q,mode;
extern double length,distance;

void stop(void);
void right1(void);
void straight1(void);
void task1(void);
void task2(void);
void task3(void);
void task4(void);
//void task5(void);
void task6(void);
void task7(void);

void right3(void);

void line1(void);
void line2(void);
void line3(void);
void line4(void);
void line5(void);
void line6(void);

void Arc1(void);
void Arc2(void);
void Arc3(void);
void Arc4(void);
void Arc5(void);

void task7_line2(void);






#endif 

