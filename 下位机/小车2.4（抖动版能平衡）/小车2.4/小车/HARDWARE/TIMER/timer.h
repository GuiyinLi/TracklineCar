#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
extern u32 js_tim2;
extern u8 Init_Flag;
extern u8 ban_flag;
extern float pitch_offset;
extern float yaw_offset;
extern u32 time;
void TIM2_IRQHandler(void);
void TIM2_Int_Init(u16 arr,u16 psc);   
#endif
