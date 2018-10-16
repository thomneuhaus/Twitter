#include <stdio.h>
#include <stdlib.h>


//**************************************  parseCmdLine  *****************************************************************
/* La funcion parseCmdLine es una funcion que se encarga de interpretar los argumentos recibidos por linea de comando
 * Puede procesar:
 *
 *	OPCIONES: Consiste de dos argumentos, el primero comienza con un guion ("-") denominado clave, y el segundo se
 * 	llama valor.
 *
 * 	PARAMETROS: Un argumento aislado que no comienza con guion.
 */

int parseCmdLine(int argc, char *argv[], int(*parseCallback) (char *key, char *value, void *UserData), void *UserData)
{
	int result = 1;	//Variable que se devuelve al main para identificar si hubo un error
	int i;
	char *key;	//Puntero para realizar la copia de la clave
	char *value;	//Puntero para realizar la copia del valor

	for (i = 1; (i < argc) && result; i++) //Analizo todos los argumentos
	{

		if (argv[i][0] == '-' && argv[i][1] == '\0') //Analizo si solamente es un guion, error
		{
			result = 0;
		}

		else if (argv[i][0] != '-') //Si no es una opcion, es un parametro
		{
			key = NULL;		//Si es un parametro, la clave es del tipo NULL
			value = argv[i];	//Se realiza una copia del valor
			result = (*parseCallback)(key, value, UserData);
		}
		else
		{
			key = argv[i] + 1;	//Se realiza una copia de la clave 
			value = argv[i + 1];	//Se realiza una copia del valor
			i++;			//Paso al siguiente argumento
			result = (*parseCallback)(key, value, UserData);
		}
	}
	return result;
}

//***********************************************************************************************************************

int str_cmp(const char * str1, const char * str2) // comaro el str1 con el 2
{
	int i, ret = 0;
	char * buffer = NULL;
	buffer = malloc((strlen(str1) + 1) * sizeof(char));
	if (buffer) // paso a minuscula
	{
		for (i = 0; i < strlen(str1); i++)   // copio todo el string en minuscula en un buffer
		{
			if (str1[i] >= 'A' && str1[i] <= 'Z')
			{
				buffer[i] = str1[i] + 'a' - 'A';
			}
			else
			{
				buffer[i] = str1[i];
			}
		}
		buffer[i] = '\0'; // le agrego el terminador al final 
		if (!strcmp(buffer, str2))
		{
			ret = 1;
		}
	}
	free(buffer);
	return ret;
}