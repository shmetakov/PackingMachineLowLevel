#include "global.h"
#include "prtlcTempCon.h"
#include "USART_temp_com.h"

bool isNewDataAvailableTempCon = false;
uint8_t storedDataTempCon[5];
uint8_t buffer_USART_temp_con_Send[5];

extern uint32_t __delay_wait_answer_USART_temp_con;
extern uint8_t buffer_USART_temp_con[5];

extern uint8_t paramTemp_1;
extern uint8_t paramTemp_2;
extern uint8_t paramTemp_3;
extern uint32_t paramWigth;
extern uint32_t paramLength;

extern uint32_t __time_for_setTemp;

uint8_t currentTemp_1 = 0x00;
uint8_t currentTemp_2 = 0x00;
uint8_t currentTemp_3 = 0x00;

void parsingPackageTempCon()
{
	currentTemp_1 = storedDataTempCon[SENS_1_BYTE];
	currentTemp_2 = storedDataTempCon[SENS_2_BYTE];
	currentTemp_3 = storedDataTempCon[SENS_3_BYTE];
	memset(storedDataTempCon, 0x00, 5);


	__time_for_setTemp = TIME_FOR_SET_TEMP;
}

uint8_t xorTempCon(uint8_t *buf, uint8_t len)
{
	uint8_t xor = 0;
	for(uint8_t i = 1; i < len; i++)
	{
		xor^=buf[i];
	}
	return xor;
}

void saveDataTempCon(uint8_t* packege)
{
	for (uint8_t i = 0; i < 5; i++)
		storedDataTempCon[i] = packege[i];

	isNewDataAvailableTempCon = true;
	__delay_wait_answer_USART_temp_con = 0;
}

void clearBuffer_TempCon_Send() { memset(buffer_USART_temp_con_Send, 0x00, 5); }

void buildPackageTempCon(uint8_t* data)
{
	clearBuffer_TempCon_Send();
	buffer_USART_temp_con_Send[0] = 0x02;

	for (uint8_t i = 0; i < 3; ++i)
		buffer_USART_temp_con_Send[1 + i] = data[i];

	buffer_USART_temp_con_Send[4] = xorTempCon(buffer_USART_temp_con_Send, 4);
	send_buffer_USART_temp_con();
	memset(storedDataTempCon, 0x00, 5);
}

void cmd_sendTempCon()
{
	uint8_t data[3];

	data[0] = paramTemp_1;
	data[1] = paramTemp_2;
	data[2] = paramTemp_3;

	buildPackageTempCon(data);

	__delay_wait_answer_USART_temp_con = 1000;
}
