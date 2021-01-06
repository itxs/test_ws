#include <stm32f0xx_hal.h>
#include <stm32_hal_legacy.h>

#include "button.h"
#include "gpio.h"
#include "tim.h"

enum LedMode
{
	MODE1 = 0,
	MODE2,
	MODE3
};

enum LedState
{
	RAMP_UP = 0,
	ACTIVE,
	RAMP_DOWN
};

struct LedSettings
{
	uint32_t pwmMaxValue = 1000;
	uint32_t LedRampUpTime = 100;
	uint32_t LedActiveTime = 1000;
	uint32_t LedRampDownTime = 100;
};

LedMode ledMode = MODE1;
LedState ledState = RAMP_UP;
LedSettings settings[3] = {
	{ 1000, 100, 1000, 100},
	{ 200, 100, 1000, 100},
	{ 1000, 1200, 100, 5000}};
LedSettings* activeSettings = &settings[0];

volatile uint32_t& pwmRegister = TIM3->CCR1;

void UserButtonAction()
{
	if (ledMode == MODE1)
		ledMode = MODE2;
	else if (ledMode == MODE2)
		ledMode = MODE3;
	else if (ledMode == MODE3)
		ledMode = MODE1;
}

void LedControl()
{
	static uint32_t timeCounter = 0;
	static float pwmValue = 0;
	
	switch (ledState)
	{
	case RAMP_UP:
		if (timeCounter < activeSettings->LedRampUpTime)
			pwmValue += (float)activeSettings->pwmMaxValue / (float)activeSettings->LedRampUpTime;
		else
		{
			timeCounter = 0;
			pwmValue = activeSettings->pwmMaxValue;
			ledState = ACTIVE;
		}
		break;
	case ACTIVE:
		if (timeCounter >= activeSettings->LedActiveTime)
		{
			timeCounter = 0;
			ledState = RAMP_DOWN;
		}
		break;
	case RAMP_DOWN:
		if (timeCounter < activeSettings->LedRampDownTime)
			pwmValue -= (float)activeSettings->pwmMaxValue / (float)activeSettings->LedRampDownTime;
		else
		{
			timeCounter = 0;
			pwmValue = 0;
			ledState = RAMP_UP;
			activeSettings = &settings[(uint8_t)ledMode];
		}
		break;
	default:
		break;
	}
	timeCounter++;
	pwmRegister = pwmValue;
}

Button userButton(GPIOA, 0, UserButtonAction);
extern "C" void SysTick_Handler(void)
{
	userButton.timer();
	LedControl();
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

extern "C" void EXTI0_1_IRQHandler(void)
{
	if (EXTI->PR & GPIO_PIN_0)
	{
		bool pinState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET;
		userButton.interruptHandler(pinState);
		EXTI->PR = GPIO_PIN_0;
	}
}

int main(void)
{
	HAL_Init();
	GPIO_Init();
	TIM_Init();
	
	for (;;)
	{
	}
}
