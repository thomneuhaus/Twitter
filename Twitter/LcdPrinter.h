#pragma once
#include "displayHitachiHD44780.h"
#include "basicLCD.h"
#include <iostream>
#include <string>

class LcdPrinter
{
public:
	LcdPrinter(basicLCD * lcd, string account);
	~LcdPrinter();
	bool downloadingTwits();
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
};

