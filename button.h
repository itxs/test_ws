#pragma once

#include <stm32f0xx_hal.h>

class Button
{
public:
	Button(GPIO_TypeDef* port,
		uint8_t pin,
		void(*action)(),
		uint8_t debounceTimeMs = 5,
		bool onRelease = false);
	void interruptHandler(bool pressed);
	void timer();
	
private:
	GPIO_TypeDef* _port;
	uint16_t _pin;
	uint8_t debounceCnt = 0;
	uint8_t debounceTime = 0;
	bool onReleaseActive = false;
	void(*btAction)() = 0;
	bool performAction = false;
};


