// global.c
#include "global.h"
#include "inttypes.h"
uint32_t x_1 = 0;
uint32_t y_1 = 0;
uint32_t horizontal1 = 0;
uint32_t vertical1 = 0;
uint32_t attitude_roll_input  = 0; // new joystick for roll angle
uint32_t attitude_pitch_input = 0; 
bool imu_ok = false;
float imu_roll = 0, imu_pitch = 0, imu_yaw = 0;
float imu_roll_rate = 0, imu_pitch_rate = 0, imu_yaw_rate = 0;
float baro_altitude = 0.0f;
float baro_vertical_speed = 0.0f;



// Add any others you’ve declared in global.h