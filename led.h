#ifndef LED_H
#define LED_H

#define ON_LED_ORANGE()									GPIO_SetBits(GPIOD, GPIO_Pin_13);
#define ON_LED_GREEN()									GPIO_SetBits(GPIOD, GPIO_Pin_12);
#define ON_LED_RED()									GPIO_SetBits(GPIOD, GPIO_Pin_14);
#define ON_LED_BLUE()									GPIO_SetBits(GPIOD, GPIO_Pin_15);
#define OFF_LED_ORANGE()								GPIO_ResetBits(GPIOD, GPIO_Pin_13);
#define OFF_LED_GREEN()									GPIO_ResetBits(GPIOD, GPIO_Pin_12);
#define OFF_LED_RED()									GPIO_ResetBits(GPIOD, GPIO_Pin_14);
#define OFF_LED_BLUE()									GPIO_ResetBits(GPIOD, GPIO_Pin_15);

void LED_init();

#endif
