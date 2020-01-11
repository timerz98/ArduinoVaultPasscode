// IICLCD.h

#ifndef _IICLCD_h
#define _IICLCD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

class IICLCD
{
public:
	IICLCD(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
	~IICLCD();

	void printEnterPin();
	void printPin(String pin, char display, bool hide);
	void printIncorrectPin(uint8_t incorrect, uint8_t limit);
	void printWaitForTimeout(uint8_t timeout);
	void printCountDown(String value);
	void printIsCorrect();
	void clearLCD();

private:
	LiquidCrystal_I2C *lcd;
	uint8_t rows;

	String clearRow();
};
#endif

