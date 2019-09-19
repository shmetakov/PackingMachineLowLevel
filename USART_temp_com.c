#include "global.h"
#include "USART_temp_com.h"
#include "prtlcTempCon.h"

uint8_t buffer_USART_temp_con[5];
uint8_t buffer_index_USART_temp_con = 0;
uint8_t leght_buffer_USART_temp_con = 0;

extern uint8_t buffer_USART_temp_con_Send[5];
extern uint32_t __delay_clear_USART_temp_con_buf;

void DMA_UART4_init()
{

	DMA_InitTypeDef DMA_ini_UART4;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	DMA_ini_UART4.DMA_Channel = DMA_Channel_4;
	DMA_ini_UART4.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
	DMA_ini_UART4.DMA_Memory0BaseAddr = (uint32_t)buffer_USART_temp_con_Send;
	DMA_ini_UART4.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_ini_UART4.DMA_BufferSize = 5;
	DMA_ini_UART4.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_ini_UART4.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_ini_UART4.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_ini_UART4.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_ini_UART4.DMA_Mode = DMA_Mode_Normal;
	DMA_ini_UART4.DMA_Priority = DMA_Priority_Medium;
	DMA_ini_UART4.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_ini_UART4.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_ini_UART4.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_ini_UART4.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA1_Stream4, &DMA_ini_UART4);
	USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);
	NVIC_EnableIRQ(DMA1_Stream4_IRQn);
	DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);
}

void init_USART_temp_con()
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600; //Скорость обмена 9600 бод
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //1 стоп-бит
	USART_InitStructure.USART_Parity = USART_Parity_No ; //Без проверки четности
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Включен передатчик и приемник USART4

	GPIO_InitTypeDef GPIO_InitStructure; //Структура содержащая настройки порта

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //Включаем тактирование порта A
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); //Включаем тактирование порта USART4

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4); //Подключаем PA3 к TX USART4
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4); //Подключаем PA2 к RX USART4

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

		//Конфигурируем PA3 как альтернативную функцию -> RX UART. Подробнее об конфигурации можно почитать во втором уроке.
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_StructInit(&USART_InitStructure); //Инициализируем UART с дефолтными настройками: скорость 9600, 8 бит данных, 1 стоп бит

	USART_Init(UART4, &USART_InitStructure);
	USART_Cmd(UART4, ENABLE); //Включаем UART
	NVIC_EnableIRQ(UART4_IRQn);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = USART4_RX_TX_GPIO_PIN;
	GPIO_Init(USART4_RX_TX_GPIOx, &GPIO_InitStructure);

	GPIO_ResetBits(USART4_RX_TX_GPIOx, USART4_RX_TX_GPIO_PIN);

	DMA_UART4_init();
}

void send_buffer_USART_temp_con()
{
	GPIO_SetBits(USART4_RX_TX_GPIOx, USART4_RX_TX_GPIO_PIN);
	DMA_Cmd(DMA1_Stream4, ENABLE);
}
void clearBuffer_USART_temp_con()
{
	memset(buffer_USART_temp_con, 0, 5);
	buffer_index_USART_temp_con = 0;
}

void UART4_IRQHandler(void)
{
	if (UART4->SR & USART_SR_RXNE)
	{
		uint8_t data_byte = UART4->DR;;

		if ((data_byte == 0x02) && buffer_USART_temp_con[0] != 0x02 ){
			clearBuffer_USART_temp_con();
			buffer_USART_temp_con[buffer_index_USART_temp_con] = data_byte;
			buffer_index_USART_temp_con++;
		}else{
			if (buffer_USART_temp_con[0] == 0x02){
				if (buffer_index_USART_temp_con < 5)
					buffer_USART_temp_con[buffer_index_USART_temp_con] = data_byte;
				if (buffer_index_USART_temp_con == 4){
					if (buffer_USART_temp_con[4] == xorTempCon(buffer_USART_temp_con, 4)){
						saveDataTempCon(buffer_USART_temp_con);
						clearBuffer_USART_temp_con();
					}
				}
				buffer_index_USART_temp_con++;
			}
		}
		__delay_clear_USART_temp_con_buf = 10;
	}

	if (USART_GetITStatus(UART4, USART_IT_TXE) == SET){
		USART_ClearITPendingBit(UART4, USART_IT_TXE);
		USART_ClearITPendingBit(UART4, USART_IT_TC);
		USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
		USART_ITConfig(UART4, USART_IT_TC, ENABLE);
	}

	if (USART_GetITStatus(UART4, USART_IT_TC) == SET){
		USART_ClearITPendingBit(UART4, USART_IT_TC);
		USART_ITConfig(UART4, USART_IT_TC, DISABLE);
		GPIO_ResetBits(USART4_RX_TX_GPIOx, USART4_RX_TX_GPIO_PIN);
	}
}

void DMA1_Stream4_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF4) == SET){
		DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);
		USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
		memset(buffer_USART_temp_con_Send, 0x00, 5);
	}
}
