#pragma once
#include "basicLCD.h"
#include "LCDconnect.h"

#define LCD_NUMBER		3	
#define FIRST_LINE		0
#define SECOND_LINE		1
#define FIRST_COLUMN	0
#define LAST_COLUMN		15
#define FIRSTL_FIRSTC	1	//Primera linea - Primera columna
#define FIRSTL_LASTC	16	//Primera linea - Ultima columna
#define SECONDL_FIRSTC	17	//Segunda linea - Primera columna
#define SECONDL_LASTC	32	//Segundo linea - Ultima columna
#define DISPLAY_OFFSET	16
#define DISPLAY_ROW_SIZE 16
class displayHitachiHD44780 :
	public basicLCD
{
public:
	displayHitachiHD44780();
	~displayHitachiHD44780();
	virtual bool lcdInitOk();
	virtual FT_STATUS lcdGetError();
	virtual bool lcdClear();
	virtual bool lcdClearToEOL();
	virtual basicLCD& operator<<(const unsigned char c);
	virtual basicLCD& operator<<(const unsigned char * c);
	virtual bool lcdMoveCursorUp();
	virtual bool lcdMoveCursorDown();
	virtual bool lcdMoveCursorRight();
	virtual bool lcdMoveCursorLeft();
	virtual bool lcdSetCursorPosition(const cursorPosition pos);
	virtual cursorPosition lcdGetCursorPosition();
	

private:
	LCDConnect lcd;
	FT_HANDLE * lcdPointer;
	cursorPosition cursor;
	int cadd;
	virtual void lcdUpdateCursor();
	

};

