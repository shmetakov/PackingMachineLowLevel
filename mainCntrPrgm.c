#include "global.h"
#include "sensor.h"
#include "magnet.h"
#include "stepMotors.h"
#include "mainCntrPrgm.h"
#include "protocol_For_Main_Motor.h"
#include "led.h"

uint32_t ticks = 0;
uint8_t flag_1 = 0;

uint32_t step_motor_1_first = 0;
uint32_t step_motor_1_second = 0;
uint32_t step_motor_2 = STEPS_STEP_MOTOR_2;

/*uint32_t TEST_tick = 0;
uint32_t TEST_var_1 = 0;
uint32_t TEST_var_2 = 0;
uint32_t TEST_var_3 = 0;
uint32_t TEST_var_4 = 0;
uint32_t TEST_var_5 = 0;
uint32_t TEST_var_6 = 0;
uint32_t TEST_var_7 = 0;
uint32_t TEST_var_8 = 0;
uint32_t TEST_var_9 = 0;
*/
bool isBroach = false;
uint8_t stateBroach = 0;

extern uint32_t __delay_wait_answer_main_motor;

inline bool init_System()
{
	if (sensorState_7 && sensorState_8){

		if (!sensorState_2){
			if (!__delay_wait_answer_main_motor && !isMainMotorStart){
				Motor_start();
			}

			return false;
		}else if (!__delay_wait_answer_main_motor && isMainMotorStart){
			Motor_stop();
			return false;
		}

		if (!sensorState_4 && !motor_2.isEnable){
			start_StepMotor_2(SPEED_STEP_MOTOR_2, SPEED_UP_STEP_MOTOR_2, DIR_CLOCKWISE, 0, true);
		}
		if (!sensorState_5 && !motor_3.isEnable){
			start_StepMotor_3(SPEED_STEP_MOTOR_3, SPEED_UP_STEP_MOTOR_3, DIR_CLOCKWISE, 0, true);
		}
		if (!sensorState_9 && !motor_4.isEnable){
			start_StepMotor_4(SPEED_STEP_MOTOR_4, SPEED_UP_STEP_MOTOR_4, DIR_CLOCKWISE, 0, true);
		}

	}else{
		stopWork();
		__STATE = __STATE_CRIT_ER;
		__ERROR = ERROR_SENS_ENV;
		return true;
	}

	if (sensorState_2 && sensorState_4 && sensorState_5 && sensorState_9 && sensorState_10){
		__STATE = __STATE_OK;
		__ERROR = ERROR_NO;
		return  true;
	}

	return false;
}


inline void main_work_funk()
{
	if (__TASK == NO_TASK){
		__STATE = __STATE_PACKING_END;
		if (sensorState_2){
			if (!__delay_wait_answer_main_motor && isMainMotorStart){
				Motor_stop();
			}
		}

		return;
	}

	switch(__TASK){
	case TASK_START_PACKAGING: {
		if (!motor_1.isEnable && !motor_4.isEnable){
			__TASK = TASK_GOODS_UP;
			//TEST_tick = 0;
		}

		if (sensorState_2){
			if (!__delay_wait_answer_main_motor && isMainMotorStart){
				Motor_stop();
			}
		}

		break;
	}

	case TASK_GOODS_UP:{

		if (sensorState_10){
			if (!sensorState_9 && !motor_4.isEnable){
				//TEST_var_1 = TEST_tick;
				start_StepMotor_4(SPEED_STEP_MOTOR_4, SPEED_UP_STEP_MOTOR_4, DIR_CLOCKWISE, 0, true);
			}
			MAGNET_5_ON();
			MAGNET_6_ON();
			MAGNET_7_ON();
			MAGNET_8_ON();

			if (!__delay_wait_answer_main_motor && !isMainMotorStart){
				Motor_start();
			}

			if (isMainMotorStart){
				__TASK = TASK_PACKAGING;
				//TEST_var_2 = TEST_tick;
			}
		}else if (!__delay_wait_answer_main_motor && isMainMotorStart){
			Motor_stop();
		}

		break;
	}

	case TASK_PACKAGING:{
		if (sensorState_1){

			start_StepMotor_2(SPEED_STEP_MOTOR_2, SPEED_UP_STEP_MOTOR_2, DIR_ANTICLOCKWISE, step_motor_2, false);
			__TASK = TASK_FINICH_PACKAGING;
			//TEST_var_3 = TEST_tick;
			break;
		}
		break;
	}

	case TASK_FINICH_PACKAGING:{
		if (motor_2.num_steps > (STEPS_STEP_MOTOR_2/2)){
			MAGNET_6_OFF();
			MAGNET_8_OFF();
		}

		if (!motor_2.isEnable){
			MAGNET_6_OFF();
			MAGNET_7_OFF();
			MAGNET_8_OFF();
		}

		if (!sensorState_1){
			if (!motor_3.isEnable)
				start_StepMotor_3(SPEED_STEP_MOTOR_3, SPEED_UP_STEP_MOTOR_3, DIR_ANTICLOCKWISE, STEPS_STEP_MOTOR_3, false);
			//TEST_var_4 = TEST_tick;
		}


		if (sensorState_3){
			MAGNET_6_OFF();
			MAGNET_7_OFF();
			MAGNET_8_OFF();
			MAGNET_5_OFF();

			if (!motor_3.isEnable)
				start_StepMotor_3(SPEED_STEP_MOTOR_3, SPEED_UP_STEP_MOTOR_3, DIR_ANTICLOCKWISE, STEPS_STEP_MOTOR_3, false);

			//TEST_var_5 = TEST_tick;
			__TASK = TASK_END;
		}
		break;
	}

	case TASK_END:{
		if (!sensorState_3 && !motor_2.isEnable && !sensorState_4){
			start_StepMotor_2(SPEED_STEP_MOTOR_2, SPEED_UP_STEP_MOTOR_2, DIR_CLOCKWISE, 0, true);
			//TEST_var_6 = TEST_tick;
		}

		if (!motor_3.isEnable && !sensorState_5){
			start_StepMotor_3(SPEED_STEP_MOTOR_3, SPEED_UP_STEP_MOTOR_3, DIR_CLOCKWISE, 0, true);
			//TEST_var_7 = TEST_tick;
		}

		if (!sensorState_3 && !motor_4.isEnable && sensorState_9){
			start_StepMotor_4(SPEED_STEP_MOTOR_4, SPEED_UP_STEP_MOTOR_4, DIR_ANTICLOCKWISE, STEPS_STEP_MOTOR_4, false);
			//TEST_var_8 = TEST_tick;
		}

		if (step_motor_1_first && !isBroach){
			isBroach = true;
			stateBroach = 0;
			ticks = 0;
		}

		if (sensorState_2){
			if (!__delay_wait_answer_main_motor && isMainMotorStart){
				Motor_stop();
			}
		}

		if (!sensorState_3 && sensorState_4 && sensorState_5 && !motor_4.isEnable && !isBroach){
			__TASK = NO_TASK;
			//TEST_var_9 = TEST_tick;
		}

		break;
	}

	}

	if (isBroach){

		switch (stateBroach){
		case 0:{

			if (!ticks){
				MAGNET_2_ON();
				MAGNET_3_ON();
				if (!sensorState_6){
					ticks = 0;
				}
			}else if (ticks == 100){
				start_StepMotor_1(SPEED_STEP_MOTOR_1, SPEED_UP_STEP_MOTOR_1, DIR_CLOCKWISE, step_motor_1_first, false);
			}else if ((ticks > 100) && (motor_1.num_steps > 5000)){
				stateBroach = 1;
				MAGNET_2_OFF();
				MAGNET_3_OFF();
				ticks = 0;
				break;
			}

			ticks++;
			break;
		}
		case 1:{

			if (!motor_1.isEnable){
				if (!ticks){
					MAGNET_4_ON();
				}else if (ticks == 150){
					MAGNET_1_ON();
				}else if (ticks == 300){
					start_StepMotor_1(SPEED_STEP_MOTOR_1, SPEED_UP_STEP_MOTOR_1, DIR_CLOCKWISE, step_motor_1_second, false);
					stateBroach = 2;
				}

				ticks++;
				break;
			}
			break;
		}

		case 2:{
			if (motor_1.num_steps > step_motor_1_second / 2){
				MAGNET_1_OFF();
				MAGNET_4_OFF();
				isBroach = false;
				step_motor_1_first = 0;
				step_motor_1_second = 0;
			}
			break;
		}
		default: break;
		}
	}

}
