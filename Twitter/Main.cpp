#include <iostream>
#include <ctime>
#include <cstdio>
#include <conio.h>
#include "lcdCOM.h"
#include "Twitcom.h"
#include "displayHitachiHD44780.h"
#include "eventHandler.h" 
extern "C"
{
#include "parse.h"
}

int callback(char *key, char *value, void *UserData);

#define  DEFAULT_TWEET_QUANTITY	15
#define	 SPEED_DEFAULT 0
#define  SPEED_MAX	0
#define  SPEED_MIN  6


using namespace std;

typedef struct
{
	string user;
	unsigned int cantTweets;
}arguments;

int main(int argc, char* argv[])
{
	arguments argumentosEntrada;
	int error = parseCmdLine(argc, argv, callback, &argumentosEntrada);

	if (argumentosEntrada.cantTweets == 0)
	{
		argumentosEntrada.cantTweets = DEFAULT_TWEET_QUANTITY; // default tweet size si no se recibio por linea de comando

	}

	displayHitachiHD44780 lcd;
	basicLCD * lcdPointer = &lcd;

	Twitcom Twitter(argumentosEntrada.user, argumentosEntrada.cantTweets, lcdPointer);

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
						if (tweetNumber >= argumentosEntrada.cantTweets);
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
			Twitter.clearScreen();
			
		}

	}
	
	return 0;
}

int callback(char *key, char *value, void *UserData)
{
	arguments * data = (arguments *)UserData;
	int result = 0; //variable para detectar error

	if (key != NULL)	//evaluo las claves
	{
		if (str_cmp(key, "user"))
		{
			string userAux(value);
			data->user = userAux;
			result = 1;
		}
		else if (str_cmp(key, "cant"))
		{
			data->cantTweets = (unsigned int)atoi(value);
			result = 1;
		}
	}
	else
	{
		return 0;
	}
	return result;
}