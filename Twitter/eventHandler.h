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
	NO_ERROR_T,
} ERROR_T;

class eventHandler
{
public:
	eventHandler();
	~eventHandler();
	EVENT getEvent(unsigned int max_time);

private:
	bool checkTimerEvent(unsigned int max_time);
	bool checkKeyEvent();
	ERROR_T geterror();
	EVENT getKeyEvent();
	time_t init, curr;
	int key;
	WINDOW * winTest;
	ERROR_T error;
};

