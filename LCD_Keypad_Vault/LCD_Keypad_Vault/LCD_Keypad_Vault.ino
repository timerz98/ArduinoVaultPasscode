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

#include "NewPing.h"
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Creating the NewPing Object.
unsigned long initial_distance = 0;

#include <Bounce2.h>
#define OPEN_BUTTON			5
Bounce open_button = Bounce(); // Instantiate a Bounce object

#include <Countimer.h>
Countimer timer;

#include <Keypad.h>
//Select only one keypad type
#define KEYPAD_4x4
//#define KEYPAD_3x4 

#ifdef KEYPAD_3x4
//For keypad config
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
#endif // KEYPAD_3x4

#ifdef KEYPAD_4x4
//For keypad config
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
					 //define the cymbols on the buttons of the keypads
char keys[ROWS][COLS] = {
	{ '1','2','3','A' },
	{ '4','5','6','B' },
	{ '7','8','9','C' },
	{ '*','0','#','D' }
};

#define R1					39
#define R2					41
#define R3					43
#define R4					45
#define C1					47
#define C2					49
#define C3					51
#define C4					53

byte rowPins[ROWS] = { R1, R2, R3, R4 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = { C1, C2, C3, C4 }; //connect to the column pinouts of the keypad
#endif // KEYPAD_4x4


Keypad keypad = Keypad((char*)(keys), rowPins, colPins, ROWS, COLS);
//End of keypad config

//User define variable
String PinCode = "1234";
String EnteredPin = "";
uint8_t WrongPinCnt = 0;
//End of user variable

#define TIMEOUT_RANGE		1 //1 - 59

//Select only one LCD size
//#define LCD_4x20
#define LCD_2x16

#ifdef LCD_4x20
#define LCD_ADDRESS			0x27
#define LCD_COLS			20
#define LCD_ROWS			4
#define LCD_PIN_CHAR		'*'
#define LCD_PIN_ISHIDE		true
#endif // LCD_4x20


#ifdef LCD_2x16
#define LCD_ADDRESS			0x27
#define LCD_COLS			16
#define LCD_ROWS			2
#define LCD_PIN_CHAR		'*'
#define LCD_PIN_ISHIDE		true
#endif // LCD_2x16


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
	
	open_button.attach(OPEN_BUTTON,INPUT_PULLUP); // Attach the open_button to a pin with INPUT_PULLUP mode
	open_button.interval(25); // Use a debounce interval of 25 milliseconds

	initial_distance = sonar.ping_in();
	delay(1000);
  
}

// the loop function runs over and over again until power down or reset
void loop() {

	open_button.update(); // Update the Bounce instance
   
	if ( open_button.fell() ) {
		lock_servo->Open();
		delay(3000);
		lock_servo->Close();
	}

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

		while (true)
		{
			unsigned long value = sonar.ping_cm();

			if (value >= initial_distance || value == 0)
				break;
		}


		delay(5000);

		lcd->clearLCD();
		lcd->printEnterPin();
		safe_process = SAFE::S_INITIALIZE;

		break;

	case S_TIMEOUT:
		lcd->printWaitForTimeout(TIMEOUT_RANGE);
		led_red->On();
		delay(3000);
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
			lcd->clearLCD();
			lcd->printEnterPin();
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
