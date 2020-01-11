// 
// 
// 

#include "IICLCD.h"

IICLCD::IICLCD(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows)
{
	lcd = new LiquidCrystal_I2C(lcd_addr, lcd_cols, lcd_rows);
	rows = lcd_rows;

	lcd->begin();
}


IICLCD::~IICLCD()
{
	if (lcd)
		delete lcd;
}

void IICLCD::printEnterPin()
{
	lcd->setCursor(0, 0);
	lcd->print(clearRow());

	lcd->setCursor(0, 0);
	lcd->print(F("Enter Pin:"));
}

void IICLCD::printPin(String pin, char display, bool hide)
{
	lcd->setCursor(0, 1);
	lcd->print(clearRow());

	lcd->setCursor(0, 1);
	if (hide == true)
	{
		String secret_pin = "";
		for (int x = 0; x < pin.length(); x++)
			secret_pin += String(display);
		lcd->print(secret_pin);
	}
	else
		lcd->print(pin);
}

void IICLCD::printIncorrectPin(uint8_t incorrect, uint8_t limit)
{
	lcd->setCursor(0, 2);
	lcd->print(clearRow());

	lcd->setCursor(0, 2);
	lcd->print(String(limit - incorrect) + "/" + String(limit) + " tries left.");
}

void IICLCD::printWaitForTimeout(uint8_t timeout)
{
	lcd->setCursor(0, 0);
	lcd->print(clearRow());

	lcd->setCursor(0, 0);
	lcd->print("Please wait " + String(timeout) + "min.  ");

	lcd->setCursor(0, 1);
	lcd->print(clearRow());

	lcd->setCursor(0, 1);
	lcd->print("to try again.       ");
}

void IICLCD::printCountDown(String value)
{
	lcd->setCursor(0, 2);
	lcd->print(clearRow());

	lcd->setCursor(0, 2);
	lcd->print("Remaining: " + value);
}

void IICLCD::printIsCorrect()
{
	lcd->setCursor(0, 2);
	lcd->print(clearRow());

	lcd->setCursor(0, 2);
	lcd->print("Pincode accepted.   ");

}

String IICLCD::clearRow()
{
	String clear = "";
	for (int x = 0; x < rows; x++)
		clear += " ";

	return clear;
}

void IICLCD::clearLCD()
{
	lcd->clear();
}

