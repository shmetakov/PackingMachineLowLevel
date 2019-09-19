#include "global.h"
#include "led.h"

void LED_init()
{
	GPIO_InitTypeDef  ledinit;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	ledinit.GPIO_Mode = GPIO_Mode_OUT;
	ledinit.GPIO_OType = GPIO_OType_PP;
	ledinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ledinit.GPIO_Speed = GPIO_Speed_2MHz;
	ledinit.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12;

	GPIO_Init(GPIOD, &ledinit);
}


