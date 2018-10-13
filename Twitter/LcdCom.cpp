#include "LcdCom.h"



//notas para considerar : convendria hacer estaws funciones en formato de clase cosa de que almacenen el lcd, la posicion del cursor
//y otras cosas necesarias como dato miembro;


bool downloadingTwits(basicLCD* lcd, string account, int twitplace)
{

	cursorPosition pos;
	bool error = (*lcd).lcdClear();
	//checkeo de errores?

	string actual = "Twits descargados = " + to_string(twitplace);
	char* cuenta = (char*)account.c_str();
	char* number = (char*)actual.c_str();
	int size = strlen(cuenta);

	if (size <= 16)
	{
		pos.column = FIRST_COLUMN;
		pos.row = FIRST_LINE;
		lcd->lcdSetCursorPosition(pos);

		*lcd << (unsigned char*)cuenta;

		pos.column = FIRST_COLUMN;
		pos.row = SECOND_LINE;
		lcd->lcdSetCursorPosition(pos);

		*lcd << (unsigned char*)number;
	}
	else
	{
		pos.column = FIRST_COLUMN;		//imprimo numero de twit en pantalla
		pos.row = SECOND_LINE;
		lcd->lcdSetCursorPosition(pos);
		*lcd << (unsigned char*)number;

		char* aux = cuenta;
		for (int i = 0; i <= (size - DISPLAY_ROW_SIZE); i++)		//marqusina de la cuenta
		{
			pos.row = FIRST_LINE;
			pos.column = FIRST_COLUMN;
			lcd->lcdSetCursorPosition(pos);

			for (int j = 0; j < DISPLAY_ROW_SIZE; j++)
			{
				*lcd << aux[j];
				pos.column++;
				lcd->lcdSetCursorPosition(pos);
			}
			aux++;
			Sleep(500); //Con las pruebas hay que ver cuanto conviene
		}
	}
	return true; //checkeo de errores?
}
bool showTwit(basicLCD* lcd,TWIT*twit)
{
	cursorPosition pos;
	bool error = (*lcd).lcdClear();
	
	char* fecha = (char*)(twit->date).c_str();
	pos.column = FIRST_COLUMN;
	pos.row = FIRST_LINE;
	lcd->lcdSetCursorPosition(pos);
	*lcd << (unsigned char*)fecha;			//imprimo fecha

	string show = (twit->name) + ": - " + (twit->content) + " -"; // contenido a mostrar
	char * mostrar = (char*)show.c_str();
	int size = strlen(mostrar);
	char* aux = mostrar;

	for (int i = 0; i <= (size - DISPLAY_ROW_SIZE); i++)		//marquesina del contenido a mostrar
	{
		pos.row = FIRST_LINE;
		pos.column = FIRST_COLUMN;
		lcd->lcdSetCursorPosition(pos);

		for (int j = 0; j < DISPLAY_ROW_SIZE; j++)
		{
			*lcd << aux[j];
			pos.column++;
			lcd->lcdSetCursorPosition(pos);
		}
		aux++;
		Sleep(500); //Con las pruebas hay que ver cuanto conviene
	}
}

