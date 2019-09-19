#include "global.h"
#include "service_COM_port.h"
#include "serviceProtocol.h"
#include "led.h"

uint8_t buffer_Service_COM_port[BUFFER_SERVICE_SIZE];
uint8_t buffer_index_Service_COM_port = 0;
uint8_t leght_buffer_service_COM_port = 0;

extern uint8_t buffer_Service_Send[BUFFER_SERVICE_SIZE];
extern uint32_t __delay_wait_answer_service_COM;

DMA_InitTypeDef DMA_ini_USART3;

void DMA_USART3_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	DMA_ini_USART3.DMA_Channel = DMA_Channel_4;
	DMA_ini_USART3.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
	DMA_ini_USART3.DMA_Memory0BaseAddr = (uint32_t)buffer_Service_Send;
	DMA_ini_USART3.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_ini_USART3.DMA_BufferSize = sizeof(buffer_Service_Send);
	DMA_ini_USART3.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_ini_USART3.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_ini_USART3.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_ini_USART3.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_ini_USART3.DMA_Mode = DMA_Mode_Normal;
	DMA_ini_USART3.DMA_Priority = DMA_Priority_Medium;
	DMA_ini_USART3.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_ini_USART3.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_ini_USART3.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_ini_USART3.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA1_Stream3, &DMA_ini_USART3);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	NVIC_EnableIRQ(DMA1_Stream3_IRQn);
	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);
}

void init_Service_COM_port()
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600; //Скорость обмена 9600 бод
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //1 стоп-бит
	USART_InitStructure.USART_Parity = USART_Parity_No ; //Без проверки четности
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Включен передатчик и приемник USART2

	GPIO_InitTypeDef GPIO_InitStructure; //Структура содержащая настройки порта

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //Включаем тактирование порта A
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //Включаем тактирование порта USART2

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); //Подключаем PA3 к TX USART2
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3); //Подключаем PA2 к RX USART2

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	  //Конфигурируем PA3 как альтернативную функцию -> RX UART. Подробнее об конфигурации можно почитать во втором уроке.
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_StructInit(&USART_InitStructure); //Инициализируем UART с дефолтными настройками: скорость 9600, 8 бит данных, 1 стоп бит

	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE); //Включаем UART
	NVIC_EnableIRQ(USART3_IRQn);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	DMA_USART3_init();
}

void send_buffer_Service_COM_port(uint8_t n)
{
	DMA_ini_USART3.DMA_BufferSize = n;
	DMA_Init(DMA1_Stream3, &DMA_ini_USART3);

	DMA_Cmd(DMA1_Stream3, ENABLE);
}

void clearBuffer_For_Service_COM_port()
{
	memset(buffer_Service_COM_port, 0, BUFFER_SERVICE_SIZE);
	buffer_index_Service_COM_port = 0;
}

void USART3_IRQHandler (void)
{
	uint8_t data_byte;

	if (USART3->SR & USART_SR_RXNE)
	{
		data_byte = USART3->DR;

		if ((data_byte == 0x02) && (buffer_Service_COM_port[0] != 0x02)){
			clearBuffer_For_Service_COM_port();
			buffer_Service_COM_port[buffer_index_Service_COM_port] = data_byte;
			buffer_index_Service_COM_port++;
		}else {
			if (buffer_Service_COM_port[0] == 0x02){
				if (buffer_index_Service_COM_port < BUFFER_SERVICE_SIZE)
					buffer_Service_COM_port[buffer_index_Service_COM_port] = data_byte;
				if (buffer_index_Service_COM_port == SERVICE_LEN_BYTE)
					leght_buffer_service_COM_port = buffer_Service_COM_port[buffer_index_Service_COM_port];
				if (buffer_index_Service_COM_port == (leght_buffer_service_COM_port + 2)){
					if (buffer_Service_COM_port[buffer_index_Service_COM_port] == xorService_COM(buffer_Service_COM_port, buffer_index_Service_COM_port)){
						saveDataService(buffer_Service_COM_port);
						clearBuffer_For_Service_COM_port();
					}
				}

				else if ((buffer_index_Service_COM_port == (leght_buffer_service_COM_port + 3)) &&
								(buffer_Service_COM_port[buffer_index_Service_COM_port - 1] == 0x13) &&
										(buffer_Service_COM_port[buffer_index_Service_COM_port] == 0x5D))
				{
					saveDataService(buffer_Service_COM_port);
					clearBuffer_For_Service_COM_port();
				}

				buffer_index_Service_COM_port++;

			}
		}

		__delay_wait_answer_service_COM = 15;

	}

}

void DMA1_Stream3_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3) == SET){
		DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
		clearBuffer_Service_Send();
	}
	if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_HTIF3) == SET){
		DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_HTIF3);
		clearBuffer_Service_Send();
	}
	if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_DMEIF3) == SET){
		DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_DMEIF3);
		clearBuffer_Service_Send();
	}
	if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_FEIF3) == SET){
		DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_FEIF3);
		clearBuffer_Service_Send();
	}
}
