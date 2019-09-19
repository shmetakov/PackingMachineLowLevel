#ifndef USART_FOR_MAIN_MOTOR_H
#define USART_FOR_MAIN_MOTOR_H

void init_USART_For_Main_Motor();
void send_package_For_Main_Motor(uint8_t* package);
void clearBuffer_For_Main_Motor();

#define USART2_RX_TX_GPIOx									GPIOA
#define USART2_RX_TX_GPIO_PIN								GPIO_Pin_4

#endif
