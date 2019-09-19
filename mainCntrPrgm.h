#ifndef MAIN_CNTRL_PR_H
#define MAIN_CNTRL_PR_H

#define SPEED_STEP_MOTOR_1									11
#define SPEED_UP_STEP_MOTOR_1								5000


#define SPEED_STEP_MOTOR_2									14
#define SPEED_UP_STEP_MOTOR_2								5000
#define STEPS_STEP_MOTOR_2									21500
#define STEPS_STEP_MOTOR_2_MAX								25000

#define SPEED_STEP_MOTOR_3									14
#define SPEED_UP_STEP_MOTOR_3								5000
#define STEPS_STEP_MOTOR_3									27000
#define STEPS_STEP_MOTOR_3_MAX								65000

#define SPEED_STEP_MOTOR_4									11
#define SPEED_UP_STEP_MOTOR_4								5000
#define STEPS_STEP_MOTOR_4									41800
#define STEPS_STEP_MOTOR_4_MAX								48000

bool init_System();
void main_work_funk();

#endif
