#include "LcdPrinter.h"



LcdPrinter::LcdPrinter(basicLCD* lcd,string cuenta)
{
	display = lcd;
	string animations = "Downloading";
	char* account = (char*)cuenta.c_str();
	char* animation = (char*)animations.c_str();
	action = 0;
	nameplace = 0;
	aux = account;
	aux2 = animation;
}


LcdPrinter::~LcdPrinter()
{
}

bool 
LcdPrinter::downloadingTwits()
{

	error = (*display).lcdClear();

	//imprimo nombre
	int size = (int)strlen(account);
	if (size <= DISPLAY_ROW_SIZE)		//imprimo la cuenta si es menor a 16 caracteres
	{
		namepos.column = FIRST_COLUMN;
		namepos.row = FIRST_LINE;
		display->lcdSetCursorPosition(namepos);

		*display << (unsigned char*)account;
	}
	else if (nameplace<=size - DISPLAY_ROW_SIZE) //cantidad de letras de la animacion
	{
		namepos.row = FIRST_LINE;
		namepos.column = FIRST_COLUMN;
		display->lcdSetCursorPosition(namepos);

		for (int j = 0; j < DISPLAY_ROW_SIZE; j++) //imprimo 16 caracteres
		{
			*display << aux[j];
			namepos.column++;
			display->lcdSetCursorPosition(namepos);
		}
		nameplace++;
		aux++;
	}
	else
	{
		nameplace = 0;
		aux = account;
		//error = (*display).lcdClear(); //verificar que no limpie innecesariamente
	}

	//imprimo animacion
		
	if (action<11) //cantidad de letras de la animacion
	{
		animationpos.row = SECOND_LINE;
		animationpos.column = FIRST_COLUMN + action;
		display->lcdSetCursorPosition(animationpos);
		*display << *aux2;
		action++;
		aux2++;
	}
	else
	{
		action = 0;
		aux2 = animation;
		animationpos.row = SECOND_LINE;
		animationpos.column = FIRST_COLUMN;
		error = (*display).lcdClearToEOL(); //verificar que no limpie innecesariamente
	}

	return true; //checkeo de errores?
}