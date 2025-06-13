#include "joystick.h"
#include "stdbool.h"
#include "global/maurice_drone_io.h"
#include "global/global.h"  // Include global variables x_1, y_1

void init_joystick() {
    // Init ADC
    adc_init();
    adc_gpio_init(adc_vrx);  // VRx
    adc_gpio_init(adc_vry);  // VRy
}

void execute_joystick() {
    // Read X-axis
    adc_select_input(VRX_ADC);
    uint32_t x_val = adc_read();  // 0–4095

    // Read Y-axis
    adc_select_input(VRY_ADC);
    uint32_t y_val = adc_read();  // 0–4095

    // Scale ADC 12-bit (0–4095) to 16-bit PWM (0–65535)
    uint32_t x = (x_val << 4); // Multiply by 16
    uint32_t y= (y_val << 4);

    // Store in global variables
    x_1 = x;
    y_1 = y;

   // printf("USB Debug: Joystick polling...\n");
   // printf("x_1= %d, y_1 = %d\n", x_1-700, y_1-700);
    sleep_ms(10);
}