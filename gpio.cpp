#include "gpio.h"

void GPIO_Init()
{
	__GPIOC_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.Alternate = GPIO_AF0_TIM3;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStructure.Pull = GPIO_NOPULL; // pulldown is on the PCB
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_EnableIRQ(EXTI0_1_IRQn);
}
