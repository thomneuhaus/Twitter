#include <iostream>
#include <ctime>
#include <cstdio>
#include <conio.h>
#include "lcdCOM.h"
#include "Twitcom.h"
#include "displayHitachiHD44780.h"

#define  DEFAULT_TWEET_QUANTITY	15




using namespace std;

int main(int argc, char* argv[])
{
	// (0)progrma.exe (1)-usuario  (2)lanacion (3)-cantidadTweets (4)5
	string usuario(argv[2]);//Esto deberia ser con un parser para que quede un poco mas lindo

	unsigned int cantTweets = (unsigned int) atoi(argv[4]);
	//Contemplar si el numero es 0, es decir, que no quiere imprimir ningun tweet, caso 1.b
	if (cantTweets == 0)
	{
		cantTweets = DEFAULT_TWEET_QUANTITY; // default tweet size si no se recibio por linea de comando

	}


	Twitcom Twitter(usuario, cantTweets);
	if (Twitter.isError() == true)
	{
		error_t errorConnect = Twitter.getError();
		cout << errorConnect.errorDescription << endl;
	}
	else 
	{
		Twitter.loadTweets();
		if (Twitter.isError() == true)
		{
			error_t errorLoad = Twitter.getError();
			cout << errorLoad.errorDescription << endl;
		}
		else
		{
			displayHitachiHD44780 lcd;
			bool exit = false;
			string tweet;
			string date;
			unsigned int tweetNumber = 0;
			unsigned int scrollPosition = 0;
			unsigned int speedScroll; 
			bool scrollAction;
			time_t start;
			time_t end;
			double time_difference;
			time(&start); // se obtiene la hora actual (tiempo 0)

			cursorPosition pos;
			while (!exit)
			{
				time(&end); // se obtiene la hora actual otra vez cada vez que se entra al while
				time_difference = difftime(end, start); // calcula diferencia de tiempo (en segundos) entre end y start
				if (time_difference > MAX_TIME)
				{
					scrollAction = true; // se indica que se debe scrollear la pantalla
					time(&start); // se actualiza el nuevo tiempo de inicio
				}
				char key_down; // donde se guardara la tecla presionada
				//if (kbhit())// si se presiono una tecla
				//{
				//	key_down = getchar(); // se guarda la tecla presionada
				//	switch (key_down)
				//	{
				//		case '+':
				//			speedScroll++;
				//			break;

				//		case '-':
				//			speedScroll--;
				//			break;

				//		case 'A': case 'a':
				//			tweetNumber--;
				//			break;

				//		case 'S': case 's':
				//			tweetNumber--;
				//			break;

				//		case 'Q': case 'q':
				//			exit = true;
				//			break;

				//		case 'R': case 'r':
				//			scrollPosition = 0;
				//			break;
				//	}
				//}
				//else
				//{
				//	date = Twitter.getDate(tweetNumber);
				//	tweet = Twitter.getTweet(tweetNumber);

				//	if (scrollAction == true)
				//	{
				//		scrollPosition++;

				//		if (scrollPosition > tweet.length())
				//		{
				//			scrollPosition = 0;
				//		}

				//		// Me paro en la primera linea, primera columna
				//		pos.row = FIRST_LINE;
				//		pos.column = FIRST_COLUMN;
				//		lcd.lcdSetCursorPosition(pos);
				//		lcd << (unsigned char *)date.c_str();

				//		// Me paro en la segunda linea, primera columna
				//		pos.row = SECOND_LINE;
				//		pos.column = FIRST_COLUMN;
				//		lcd.lcdSetCursorPosition(pos);
				//		tweet = tweet.substr(scrollPosition);
				//		lcd << (unsigned char *)tweet.c_str();
	
				//	}

				//
				//}
			}
		}

	}
	
	return 0;

	////basicLCD * lcd = &hitachi;

	//TWIT* twitList = new TWIT[cantTweets];
	//bool positiveDownload;
	////conexiones con twitter que deberia estar en twitcom.cpp

	//
	//

	////Aca se deberian ir parseando los twits y metiendolos en el arreglo
	////no se como se maneja la api de twitter, pero a dowloading tiwts se deberia llamar cada vez que se descarrga un twit
	//for (int i = 0; i < quantity; i++)
	//{
	//	positiveDownload = downloadingTwits(lcd, account, i);//muestra que se descargo un twit
	//	twitList[i].name = account;
	//	twitList[i].date = 1;//cosa parseada(puse 1 para que no tire warning), hay que ver si el formato parseado es el deseado, si no, modificarlo
	//	twitList[i].content = 1;//cosa parseada(puse 1 para que no tire warning)
	//}


	/*delete[]twitList;*/
}