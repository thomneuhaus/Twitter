#include "Twitcom.h"

Twitcom::Twitcom(string account, unsigned int cantTweets, basicLCD *lcdPointer)
{
	user = account;
	numberTweets = cantTweets;
	lcd = lcdPointer;
	print.setAccount(account);
	print.setDisplay(lcd);
	scrollPosition = 0;
	query = "https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=" + user + "&count=" + to_string(cantTweets);
	API_key = "HCB39Q15wIoH61KIkY5faRDf6";
	API_SecretKey = "7s8uvgQnJqjJDqA6JsLIFp90FcOaoR5Ic41LWyHOic0Ht3SRJ6";
	errorBool = false;
	curl = curl_easy_init(); //CURL es un puntero donde se guardan las configuraciones de la transferencia
	
	if (curl)
	{
		//Seteamos primero la pagina donde nos vamos a conectar. Para buscar el token es siempre la misma
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/oauth2/token");

		// Si la página nos redirije a algún lado, le decimos a curl que siga dicha redirección.
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		// Le decimos a CURL que trabaje tanto con HTTP como HTTPS, Autenticación por HTTP en modo básico.
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

		//Preparamos el password para la autenticación
		string userPwd = API_key + ":" + API_SecretKey;

		//Se lo seteamos a CURL.
		curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.c_str());

		struct curl_slist *list = NULL;
		//Pueden setear el Header con la linea de abajo, pero necesitan codificar las claves en Base64
		//list = curl_slist_append(list, "Authorization: Basic YlB5alk1bWRMR2V4TlhPTHhSUjd3MUVjUzpkR2liU3FIcURrektQMUtPbzFJTjRBd21tZGI1Tnl5ZjBFQTZkTDBLWlpmZDE0ZnhQQw==");

		//Le decimos a CURL que vamos a mandar URLs codificadas y además en formato UTF8.
		list = curl_slist_append(list, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		//Le decimos a CURL que trabaje con credentials.
		string data = "grant_type=client_credentials";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

		//Le decimos a curl que cuando haya que escribir llame a myCallback
		//y que use al string readString como user data.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

		// Perform the request, res will get the return code
		// Con lo de abajo le decimos a curl que intente conectarse a la página (recordemos que la idea es
		// obtener la autenticación así luego Twitter nos deja conectarnos a cualquier usuario público)
		//Recordar que easy_perform es bloqueante, en este caso es correcto asi al ser inicializacion y no tardar mucho tiempo.
		res = curl_easy_perform(curl);

		// Nos fijamos si hubo algún error
		if (res != CURLE_OK)
		{
			cerr << "Hubo un error con curl_easy_perform(): " << curl_easy_strerror(res) << endl;
			//Hacemos un clean up de curl antes de salir.
			errorDescription.errorType = ERROR_WITH_CURL_EASY_PERFORM;
			errorDescription.errorDescription = "Hubo un error con curl_easy_perform()";
			errorBool = true;
			
			curl_easy_cleanup(curl);
		}
		else
		{
			// Si no hubo errores hago un clean up antes de realizar un nuevo query.
			curl_easy_cleanup(curl);

			//Si el request de CURL fue exitoso entonces twitter devuelve un JSON
			//Abajo busco el token en el JSON para luego acceder a los twits.
			j = json::parse(readString);

			//Se encierra el parseo en un bloque try-catch porque la libreria maneja errores por excepciones.
			//Pueden tambien parsear usando iteradores o la forma que quieras, buscar en la documentacion las diferentes formas.
			try
			{
				//Tratamos de acceder al campo acces_token del JSON
				string aux = j["access_token"];
				token = aux;
				cout << "El token obtenido por Twitter API: \n" << token << endl;
			}
			catch (exception& e)
			{
				//Si hubo algun error, se muestra el error que devuelve la libreria
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
	// Nuevamente reinicializo todo curl.
	curl = curl_easy_init();
	multiHandle = curl_multi_init();
	readString = "";
	int stillRunning = 0;
	if ((curl != NULL) & (multiHandle != NULL))
	{
		//Attacheo el easy handle para manejar una coneccion no bloqueante.
		curl_multi_add_handle(multiHandle, curl);

		//Seteamos URL FOLLOWLOCATION y los protocolos a utilizar igual que antes.
		curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

		//Construimos el Header de autenticacion como lo especifica la API
		//usando el token que obtuvimos antes
		struct curl_slist *list = NULL;
		string aux = "Authorization: Bearer ";
		aux += token;
		list = curl_slist_append(list, aux.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		//Seteamos los callback igual que antes
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

		//Realizamos ahora un perform no bloqueante
		curl_multi_perform(multiHandle, &stillRunning);
		while (stillRunning)
		{
			//Debemos hacer polling de la transferencia hasta que haya terminado
			curl_multi_perform(multiHandle, &stillRunning);

			//Mientras tanto podemos hacer otras cosas
			print.downloadingTwits();
		}

		//Checkeamos errores
		if (res != CURLE_OK)
		{
			errorDescription.errorType = CURL_EASY_PERFORM_FAILED;
			errorDescription.errorDescription = "CURL easy perform failed";
			errorBool = true;
			
		}
		else
		{
			//Siempre realizamos el cleanup al final
			curl_easy_cleanup(curl);

			//Si el request de CURL fue exitoso entonces twitter devuelve un JSON
			//con toda la informacion de los tweets que le pedimos
			j = json::parse(readString);
			try
			{
				//Al ser el JSON un arreglo de objetos JSON se busca el campo text para cada elemento
				for (auto element : j)
				{
					tweets.push_back(element["text"]);
					dates.push_back(element["created_at"]);
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

void Twitcom::showTweet(unsigned int numberTweet)
{
	if (scrollPosition > tweets[numberTweet].length())
	{
		scrollPosition = 0;
	}
	string tweet = tweets[numberTweet].substr(scrollPosition);
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
	//fprintf(stdout, "%s",data);
	string* s = (string*)userp;
	s->append(data, realsize);
	return realsize;						//recordar siempre devolver realsize
}


