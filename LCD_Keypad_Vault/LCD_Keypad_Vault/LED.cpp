// 
// 
// 

#include "LED.h"


LED::LED(uint8_t pin, bool is_high)
{
	this->pin = pin;

	pinMode(pin, OUTPUT);

	if (is_high)
		digitalWrite(pin, HIGH);
	else
		digitalWrite(pin, LOW);
}

LED::~LED()
{

}

void LED::On()
{
	digitalWrite(pin, HIGH);
}

void LED::Off()
{
	digitalWrite(pin, LOW);
}