#include "global.h"
#include "stepMotors.h"

//*********************************************************************
// TIM6 - 84 mHz
//*********************************************************************

void init_TIM_motors()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	TIM6->PSC = 21 - 1; //250 ns
	TIM6->ARR = 11; //Отработка прерывания раз в 3 us
	TIM6->DIER |= TIM_DIER_UIE; //Разрешения прерывание от таймера
	TIM6->CR1 |= TIM_CR1_CEN; //Запуск таймера
}

//*********************************************************************
//
//*********************************************************************

void init_Motors_PIN()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	motors_pin_init.GPIO_Mode = GPIO_Mode_OUT;
	motors_pin_init.GPIO_OType = GPIO_OType_PP;
	motors_pin_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	motors_pin_init.GPIO_Speed = GPIO_Speed_2MHz;

	motors_pin_init.GPIO_Pin = STEP_MOTOR_PIN_EN_1;
	GPIO_Init(STEP_MOTOR_GPIOx_EN_1, &motors_pin_init);
	motors_pin_init.GPIO_Pin = STEP_MOTOR_PIN_EN_2;
	GPIO_Init(STEP_MOTOR_GPIOx_EN_2, &motors_pin_init);
	motors_pin_init.GPIO_Pin = STEP_MOTOR_PIN_EN_3;
	GPIO_Init(STEP_MOTOR_GPIOx_EN_3, &motors_pin_init);
	motors_pin_init.GPIO_Pin = STEP_MOTOR_PIN_EN_4;
	GPIO_Init(STEP_MOTOR_GPIOx_EN_4, &motors_pin_init);

	motors_pin_init.GPIO_Pin = STEP_MOTOR_PIN_DIR_1;
	GPIO_Init(STEP_MOTOR_GPIOx_DIR_1, &motors_pin_init);
	motors_pin_init.GPIO_Pin = STEP_MOTOR_PIN_DIR_2;
	GPIO_Init(STEP_MOTOR_GPIOx_DIR_2, &motors_pin_init);
	motors_pin_init.GPIO_Pin = STEP_MOTOR_PIN_DIR_3;
	GPIO_Init(STEP_MOTOR_GPIOx_DIR_3, &motors_pin_init);
	motors_pin_init.GPIO_Pin = STEP_MOTOR_PIN_DIR_4;
	GPIO_Init(STEP_MOTOR_GPIOx_DIR_4, &motors_pin_init);

	motors_pin_init.GPIO_Speed = GPIO_Speed_100MHz;

	motors_pin_init.GPIO_Pin = STEP_MOTOR_PIN_STEP_1;
	GPIO_Init(STEP_MOTOR_GPIOx_STEP_1, &motors_pin_init);
	motors_pin_init.GPIO_Pin = STEP_MOTOR_PIN_STEP_2;
	GPIO_Init(STEP_MOTOR_GPIOx_STEP_2, &motors_pin_init);
	motors_pin_init.GPIO_Pin = STEP_MOTOR_PIN_STEP_3;
	GPIO_Init(STEP_MOTOR_GPIOx_STEP_3, &motors_pin_init);
	motors_pin_init.GPIO_Pin = STEP_MOTOR_PIN_STEP_4;
	GPIO_Init(STEP_MOTOR_GPIOx_STEP_4, &motors_pin_init);

	GPIO_SetBits(STEP_MOTOR_GPIOx_EN_1, STEP_MOTOR_PIN_EN_1);
	GPIO_SetBits(STEP_MOTOR_GPIOx_EN_2, STEP_MOTOR_PIN_EN_2);
	GPIO_SetBits(STEP_MOTOR_GPIOx_EN_3, STEP_MOTOR_PIN_EN_3);
	GPIO_SetBits(STEP_MOTOR_GPIOx_EN_4, STEP_MOTOR_PIN_EN_4);

	motor_1.isEnable = 0;
	motor_2.isEnable = 0;
	motor_3.isEnable = 0;
	motor_4.isEnable = 0;

	sens_step_motor_1 = false;
	sens_step_motor_2 = false;
	sens_step_motor_3 = false;
	sens_step_motor_4 = false;

	init_TIM_motors();

	NVIC_EnableIRQ(TIM6_DAC_IRQn); //Разрешение TIM6_DAC_IRQn прерывания
}

//*********************************************************************
//
//*********************************************************************

void TIM6_DAC_IRQHandler(void)
{
	TIM6->SR &= ~TIM_SR_UIF;

	if (motor_1.isEnable){
		motor_1.tick_up_speed++;
		motor_1.tick++;

		if (motor_1.tick == motor_1.speed){
			motor_1.num_steps++;
			motor_1.isOverflow = true;
			motor_1.tick = 0;
			GPIO_ResetBits(STEP_MOTOR_GPIOx_STEP_1, STEP_MOTOR_PIN_STEP_1);
		}else if (motor_1.tick == 2){
			GPIO_SetBits(STEP_MOTOR_GPIOx_STEP_1, STEP_MOTOR_PIN_STEP_1);
		}
	}

	if (motor_2.isEnable){
		motor_2.tick_up_speed++;
		motor_2.tick++;

		if (motor_2.tick == motor_2.speed){
			motor_2.num_steps++;
			motor_2.isOverflow = true;
			motor_2.tick = 0;
			GPIO_ResetBits(STEP_MOTOR_GPIOx_STEP_2, STEP_MOTOR_PIN_STEP_2);
		}else if (motor_2.tick == 2){
			GPIO_SetBits(STEP_MOTOR_GPIOx_STEP_2, STEP_MOTOR_PIN_STEP_2);
		}
	}

	if (motor_3.isEnable){
		motor_3.tick_up_speed++;
		motor_3.tick++;

		if (motor_3.tick == motor_3.speed){
			motor_3.num_steps++;
			motor_3.isOverflow = true;
			motor_3.tick = 0;
			GPIO_ResetBits(STEP_MOTOR_GPIOx_STEP_3, STEP_MOTOR_PIN_STEP_3);
		}else if (motor_3.tick == 2){
			GPIO_SetBits(STEP_MOTOR_GPIOx_STEP_3, STEP_MOTOR_PIN_STEP_3);
		}
	}

	if (motor_4.isEnable){
		motor_4.tick_up_speed++;
		motor_4.tick++;

		if (motor_4.tick == motor_4.speed){
			motor_4.num_steps++;
			motor_4.isOverflow = true;
			motor_4.tick = 0;
			GPIO_ResetBits(STEP_MOTOR_GPIOx_STEP_4, STEP_MOTOR_PIN_STEP_4);
		}else if (motor_4.tick == 2){
			GPIO_SetBits(STEP_MOTOR_GPIOx_STEP_4, STEP_MOTOR_PIN_STEP_4);
		}
	}

}

//*********************************************************************
//
//*********************************************************************

inline void calculate_param()
{
	/*if (!motor_1.isEnable && !motor_2.isEnable && !motor_3.isEnable && !motor_4.isEnable){
		NVIC_DisableIRQ(TIM6_DAC_IRQn);
		return;
	}*/

	if (motor_1.isEnable){
		if ((motor_1.num_steps > motor_1.target_num_steps) && motor_1.target_num_steps){
			GPIO_SetBits(STEP_MOTOR_GPIOx_EN_1, STEP_MOTOR_PIN_EN_1);
			motor_1.isEnable = 0;
		}else if (motor_1.isOverflow){
			if (motor_1.tick_up_speed >= motor_1.up_speed){
				motor_1.tick_up_speed = 0;
				if (motor_1.speed > motor_1.target_speed){
					motor_1.speed--;
				}
			}
			motor_1.isOverflow = false;
		}
	}

	if (motor_2.isEnable){
		if ((motor_2.num_steps > motor_2.target_num_steps) && motor_2.target_num_steps){
			GPIO_SetBits(STEP_MOTOR_GPIOx_EN_2, STEP_MOTOR_PIN_EN_2);
			motor_2.isEnable = 0;
		}else if (motor_2.isOverflow){
			if (motor_2.tick_up_speed >= motor_2.up_speed){
				motor_2.tick_up_speed = 0;
				if (motor_2.speed > motor_2.target_speed){
					motor_2.speed--;
				}
			}
			motor_2.isOverflow = false;
		}
	}

	if (motor_3.isEnable){
		if ((motor_3.num_steps > motor_3.target_num_steps) && motor_3.target_num_steps){
			GPIO_SetBits(STEP_MOTOR_GPIOx_EN_3, STEP_MOTOR_PIN_EN_3);
			motor_3.isEnable = 0;
		}else if (motor_3.isOverflow){
			if (motor_3.tick_up_speed >= motor_3.up_speed){
				motor_3.tick_up_speed = 0;
				if (motor_3.speed > motor_3.target_speed){
					motor_3.speed--;
				}
			}
			motor_3.isOverflow = false;
		}
	}

	if (motor_4.isEnable){
		if ((motor_4.num_steps > motor_4.target_num_steps) && motor_4.target_num_steps){
			GPIO_SetBits(STEP_MOTOR_GPIOx_EN_4, STEP_MOTOR_PIN_EN_4);
			motor_4.isEnable = 0;
		}else if (motor_4.isOverflow){
			if (motor_4.tick_up_speed >= motor_4.up_speed){
				motor_4.tick_up_speed = 0;
				if (motor_4.speed > motor_4.target_speed){
					motor_4.speed--;
				}
			}
			motor_4.isOverflow = false;
		}
	}
}

//*********************************************************************
//
//*********************************************************************

void start_StepMotor_1(uint32_t speed, uint32_t up_speed, uint8_t dir, uint64_t steps, _Bool sens)
{
	motor_1.target_num_steps = steps;
	motor_1.num_steps = 0;
	motor_1.speed = (1000000/(START_SPEED * STEP_ON_REV))/3;
	motor_1.target_speed = (1000000/(speed * STEP_ON_REV))/3;
	motor_1.up_speed = (up_speed)/3 ;
	motor_1.tick_up_speed = 0;
	motor_1.kof_up_speed = 0;
	motor_1.isEnable = 1;
	motor_1.tick = 0;
	sens_step_motor_1 = sens;
	motor_1.isOverflow = false;

	if (dir == DIR_CLOCKWISE){
		GPIO_ResetBits(STEP_MOTOR_GPIOx_DIR_1, STEP_MOTOR_PIN_DIR_1);
	}else{
		GPIO_SetBits(STEP_MOTOR_GPIOx_DIR_1, STEP_MOTOR_PIN_DIR_1);
	}

	GPIO_ResetBits(STEP_MOTOR_GPIOx_EN_1, STEP_MOTOR_PIN_EN_1);

	NVIC_EnableIRQ(TIM6_DAC_IRQn); //Разрешение TIM6_DAC_IRQn прерывания
}

//*********************************************************************
//
//*********************************************************************

void start_StepMotor_2(uint32_t speed, uint32_t up_speed, uint8_t dir, uint64_t steps, _Bool sens)
{
	motor_2.target_num_steps = steps;
	motor_2.num_steps = 0;
	motor_2.speed = (1000000/(START_SPEED * STEP_ON_REV))/3;
	motor_2.target_speed = (1000000/(speed * STEP_ON_REV))/3;
	motor_2.up_speed = (up_speed)/3 ;
	motor_2.tick_up_speed = 0;
	motor_2.kof_up_speed = 0;
	motor_2.isEnable = 1;
	motor_2.tick = 0;
	sens_step_motor_2 = sens;
	motor_2.isOverflow = false;

	if (dir == DIR_CLOCKWISE){
		GPIO_ResetBits(STEP_MOTOR_GPIOx_DIR_2, STEP_MOTOR_PIN_DIR_2);
	}else{
		GPIO_SetBits(STEP_MOTOR_GPIOx_DIR_2, STEP_MOTOR_PIN_DIR_2);
	}

	GPIO_ResetBits(STEP_MOTOR_GPIOx_EN_2, STEP_MOTOR_PIN_EN_2);

	NVIC_EnableIRQ(TIM6_DAC_IRQn); //Разрешение TIM6_DAC_IRQn прерывания
}

//*********************************************************************
//
//*********************************************************************

void start_StepMotor_3(uint32_t speed, uint32_t up_speed, uint8_t dir, uint64_t steps, _Bool sens)
{
	motor_3.target_num_steps = steps;
	motor_3.num_steps = 0;
	motor_3.speed = (1000000/(START_SPEED * STEP_ON_REV))/3;
	motor_3.target_speed = (1000000/(speed * STEP_ON_REV))/3;
	motor_3.up_speed = (up_speed)/3 ;
	motor_3.tick_up_speed = 0;
	motor_3.kof_up_speed = 0;
	motor_3.isEnable = 1;
	motor_3.tick = 0;
	sens_step_motor_3 = sens;
	motor_3.isOverflow = false;

	if (dir == DIR_CLOCKWISE){
		GPIO_ResetBits(STEP_MOTOR_GPIOx_DIR_3, STEP_MOTOR_PIN_DIR_3);
	}else{
		GPIO_SetBits(STEP_MOTOR_GPIOx_DIR_3, STEP_MOTOR_PIN_DIR_3);
	}

	GPIO_ResetBits(STEP_MOTOR_GPIOx_EN_3, STEP_MOTOR_PIN_EN_3);

	NVIC_EnableIRQ(TIM6_DAC_IRQn); //Разрешение TIM6_DAC_IRQn прерывания
}

//*********************************************************************
//
//*********************************************************************

void start_StepMotor_4(uint32_t speed, uint32_t up_speed, uint8_t dir, uint64_t steps, _Bool sens)
{
	motor_4.target_num_steps = steps;
	motor_4.num_steps = 0;
	motor_4.speed = (1000000/(START_SPEED * STEP_ON_REV))/3;
	motor_4.target_speed = (1000000/(speed * STEP_ON_REV))/3;
	motor_4.up_speed = (up_speed)/3 ;
	motor_4.tick_up_speed = 0;
	motor_4.kof_up_speed = 0;
	motor_4.isEnable = 1;
	motor_4.tick = 0;
	sens_step_motor_4 = sens;
	motor_4.isOverflow = false;

	if (dir == DIR_CLOCKWISE){
		GPIO_ResetBits(STEP_MOTOR_GPIOx_DIR_4, STEP_MOTOR_PIN_DIR_4);
	}else{
		GPIO_SetBits(STEP_MOTOR_GPIOx_DIR_4, STEP_MOTOR_PIN_DIR_4);
	}

	GPIO_ResetBits(STEP_MOTOR_GPIOx_EN_4, STEP_MOTOR_PIN_EN_4);

	NVIC_EnableIRQ(TIM6_DAC_IRQn); //Разрешение TIM6_DAC_IRQn прерывания
}

//*********************************************************************
//
//*********************************************************************

inline uint64_t stop_motor_1()
{
	GPIO_SetBits(STEP_MOTOR_GPIOx_EN_1, STEP_MOTOR_PIN_EN_1);
	motor_1.isEnable = 0;
	return motor_1.num_steps;
}

//*********************************************************************
//
//*********************************************************************

inline uint64_t stop_motor_2()
{
	GPIO_SetBits(STEP_MOTOR_GPIOx_EN_2, STEP_MOTOR_PIN_EN_2);
	motor_2.isEnable = 0;
	return motor_2.num_steps;
}

//*********************************************************************
//
//*********************************************************************

inline uint64_t stop_motor_3()
{
	GPIO_SetBits(STEP_MOTOR_GPIOx_EN_3, STEP_MOTOR_PIN_EN_3);
	motor_3.isEnable = 0;
	return motor_3.num_steps;
}

//*********************************************************************
//
//*********************************************************************

inline uint64_t stop_motor_4()
{
	GPIO_SetBits(STEP_MOTOR_GPIOx_EN_4, STEP_MOTOR_PIN_EN_4);
	motor_4.isEnable = 0;
	return motor_4.num_steps;
}






