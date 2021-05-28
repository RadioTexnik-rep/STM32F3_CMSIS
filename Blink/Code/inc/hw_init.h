//----------------------------------------------------/
//                    File: hw_init.h
//----------------------------------------------------/
#ifndef __HW_INIT_H
#define __HW_INIT_H
//----------------------------------------------------/
//                    Includes
//----------------------------------------------------/
#include "stm32f3xx.h"

//----------------------------------------------------/
//                    Defines
//----------------------------------------------------/
#define LED_OFF GPIOB->BSRR |= GPIO_BSRR_BS_9
#define LED_ON GPIOB->BSRR |= GPIO_BSRR_BR_9

#define PB15_SET GPIOB->BSRR |= GPIO_BSRR_BS_9
#define PB15_RESET GPIOB->BSRR |= GPIO_BSRR_BR_9
#define PB15_TOGGLE GPIOB->ODR ^= GPIO_ODR_15

#define BUTTON_STATE (GPIOB->IDR)&GPIO_IDR_8
//----------------------------------------------------/
//                Prototype functions
//----------------------------------------------------/
void vInitGPIO(void);
int iInitClock(void);

#endif
