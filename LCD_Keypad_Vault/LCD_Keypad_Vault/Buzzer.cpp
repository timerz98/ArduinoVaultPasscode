// 
// 
// 

#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin, bool is_high)
{
	this->pin = pin;

	pinMode(pin, OUTPUT);
	
	if (is_high)
		On();
	else
		Off();
		
}

Buzzer::~Buzzer()
{

}

void Buzzer::On()
{
	//digitalWrite(pin, HIGH);
	tone(pin, 1000);
}

void Buzzer::Off()
{
	//digitalWrite(pin, LOW);
	noTone(pin);
}
