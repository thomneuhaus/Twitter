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
	void setAccount(string account);

	bool downloadingTwits();

	void printTweets(string date, string tweet);

private:	
	basicLCD * display;
	cursorPosition namepos;
	cursorPosition animationpos;
	bool error;
	int action;
	int nameplace;
	char* account;
	char* animation;
	char* aux;
	char* aux2;
	cursorPosition pos;
};

