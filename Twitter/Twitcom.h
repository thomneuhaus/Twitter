#pragma once
#include "json.hpp"
#include "displayHitachiHD44780.h"
#include "LcdPrinter.h"
#include <curl/curl.h>
#include <iostream>

#define NOT_ERROR 0
#define ERROR_INITIALIZING_CURL	1
#define ERROR_WITH_CURL_EASY_PERFORM	2
#define ERROR_DOWNLOADING_TWEETS	3
#define CURL_EASY_PERFORM_FAILED	4


using namespace std;
using json = nlohmann::json;




typedef struct
{
	unsigned int errorType; // para que el programa sepa el error
	string errorDescription;// para que el usuario sepa el error
	//void Description_error(error); // para cargar errorDescription con el error que se produjo
}error_t;

class Twitcom
{
public:
	Twitcom(string account, unsigned int cantTweets, basicLCD *lcdPointer);
	void loadTweets(void);
	string getTweet(unsigned int numberTweet);
	string getDate(unsigned int numberTweet);
	void clearScreen(void);
	void showTweet(unsigned int numberTweet);

	unsigned int scrollPosition;

	bool isError(void);
	error_t getError(void);
	~Twitcom();

private:
	json j;				//Variable donde vamos a guardar lo que devuelva Twitter
	CURL *curl;			//Variable donde vamos a guardar las configuraciones de una transferencia
	CURLM *multiHandle;	//Variable donde vamos a atachear los easy handles
	CURLcode res;

	string query;

	string user;
	unsigned int numberTweets;

	bool errorBool;
	error_t errorDescription;

	string API_key;
	string API_SecretKey;

	static size_t myCallback(void *contents, size_t size, size_t nmemb, void *userp);

	string token;
	string readString;

	vector<string> tweets;
	vector<string> dates;

	basicLCD * lcd;

	LcdPrinter print;
};

