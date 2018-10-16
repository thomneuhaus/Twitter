#include <iostream>
#include <ctime>
#include <cstdio>
#include <conio.h>
#include "lcdCOM.h"
#include "Twitcom.h"
#include "displayHitachiHD44780.h"
#include "eventHandler.h" 
#define  DEFAULT_TWEET_QUANTITY	15
#define	 SPEED_DEFAULT 3
#define  SPEED_MAX	1
#define  SPEED_MIN  6


using namespace std;

int main(int argc, char* argv[])
{
	// (0)progrma.exe (1)-usuario  (2)lanacion (3)-cantidadTweets (4)5
	//string usuario(argv[2]);//Esto deberia ser con un parser para que quede un poco mas lindo
	string usuario = "realDonaldTrump";
	unsigned int cantTweets = 20;
	//unsigned int cantTweets = (unsigned int) atoi(argv[4]);
	//Contemplar si el numero es 0, es decir, que no quiere imprimir ningun tweet, caso 1.b
	if (cantTweets == 0)
	{
		cantTweets = DEFAULT_TWEET_QUANTITY; // default tweet size si no se recibio por linea de comando

	}

	displayHitachiHD44780 lcd;
	basicLCD * lcdPointer = &lcd;

	Twitcom Twitter(usuario, cantTweets, lcdPointer);

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
			eventHandler ev;
			unsigned int evInput;
			bool exit = false;
			string tweet;
			string date;
			unsigned int tweetNumber = 0;
			unsigned int speedScroll = SPEED_DEFAULT;
			while (!exit)
			{
				evInput = ev.getEvent(speedScroll);
				if (evInput != NO_EVENT)
				{
					switch (evInput)
					{
					case SPEED_UP:
						if ((speedScroll - 1) < SPEED_MIN)
						{
							speedScroll--;
							cout << speedScroll << endl;
						}
						break;

					case SPEED_DOWN:
						if ((speedScroll + 1) > SPEED_MAX)
						{
							speedScroll++;
							cout << speedScroll << endl;
						}
						break;

					case REPEAT:
						Twitter.scrollPosition = 0;
						break;

					case PREVIOUS:
						if (tweetNumber <= 0);
						else
						{
							tweetNumber--;
							Twitter.scrollPosition = 0;
						}
						break;

					case NEXT:
						if (tweetNumber >= cantTweets);
						else
						{
							tweetNumber++;
							Twitter.scrollPosition = 0;
						}
						break;

					case EXIT:
						exit = true;
						break;

					case SCROLL_TIME:
						Twitter.scrollPosition++;
						break;

					}

					Twitter.showTweet(tweetNumber);

				}

			}
			
		}

	}
	
	return 0;
}