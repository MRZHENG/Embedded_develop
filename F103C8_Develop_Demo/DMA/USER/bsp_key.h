#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f10x.h"
 /*******
 *�������±���
 KEY_ON 0
 KEY_OFF 1
 ********/
#define KEY_ON	0
#define KEY_OFF	1

void Key_GPIO_Config(void);
u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin);


#endif /*__BSP_KEY_H */
