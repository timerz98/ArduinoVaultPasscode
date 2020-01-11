// 
// 
// 

#include "LockServo.h"


LockServo::LockServo(uint8_t servo_pin)
{
	lock = new Servo();
	lock->attach(9);
}

LockServo::~LockServo()
{
	if (lock)
		delete lock;
}

void LockServo::Open()
{
	lock->write(60);
}

void LockServo::Close()
{
	lock->write(0);
}