// LockServo.h

#ifndef _LOCKSERVO_h
#define _LOCKSERVO_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Servo.h>

class LockServo
{
public:
	LockServo(uint8_t servo_pin);
	~LockServo();

	void Open();
	void Close();

private:
	Servo *lock;
};


#endif

