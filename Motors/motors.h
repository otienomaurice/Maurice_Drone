#include <inttypes.h>
/*
Define the pico pins to drive the motors
*/
#include <stdbool.h>
#ifndef MOTORS_H 
#define MOTORS_H
#define MOTOR_FL 16
#define MOTOR_FR 17
#define MOTOR_BL 18
#define MOTOR_BR 19
static int32_t wrap = 65655;
static int32_t level = 0x7777;
static uint32_t timer_counter;
static uint32_t periods = 10;
static bool period_elapsed;

 typedef enum {
INITIAL_STATE,
 SAMPLE_STATE
} MOTOR_STATE;
//
 static MOTOR_STATE state;
 //
void motors_init();
//
void tick_motor_fsm();
void init_motor_outputs(uint32_t pwm_level);
//define a function to set outputs
void set_motor_outputs(uint32_t FL_level, uint32_t FR_level,uint32_t BL_level,uint32_t BR_level );
#endif