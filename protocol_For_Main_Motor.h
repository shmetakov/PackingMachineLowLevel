#ifndef PROTOCOL_FOR_MAIN_MOTOR_H
#define PROTOCOL_FOR_MAIN_MOTOR_H

uint8_t crc_buffer_main_motor(uint8_t *buf);

#define BUFFER_MAIN_MOTOR_SIZE								8

//state

#define STATE_SPEED_UP_IS_SET								0xA5
#define STATE_SPEED_DOWN_IS_SET								0xA6
#define STATE_DIR_IS_SET									0xA7
#define STATE_SPEED_IS_SET									0xA3
#define STATE_MOTOR_IS_START								0x51
#define STATE_MOTOR_IS_STOP									0x52
#define STATE_WAIT_ANSWER									0xFF
#define STATE_WAIT_ANSWER_ERRROR							0xF0
#define STATE_IS_NOT_INIT									0x00

#define MAIN_MOTOR_R_ADR_BYTE								0
#define MAIN_MOTOR_R_CMD_BYTE								1
#define MAIN_MOTOR_R_DATA_BYTE								3
#define MAIN_MOTOR_R_CRC_BYTE								4

#define ADR_MOTOR											0x01
#define NUM_REG												10

void parsingPackageRS232();
void saveDataMainMotor(uint8_t* packege);
void clearBuffer_Motor_Send();
uint8_t crc_buffer_main_motor(uint8_t *buf);
_Bool init_Main_motor();
bool init_motor();
void Motor_start();
void Motor_stop();

struct mainMotorParams {
	uint32_t speed;
	uint8_t speed_up;
	uint8_t speed_down;
	uint8_t dir;
} motorParams;

#define MAIN_MOTOR_SPEED									2700
#define MAIN_MOTOR_SPEED_UP									24
#define MAIN_MOTOR_SPEED_DOWN								24
#define MAIN_MOTOR_DIR										0

bool isMainMotorStart;

#endif
