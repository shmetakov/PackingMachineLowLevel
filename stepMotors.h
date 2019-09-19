#ifndef STEPMOTORS_H
#define STEPMOTORS_H

/*
 * Libs for 4 step motor / Driver SMD-4.2 / STM32F407VGT6 (STM32f4-Discovery)
 * Recommended the system frequency 168 Mhz, TIM frequency 84 and interrupt 3us (for 1/16 mode)
 */

/**
 * For 1/16 mode
 *
 *     REV   *   REAL REV
 ***************************
 *      5    *     5.2     *
 *      6    *     6.1     *
 *      7    *     7.4     *
 *      8    *      8      *
 *      9    *     9.5     *
 *     10    *    10.4     *
 *     11    *    11.5     *
 *    12,13  *     13      *
 *     14    *    14.8     *
 *  15,16,17 *    17.3     * - it's not desirable to use
 ***************************
 *
**/

/*
 * ШД-1 - привод протяжки пленки
 * ШД-2 - привод упаковочных пластин
 * ШД-3 - придод сдвигателя упаковонного лотка
 * ШД-4 - привод загрузки лотка с весов
 *
 */

//GPIO group for step motors

#define STEP_MOTOR_GPIOx_EN_1								GPIOC
#define STEP_MOTOR_GPIOx_DIR_1								GPIOE
#define STEP_MOTOR_GPIOx_STEP_1								GPIOE

#define STEP_MOTOR_GPIOx_EN_2								GPIOE
#define STEP_MOTOR_GPIOx_DIR_2								GPIOE
#define STEP_MOTOR_GPIOx_STEP_2								GPIOB

#define STEP_MOTOR_GPIOx_EN_3								GPIOB
#define STEP_MOTOR_GPIOx_DIR_3								GPIOB
#define STEP_MOTOR_GPIOx_STEP_3								GPIOD

#define STEP_MOTOR_GPIOx_EN_4								GPIOD
#define STEP_MOTOR_GPIOx_DIR_4								GPIOD
#define STEP_MOTOR_GPIOx_STEP_4								GPIOD

//PIN for step motors

#define STEP_MOTOR_PIN_EN_1									GPIO_Pin_13
#define STEP_MOTOR_PIN_DIR_1								GPIO_Pin_4
#define STEP_MOTOR_PIN_STEP_1								GPIO_Pin_5

#define STEP_MOTOR_PIN_EN_2									GPIO_Pin_3
#define STEP_MOTOR_PIN_DIR_2								GPIO_Pin_2
#define STEP_MOTOR_PIN_STEP_2								GPIO_Pin_8

#define STEP_MOTOR_PIN_EN_3									GPIO_Pin_4
#define STEP_MOTOR_PIN_DIR_3								GPIO_Pin_5
#define STEP_MOTOR_PIN_STEP_3								GPIO_Pin_7

#define STEP_MOTOR_PIN_EN_4									GPIO_Pin_6
#define STEP_MOTOR_PIN_DIR_4								GPIO_Pin_3
#define STEP_MOTOR_PIN_STEP_4								GPIO_Pin_2

//DIR

#define DIR_CLOCKWISE										1
#define DIR_ANTICLOCKWISE									0

/* STEP_ON_REV:
 * 1/1  - 200
 * 1/2  - 400
 * 1/4  - 800
 * 1/16 - 3200
 */

//#define SPLITIING_STEP_1
//#define SPLITIING_STEP_2
//#define SPLITIING_STEP_4
#define SPLITIING_STEP_16

#if defined(SPLITIING_STEP_1)
	#define STEP_ON_REV										200
#elif defined(SPLITIING_STEP_2)
	#define STEP_ON_REV										400
#elif defined(SPLITIING_STEP_4)
	#define STEP_ON_REV										800
#else
	#define STEP_ON_REV										3200
#endif

 /*
  * Start rev
  */

#define START_SPEED											3

/*
 * Struct for init pin motors
 */

GPIO_InitTypeDef  motors_pin_init;

//Funcs

/* in while() (main funk) */
void calculate_param();
/* in main() */
void init_Motors_PIN();

/*
 * speed      	- Target speed
 * up_speed  	- Speed-up us
 * dir			- Direction
 * steps		- Count steps
 */

void start_StepMotor_1(uint32_t speed, uint32_t up_speed, uint8_t dir, uint64_t steps, _Bool sens);
void start_StepMotor_2(uint32_t speed, uint32_t up_speed, uint8_t dir, uint64_t steps, _Bool sens);
void start_StepMotor_3(uint32_t speed, uint32_t up_speed, uint8_t dir, uint64_t steps, _Bool sens);
void start_StepMotor_4(uint32_t speed, uint32_t up_speed, uint8_t dir, uint64_t steps, _Bool sens);

uint64_t stop_motor_1();
uint64_t stop_motor_2();
uint64_t stop_motor_3();
uint64_t stop_motor_4();

_Bool sens_step_motor_1;
_Bool sens_step_motor_2;
_Bool sens_step_motor_3;
_Bool sens_step_motor_4;

struct motorParams {
	uint64_t num_steps; 		/*!<Current number steps*/
	uint64_t target_num_steps;	/*!<Target number steps*/
	uint32_t target_speed;		/*!<Target speed*/
	uint32_t speed;				/*!<Current modified speed for timer*/
	uint32_t up_speed;			/*!<Speed-up in us/3*/
	uint32_t kof_up_speed;		/*!<Coefficient for speed-up*/
	uint32_t tick_up_speed;		/*!<Counter speed-up*/
	uint32_t tick;				/*!<Main counter for motors*/
	uint8_t  isEnable;			/*!<Permission to work*/
	uint8_t  sens;				/*!<Sensor for motor*/
	bool 	 isOverflow;
};

struct motorParams motor_1;
struct motorParams motor_2;
struct motorParams motor_3;
struct motorParams motor_4;

#endif
