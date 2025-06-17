#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>
#include <stdbool.h>

/* === Pin definitions === */
#define MOTOR_FL 14  // Front Left
#define MOTOR_FR 15  // Front Right
#define MOTOR_BL 16  // Back Left
#define MOTOR_BR 17  // Back Right

#define WRAP_MAX 65535
#define LEVEL_CENTER 0x7777
#define QUEUE_SIZE 20

/* === FSM states === */
typedef enum {
    MOTOR_STATE_INIT,
    MOTOR_STATE_ARMING,
    MOTOR_STATE_ARMED,
    MOTOR_STATE_FAILSAFE,
    MOTOR_STATE_DISARMED
} MOTOR_STATE;

/* === Pilot input: attitude + altitude === */
typedef struct {
    float desired_roll;   // degrees
    float desired_pitch;  // degrees
    float desired_yaw;    // degrees
    float desired_alt;    // meters
} PilotInput;

/* === Fused state estimate === */
typedef struct {
    float roll, pitch, yaw;         // degrees
    float roll_rate, pitch_rate, yaw_rate; // deg/s
    float altitude;                 // meters
    float vertical_speed;           // m/s
} StateEstimate;

/* === Motor commands (0.0 to 1.0) === */
typedef struct {
    float m1;
    float m2;
    float m3;
    float m4;
} MotorCommands;

/* === Global API === */
void motors_init(void);
void tick_motor_fsm(void);
void enqueue_new_sample(uint16_t x, uint16_t y);
void update_motors(void);
void set_motor_outputs(uint32_t FL_level, uint32_t FR_level,
                       uint32_t BL_level, uint32_t BR_level);
void stabilization_update(const StateEstimate *state,
                          const PilotInput *pilot,
                          MotorCommands *motors);

/* === Time tick flag === */
extern bool period_elapsed;

#endif
