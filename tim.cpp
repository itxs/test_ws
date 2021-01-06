#include "gpio.h"

TIM_HandleTypeDef htim3;

void TIM_Init()
{
	__TIM3_CLK_ENABLE();
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.Period = 999;
	htim3.Init.Prescaler = 1;
	htim3.Init.RepetitionCounter = 0;
	htim3.Instance = TIM3;
	HAL_TIM_OC_Init(&htim3);
	
	TIM_OC_InitTypeDef ocInit;
	ocInit.OCFastMode = TIM_OCFAST_DISABLE;
	ocInit.OCIdleState = TIM_OCIDLESTATE_RESET;
	ocInit.OCMode = TIM_OCMODE_PWM1;
	ocInit.OCPolarity = TIM_OCPOLARITY_HIGH;
	ocInit.Pulse = 50;
	HAL_TIM_OC_ConfigChannel(&htim3, &ocInit, TIM_CHANNEL_1);
	HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_1);
}
