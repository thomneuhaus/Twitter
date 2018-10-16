#include "eventHandler.h"
#include <ctime>





eventHandler::eventHandler()
{
	time(&init);
}


eventHandler::~eventHandler()
{
}


EVENT
eventHandler::getEvent(unsigned int max_time)
{
	EVENT event;
	if (checkTimerEvent(max_time))
	{
		event = SCROLL_TIME;
	}
	else if (_kbhit())	
	{
		key = getchar();
		event = getKeyEvent();
	}
	else 
	{
		event = NO_EVENT;
	}
	return event;
}

bool 
eventHandler::checkTimerEvent(unsigned int max_time)
{
	bool result = false;
	time(&curr);								// se obtiene la hora actual 
	double time_difference = difftime(curr, init); // calcula diferencia de tiempo (en segundos) entre curr e init
	if (time_difference > max_time)
	{
		result = true; // se indica que se debe scrollear la pantalla
		time(&init); // se actualiza el nuevo tiempo de inicio
	}
	return result;
}

bool
eventHandler::checkKeyEvent()
{
	bool result = false;
	if(error != WINDOW_ERROR)
	{
		nodelay(winTest, TRUE);
		noecho();
		key = getchar();
		if (key != ERR)
		{
			result = true;
		}
		
	}
	
	return result;
}

ERROR_T
eventHandler::geterror()
{
	return error;
}

EVENT
eventHandler::getKeyEvent()
{
	EVENT action =	NO_EVENT;
	switch (key)
			{
				case '+':
					action=SPEED_UP;
					break;

				case '-':
					action=SPEED_DOWN;
					break;

				case 'A': case 'a':
					action=PREVIOUS;
					break;

				case 'S': case 's':
					action=NEXT;
					break;

				case 'Q': case 'q':
					action = EXIT;
					break;

				case 'R': case 'r':
					action=REPEAT;
					break;
			}
	return action;
}

