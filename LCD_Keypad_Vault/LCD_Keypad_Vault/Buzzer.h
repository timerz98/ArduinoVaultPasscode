// Buzzer.h

#ifndef _BUZZER_h
#define _BUZZER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Buzzer
{
public:
	Buzzer(uint8_t pin, bool is_high);
	~Buzzer();

	void On();
	void Off();

private:
	uint8_t pin;

};

#endif

