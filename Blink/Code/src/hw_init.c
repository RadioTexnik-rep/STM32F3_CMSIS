//----------------------------------------------------/
//                    File: hw_init.c
//----------------------------------------------------/

//----------------------------------------------------/
//                    Includes
//----------------------------------------------------/
#include "hw_init.h"


//----------------------------------------------------/
//                    Init GPIO
//----------------------------------------------------/
void vInitGPIO(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN; //�������� ������������ ����� B
    //PB9 -> USER LED
    GPIOB->MODER |= GPIO_MODER_MODER9_0; //�� �����
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR9_0; //�������� � 1
    //PB8 -> USER BUTTON
    GPIOB->MODER &= ~GPIO_MODER_MODER8; //pin8 �� ����
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR8_0; //�������� � 1
    //PB15 -> clock test
    GPIOB->MODER |= GPIO_MODER_MODER15_0; //�� �����
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR15_0; //�������� � 1
    //PA8 -> MCO(AF) -> SYSCLK
    GPIOA->MODER |= GPIO_MODER_MODER8_1; //AF
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_8; //PP
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR8_0; //PP+PU
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8; //High speed
    GPIOA->AFR[1] = 0; //AF0->MCO
    RCC->CFGR |= RCC_CFGR_MCO_2; //SYSCLK
}

//----------------------------------------------------/
//                    Init RCC
//----------------------------------------------------/
int iInitClock(void)
{
    __IO int StartUpCounter;

    //запускаем кварцевый генератор
    RCC->CR |= RCC_CR_HSEON;
    //ждём успешного запуска или окончания таймаута
    for (StartUpCounter=0; ; StartUpCounter++)
    {
        //если успешно запустилось, то выходим из цикла
        if (RCC->CR & RCC_CR_HSERDY)
            break;
        //если не запустилось,то всё отключаем и возвращаем ошибку
        if (StartUpCounter > 0x1000)
        {
            RCC->CR &= ~RCC_CR_HSEON;
            return 1;
        }
    }

    //запуск и настройка PLL
    RCC->CFGR |= RCC_CFGR_PLLMUL9 | RCC_CFGR_PLLSRC_HSE_PREDIV; //x9, HSE source
    RCC->CR |= RCC_CR_PLLON;    //start PLL
    for (StartUpCounter=0; ; StartUpCounter++)
    {
        //если успешно запустилось, то выходим из цикла
        if (RCC->CR & RCC_CR_PLLRDY)
            break;
         //если не запустилось,то всё отключаем и возвращаем ошибку
        if (StartUpCounter > 0x1000)
        {
            RCC->CR &= ~RCC_CR_HSEON;   //отключаем внешнее тактирование
            RCC->CR &= ~RCC_CR_PLLON;   //отключаем PLL
            return 2;
        } 
    }

    //настройка делителей и FLASH
    //устанавливаем 2 цикла ожидания FLASH, т.к. 48 MHz < SYSCLK <= 72 MHz
    FLASH->ACR |= FLASH_ACR_LATENCY_1;

    //делители
    RCC->CFGR &= ~RCC_CFGR_PPRE2; //APB2 Pre = 1 
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; //APB1 Pre = 2 
    RCC->CFGR &= ~RCC_CFGR_HPRE; //AHB = 1
    //переключаемся на внешнее тактирование
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    //ждём пока переключится
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)){ }
    //отключаем внутр. RC-генератор
    RCC->CR &= ~RCC_CR_HSION;
    return 0;
    
}


