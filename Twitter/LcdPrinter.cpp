#include "LcdPrinter.h"



LcdPrinter::LcdPrinter(void)
{
}

void LcdPrinter::setDisplay(basicLCD * lcd)
{
	display = lcd;
}



LcdPrinter::~LcdPrinter()
{
}

void 
LcdPrinter::downloadingTwits(string download, string account)
{
	// Me paro en la primera linea, primera columna
	pos.row = FIRST_LINE;
	pos.column = FIRST_COLUMN;
	display->lcdSetCursorPosition(pos);
	(*display) << (unsigned char *)account.c_str();


	// Me paro en la segunda linea, primera columna
	pos.row = SECOND_LINE;
	pos.column = FIRST_COLUMN;
	display->lcdSetCursorPosition(pos);
	(*display) << (unsigned char *)download.c_str();

}

void LcdPrinter::printTweets(string date, string tweet)
{
	// Me paro en la primera linea, primera columna
	pos.row = FIRST_LINE;
	pos.column = FIRST_COLUMN;
	display->lcdSetCursorPosition(pos);
	(*display) << (unsigned char *)date.c_str();

	// Me paro en la segunda linea, primera columna
	pos.row = SECOND_LINE;
	pos.column = FIRST_COLUMN;
	display->lcdSetCursorPosition(pos);
	(*display) << (unsigned char *)tweet.c_str();
}

void LcdPrinter::printSucces(void)
{
	pos.row = FIRST_LINE;
	pos.column = FIRST_COLUMN;
	display->lcdSetCursorPosition(pos);
	(*display) << (unsigned char *)"EXITO!";
}

