/*******************************************************************
*                          boot-splash.h                           *
* This file effectively loads the splash, then initialises the OS. *
*    It is used to interpret executeables, kernel modules etc.     *
*******************************************************************/
#include <LiquidCrystal.h> // We need to load that boot screen
#include "opcode-base.h"   // And be able to interpret the OS.

void boot() {
	const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
	LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
	
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	
	lcd.setCursor(0, 0);
	lcd.print("      UN16      ");
	lcd.setCursor(0, 1);
	lcd.print("   Booting...   ");
	delay(2500);
	
	if (!SD.begin(4)) {
		lcd.print("No SD card was found. Please connect a MicroSD to the port, then reboot.");
		for (int positionCounter = 0; positionCounter < 72; positionCounter++) {
			// scroll one position left:
			lcd.scrollDisplayLeft();
			// wait a bit:
			delay(150);
		}
		while (1);
	}
}

