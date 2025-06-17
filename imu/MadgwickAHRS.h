#ifndef MADGWICK_AHRS_H
#define MADGWICK_AHRS_H

#include <stdint.h>

/* === Tuning parameter === */
#define BETA_DEF 0.1f  // 2 * proportional gain

/* === API === */

/* Initialize filter state */
void MadgwickAHRSinit(float sampleFreqHz, float beta);

/* Update quaternion using IMU only (gyro + accel) */
void MadgwickAHRSupdateIMU(float gx, float gy, float gz,
                           float ax, float ay, float az);

/* Get current quaternion */
void MadgwickGetQuaternion(float *q0, float *q1, float *q2, float *q3);

#endif
