#include "global.h"
#include "USART_For_Main_Motor.h"
#include "protocol_For_Main_Motor.h"
#include "led.h"

uint8_t buffer_For_Main_Motor[BUFFER_MAIN_MOTOR_SIZE];
uint8_t buffer_index_For_Main_Motor = 0;

extern uint32_t __delay_clear_motor_buf;
extern uint8_t buffer_Motor_Send[BUFFER_MAIN_MOTOR_SIZE];

DMA_InitTypeDef DMA_ini_USART2;

void DMA_USART2_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	DMA_ini_USART2.DMA_Channel = DMA_Channel_4;
	DMA_ini_USART2.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
	DMA_ini_USART2.DMA_Memory0BaseAddr = (uint32_t)buffer_Motor_Send;
	DMA_ini_USART2.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_ini_USART2.DMA_BufferSize = 5;
	DMA_ini_USART2.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_ini_USART2.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_ini_USART2.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_ini_USART2.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_ini_USART2.DMA_Mode = DMA_Mode_Normal;
	DMA_ini_USART2.DMA_Priority = DMA_Priority_Medium;
	DMA_ini_USART2.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_ini_USART2.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_ini_USART2.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_ini_USART2.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA1_Stream6, &DMA_ini_USART2);
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
	DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);
}

void init_USART_For_Main_Motor()
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600; //Скорость обмена 9600 бод
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //1 стоп-бит
	USART_InitStructure.USART_Parity = USART_Parity_No ; //Без проверки четности
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Включен передатчик и приемник USART2

	GPIO_InitTypeDef GPIO_InitStructure; //Структура содержащая настройки порта

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //Включаем тактирование порта A
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //Включаем тактирование порта USART2

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //Подключаем PA3 к TX USART2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //Подключаем PA2 к RX USART2

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Конфигурируем PA3 как альтернативную функцию -> RX UART. Подробнее об конфигурации можно почитать во втором уроке.
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_StructInit(&USART_InitStructure); //Инициализируем UART с дефолтными настройками: скорость 9600, 8 бит данных, 1 стоп бит

	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE); //Включаем UART
	NVIC_EnableIRQ(USART2_IRQn);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = USART2_RX_TX_GPIO_PIN;

	GPIO_Init(USART2_RX_TX_GPIOx, &GPIO_InitStructure);
	GPIO_ResetBits(USART2_RX_TX_GPIOx, USART2_RX_TX_GPIO_PIN);

	DMA_USART2_init();
}

void send_package_For_Main_Motor(uint8_t* package)
{
	GPIO_SetBits(USART2_RX_TX_GPIOx, USART2_RX_TX_GPIO_PIN);
	DMA_Cmd(DMA1_Stream6, ENABLE);
}

void clearBuffer_For_Main_Motor()
{
	memset(buffer_For_Main_Motor, 0, BUFFER_MAIN_MOTOR_SIZE);
	buffer_index_For_Main_Motor = 0;
}

void USART2_IRQHandler (void)
{

	if (USART2->SR & USART_SR_RXNE)
	{
		uint8_t data_byte = USART2->DR;;

		if ((data_byte == ADR_MOTOR) && buffer_For_Main_Motor[0] != ADR_MOTOR ){
			clearBuffer_For_Main_Motor();
			buffer_For_Main_Motor[buffer_index_For_Main_Motor] = data_byte;
			buffer_index_For_Main_Motor++;
		}else{
			if (buffer_For_Main_Motor[0] == ADR_MOTOR){
				if (buffer_index_For_Main_Motor < 8)
					buffer_For_Main_Motor[buffer_index_For_Main_Motor] = data_byte;
				if (buffer_index_For_Main_Motor == MAIN_MOTOR_R_CRC_BYTE){
					//if (buffer_For_Main_Motor[MAIN_MOTOR_R_CRC_BYTE] == crc_buffer_main_motor(buffer_For_Main_Motor)){
						saveDataMainMotor(buffer_For_Main_Motor);
						clearBuffer_For_Main_Motor();
					//}
				}

				buffer_index_For_Main_Motor++;
			}
		}
		__delay_clear_motor_buf = 50;
	}

	if (USART_GetITStatus(USART2, USART_IT_TXE) == SET){
		USART_ClearITPendingBit(USART2, USART_IT_TXE);
		USART_ClearITPendingBit(USART2, USART_IT_TC);
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		USART_ITConfig(USART2, USART_IT_TC, ENABLE);
		//GPIO_ResetBits(USART2_RX_TX_GPIOx, USART2_RX_TX_GPIO_PIN);
	}

	if (USART_GetITStatus(USART2, USART_IT_TC) == SET){
		USART_ClearITPendingBit(USART2, USART_IT_TC);
		USART_ITConfig(USART2, USART_IT_TC, DISABLE);
		GPIO_ResetBits(USART2_RX_TX_GPIOx, USART2_RX_TX_GPIO_PIN);
	}
}

void DMA1_Stream6_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) == SET){
		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
		clearBuffer_Motor_Send();

	}
}

