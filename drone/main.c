/*
Drone main program
*/
#include "Motors/motors.h"
#include "global/global.h"
#include "global/timer.h"
#include "global/maurice_drone_io.h"
#include "stdbool.h"
#include "joysticks/joystick.h"
#include  "pico/stdlib.h"
#include "Transmitter/transmitter.h"
#include "global/global.h"
#include "lcd_16x2/lcd_16x2.h"
#include "lcd_16x2/lcd_16x2_execute.h"
#include "receiver/receiver.h"
#include "lcd/ts_lcd.h"
#include "pico/stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "Pico_to_Pico_direct/pico_to_pico_direct.h"
#include "hardware/watchdog.h"
#include "imu/imu.h"
#include "barometer/barometer.h"

int main(int arg_c, char* argv[]){
//the main loop
int time,period_ms = 1000;
int current_time;
int init_time = timer_read();
bool time_tick = false;

//initialize all
 stdio_init_all();
 //motors_init();
//init_joystick();
 //init_transmitter();
 lcd_init();
 init_uart();
 //ts_lcd_init();
 barometer_init();
 motors_init();
 imu_init();
while(1){
     
 sleep_ms(100);
    
     
/*
sl
time_tick = false;
 current_time =timer_read();
  if( timer_elapsed_ms(init_time,current_time)>= period_ms ){
 time_tick = true;
*/
imu_update();
barometer_update();
 uart_read();
 lcd_execute();
 tick_motor_fsm();
  //transmit();
 // ts_test();
}
}