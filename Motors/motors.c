#include "Motors.h"
#include <pico/stdlib.h>
#include "global/maurice_drone_io.h"
#include "global/global.h"  // ✅ to get imu_ok + IMU data
#include <stdbool.h>

bool period_elapsed = false;

static uint32_t timer_counter = 0;
static uint32_t periods = 10;
static MOTOR_STATE state = MOTOR_STATE_INIT;

static uint16_t x_buffer[QUEUE_SIZE] = {0};
static uint16_t y_buffer[QUEUE_SIZE] = {0};
static int buffer_index = 0;

static float motor1_pwm = 0, motor2_pwm = 0, motor3_pwm = 0, motor4_pwm = 0;
static volatile bool sudden_stop = false;

static uint32_t pwm_wrap = WRAP_MAX;
static uint32_t level = LEVEL_CENTER;

static float roll_rate_integral = 0, pitch_rate_integral = 0, yaw_rate_integral = 0;

#define ATT_KP 4.5f
#define RATE_KP 0.15f
#define RATE_KI 0.01f
#define RATE_KD 0.002f
#define RATE_I_LIMIT 50.0f

#define ALT_KP 1.0f
#define ALT_KD 0.5f

/* === unchanged functions === */
void init_motor_outputs(void) {
    sleep_ms(2000);
    pwm_set_gpio(MOTOR_FL, level, pwm_wrap);
    sleep_ms(100);
    pwm_set_gpio(MOTOR_FR, level, pwm_wrap);
    sleep_ms(100);
    pwm_set_gpio(MOTOR_BL, level, pwm_wrap);
    sleep_ms(100);
    pwm_set_gpio(MOTOR_BR, level, pwm_wrap);
    sleep_ms(100);
}

void motors_init(void) {
    init_motor_outputs();
    state = MOTOR_STATE_INIT;
    timer_counter = 0;
    period_elapsed = false;
    roll_rate_integral = pitch_rate_integral = yaw_rate_integral = 0;
}

void set_motor_outputs(uint32_t FL_level, uint32_t FR_level,
                       uint32_t BL_level, uint32_t BR_level) {
    pwm_set_gpio(MOTOR_FL, FL_level, pwm_wrap);
    pwm_set_gpio(MOTOR_FR, FR_level, pwm_wrap);
    pwm_set_gpio(MOTOR_BL, BL_level, pwm_wrap);
    pwm_set_gpio(MOTOR_BR, BR_level, pwm_wrap);
}

void enqueue_new_sample(uint16_t x, uint16_t y) {
    x_buffer[buffer_index] = x;
    y_buffer[buffer_index] = y;
    buffer_index = (buffer_index + 1) % QUEUE_SIZE;
}

void stabilization_update(const StateEstimate *state,
                          const PilotInput *pilot,
                          MotorCommands *motors) {
    float roll_rate_sp  = ATT_KP * (pilot->desired_roll - state->roll);
    float pitch_rate_sp = ATT_KP * (pilot->desired_pitch - state->pitch);
    float yaw_rate_sp   = ATT_KP * (pilot->desired_yaw - state->yaw);

    float roll_err  = roll_rate_sp - state->roll_rate;
    float pitch_err = pitch_rate_sp - state->pitch_rate;
    float yaw_err   = yaw_rate_sp - state->yaw_rate;

    roll_rate_integral  += roll_err;
    pitch_rate_integral += pitch_err;
    yaw_rate_integral   += yaw_err;

    if (roll_rate_integral > RATE_I_LIMIT) roll_rate_integral = RATE_I_LIMIT;
    if (roll_rate_integral < -RATE_I_LIMIT) roll_rate_integral = -RATE_I_LIMIT;
    if (pitch_rate_integral > RATE_I_LIMIT) pitch_rate_integral = RATE_I_LIMIT;
    if (pitch_rate_integral < -RATE_I_LIMIT) pitch_rate_integral = -RATE_I_LIMIT;
    if (yaw_rate_integral > RATE_I_LIMIT) yaw_rate_integral = RATE_I_LIMIT;
    if (yaw_rate_integral < -RATE_I_LIMIT) yaw_rate_integral = -RATE_I_LIMIT;

    float roll_output  = RATE_KP * roll_err + RATE_KI * roll_rate_integral + RATE_KD * (0 - state->roll_rate);
    float pitch_output = RATE_KP * pitch_err + RATE_KI * pitch_rate_integral + RATE_KD * (0 - state->pitch_rate);
    float yaw_output   = RATE_KP * yaw_err + RATE_KI * yaw_rate_integral + RATE_KD * (0 - state->yaw_rate);

    float thrust = ALT_KP * (pilot->desired_alt - state->altitude)
                 + ALT_KD * (0 - state->vertical_speed);

    float base = thrust;

    motors->m1 = base + pitch_output + roll_output - yaw_output;
    motors->m2 = base + pitch_output - roll_output + yaw_output;
    motors->m3 = base - pitch_output + roll_output + yaw_output;
    motors->m4 = base - pitch_output - roll_output - yaw_output;

    if (motors->m1 < 0) motors->m1 = 0; if (motors->m1 > 1) motors->m1 = 1;
    if (motors->m2 < 0) motors->m2 = 0; if (motors->m2 > 1) motors->m2 = 1;
    if (motors->m3 < 0) motors->m3 = 0; if (motors->m3 > 1) motors->m3 = 1;
    if (motors->m4 < 0) motors->m4 = 0; if (motors->m4 > 1) motors->m4 = 1;
}

void update_motors(void) {
    uint32_t x_sum = 0, y_sum = 0;
    for (int i = 0; i < QUEUE_SIZE; ++i) {
        x_sum += x_buffer[i];
        y_sum += y_buffer[i];
    }
    float x_mean = (float)x_sum / QUEUE_SIZE;
    float y_mean = (float)y_sum / QUEUE_SIZE;

    float x = (x_mean - 32768.0f) / 32768.0f;
    float y = (y_mean - 32768.0f) / 32768.0f;

    float desired_roll  = ((float)attitude_roll_input  - 32768.0f) / 32768.0f * 30.0f;
    float desired_pitch = ((float)attitude_pitch_input - 32768.0f) / 32768.0f * 30.0f;
    float desired_alt = 1.0f + y * 0.5f;

    PilotInput pilot = {
        .desired_roll = desired_roll,
        .desired_pitch = desired_pitch,
        .desired_yaw = 0.0f,
        .desired_alt = desired_alt
    };

    StateEstimate estimate = {
        .roll = imu_roll,
        .pitch = imu_pitch,
        .yaw = imu_yaw,
        .roll_rate = imu_roll_rate,
        .pitch_rate = imu_pitch_rate,
        .yaw_rate = imu_yaw_rate,
        .altitude = baro_altitude,
        .vertical_speed = baro_vertical_speed
    };

    MotorCommands motors;
    stabilization_update(&estimate, &pilot, &motors);

    if (sudden_stop) {
        motor1_pwm = motor2_pwm = motor3_pwm = motor4_pwm = 0;
    } else {
        motor1_pwm = motors.m1;
        motor2_pwm = motors.m2;
        motor3_pwm = motors.m3;
        motor4_pwm = motors.m4;
    }

    set_motor_outputs((uint32_t)(motor1_pwm * pwm_wrap),
                      (uint32_t)(motor2_pwm * pwm_wrap),
                      (uint32_t)(motor3_pwm * pwm_wrap),
                      (uint32_t)(motor4_pwm * pwm_wrap));
}

void tick_motor_fsm(void) {
    bool throttle_low;
    if (!period_elapsed) return;

    switch (state) {
    case MOTOR_STATE_INIT:
        init_motor_outputs();
        state = MOTOR_STATE_ARMING;
        break;

    case MOTOR_STATE_ARMING:
        throttle_low = (horizontal1 < 500 && vertical1 < 500);

        if (imu_ok && throttle_low) {
            state = MOTOR_STATE_ARMED;
        } else if (!imu_ok) {
            state = MOTOR_STATE_FAILSAFE;
        }
        break;

    case MOTOR_STATE_ARMED:
        enqueue_new_sample(horizontal1, vertical1);
        update_motors();
        break;

    case MOTOR_STATE_FAILSAFE:
        set_motor_outputs(0, 0, 0, 0);
        motor1_pwm = motor2_pwm = motor3_pwm = motor4_pwm = 0;
        state = MOTOR_STATE_DISARMED;
        break;

    case MOTOR_STATE_DISARMED:
        break;

    default:
        state = MOTOR_STATE_FAILSAFE;
        break;
    }

    period_elapsed = false;
}
