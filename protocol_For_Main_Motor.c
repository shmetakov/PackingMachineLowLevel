#include "global.h"
#include "USART_For_Main_Motor.h"
#include "protocol_For_Main_Motor.h"
#include "stdbool.h"

bool isNewDataAvailableMainMotor = false;
uint8_t storedDataMainMotor[BUFFER_MAIN_MOTOR_SIZE];
uint8_t buffer_Motor_Send[BUFFER_MAIN_MOTOR_SIZE];

uint8_t MainMotorState = STATE_IS_NOT_INIT;
uint8_t LastMainMotorState = STATE_IS_NOT_INIT;
extern uint32_t __delay_wait_answer_main_motor;
extern uint32_t __time_for_init_main_motor;

uint32_t numRegWithoutAnswer = 0;

bool isMainMotorStart = false;

void send_package_For_Main_Motor(uint8_t* package);

//********************************************************************
//
//********************************************************************

void wait_answer(uint8_t ms)
{
	__delay_wait_answer_main_motor = ms;
	LastMainMotorState = MainMotorState;
	MainMotorState = STATE_WAIT_ANSWER;
}

//********************************************************************
//
//********************************************************************

uint8_t crc(uint8_t inData, uint8_t seed)
{
	uint8_t bitsLeft = 0;
	uint8_t temp = 0;

	for(bitsLeft = 8; bitsLeft > 0; bitsLeft--){
		temp =((seed^inData)&0x01);
		if(temp == 0){
			seed >>= 1;
		}else{
			seed ^=0x18;
			seed >>= 1;
			seed |= 0x80;
		}
		inData >>= 1;
	}
	return seed;
}

//********************************************************************
//
//********************************************************************

uint8_t crc_buffer_main_motor(uint8_t *buf) // дл¤ подсчета контрольной суммы
{
	uint8_t var = 0x00;
    for (int i = 1; i < MAIN_MOTOR_R_CRC_BYTE; i++){
    	var = crc(buf[i], var);
    }
    return var;
}

//********************************************************************
//
//********************************************************************

void clearBuffer_Motor_Send() 	{ memset(buffer_Motor_Send, 0x00, BUFFER_MAIN_MOTOR_SIZE); }
void clearStoredDataMainMotor() { memset(storedDataMainMotor, 0x00, BUFFER_MAIN_MOTOR_SIZE); }

//********************************************************************
//
//********************************************************************

void saveDataMainMotor(uint8_t* packege)
{
	for (uint8_t i = 0; i < BUFFER_MAIN_MOTOR_SIZE; i++)
		storedDataMainMotor[i] = packege[i];

	numRegWithoutAnswer = 0;
	isNewDataAvailableMainMotor = true;
	__delay_wait_answer_main_motor = 0;
}

//********************************************************************
//
//********************************************************************

void parsingPackageRS232() //разбор прин¤того пакета
{
	switch (storedDataMainMotor[MAIN_MOTOR_R_CMD_BYTE])
	{
		case STATE_SPEED_UP_IS_SET:
		case STATE_SPEED_DOWN_IS_SET:
		case STATE_DIR_IS_SET:
		case STATE_SPEED_IS_SET:
			MainMotorState = storedDataMainMotor[MAIN_MOTOR_R_CMD_BYTE];
			break;

		case STATE_MOTOR_IS_START:{
			isMainMotorStart = true;
			MainMotorState = storedDataMainMotor[MAIN_MOTOR_R_CMD_BYTE];
			break;
		}

		case STATE_MOTOR_IS_STOP:{
			isMainMotorStart = false;
			MainMotorState = STATE_IS_NOT_INIT;
			LastMainMotorState = STATE_IS_NOT_INIT;
			//MainMotorState = storedDataMainMotor[MAIN_MOTOR_R_CMD_BYTE];
			break;
		}

		default: break;
	}
}

//********************************************************************
//
//********************************************************************

void buildPackageMainMotor(uint8_t data, uint8_t cmd)
{
	numRegWithoutAnswer++;

	if (numRegWithoutAnswer > NUM_REG){
		__STATE = __STATE_CRIT_ER;
		__ERROR = ERROR_INIT_MOTOR;
		MainMotorState = STATE_IS_NOT_INIT;
		LastMainMotorState = STATE_IS_NOT_INIT;
	}

	clearBuffer_Motor_Send();
	buffer_Motor_Send[0] = 0xE6;
	buffer_Motor_Send[1] = ADR_MOTOR;
	buffer_Motor_Send[2] = cmd;
	buffer_Motor_Send[3] = data;
	buffer_Motor_Send[4] = crc_buffer_main_motor(buffer_Motor_Send);
	send_package_For_Main_Motor(buffer_Motor_Send);
	clearStoredDataMainMotor();
}

//********************************************************************
//
//********************************************************************

inline bool init_Main_motor()
{
	if (MainMotorState == STATE_WAIT_ANSWER){
		return false;
	}

	if (MainMotorState == STATE_WAIT_ANSWER_ERRROR){
		MainMotorState = LastMainMotorState;
	}

	uint8_t speed_byte = motorParams.speed/60;
	switch(MainMotorState){
		case STATE_IS_NOT_INIT:{
			buildPackageMainMotor(motorParams.speed_up, 0xA5);
			wait_answer(100);
			break;
		}
		case STATE_SPEED_UP_IS_SET:{
			buildPackageMainMotor(motorParams.speed_down, 0xA6);
			wait_answer(100);
			break;
		}
		case STATE_SPEED_DOWN_IS_SET:{
			buildPackageMainMotor(motorParams.dir, 0xA7);
			wait_answer(100);
			break;
		}
		case STATE_DIR_IS_SET:{
			buildPackageMainMotor(speed_byte, 0xA3);
			wait_answer(100);
			break;
		}
		case STATE_SPEED_IS_SET: return true;
		default: return true;

	}

	return false;
}

//********************************************************************
//
//********************************************************************

void Motor_start()
{
	if (init_Main_motor()){
		if (MainMotorState != STATE_MOTOR_IS_START){
			buildPackageMainMotor(0x00, 0x51);
			wait_answer(100);
		}
	}

}

void Motor_stop()
{
	if (MainMotorState != STATE_WAIT_ANSWER){
		buildPackageMainMotor(0x00, 0x52);
		wait_answer(100);
	}
}
