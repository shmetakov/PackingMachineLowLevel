#include "global.h"
#include "sensor.h"
#include "stepMotors.h"

void init_Sensor()
{
	GPIO_InitTypeDef sensorInit;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	sensorInit.GPIO_Mode = GPIO_Mode_IN;
	sensorInit.GPIO_OType = GPIO_OType_PP;
	sensorInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	sensorInit.GPIO_Speed = GPIO_Speed_2MHz;

	sensorInit.GPIO_Pin = SENSOR_PIN_1;
	GPIO_Init(SENSOR_GPIOx_1, &sensorInit);

	sensorInit.GPIO_Pin = SENSOR_PIN_2;
	GPIO_Init(SENSOR_GPIOx_2, &sensorInit);

	sensorInit.GPIO_Pin = SENSOR_PIN_3;
	GPIO_Init(SENSOR_GPIOx_3, &sensorInit);

	sensorInit.GPIO_Pin = SENSOR_PIN_4;
	GPIO_Init(SENSOR_GPIOx_4, &sensorInit);

	sensorInit.GPIO_Pin = SENSOR_PIN_5;
	GPIO_Init(SENSOR_GPIOx_5, &sensorInit);

	sensorInit.GPIO_Pin = SENSOR_PIN_6;
	GPIO_Init(SENSOR_GPIOx_6, &sensorInit);

	sensorInit.GPIO_Pin = SENSOR_PIN_7;
	GPIO_Init(SENSOR_GPIOx_7, &sensorInit);

	sensorInit.GPIO_Pin = SENSOR_PIN_8;
	GPIO_Init(SENSOR_GPIOx_8, &sensorInit);

	sensorInit.GPIO_Pin = SENSOR_PIN_9;
	GPIO_Init(SENSOR_GPIOx_9, &sensorInit);

	sensorInit.GPIO_Pin = SENSOR_PIN_10;
	GPIO_Init(SENSOR_GPIOx_10, &sensorInit);

	sensorNumPoll_1 = SENSOR_GISTR_POLL;
	sensorNumPoll_2 = SENSOR_GISTR_POLL;
	sensorNumPoll_3 = SENSOR_GISTR_POLL;
	sensorNumPoll_4 = SENSOR_GISTR_POLL;
	sensorNumPoll_5 = SENSOR_GISTR_POLL;
	sensorNumPoll_6 = SENSOR_GISTR_POLL;
	sensorNumPoll_7 = SENSOR_GISTR_POLL;
	sensorNumPoll_8 = SENSOR_GISTR_POLL;
	sensorNumPoll_9 = SENSOR_GISTR_POLL;
	sensorNumPoll_10 = SENSOR_GISTR_POLL;

	sensorState_1 = false;
	sensorState_2 = false;
	sensorState_3 = false;
	sensorState_4 = false;
	sensorState_5 = false;
	sensorState_6 = false;
	sensorState_7 = false;
	sensorState_8 = false;
	sensorState_9 = false;
	sensorState_10 = false;
}

inline void poll_sens()
{
	if (SENSOR_STATE_1()){
		if (!sensorState_1){
			sensorNumPoll_1--;
			if (!sensorNumPoll_1){
				sensorState_1 = true;
				sensorNumPoll_1 = SENSOR_GISTR_POLL;
			}
		}
	}else{
		if (sensorState_1){
			sensorNumPoll_1--;
			if (!sensorNumPoll_1){
				sensorState_1 = false;
				sensorNumPoll_1 = SENSOR_GISTR_POLL;
			}
		}
	}

	if (SENSOR_STATE_2()){
		if (!sensorState_2){
			sensorNumPoll_2--;
			if (!sensorNumPoll_2){
				sensorState_2 = true;
				sensorNumPoll_2 = SENSOR_GISTR_POLL;
			}
		}
	}else{
		if (sensorState_2){
			sensorNumPoll_2--;
			if (!sensorNumPoll_2){
				sensorState_2 = false;
				sensorNumPoll_2 = SENSOR_GISTR_POLL;
			}
		}
	}

	if (SENSOR_STATE_3()){
		if (!sensorState_3){
			sensorNumPoll_3--;
			if (!sensorNumPoll_3){
				sensorState_3 = true;
				sensorNumPoll_3 = SENSOR_GISTR_POLL;
			}
		}
	}else{
		if (sensorState_3){
			sensorNumPoll_3--;
			if (!sensorNumPoll_3){
				sensorState_3 = false;
				sensorNumPoll_3 = SENSOR_GISTR_POLL;
			}
		}
	}

	if (SENSOR_STATE_4()){
		if (!sensorState_4){
			sensorNumPoll_4--;
			if (!sensorNumPoll_4){
				sensorState_4 = true;
				sensorNumPoll_4 = SENSOR_GISTR_POLL;

				if (sens_step_motor_2){
					sens_step_motor_2 = false;
					stop_motor_2();
				}
			}
		}
	}else{
		if (sensorState_4){
			sensorNumPoll_4--;
			if (!sensorNumPoll_4){
				sensorState_4 = false;
				sensorNumPoll_4 = SENSOR_GISTR_POLL;
			}
		}
	}

	if (SENSOR_STATE_5()){
		if (!sensorState_5){
			sensorNumPoll_5--;
			if (!sensorNumPoll_5){
				sensorState_5 = true;
				sensorNumPoll_5 = SENSOR_GISTR_POLL;

				if (sens_step_motor_3){
					sens_step_motor_3 = false;
					stop_motor_3();
				}
			}
		}
	}else{
		if (sensorState_5){
			sensorNumPoll_5--;
			if (!sensorNumPoll_5){
				sensorState_5 = false;
				sensorNumPoll_5 = SENSOR_GISTR_POLL;
			}
		}
	}

	if (SENSOR_STATE_6()){
		if (!sensorState_6){
			sensorNumPoll_6--;
			if (!sensorNumPoll_6){
				sensorState_6 = true;
				sensorNumPoll_6 = SENSOR_GISTR_POLL;
			}
		}
	}else{
		if (sensorState_6){
			sensorNumPoll_6--;
			if (!sensorNumPoll_6){
				sensorState_6 = false;
				sensorNumPoll_6 = SENSOR_GISTR_POLL;
			}
		}
	}

	if (SENSOR_STATE_7()){
		if (!sensorState_7){
			sensorNumPoll_7--;
			if (!sensorNumPoll_7){
				sensorState_7 = true;
				sensorNumPoll_7 = SENSOR_GISTR_POLL;
			}
		}
	}else{
		if (sensorState_7){
			sensorNumPoll_7--;
			if (!sensorNumPoll_7){
				sensorState_7 = false;
				sensorNumPoll_7 = SENSOR_GISTR_POLL;
			}
		}
	}

	if (SENSOR_STATE_8()){
		if (!sensorState_8){
			sensorNumPoll_8--;
			if (!sensorNumPoll_8){
				sensorState_8 = true;
				sensorNumPoll_8 = SENSOR_GISTR_POLL;
			}
		}
	}else{
		if (sensorState_8){
			sensorNumPoll_8--;
			if (!sensorNumPoll_8){
				sensorState_8 = false;
				sensorNumPoll_8 = SENSOR_GISTR_POLL;
			}
		}
	}

	if (SENSOR_STATE_9()){
		if (!sensorState_9){
			sensorNumPoll_9--;
			if (!sensorNumPoll_9){
				sensorState_9 = true;
				sensorNumPoll_9 = SENSOR_GISTR_POLL;

				if (sens_step_motor_4){
					sens_step_motor_4 = false;
					stop_motor_4();
				}
			}
		}
	}else{
		if (sensorState_9){
			sensorNumPoll_9--;
			if (!sensorNumPoll_9){
				sensorState_9 = false;
				sensorNumPoll_9 = SENSOR_GISTR_POLL;
			}
		}
	}

	if (SENSOR_STATE_10()){
		if (!sensorState_10){
			sensorNumPoll_10--;
			if (!sensorNumPoll_10){
				sensorState_10 = true;
				sensorNumPoll_10 = SENSOR_GISTR_POLL;
			}
		}
	}else{
		if (sensorState_10){
			sensorNumPoll_10--;
			if (!sensorNumPoll_10){
				sensorState_10 = false;
				sensorNumPoll_10 = SENSOR_GISTR_POLL;
			}
		}
	}

}


