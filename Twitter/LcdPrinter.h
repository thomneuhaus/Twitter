#pragma once

#include "displayHitachiHD44780.h"
#include "basicLCD.h"
#include <iostream>

#include <string>

class LcdPrinter
{
public:
	LcdPrinter(void);
	~LcdPrinter();

	void setDisplay(basicLCD * lcd);


	void downloadingTwits(string download, string account);

	void printTweets(string date, string tweet);

	void printSucces(void);

private:	
	basicLCD * display;
	cursorPosition pos;
};

