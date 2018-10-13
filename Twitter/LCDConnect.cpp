#include "LCDConnect.h"


using namespace std;


LCDConnect::LCDConnect(void)
{
	status = !FT_OK;
}

void LCDConnect::lcdWriteDR(FT_HANDLE deviceHandler, BYTE valor)
{
	//valor = XXXXYYYY
	BYTE temp = 0x00;
	temp = valor & 0xF0;					//b: XXXX0000 (RS = 0)
	temp = temp | LCD_RS_ON;				//b: XXXX0010 (RS = 1)	
	lcdWriteNibble(deviceHandler, temp);
	Sleep(1);
	temp = 0x00;
	temp = ((valor & 0x0F) << 4) & 0xF0;	//b: YYYY0000 (RS = 0)	
	temp = temp | LCD_RS_ON;				//b: YYYY0010 (RS = 1)
	lcdWriteNibble(deviceHandler, temp);
	Sleep(1);
}

FT_HANDLE * LCDConnect::init_ftdi_lcd(int iDevice)
{
	DWORD sizeSent = 0;
	string lcdDescriptionS = MY_LCD_DESCRIPTION + to_string(iDevice) + PORT_B;
	const char *lcdDescriptionC = lcdDescriptionS.c_str();

	chrono::seconds MaxTime(CONNECTING_TIME);	// Determino un tiempo maximo para intentar conectarse 

	chrono::time_point<chrono::system_clock> start = chrono::system_clock::now(); //Dermino el tiempo actual
	chrono::time_point<chrono::system_clock> current = start;

	cout << "Conectando el LCD..." << endl;

	while (status != FT_OK && ((current - start) < MaxTime))
	{
		status = FT_OpenEx((void *)lcdDescriptionC, FT_OPEN_BY_DESCRIPTION, &lcdHandle);

		if (status == FT_OK)
		{
			cout << "Se ha podidio conectar el LCD" << endl;
			UCHAR Mask = 0xFF;	//Selects all FTDI pins.
			UCHAR Mode = 1; 	// Set asynchronous bit-bang mode
			status = FT_SetBitMode(lcdHandle, Mask, Mode);
			if (status == FT_OK)
			{
				cout << "Se configuro el LCD de forma correcta" << endl;
				status = send((LCD_FUNCTION_SET | LCD_MODE_8), lcdHandle);
				Sleep(5);
				status = send((LCD_FUNCTION_SET | LCD_MODE_8), lcdHandle);
				Sleep(1);
				status = send((LCD_FUNCTION_SET | LCD_MODE_8), lcdHandle);
				Sleep(1);
				status = send((LCD_FUNCTION_SET | LCD_MODE_4), lcdHandle);
				Sleep(1);
				status = send((LCD_FUNCTION_SET | LCD_MODE_4 | LINE_2 | FONT_5X8), lcdHandle);
				Sleep(1);
				status = send((LCD_DISPLAY_ON_OF_CONTROL | 0x04 | 0X02 | 0x01), lcdHandle);
				Sleep(1);
				//status = FT_Write(lcdPointer, &set, 1, &byteSent);
				status = send((LCD_CLEAR), lcdHandle);
				Sleep(10);
				status = send((LCD_ENTRY_MODE_SET), lcdHandle);
			}
			else
			{
				cout << "Ocurrio un error a la hora de configurar el LCD" << endl;
			}

		}
		else
		{
			cout << "Ocurrio un error a la hora de contar el LCD" << endl;
		}
		current = std::chrono::system_clock::now();
	}

	return &lcdHandle;
}

void LCDConnect::lcdWriteIR(FT_HANDLE deviceHandler, BYTE valor)
{
	//valor: XXXXYYYY
	unsigned char temp = valor & 0xF0;		//b: XXXX0000 (RS = 0)
	temp = temp | LCD_RS_OFF;				//b: XXXX0000 (RS = 0)
	lcdWriteNibble(deviceHandler, temp);
	temp = 0x00;
	temp = ((valor & 0x0F) << 4) & 0xF0;	//b: YYYY0000 (RS = 0)
	temp = temp | LCD_RS_OFF;				//b: XXXX0000 (RS = 0)
	lcdWriteNibble(deviceHandler, temp);
}

FT_STATUS LCDConnect::getStatus(void)
{
	return status;
}

FT_STATUS LCDConnect::send(BYTE valor, FT_HANDLE lcdPointer)
{
	DWORD byteSent;
	valor &= 0xfe;
	FT_STATUS status = FT_OK;
	status = FT_Write(lcdPointer, &valor, 1, &byteSent);
	Sleep(1);
	BYTE valorE = (valor | 0x01);
	status = FT_Write(lcdPointer, &valorE, 1, &byteSent);
	Sleep(3);
	status = FT_Write(lcdPointer, &valor, 1, &byteSent);
	Sleep(1);
	return status;
}

void LCDConnect::lcdWriteNibble(FT_HANDLE ft, BYTE nibble)
{
	//nibble: XXXXYYYY
	FT_STATUS statusa;
	BYTE valueToSend = (nibble);						//b: XXXXYYY0 (E = 0)
	DWORD byteSent;
	statusa = FT_Write(ft,&valueToSend, 1, &byteSent);
	Sleep(1);
	valueToSend = nibble | LCD_E_ON;					//b: XXXXYYY1 (E = 1)			
	FT_Write(ft, &valueToSend, 1, &byteSent);
	Sleep(3);
	valueToSend = 0x00;
	valueToSend = nibble;								//b: XXXXYYY0 (E = 0)
	FT_Write(ft, &valueToSend, 1, &byteSent);
	Sleep(1);
}
