#include "Twitcom.h"

Twitcom::Twitcom(string account, unsigned int cantTweets, basicLCD *lcdPointer)
{
	//Backup de los datos recibidos
	user = account;
	numberTweets = cantTweets;
	lcd = lcdPointer;

	print.setDisplay(lcd);

	scrollPosition = 0;

	//Ejemplo: https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=lanacion&count=5
	query = "https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=" + user + "&count=" + to_string(cantTweets);

	//Keys
	API_key = "HCB39Q15wIoH61KIkY5faRDf6";
	API_SecretKey = "7s8uvgQnJqjJDqA6JsLIFp90FcOaoR5Ic41LWyHOic0Ht3SRJ6";

	errorBool = false;

	curl = curl_easy_init(); 

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/oauth2/token");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
		string userPwd = API_key + ":" + API_SecretKey;
		curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.c_str());
		struct curl_slist *list = NULL;
		list = curl_slist_append(list, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		string data = "grant_type=client_credentials";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			cerr << "Hubo un error con curl_easy_perform(): " << curl_easy_strerror(res) << endl;
			errorDescription.errorType = ERROR_WITH_CURL_EASY_PERFORM;
			errorDescription.errorDescription = "Hubo un error con curl_easy_perform()";
			errorBool = true;
			
			curl_easy_cleanup(curl);
		}
		else
		{
			curl_easy_cleanup(curl);
			j = json::parse(readString);
			try
			{
				string aux = j["access_token"];
				token = aux;
				cout << "El token obtenido por Twitter API: \n" << token << endl;
			}
			catch (exception& e)
			{
				cerr << e.what() << endl;
			}
		}
	}

	else
	{

		errorDescription.errorType = ERROR_INITIALIZING_CURL;
		errorDescription.errorDescription = "Cannot download tweets. Unable to start cURL";
		errorBool = true;
	}

}

void Twitcom::loadTweets(void)
{
	errorBool = false;
	curl = curl_easy_init();
	multiHandle = curl_multi_init();
	readString = "";
	int stillRunning = 0;
	if ((curl != NULL) & (multiHandle != NULL))
	{
		curl_multi_add_handle(multiHandle, curl);
		curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

		struct curl_slist *list = NULL;
		string aux = "Authorization: Bearer ";
		aux += token;
		list = curl_slist_append(list, aux.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

		curl_multi_perform(multiHandle, &stillRunning);

		string animation = "Downloading...";
		string animationAux;
		unsigned int i = 0;
		while (stillRunning)
		{
			curl_multi_perform(multiHandle, &stillRunning);
			animationAux = animation.substr(0, 11 + i);
			print.downloadingTwits(animationAux, user);
			Sleep(750);
			if ((i + 1) > 3)
			{
				i = 0;
			}
			else
			{
				i++;
			}
			
			lcd->lcdClear();
		
		}

		print.printSucces();
		Sleep(2000);
		lcd->lcdClear();

		if (res != CURLE_OK)
		{
			errorDescription.errorType = CURL_EASY_PERFORM_FAILED;
			errorDescription.errorDescription = "CURL easy perform failed";
			errorBool = true;
			
		}
		else
		{
			curl_easy_cleanup(curl);
			j = json::parse(readString);
			try
			{
				for (auto element : j)
				{
					string content = element["text"];
					string tweetAux = "@" + user + ":" + content;
					tweets.push_back(tweetAux);

					string dateAux = element["created_at"];
					string dateA = dateAux.substr(0, 16);
					dates.push_back(dateA);
				}
				cout << "Tweets cargados de la cuenta de Twitter: " << user << endl;
			}
			catch (std::exception& e)
			{
				//Muestro si hubo un error de la libreria
				std::cerr << e.what() << std::endl;
			}
		}
	}
	else
	{
		
		errorDescription.errorType = ERROR_DOWNLOADING_TWEETS;
		errorDescription.errorDescription = "Cannot download tweets. Unable to start cURL";
		errorBool = true;
	}

}

string Twitcom::getTweet(unsigned int numberTweet)
{
	string tweet = tweets[numberTweets];
	string answer = "@" + user + ": " + "- " + tweet + " -";
	return answer;
}

string Twitcom::getDate(unsigned int numberTweet)
{
	return dates[numberTweets];
}

void Twitcom::clearScreen(void)
{
	lcd->lcdClear();
}

void Twitcom::showTweet(unsigned int numberTweet)
{
	if (scrollPosition > tweets[numberTweet].length())
	{
		scrollPosition = 0;
	}
	string tweet = tweets[numberTweet].substr(scrollPosition, scrollPosition+15);
	print.printTweets(dates[numberTweet], tweet);
}

bool Twitcom::isError(void)
{
	return errorBool;
}

error_t Twitcom::getError(void)
{
	return errorDescription;
}

Twitcom::~Twitcom()
{

}

size_t Twitcom::myCallback(void * contents, size_t size, size_t nmemb, void * userp)
{
	size_t realsize = size * nmemb;
	char* data = (char *)contents;
	string* s = (string*)userp;
	s->append(data, realsize);
	return realsize;				
}


