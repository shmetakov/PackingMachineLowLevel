#ifndef USART_TEMP_COM_H
#define USART_TEMP_COM_H

void init_USART_temp_con();
void send_buffer_USART_temp_con();
void clearBuffer_USART_temp_con();

#define USART4_RX_TX_GPIOx									GPIOC
#define USART4_RX_TX_GPIO_PIN								GPIO_Pin_2

#endif
