#ifndef __matrix_H
#define __matrix_H

#include <stm32f10x.h>
#include "sys.h"
#include "delay.h"

#define  bootOUT  (GPIO_Pin_8|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_15)
#define  bootIN		(GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12)

#define K_4  PBin(15)
#define K_5  PBin(14)
#define K_6  PBin(13)
#define K_7  PBin(12)

#define KEY_1  1
#define KEY_2  2
#define KEY_3  3
#define KEY_4  4
#define KEY_5  5
#define KEY_6  6
#define KEY_7  7
#define KEY_8  8
#define KEY_9  9
#define KEY_10  10
#define KEY_11  11
#define KEY_12  12
#define KEY_13  13
#define KEY_14  14
#define KEY_15  15
#define KEY_16  16

#define SetK_0 PAout(15)
#define SetK_1 PAout(12)
#define SetK_2 PAout(11)
#define SetK_3 PAout(8)

#define RestA GPIO_ResetBits(GPIOA,bootOUT);
//#define RestB GPIO_ResetBits(GPIOB,GPIO_Pin_3);

void Matrix_init(void);
u8 KEY_Scan (u8 mode);


#endif 

