#include "barometer.h"
#include "global/global.h"
#include "hardware/i2c.h"
#include <math.h>
#include <stdio.h>
#include "pico/stdlib.h"

#define BMP180_ADDR 0x77

/* === BMP180 Calibration data === */
static int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
static uint16_t AC4, AC5, AC6;

static int32_t B5;

/* === Previous altitude for vertical speed === */
static float last_altitude = 0.0f;

/* === BMP180 Read helpers === */
static uint16_t bmp180_read16(uint8_t reg) {
    uint8_t buf[2];
    i2c_write_blocking(i2c0, BMP180_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c0, BMP180_ADDR, buf, 2, false);
    return (buf[0] << 8) | buf[1];
}

/* === BMP180 Initialization === */
void barometer_init(void) {
    // I2C init (use your pins!)
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);

    // Read calibration data
    AC1 = bmp180_read16(0xAA);
    AC2 = bmp180_read16(0xAC);
    AC3 = bmp180_read16(0xAE);
    AC4 = bmp180_read16(0xB0);
    AC5 = bmp180_read16(0xB2);
    AC6 = bmp180_read16(0xB4);
    B1  = bmp180_read16(0xB6);
    B2  = bmp180_read16(0xB8);
    MB  = bmp180_read16(0xBA);
    MC  = bmp180_read16(0xBC);
    MD  = bmp180_read16(0xBE);

    // Initialize global altitude
    baro_altitude = 0.0f;
    baro_vertical_speed = 0.0f;
}

/* === BMP180 Compensate temperature === */
static int32_t bmp180_read_temperature(void) {
    uint8_t cmd[2] = {0xF4, 0x2E};
    i2c_write_blocking(i2c0, BMP180_ADDR, cmd, 2, false);
    sleep_ms(5);

    int32_t UT = bmp180_read16(0xF6);

    int32_t X1 = (UT - AC6) * AC5 >> 15;
    int32_t X2 = (MC << 11) / (X1 + MD);
    B5 = X1 + X2;

    return (B5 + 8) >> 4;
}

/* === BMP180 Compensate pressure === */
static int32_t bmp180_read_pressure(void) {
    uint8_t cmd[2] = {0xF4, 0x34};
    i2c_write_blocking(i2c0, BMP180_ADDR, cmd, 2, false);
    sleep_ms(8);

    int32_t UP = bmp180_read16(0xF6);
    UP = (UP << 8) | (0x00);  // OSS=0

    int32_t B6 = B5 - 4000;
    int32_t X1 = (B2 * (B6 * B6 >> 12)) >> 11;
    int32_t X2 = (AC2 * B6) >> 11;
    int32_t X3 = X1 + X2;
    int32_t B3 = (((((int32_t)AC1) * 4 + X3) + 2) >> 2);
    X1 = (AC3 * B6) >> 13;
    X2 = (B1 * (B6 * B6 >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    uint32_t B4 = (AC4 * (uint32_t)(X3 + 32768)) >> 15;
    uint32_t B7 = ((uint32_t)UP - B3) * (50000 >> 0);

    int32_t p;
    if (B7 < 0x80000000) {
        p = (B7 << 1) / B4;
    } else {
        p = (B7 / B4) << 1;
    }

    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;
    p = p + ((X1 + X2 + 3791) >> 4);

    return p;
}

/* === Update altitude and vertical speed === */
void barometer_update(void) {
    bmp180_read_temperature();
    int32_t pressure = bmp180_read_pressure();

    // Calculate altitude from pressure (simple barometric formula)
    float altitude = 44330.0f * (1.0f - powf((pressure / 101325.0f), 0.1903f));

    baro_vertical_speed = (altitude - last_altitude);
    last_altitude = altitude;

    baro_altitude = altitude;
}
