#ifndef SENSOR_H
#define SENSOR_H

/*
 * Датчик 1 - Датчик верхнего положения платформы
 * Датчик 2 - Датчик 0 кулачка
 * Датчик 3 - Датчик крайнего положения продольного упакователя
 * Датчик 4 - Датчик 0 паперечных упаковательных пластин
 * Датчик 5 - Датчик 0 сдвигателя упаковательного лотка
 * Датчик 6 - Датчик откинутой рамки
 * Датчик 7 - Датчик перекрутки пленки
 * Датчик 8 - Датчик перекрутки пленки
 * Датчик 9 - Датчик 0 загрузки лотка с весов
 * Датчик 10 - Датчик наличия лотка на выдаче
 *
 */

#define SENSOR_GPIOx_1										GPIOD
#define SENSOR_GPIOx_2										GPIOD
#define SENSOR_GPIOx_3										GPIOC
#define SENSOR_GPIOx_4										GPIOA
#define SENSOR_GPIOx_5										GPIOC
#define SENSOR_GPIOx_6										GPIOC
#define SENSOR_GPIOx_7										GPIOC
#define SENSOR_GPIOx_8										GPIOB
#define SENSOR_GPIOx_9										GPIOB
#define SENSOR_GPIOx_10										GPIOE

#define SENSOR_PIN_1										GPIO_Pin_1
#define SENSOR_PIN_2										GPIO_Pin_0
#define SENSOR_PIN_3										GPIO_Pin_11
#define SENSOR_PIN_4										GPIO_Pin_15
#define SENSOR_PIN_5										GPIO_Pin_8
#define SENSOR_PIN_6										GPIO_Pin_6
#define SENSOR_PIN_7										GPIO_Pin_1
#define SENSOR_PIN_8										GPIO_Pin_0
#define SENSOR_PIN_9										GPIO_Pin_1
#define SENSOR_PIN_10										GPIO_Pin_9

#define HIGH_LEVEL											0x01
#define LOW_LEVEL											0x00

#define SENSOR_LEVEL_1										HIGH_LEVEL
#define SENSOR_LEVEL_2										HIGH_LEVEL
#define SENSOR_LEVEL_3										HIGH_LEVEL
#define SENSOR_LEVEL_4										HIGH_LEVEL
#define SENSOR_LEVEL_5										HIGH_LEVEL
#define SENSOR_LEVEL_6										HIGH_LEVEL
#define SENSOR_LEVEL_7										HIGH_LEVEL
#define SENSOR_LEVEL_8										HIGH_LEVEL
#define SENSOR_LEVEL_9										HIGH_LEVEL
#define SENSOR_LEVEL_10										HIGH_LEVEL

#define SENSOR_STATE_1()									GPIO_ReadInputDataBit(SENSOR_GPIOx_1, SENSOR_PIN_1)
#define SENSOR_STATE_2()									GPIO_ReadInputDataBit(SENSOR_GPIOx_2, SENSOR_PIN_2)
#define SENSOR_STATE_3()									GPIO_ReadInputDataBit(SENSOR_GPIOx_3, SENSOR_PIN_3)
#define SENSOR_STATE_4()									GPIO_ReadInputDataBit(SENSOR_GPIOx_4, SENSOR_PIN_4)
#define SENSOR_STATE_5()									GPIO_ReadInputDataBit(SENSOR_GPIOx_5, SENSOR_PIN_5)
#define SENSOR_STATE_6()									GPIO_ReadInputDataBit(SENSOR_GPIOx_6, SENSOR_PIN_6)
#define SENSOR_STATE_7()									GPIO_ReadInputDataBit(SENSOR_GPIOx_7, SENSOR_PIN_7)
#define SENSOR_STATE_8()									GPIO_ReadInputDataBit(SENSOR_GPIOx_8, SENSOR_PIN_8)
#define SENSOR_STATE_9()									GPIO_ReadInputDataBit(SENSOR_GPIOx_9, SENSOR_PIN_9)
#define SENSOR_STATE_10()									GPIO_ReadInputDataBit(SENSOR_GPIOx_10, SENSOR_PIN_10)

#define SENSOR_GISTR_POLL									10

void init_Sensor();
void poll_sens();

bool 	sensorState_1;
uint8_t sensorNumPoll_1;
bool 	sensorState_2;
uint8_t sensorNumPoll_2;
bool 	sensorState_3;
uint8_t sensorNumPoll_3;
bool 	sensorState_4;
uint8_t sensorNumPoll_4;
bool 	sensorState_5;
uint8_t sensorNumPoll_5;
bool 	sensorState_6;
uint8_t sensorNumPoll_6;
bool 	sensorState_7;
uint8_t sensorNumPoll_7;
bool 	sensorState_8;
uint8_t sensorNumPoll_8;
bool 	sensorState_9;
uint8_t sensorNumPoll_9;
bool 	sensorState_10;
uint8_t sensorNumPoll_10;


#endif
