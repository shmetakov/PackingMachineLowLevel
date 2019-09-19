#include "global.h"
#include "led.h"
#include "TestFile.h"
#include "sensor.h"
#include "magnet.h"
#include "stepMotors.h"
#include "protocol_For_Main_Motor.h"
#include "USART_For_Main_Motor.h"
#include "serviceProtocol.h"
#include "service_COM_port.h"
#include "mainCntrPrgm.h"
#include "USART_temp_com.h"
#include "prtlcTempCon.h"

volatile uint32_t __delay = 0;
uint32_t __delay_clear_motor_buf = 0;
uint32_t __delay_clear_USART_temp_con_buf = 0;
uint32_t __delay_wait_answer_main_motor = 0;
uint32_t __delay_wait_answer_service_COM = 0;
uint32_t __delay_wait_answer_USART_temp_con = 0;
uint32_t __time_for_init_main_motor = TIME_FOR_INIT_MOTOR;
uint32_t __time_for_init_sys = 0;
uint32_t __time_for_reset_RXTX_main_motor = 0;
uint32_t __time_for_packing = 0;
uint32_t __time_for_load_pellicle = 0;
uint32_t __time_for_setTemp = 0;

uint32_t tickErrorSens_6 = 1000;
uint32_t tickInitSensors = 100;
uint32_t tickLoadPallicle = 0;

extern bool isNewDataAvailableMainMotor;
extern bool isNewDataAvailableService;
extern bool isNewDataAvailableTempCon;
extern uint8_t MainMotorState;
extern uint8_t LastMainMotorState;
extern uint8_t buffer_Service_Send[BUFFER_SERVICE_SIZE];

//extern uint32_t TEST_tick;

uint8_t paramTemp_1 = 0;
uint8_t paramTemp_2 = 0;
uint8_t paramTemp_3 = 0;

uint8_t lastSensorMainMotor = 0;
uint8_t stateBroachLoadPallicle = 0;

extern uint32_t step_motor_1_first;
extern uint32_t step_motor_1_second;
extern uint32_t step_motor_2;

//*********************************************************************
//
//*********************************************************************

void Soft_Delay(volatile uint32_t number)
{
	__delay = number;
        while(__delay);
}

//*********************************************************************
//
//*********************************************************************

void checkErrors()
{
	if (!sensorState_7 || !sensorState_8){
		stop_motor_1();
		__STATE = __STATE_CRIT_ER;
		__ERROR = ERROR_SENS_ENV;
	}

	if (motor_2.isEnable && sens_step_motor_2){
		if (motor_2.num_steps > STEPS_STEP_MOTOR_2_MAX){
			stop_motor_2();
			__STATE = __STATE_CRIT_ER;
			__ERROR = ERROR_MOTOTR_2_RETURN_TO_HOME_POS;
		}
	}

	if (motor_3.isEnable && sens_step_motor_3){
		if (motor_3.num_steps > STEPS_STEP_MOTOR_3_MAX){
			stop_motor_3();
			__STATE = __STATE_CRIT_ER;
			__ERROR = ERROR_MOTOTR_3_RETURN_TO_HOME_POS;
		}
	}

	if (motor_4.isEnable && sens_step_motor_4){
		if (motor_4.num_steps > STEPS_STEP_MOTOR_4_MAX){
			stop_motor_4();
			__STATE = __STATE_CRIT_ER;
			__ERROR = ERROR_MOTOTR_4_RETURN_TO_HOME_POS;
		}
	}

	if (MAGNET_STATE_4() && sensorState_6){
		tickErrorSens_6--;
		if (!tickErrorSens_6){
			__STATE = __STATE_CRIT_ER;
			__ERROR = ERROR_SENS_6;
		}
	}else if (MAGNET_STATE_2() && MAGNET_STATE_3() && !sensorState_6){
		tickErrorSens_6--;
		if (!tickErrorSens_6){
			__STATE = __STATE_CRIT_ER;
			__ERROR = ERROR_SENS_6;
		}
	}else { tickErrorSens_6 = 1000; }

	if ((__STATE == __STATE_INIT) && (!sensorState_10)){
		__STATE = __STATE_CRIT_ER;
		__ERROR = ERROR_SENS_10;
	}

	if ((sensorState_1 && (sensorState_2 || sensorState_3)) ||
			(sensorState_2 && sensorState_3)){

		__STATE = __STATE_CRIT_ER;
		__ERROR = ERROR_SENS_MOTOR;
	}

	if (sensorState_2){
		if (lastSensorMainMotor == 1){
			__STATE = __STATE_CRIT_ER;
			__ERROR = ERROR_SENS_3;
		}
		lastSensorMainMotor = 2;
	}

	if (sensorState_1){
		if (lastSensorMainMotor == 3){
			__STATE = __STATE_CRIT_ER;
			__ERROR = ERROR_SENS_2;
		}
		lastSensorMainMotor = 1;
	}

	if (sensorState_3){
		if (lastSensorMainMotor == 2){
			__STATE = __STATE_CRIT_ER;
			__ERROR = ERROR_SENS_1;
		}
		lastSensorMainMotor = 3;
	}
}

//*********************************************************************
//
//*********************************************************************

void SysTick_Handler(void)
{
	poll_sens();
	magnet_delay_work();

	//TEST_tick++;

	if (tickInitSensors) tickInitSensors--;
	else checkErrors();

	if (__delay){
		__delay--;
	}

	if (__delay_clear_motor_buf){
		__delay_clear_motor_buf--;
		if (!__delay_clear_motor_buf){
			clearBuffer_For_Main_Motor();
		}
	}

	if (__delay_wait_answer_main_motor){
		__delay_wait_answer_main_motor--;
		if (!__delay_wait_answer_main_motor){
			MainMotorState = STATE_WAIT_ANSWER_ERRROR;
		}
	}

	if (__delay_wait_answer_service_COM){
		__delay_wait_answer_service_COM--;
		if (!__delay_wait_answer_service_COM){
			clearBuffer_For_Service_COM_port();
		}
	}

	if (__delay_wait_answer_USART_temp_con){
		__delay_wait_answer_USART_temp_con--;
		if (!__delay_wait_answer_USART_temp_con){
			cmd_sendTempCon();
		}
	}

	if (__delay_clear_USART_temp_con_buf){
		__delay_clear_USART_temp_con_buf--;
		if (!__delay_clear_USART_temp_con_buf){
			clearBuffer_USART_temp_con();
		}
	}

	if (__time_for_setTemp){
		__time_for_setTemp--;
		if (!__time_for_setTemp){
			cmd_sendTempCon();
		}
	}

	switch (__STATE){
		case __STATE_INIT:{

			switch (__TASK){
				case TASK_INIT_MOTOR:{

					if (__time_for_init_main_motor) __time_for_init_main_motor--;

					if (!__time_for_init_main_motor){
						stopWork();
						__STATE = __STATE_CRIT_ER;
						__ERROR = ERROR_INIT_MOTOR;
					}else if (init_Main_motor()){
						__TASK = TASK_INIT_SYS;
						__ERROR = ERROR_NO;
						__time_for_init_sys = TIME_FOR_INIT_SYS;
					}

					break;
				}

				case TASK_INIT_SYS:{

					if (__time_for_init_sys) __time_for_init_sys--;

					if (!__time_for_init_sys){
						stopWork();
						__STATE = __STATE_CRIT_ER;
						__ERROR = ERROR_INIT_SYS;
					}else if (init_System()){
						__time_for_init_sys = 0;
					}
					break;
				}

			}

			break;

		}

		case __STATE_PACKING:{
/*
			if (__time_for_packing && !sensorState_10) __time_for_packing--;

			if (!__time_for_packing){
				stopWork();
				__STATE = __STATE_CRIT_ER;
				__ERROR = ERROR_PACKING;
			}else{*/
				main_work_funk();
//			}

			break;

		}

		case __STATE_PACKING_END: {
			main_work_funk();
			break;
		}

		case __STATE_LOAD_PELLICLE:{

			switch (stateBroachLoadPallicle){
			case 0:{
				if (!motor_1.isEnable)
					start_StepMotor_1(SPEED_STEP_MOTOR_1, SPEED_UP_STEP_MOTOR_1, DIR_CLOCKWISE, 10000, false);
				else{
					stateBroachLoadPallicle = 1;
					tickLoadPallicle = 0;
				}
				break;
			}
			case 1:{

				if (!motor_1.isEnable){
					if (!tickLoadPallicle){
						MAGNET_4_ON();
					}else if (tickLoadPallicle == 150){
						MAGNET_1_ON();
					}else if (tickLoadPallicle == 300){
						start_StepMotor_1(SPEED_STEP_MOTOR_1, SPEED_UP_STEP_MOTOR_1, DIR_CLOCKWISE, 15000, false);
						stateBroachLoadPallicle = 2;
					}

					tickLoadPallicle++;
					break;
				}
				break;
			}

			case 2:{
				if (motor_1.num_steps > 10000){
					MAGNET_1_OFF();
					MAGNET_4_OFF();
					__STATE = __STATE_OK;
				}
				break;
			}
			default: break;
			}
			break;
		}
	}
}

//*********************************************************************
//
//*********************************************************************

inline void stopWork()
{
	if (!DMA_GetCurrDataCounter(DMA1_Stream6)) Motor_stop();

	MAGNET_1_OFF();
	MAGNET_2_OFF();
	MAGNET_3_OFF();
	MAGNET_4_OFF();
	MAGNET_5_OFF();
	MAGNET_6_OFF();
	MAGNET_7_OFF();
	MAGNET_8_OFF();

	stop_motor_1();
	stop_motor_2();
	stop_motor_3();
	stop_motor_4();
}

//*********************************************************************
//
//*********************************************************************

void startPacking(uint16_t l, uint16_t w, uint16_t h)
{
	if (__STATE == __STATE_OK || __STATE == __STATE_PACKING_END){

		if (l && w && h){
			step_motor_1_first = l + (h * 2) + 100;
			step_motor_1_first = (step_motor_1_first * 100) * 1.1;
			if (step_motor_1_first > 64000) step_motor_1_first = 64000;
			step_motor_1_second = (66000 - step_motor_1_first) / 2 + 1100;
		}else{
			step_motor_1_first = 0;
			step_motor_1_second = 0;
		}

		if (l){
			step_motor_2 = (185 - ((l/2) - 80)) * 143;
		}else{
			step_motor_2 = STEPS_STEP_MOTOR_2;
		}

		if (step_motor_2 > STEPS_STEP_MOTOR_2){
			step_motor_2 = STEPS_STEP_MOTOR_2;
		}

		__time_for_packing = TIME_FOR_PACKING;
		__TASK = TASK_START_PACKAGING;
		__STATE = __STATE_PACKING;

	}
}

inline void startInitSys()
{
	if (__STATE != __STATE_PACKING){
		MainMotorState = STATE_IS_NOT_INIT;
		LastMainMotorState = STATE_IS_NOT_INIT;
		__ERROR = ERROR_NO;
		__STATE = __STATE_INIT;
		__TASK = TASK_INIT_MOTOR;
	}
}

void loadPellicle()
{
	if (__STATE == __STATE_OK || __STATE == __STATE_PACKING_END){
		tickLoadPallicle = 0;
		stateBroachLoadPallicle = 0;
		__STATE = __STATE_LOAD_PELLICLE;
	}
}

//*********************************************************************
//
//*********************************************************************

void init_SystemsParam()
{
	SystemInit();
	SystemCoreClockUpdate();

	__enable_irq();

	LED_init();
	init_Magnet();
	init_Sensor();
	init_Motors_PIN();
	init_USART_For_Main_Motor();
	init_Service_COM_port();
	init_USART_temp_con();


	buffer_Service_Send[0] = 0x00;
	send_buffer_Service_COM_port(1);

	motorParams.dir = MAIN_MOTOR_DIR;
	motorParams.speed = MAIN_MOTOR_SPEED;
	motorParams.speed_up = MAIN_MOTOR_SPEED_UP;
	motorParams.speed_down  = MAIN_MOTOR_SPEED_DOWN;

	SysTick_Config(SystemCoreClock/1000);

	__STATE = __STATE_NO_INIT;
}

//*********************************************************************
//
//*********************************************************************

void init_IWDG()
{
	/* Enable the LSI OSC */
	RCC_LSICmd(ENABLE);
	/* Wait till LSI is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}

	/* Enable Watchdog*/
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32); // 4, 8, 16 ... 256
	IWDG_SetReload(10);//This parameter must be a number between 0 and 0x0FFF.
	IWDG_ReloadCounter();
	IWDG_Enable();
}

//*********************************************************************
//
//*********************************************************************

int main(void)
{
	init_IWDG();
	init_SystemsParam();

	ON_LED_BLUE();
	cmd_sendTempCon();

    while(1)
    {
    	IWDG_ReloadCounter();

    	if (isNewDataAvailableService){
    		if (!DMA_GetCurrDataCounter(DMA1_Stream3)){
    			DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);
        		isNewDataAvailableService = false;
        		parsingPackageService();
    		}
    	}

    	if (isNewDataAvailableMainMotor){
    		parsingPackageRS232();
    		isNewDataAvailableMainMotor = false;
    	}

    	if (isNewDataAvailableTempCon){
    		isNewDataAvailableTempCon = false;
    		parsingPackageTempCon();
    	}

		calculate_param();

		if (__STATE == __STATE_OK ||
				__STATE == __STATE_PACKING_END){
			OFF_LED_BLUE();
			OFF_LED_ORANGE();
			OFF_LED_RED();
			ON_LED_GREEN();
		}

		if (__STATE == __STATE_PACKING){
			OFF_LED_BLUE();
			ON_LED_ORANGE();
			OFF_LED_RED();
			OFF_LED_GREEN();
		}

		if (__STATE == __STATE_CRIT_ER){
			stopWork();
			OFF_LED_BLUE();
			OFF_LED_GREEN();
			OFF_LED_ORANGE();
			ON_LED_RED();
		}

		if (__STATE == __STATE_INIT ||
				__STATE == __STATE_INIT_MOTOR){
			ON_LED_BLUE();
			OFF_LED_GREEN();
			OFF_LED_ORANGE();
			OFF_LED_RED();
		}
    }
}
