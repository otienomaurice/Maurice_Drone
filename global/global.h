
#ifndef GLOBAL_H
#define GLOBAL_H
#include "inttypes.h"
#include <stdbool.h>
extern uint32_t x_1;
extern uint32_t y_1;
extern uint32_t horizontal1;
extern uint32_t vertical1;
// === Second joystick: attitude control ===
extern uint32_t attitude_roll_input ; // new joystick for roll angle
extern uint32_t attitude_pitch_input; // new joystick for pitch angle
/* === Shared IMU health flag === */
extern bool imu_ok;

/* === Shared fused attitude === */
extern float imu_roll, imu_pitch, imu_yaw;
extern float imu_roll_rate, imu_pitch_rate, imu_yaw_rate;
extern float baro_altitude;
extern float baro_vertical_speed;


#endif