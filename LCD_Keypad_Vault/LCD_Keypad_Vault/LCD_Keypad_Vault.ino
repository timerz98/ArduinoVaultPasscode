/*
 Name:		LCD_Keypad_Vault.ino
 Created:	1/9/2020 8:26:23 PM
 Author:	CrisantoIII
*/

// the setup function runs once when you press reset or power the board
#include "LockServo.h"
#include "LED.h"
#include "Buzzer.h"
#include "IICLCD.h"

#include <Countimer.h>
Countimer timer;

//For keypad config
#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
	{ '1','2','3' },
	{ '4','5','6' },
	{ '7','8','9' },
	{ '*','0','#' }
};

#define R1					41
#define R2					43
#define R3					45
#define R4					47
#define C1					49
#define C2					51
#define C3					53

byte rowPins[ROWS] = { R1, R2, R3, R4 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = { C1, C2, C3 }; //connect to the column pinouts of the keypad

Keypad keypad = Keypad((char*)(keys), rowPins, colPins, ROWS, COLS);
//End of keypad config

//User define variable
String PinCode = "1234";
String EnteredPin = "";
uint8_t WrongPinCnt = 0;
//End of user variable

#define TIMEOUT_RANGE		1 //1 - 59

#define LCD_ADDRESS			0x3F
#define LCD_COLS			20
#define LCD_ROWS			4
#define LCD_PIN_CHAR		'*'
#define LCD_PIN_ISHIDE		true

#define SERVO_PIN			9

#define GREEN_PIN			8
#define RED_PIN				7

#define BUZZER_PIN			6

#define WRONG_PIN_LIMIT		3

LockServo *lock_servo;
LED *led_green;
LED *led_red;
Buzzer *buzzer;
IICLCD *lcd;

enum SAFE
{
	S_INITIALIZE,
	S_WAITING_INPUT,
	S_INCORRECT,
	S_CORRECT,
	S_TIMEOUT
};

SAFE safe_process = SAFE::S_INITIALIZE;

void setup() {
	Serial.begin(9600);
	lcd = new IICLCD(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

	led_green = new LED(GREEN_PIN, false);
	led_red = new LED(RED_PIN, false);

	buzzer = new Buzzer(BUZZER_PIN, true);

	lock_servo = new LockServo(SERVO_PIN);

	timer.setCounter(0, TIMEOUT_RANGE, 0, timer.COUNT_DOWN, NULL);
	timer.setInterval(refreshClock, 1000);
}

// the loop function runs over and over again until power down or reset
void loop() {

	switch (safe_process)
	{
	case S_INITIALIZE:
		EnteredPin = "";
		lcd->clearLCD();
		lcd->printEnterPin();
		led_green->Off();
		led_red->Off();
		lock_servo->Close();
		buzzer->Off();
		safe_process = SAFE::S_WAITING_INPUT;
		break;
			
	case S_WAITING_INPUT:
		enterPin();
		break;

	case S_INCORRECT:
		WrongPinCnt += 1;

		lcd->printIncorrectPin(WrongPinCnt, WRONG_PIN_LIMIT);
		led_green->Off();
		led_red->On();
		lock_servo->Close();
		buzzer->On();
		delay(3000);
		buzzer->Off();
		led_red->Off();

		lcd->clearLCD();
		lcd->printEnterPin();

		if (WrongPinCnt >= WRONG_PIN_LIMIT)
		{
			WrongPinCnt = 0;
			safe_process = SAFE::S_TIMEOUT;
		}
		else
			safe_process = SAFE::S_WAITING_INPUT;		
			
		break;

	case S_CORRECT:
		lcd->printIsCorrect();
		led_green->On();
		led_red->Off();
		lock_servo->Open();
		buzzer->Off();
		delay(5000);

		lcd->clearLCD();
		lcd->printEnterPin();
		safe_process = SAFE::S_INITIALIZE;

		break;

	case S_TIMEOUT:
		lcd->printWaitForTimeout(TIMEOUT_RANGE);
		while (1)
		{
			timer.run();

			if (!timer.isCounterCompleted()) {
				timer.start();
			}
			else
				break;
		}
		timer.restart();
		safe_process = SAFE::S_INITIALIZE;
		break;
	}

}

void enterPin()
{
	char key = keypad.getKey();

	if (key)
	{
		if (key == '*')
		{
			EnteredPin = "";
			lcd->printPin(EnteredPin, LCD_PIN_CHAR, LCD_PIN_ISHIDE);
		}
		else if (key == '#')
		{
			if (EnteredPin.compareTo(PinCode) == 0)
				safe_process = SAFE::S_CORRECT;
			else
				safe_process = SAFE::S_INCORRECT;	
			EnteredPin = "";
		}
		else
		{
			EnteredPin += String(key);
			lcd->printPin(EnteredPin, LCD_PIN_CHAR, LCD_PIN_ISHIDE);
		}
	}
}

void refreshClock() {
	lcd->printCountDown(timer.getCurrentTime());
}
