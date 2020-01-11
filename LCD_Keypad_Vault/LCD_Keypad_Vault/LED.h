// LED.h

#ifndef _LED_h
#define _LED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class LED
{
public:
	LED(uint8_t pin, bool is_high);
	~LED();

	void On();
	void Off();

private:
	uint8_t pin;

};

#endif

