#ifndef MAURICE_DRONE_IO_H
#define MAURICE_DRONE_IO_H
#include <pico/stdlib.h>
#include <inttypes.h>
#include <hardware/pwm.h>
#include <hardware/gpio.h>
//function to initialize gpio pins
void gpio_initialize(uint32_t GPIO_PIN,uint32_t MODE,uint32_t pull_dir);
//function to drive gpio pins
void gpio_set_val(uint32_t GPIO_PIN,uint32_t value);
//function to read gpio
uint32_t gpio_read_val(uint32_t GPIO_PIN);
//PWM
void pwm_set_gpio(uint32_t GPIO_PIN,int32_t level,int32_t wrap);
//USB
//UART
#endif