#include "global.h"
#include "serviceProtocol.h"
#include "service_COM_port.h"
#include "sensor.h"
#include "magnet.h"
#include "stepMotors.h"
#include "protocol_For_Main_Motor.h"

bool isNewDataAvailableService = false;
uint8_t storedDataService[BUFFER_SERVICE_SIZE];
uint8_t buffer_Service_Send[BUFFER_SERVICE_SIZE];

extern uint8_t MainMotorState;
extern uint32_t __time_for_init_main_motor;
extern uint8_t paramTemp_1;
extern uint8_t currentTemp_1;

//********************************************************************
//
//********************************************************************

void service_cmd_send_status();
void service_cmd_get_temp();
void service_cmd_start_step_motor_by_step();
void service_cmd_start_step_motor_to_sensor();
void service_cmd_stop_step_motor();
void service_cmd_set_state_magnet();
void service_cmd_get_magnet_state();
void service_cmd_get_sensor_state();
void service_cmd_main_motor_start();
void service_cmd_main_motor_init();
void service_cmd_main_motor_stop();

void main_cmd_start_paking();
void main_cmd_init();
void main_cmd_set_param();
void main_cmd_reset();
void main_cmd_load_pellicle();

//********************************************************************
//
//********************************************************************

inline uint8_t xorService_COM(uint8_t *buf, uint8_t len) // дл¤ подсчета контрольной суммы
{
	uint8_t xor = 0;
	for(uint8_t i = 1; i < len; i++)
	{
		xor^=buf[i];
	}
	return xor;
}

//********************************************************************
//
//********************************************************************

void clearBuffer_Service_Send() { memset(buffer_Service_Send, 0x00, BUFFER_SERVICE_SIZE); }
void clearStoredDataService() 	{ memset(storedDataService, 0x00, BUFFER_SERVICE_SIZE); }

//********************************************************************
//
//********************************************************************

inline void saveDataService(uint8_t* packege)
{
	for (uint8_t i = 0; i < BUFFER_SERVICE_SIZE; i++)
		storedDataService[i] = packege[i];

	isNewDataAvailableService = true;
}

//********************************************************************
//
//********************************************************************

void parsingPackageService()
{
	switch (storedDataService[SERVICE_CMD_BYTE])
	{
		case 0x00: break;
		case 0x31: service_cmd_send_status(); 						break;
		case 0x34: service_cmd_get_temp(); 							break;
		case 0x41: service_cmd_start_step_motor_by_step(); 			break;
		case 0x42: service_cmd_start_step_motor_to_sensor(); 		break;
		case 0x45: service_cmd_stop_step_motor(); 					break;
		case 0x51: service_cmd_set_state_magnet(); 					break;
		case 0x32: service_cmd_get_magnet_state(); 					break;
		case 0x33: service_cmd_get_sensor_state(); 					break;
		case 0x61: service_cmd_main_motor_start(); 					break;
		case 0x62: service_cmd_main_motor_init(); 					break;
		case 0x65: service_cmd_main_motor_stop(); 					break;

		case 0x71: main_cmd_start_paking();     					break;
		case 0x72: main_cmd_load_pellicle();						break;
		case 0x73: main_cmd_init();									break;
		case 0x74: main_cmd_reset();								break;
		case 0x82: main_cmd_set_param();							break;
	}
}

//********************************************************************
//
//********************************************************************

void buildPackageServiceCOM(uint8_t* data, uint8_t cmd, uint8_t len)
{
	clearBuffer_Service_Send();
	buffer_Service_Send[0] = 0x02;
	buffer_Service_Send[1] = len;
	buffer_Service_Send[2] = cmd;

	for (uint8_t i = 0; i < (len-1); ++i)
		buffer_Service_Send[3 + i] = data[i];

	buffer_Service_Send[2 + len] = xorService_COM(buffer_Service_Send, len + 2);
	send_buffer_Service_COM_port(len + 3);
	clearStoredDataService();
	//__ERROR = ERROR_NO;
}

//********************************************************************
//
//********************************************************************

void service_cmd_send_status()
{
	uint8_t data[2];
	data[0] = __STATE;
	data[1] = __ERROR;
	buildPackageServiceCOM(data, storedDataService[SERVICE_CMD_BYTE], 3);
}

//********************************************************************
//
//********************************************************************

void service_cmd_get_temp()
{
	uint8_t data[3];
	data[0] = __STATE;
	data[1] = __ERROR;
	data[2] = currentTemp_1;
	buildPackageServiceCOM(data, storedDataService[SERVICE_CMD_BYTE], 4);
}

//********************************************************************
//
//********************************************************************

void service_cmd_start_step_motor_by_step()
{
	if (__STATE != __STATE_PACKING){
		uint32_t speed = storedDataService[SERVICE_DATA_BYTE + 1];
		uint32_t up_speed = (storedDataService[SERVICE_DATA_BYTE + 2] << 8) + storedDataService[SERVICE_DATA_BYTE + 3];
		uint8_t dir = storedDataService[SERVICE_DATA_BYTE + 4];
		uint64_t steps = (uint64_t)storedDataService[SERVICE_DATA_BYTE + 5] << 56;
		steps += (uint64_t)storedDataService[SERVICE_DATA_BYTE + 6] << 48;
		steps += (uint64_t)storedDataService[SERVICE_DATA_BYTE + 7] << 40;
		steps += (uint64_t)storedDataService[SERVICE_DATA_BYTE + 8] << 32;
		steps += (uint64_t)storedDataService[SERVICE_DATA_BYTE + 9] << 24;
		steps += (uint64_t)storedDataService[SERVICE_DATA_BYTE + 10] << 16;
		steps += (uint64_t)storedDataService[SERVICE_DATA_BYTE + 11] << 8;
		steps += (uint64_t)storedDataService[SERVICE_DATA_BYTE + 12];

		switch(storedDataService[SERVICE_DATA_BYTE]){
			case 0x01:{
				start_StepMotor_1(speed, up_speed, dir, steps, false);
				break;
			}
			case 0x02:{
				start_StepMotor_2(speed, up_speed, dir, steps, false);
				break;
			}
			case 0x03:{
				start_StepMotor_3(speed, up_speed, dir, steps, false);
				break;
			}
			case 0x04:{
				start_StepMotor_4(speed, up_speed, dir, steps, false);
				break;
			}
			default: __ERROR = ERROR_DATA; break;
		}
	}

	service_cmd_send_status();
}

//********************************************************************
//
//********************************************************************

void service_cmd_start_step_motor_to_sensor()
{
	if (__STATE != __STATE_PACKING){
		uint32_t speed = storedDataService[SERVICE_DATA_BYTE + 1];
		uint32_t up_speed = (storedDataService[SERVICE_DATA_BYTE + 2] << 8) + storedDataService[SERVICE_DATA_BYTE + 3];
		uint8_t dir = storedDataService[SERVICE_DATA_BYTE + 4];

		switch(storedDataService[SERVICE_DATA_BYTE]){
			case 0x01:{
				start_StepMotor_1(speed, up_speed, dir, 0, true);
				break;
			}
			case 0x02:{
				start_StepMotor_2(speed, up_speed, dir, 0, true);
				break;
			}
			case 0x03:{
				start_StepMotor_3(speed, up_speed, dir, 0, true);
				break;
			}
			case 0x04:{
				start_StepMotor_4(speed, up_speed, dir, 0, true);
				break;
			}
			default: __ERROR = ERROR_DATA; break;
		}
	}

	service_cmd_send_status();
}

//********************************************************************
//
//********************************************************************

void service_cmd_stop_step_motor()
{
	if (__STATE != __STATE_PACKING){
		uint8_t data[10];
		data[0] = __STATE;
		data[1] = __ERROR;
		uint64_t step = 0;;
		switch(storedDataService[SERVICE_DATA_BYTE]){
			case 0x01: step = stop_motor_1(); break;
			case 0x02: step = stop_motor_2(); break;
			case 0x03: step = stop_motor_3(); break;
			case 0x04: step = stop_motor_4(); break;
			default: __ERROR = ERROR_DATA; break;
		}

		data[2] = step >> 56;
		data[3] = step >> 48;
		data[4] = step >> 40;
		data[5] = step >> 32;
		data[6] = step >> 24;
		data[7] = step >> 16;
		data[8] = step >> 8;
		data[9] = step;

		buildPackageServiceCOM(data, storedDataService[SERVICE_CMD_BYTE], 11);
	}else{
		service_cmd_send_status();
	}

}

//********************************************************************
//
//********************************************************************

void service_cmd_set_state_magnet()
{
	if (__STATE != __STATE_PACKING){
		uint16_t ms = storedDataService[SERVICE_DATA_BYTE + 2] + (storedDataService[SERVICE_DATA_BYTE + 1] * 256);
		if (!magnet_set_state(storedDataService[SERVICE_DATA_BYTE],  ms))
			__ERROR = ERROR_DATA;
		service_cmd_get_magnet_state();
	}else{
		service_cmd_send_status();
	}
}

//********************************************************************
//
//********************************************************************

void service_cmd_get_magnet_state()
{
	if (__STATE != __STATE_PACKING){
		uint8_t data[3];
		data[0] = __STATE;
		data[1] = __ERROR;
		data[2] = 0x00;
		if (MAGNET_STATE_1()) data[2] |= (1 << 0);
		if (MAGNET_STATE_2()) data[2] |= (1 << 1);
		if (MAGNET_STATE_3()) data[2] |= (1 << 2);
		if (MAGNET_STATE_4()) data[2] |= (1 << 3);
		if (MAGNET_STATE_5()) data[2] |= (1 << 4);
		if (MAGNET_STATE_6()) data[2] |= (1 << 5);
		if (MAGNET_STATE_7()) data[2] |= (1 << 6);
		if (MAGNET_STATE_8()) data[2] |= (1 << 7);

		buildPackageServiceCOM(data, storedDataService[SERVICE_CMD_BYTE], 4);
	}else{
		service_cmd_send_status();
	}
}

//********************************************************************
//
//********************************************************************

void service_cmd_get_sensor_state()
{
		uint8_t data[4];
		data[0] = __STATE;
		data[1] = __ERROR;
		data[2] = 0x00;
		data[3] = 0x00;

		if (sensorState_1) data[3] |= (1 << 0);
		if (sensorState_2) data[3] |= (1 << 1);
		if (sensorState_3) data[3] |= (1 << 2);
		if (sensorState_4) data[3] |= (1 << 3);
		if (sensorState_5) data[3] |= (1 << 4);
		if (sensorState_6) data[3] |= (1 << 5);
		if (sensorState_7) data[3] |= (1 << 6);
		if (sensorState_8) data[3] |= (1 << 7);
		if (sensorState_9) data[2] |= (1 << 0);
		if (sensorState_10) data[2] |= (1 << 1);

		buildPackageServiceCOM(data, storedDataService[SERVICE_CMD_BYTE], 5);

	//__TASK = TASK_FENCE_GOODS;
}

//********************************************************************
//
//********************************************************************

void service_cmd_main_motor_start()
{
	if (__STATE != __STATE_PACKING){
		Motor_start();
	}
	service_cmd_send_status();
}

//********************************************************************
//
//********************************************************************

void service_cmd_main_motor_stop()
{
	if (__STATE != __STATE_PACKING){
		Motor_stop();
	}
	service_cmd_send_status();
}

//********************************************************************
//
//********************************************************************

void service_cmd_main_motor_init()
{
	if (__STATE != __STATE_PACKING){
		__STATE = __STATE_INIT_MOTOR;
		MainMotorState = STATE_IS_NOT_INIT;

		motorParams.speed = storedDataService[SERVICE_DATA_BYTE] << 24;
		motorParams.speed += storedDataService[SERVICE_DATA_BYTE + 1] << 16;
		motorParams.speed += storedDataService[SERVICE_DATA_BYTE + 2] << 8;
		motorParams.speed += storedDataService[SERVICE_DATA_BYTE + 3];
		motorParams.speed_up = storedDataService[SERVICE_DATA_BYTE + 4];
		motorParams.speed_down = storedDataService[SERVICE_DATA_BYTE + 5];
		motorParams.dir = storedDataService[SERVICE_DATA_BYTE + 6];
	}

	service_cmd_send_status();
}

//********************************************************************
//
//********************************************************************

void main_cmd_start_paking()
{
	uint16_t l = 0;
	uint16_t w = 0;
	uint16_t h = 0;

	if (storedDataService[SERVICE_LEN_BYTE] == 0x06){
		l = storedDataService[SERVICE_DATA_BYTE] << 8;
		l += storedDataService[SERVICE_DATA_BYTE + 1];
		w = storedDataService[SERVICE_DATA_BYTE + 2] << 8;
		w += storedDataService[SERVICE_DATA_BYTE + 3];
		h = storedDataService[SERVICE_DATA_BYTE + 4];
	}

	startPacking(l, w, h);

	service_cmd_send_status();
}

//********************************************************************
//
//********************************************************************

void main_cmd_init()
{
	startInitSys();
	service_cmd_send_status();
}

//********************************************************************
//
//********************************************************************

void main_cmd_set_param()
{
	paramTemp_1 = storedDataService[SERVICE_DATA_BYTE];
	service_cmd_send_status();
}
//********************************************************************
//
//********************************************************************

void main_cmd_load_pellicle()
{
	loadPellicle();
	service_cmd_send_status();
}

//********************************************************************
//
//********************************************************************

void main_cmd_reset()
{
	service_cmd_send_status();
	while(1);
}
