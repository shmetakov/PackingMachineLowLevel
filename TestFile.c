#include "global.h"
#include "TestFile.h"
#include "led.h"

#define TEST 1000

uint32_t flag = 0;
uint32_t tick = TEST;
uint32_t tickBlink = 0;

//*********************************************************************
//
//*********************************************************************

void TestFunc_1()
{
	if (tick){
			tick--;
			if (!tick){
				tick = TEST;
			}
		}

		if (!flag){
			if (tick == TEST){
				OFF_LED_ORANGE();
				ON_LED_RED();
			}else if (tick == 750){
				OFF_LED_RED();
				ON_LED_BLUE();
			}else if (tick == 500){
				OFF_LED_BLUE();
				ON_LED_GREEN();
			}else if (tick == 250){
				OFF_LED_GREEN();
				ON_LED_ORANGE();
				tickBlink++;
			}

			if (tickBlink == 5){
				flag = 1;
				tick = 1;
			}
		}else{
			tickBlink = 0;
			if (!(tick%200)){
				ON_LED_BLUE();
				ON_LED_GREEN();
				ON_LED_ORANGE();
				ON_LED_RED();
			}else if (!(tick%100)){
				OFF_LED_BLUE();
				OFF_LED_GREEN();
				OFF_LED_ORANGE();
				OFF_LED_RED();
			}

			if (tick == 1){
				flag = 0;
			}
		}
}

