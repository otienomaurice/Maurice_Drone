#include "imu.h"
#include "global/global.h"
#include "MadgwickAHRS.h"
#include "hardware/i2c.h"
#include <math.h>
#include "pico/stdlib.h"

/* === MPU6050 register definitions === */
#define MPU6050_ADDR  0x68
#define MPU6050_REG_PWR_MGMT_1 0x6B
#define MPU6050_REG_GYRO_XOUT_H 0x43
#define MPU6050_REG_ACCEL_XOUT_H 0x3B

/* === Local buffer === */
static int16_t raw_ax, raw_ay, raw_az;
static int16_t raw_gx, raw_gy, raw_gz;

/* === IMU init === */
void imu_init(void) {
    // Example: configure I2C bus first!
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);

    // Wake up MPU6050
    uint8_t buf[2] = {MPU6050_REG_PWR_MGMT_1, 0x00};
    i2c_write_blocking(i2c0, MPU6050_ADDR, buf, 2, false);

    // Initialize Madgwick
    MadgwickAHRSinit(512.0f, 0.1f);

    imu_ok = false;
}

/* === Helper: Read MPU6050 raw === */
static void mpu6050_read_raw(void) {
    uint8_t reg = MPU6050_REG_ACCEL_XOUT_H;
    uint8_t data[14];
    i2c_write_blocking(i2c0, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c0, MPU6050_ADDR, data, 14, false);

    raw_ax = (data[0] << 8) | data[1];
    raw_ay = (data[2] << 8) | data[3];
    raw_az = (data[4] << 8) | data[5];
    raw_gx = (data[8] << 8) | data[9];
    raw_gy = (data[10] << 8) | data[11];
    raw_gz = (data[12] << 8) | data[13];
}

/* === Update === */
void imu_update(void) {
    mpu6050_read_raw();

    // Convert to real units:
    float ax = raw_ax / 16384.0f; // +/-2g
    float ay = raw_ay / 16384.0f;
    float az = raw_az / 16384.0f;

    float gx = raw_gx / 131.0f;   // +/-250 deg/s
    float gy = raw_gy / 131.0f;
    float gz = raw_gz / 131.0f;

    // Run Madgwick (convert gyro to rad/s)
    MadgwickAHRSupdateIMU(gx * 0.0174533f, gy * 0.0174533f, gz * 0.0174533f, ax, ay, az);

    // Get quaternion
    float q0, q1, q2, q3;
    MadgwickGetQuaternion(&q0, &q1, &q2, &q3);

    // Convert to Euler (degrees)
    imu_roll  = atan2f(2*(q0*q1 + q2*q3), 1 - 2*(q1*q1 + q2*q2)) * 57.2958f;
    imu_pitch = asinf(2*(q0*q2 - q3*q1)) * 57.2958f;
    imu_yaw   = atan2f(2*(q0*q3 + q1*q2), 1 - 2*(q2*q2 + q3*q3)) * 57.2958f;

    imu_roll_rate  = gx; // deg/s
    imu_pitch_rate = gy;
    imu_yaw_rate   = gz;

    // Simple norm check for sanity
    float q_norm = q0*q0 + q1*q1 + q2*q2 + q3*q3;
    if (fabsf(q_norm - 1.0f) < 0.01f) {
        imu_ok = true;
    } else {
        imu_ok = false;
    }
}
