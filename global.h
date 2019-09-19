#ifndef GLOBAL_H
#define GLOBAL_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_iwdg.h"
#include "stdbool.h"
#include "string.h"

void Soft_Delay(volatile uint32_t number);
void stopWork();
void startPacking(uint16_t l, uint16_t w, uint16_t h);
void startInitSys();
void loadPellicle();
//__ERROR

#define ERROR_NO											0x00
#define ERROR_CMD											0x31
#define ERROR_DATA											0x32

#define ERROR_INIT_MOTOR									0x41
#define ERROR_INIT_SYS										0x42
#define ERROR_SENS_ENV										0x43
#define ERROR_PACKING										0x44
#define ERROR_SENS_MOTOR									0x45

#define ERROR_MOTOTR_2_RETURN_TO_HOME_POS					0x52
#define ERROR_MOTOTR_3_RETURN_TO_HOME_POS					0x53
#define ERROR_MOTOTR_4_RETURN_TO_HOME_POS					0x54

#define ERROR_SENS_1										0x61
#define ERROR_SENS_2										0x62
#define ERROR_SENS_3										0x63
#define ERROR_SENS_6										0x66
#define ERROR_SENS_10										0x60

//__STATE

#define __STATE_OK											0x30
#define __STATE_NO_INIT										0x10
#define __STATE_INIT										0x31
#define __STATE_INIT_MOTOR									0x32
#define __STATE_CRIT_ER										0x41
#define __STATE_PACKING										0x51
#define __STATE_LOAD_PELLICLE								0x52
#define __STATE_PACKING_END									0x53

//__TASK


#define TASK_INIT_MOTOR										0x50
#define TASK_INIT_SYS										0x51

#define NO_TASK												0x00
#define TASK_FENCE_GOODS									0x01
#define TASK_BROACH											0x02
#define TASK_CUT											0x03
#define TASK_CUT_2											0x04
#define TASK_GOODS_UP										0x05
#define TASK_PACKAGING										0x06
#define TASK_FINICH_PACKAGING								0x07
#define TASK_END											0x08
#define TASK_START_PACKAGING								0x09

//times

#define TIME_FOR_INIT_MOTOR									1000
#define TIME_FOR_INIT_SYS									10000
#define TIME_FOR_PACKING									5000

uint8_t __TASK;
uint8_t __ERROR;
uint8_t __STATE;

#endif
