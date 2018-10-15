#include "eventHandler.h"
#include <ctime>



eventHandler::eventHandler()
{
	time(&init);
	winTest = NULL;
	error = NO_ERROR;
}


eventHandler::~eventHandler()
{
}


EVENT
eventHandler::getEvent()
{
	EVENT event;
	if (checkTimerEvent())
	{
		event = SCROLL_TIME;
	}
	else if (checkKeyEvent())
	{
		event = getKeyEvent();
	}
	else 
	{
		event = NO_EVENT;
	}
	return event;
}

bool 
eventHandler::checkTimerEvent()
{
	bool result = false;
	time(&curr);								// se obtiene la hora actual 
	double time_difference = difftime(curr, init); // calcula diferencia de tiempo (en segundos) entre curr e init
	if (time_difference > MAX_TIME)
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
	winTest = initscr();
	if (winTest == NULL)
	{
		error = WINDOW_ERROR;
	}
	else
	{
		nodelay(winTest, TRUE);
		noecho();
		key = getch();
		if (key != ERR)
		{
			result = true;
		}
		endwin();
	}
	
	return result;
}

ERROR
eventHandler::geterror()
{
	return error;
}

EVENT
eventHandler::getKeyEvent()
{
	EVENT action;
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

