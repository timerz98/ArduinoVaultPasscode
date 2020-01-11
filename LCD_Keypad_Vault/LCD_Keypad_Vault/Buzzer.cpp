// 
// 
// 

#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin, bool is_high)
{
	this->pin = pin;

	pinMode(pin, OUTPUT);
	
	if (is_high)
		digitalWrite(pin, HIGH);
	else
		digitalWrite(pin, LOW);
}

Buzzer::~Buzzer()
{

}

void Buzzer::On()
{
	digitalWrite(pin, HIGH);
}

void Buzzer::Off()
{
	digitalWrite(pin, LOW);
}
