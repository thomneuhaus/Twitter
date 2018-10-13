#pragma once
#include "displayHitachiHD44780.h"
#include "basicLCD.h"
#include <iostream>
#include <string>


using namespace std;

typedef struct
{
	string name;
	string date;
	string content;

}TWIT;

// Indica cantidad de twits descargados
//bool downloadingTwits(basicLCD* lcd, string account, int twitplace);

//Muestra un twit

//bool showTwit(basicLCD* lcd, TWIT*twit);
