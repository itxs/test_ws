#include "button.h"

Button::Button(GPIO_TypeDef* port, uint8_t pin, void(*action)(),
	uint8_t debounceTimeMs, bool onRelease)
	: _port(port),
	_pin(1 << ((uint16_t)pin)),
	btAction(action),
	debounceTime(debounceTimeMs),
	onReleaseActive(onRelease)
{
}
	
void Button::interruptHandler(bool pressed)
{
	CLEAR_BIT(EXTI->IMR, _pin); // disable interrupt
	if(pressed ^ onReleaseActive)
		performAction = true;
}

void Button::timer()
{
	if (!(EXTI->IMR & _pin))
	{
		if (debounceCnt < debounceTime)
			debounceCnt++;
		else
		{
			debounceCnt = 0;
			SET_BIT(EXTI->IMR, _pin);  // enable interrupt after button bounce
		}
	}
	if (performAction && btAction)
	{
		performAction = false;
		btAction();
	}
}
