#ifndef MAGNET_H
#define MAGNET_H

/*
 * Магнит 1 - Магнит ножа пленки
 * Магнит 2 - Магнит прижима рамки
 * Магнит 3 - Магнит прижима рамки
 * Магнит 4 - Магнит отвода рамки
 * Магнит 5 - Магнит прижима пленки
 * Магнит 6 - Магнит прижима пленки
 * Магнит 7 - Магнит прижима пленки
 * Магнит 8 - Магнит прижима пленки
 *
 */

#define MAGNET_GPIOx_8										GPIOB
#define MAGNET_GPIOx_7										GPIOB
#define MAGNET_GPIOx_6										GPIOB
#define MAGNET_GPIOx_5										GPIOB
#define MAGNET_GPIOx_4										GPIOD
#define MAGNET_GPIOx_3										GPIOD
#define MAGNET_GPIOx_2										GPIOD
#define MAGNET_GPIOx_1										GPIOD

#define MAGNET_PIN_8										GPIO_Pin_12
#define MAGNET_PIN_7										GPIO_Pin_13
#define MAGNET_PIN_6										GPIO_Pin_14
#define MAGNET_PIN_5										GPIO_Pin_15
#define MAGNET_PIN_4										GPIO_Pin_8
#define MAGNET_PIN_3										GPIO_Pin_9
#define MAGNET_PIN_2										GPIO_Pin_10
#define MAGNET_PIN_1										GPIO_Pin_11


#define MAGNET_1_OFF()										GPIO_ResetBits(MAGNET_GPIOx_1, MAGNET_PIN_1);
#define MAGNET_2_OFF()										GPIO_ResetBits(MAGNET_GPIOx_2, MAGNET_PIN_2);
#define MAGNET_3_OFF()										GPIO_ResetBits(MAGNET_GPIOx_3, MAGNET_PIN_3);
#define MAGNET_4_OFF()										GPIO_ResetBits(MAGNET_GPIOx_4, MAGNET_PIN_4);
#define MAGNET_5_OFF()										GPIO_ResetBits(MAGNET_GPIOx_5, MAGNET_PIN_5);
#define MAGNET_6_OFF()										GPIO_ResetBits(MAGNET_GPIOx_6, MAGNET_PIN_6);
#define MAGNET_7_OFF()										GPIO_ResetBits(MAGNET_GPIOx_7, MAGNET_PIN_7);
#define MAGNET_8_OFF()										GPIO_ResetBits(MAGNET_GPIOx_8, MAGNET_PIN_8);

#define MAGNET_1_ON()										GPIO_SetBits(MAGNET_GPIOx_1, MAGNET_PIN_1);
#define MAGNET_2_ON()										GPIO_SetBits(MAGNET_GPIOx_2, MAGNET_PIN_2);
#define MAGNET_3_ON()										GPIO_SetBits(MAGNET_GPIOx_3, MAGNET_PIN_3);
#define MAGNET_4_ON()										GPIO_SetBits(MAGNET_GPIOx_4, MAGNET_PIN_4);
#define MAGNET_5_ON()										GPIO_SetBits(MAGNET_GPIOx_5, MAGNET_PIN_5);
#define MAGNET_6_ON()										GPIO_SetBits(MAGNET_GPIOx_6, MAGNET_PIN_6);
#define MAGNET_7_ON()										GPIO_SetBits(MAGNET_GPIOx_7, MAGNET_PIN_7);
#define MAGNET_8_ON()										GPIO_SetBits(MAGNET_GPIOx_8, MAGNET_PIN_8);

#define MAGNET_STATE_1()									GPIO_ReadOutputDataBit(MAGNET_GPIOx_1, MAGNET_PIN_1)
#define MAGNET_STATE_2()									GPIO_ReadOutputDataBit(MAGNET_GPIOx_2, MAGNET_PIN_2)
#define MAGNET_STATE_3()									GPIO_ReadOutputDataBit(MAGNET_GPIOx_3, MAGNET_PIN_3)
#define MAGNET_STATE_4()									GPIO_ReadOutputDataBit(MAGNET_GPIOx_4, MAGNET_PIN_4)
#define MAGNET_STATE_5()									GPIO_ReadOutputDataBit(MAGNET_GPIOx_5, MAGNET_PIN_5)
#define MAGNET_STATE_6()									GPIO_ReadOutputDataBit(MAGNET_GPIOx_6, MAGNET_PIN_6)
#define MAGNET_STATE_7()									GPIO_ReadOutputDataBit(MAGNET_GPIOx_7, MAGNET_PIN_7)
#define MAGNET_STATE_8()									GPIO_ReadOutputDataBit(MAGNET_GPIOx_8, MAGNET_PIN_8)

void init_Magnet();
_Bool magnet_set_state(uint8_t mag,  uint16_t ms);
void magnet_delay_work();

uint32_t _delay_magnet_work_1;
uint32_t _delay_magnet_work_2;
uint32_t _delay_magnet_work_3;
uint32_t _delay_magnet_work_4;
uint32_t _delay_magnet_work_5;
uint32_t _delay_magnet_work_6;
uint32_t _delay_magnet_work_7;
uint32_t _delay_magnet_work_8;

#endif
