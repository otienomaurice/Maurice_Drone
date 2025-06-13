#include "stdint.h"
#include "inttypes.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include <stdio.h>

//ADC GPIO
#define adc_vrx 26
#define adc_vry 27
// ADC channels
#define VRX_ADC 0    // GP26 = ADC0
#define VRY_ADC 1    // GP27 = ADC1
//
static int32_t wrap1 = 65655;
//execute joystick code
void init_joystick();
void execute_joystick();