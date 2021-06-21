#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define led     PCout(13)

#define RIGHT   PAin(1)
#define LEFT    PAin(0)
//#define MIDDLE  PAin(2)

void LED_Init(void);
void WHEEL_GPIO_Init(void);
void MPU_GPIO_Init(void);
void TRAC_GPIO_Init(void);

#endif
