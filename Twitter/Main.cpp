#include <iostream>
#include "lcdCOM.h"
#include "displayHitachiHD44780.h"

using namespace std;

void main (int argc,char** argv)
{

	string account(argv[2]);//Esto deberia ser con un parser para que quede un poco mas lindo
	int quantity = *argv[4];
	displayHitachiHD44780 hitachi;
	basicLCD * lcd = &hitachi;
	TWIT* twitList = new TWIT[quantity];
	bool positiveDownload;
	//conexiones con twitter que deberia estar en twitcom.cpp

	
	
	

	//Aca se deberian ir parseando los twits y metiendolos en el arreglo
	//no se como se maneja la api de titter, pero a dowloading tiwts se deberia llamar cada vez que se descarrga un twit
	for (int i = 0; i < quantity; i++)
	{
		positiveDownload = downloadingTwits(lcd, account, i);//muestra que se descargo un twit
		twitList[i].name = account;
		twitList[i].date = 1;//cosa parseada(puse 1 para que no tire warning), hay que ver si el formato parseado es el deseado, si no, modificarlo
		twitList[i].content = 1;//cosa parseada(puse 1 para que no tire warning)
	}


	delete[]twitList;
}