#pragma once
#include <ctime>
#include "curses.h"
#define MAX_TIME	1 //(EN SEGUNDOS!!)
typedef enum 
{
	SPEED_UP,
	SPEED_DOWN,
	REPEAT,
	NEXT,
	PREVIOUS,
	SCROLL_TIME,
	EXIT,
	NO_EVENT,
} EVENT;

typedef enum
{
	WINDOW_ERROR,
	NO_ERROR
} ERROR;

class eventHandler
{
public:
	eventHandler();
	~eventHandler();
	EVENT getEvent();

private:
	bool checkTimerEvent();
	bool checkKeyEvent();
	ERROR geterror();
	EVENT getKeyEvent();
	time_t init, curr;
	int key;
	WINDOW * winTest;
	ERROR error;
};

