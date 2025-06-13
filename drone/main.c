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
/*


   

    while (1)
    {

      
    }
}
*/
/*
int main() {
 stdio_init_all();
 motors_init();
 init_joystick();
 init_transmitter();
 init_receiver();
 lcd_init();
 uint8_t version = spi_read_register(0x42);

    while (1) {
         printf("USB Debug: Joystick polling...\n");
         execute_joystick();
         version = spi_read_register(0x42);
         printf("LoRa REG_VERSION: 0x%02X\n", version);
         transmit();
        //  receive();
        // lcd_execute();
             // Display current values of x_1 and y_1
         sleep_ms(500);     // Update every half second
    }
}
/*

Drone main program
*/
/*
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
//joystick values
*/
int main(int arg_c, char* argv[]){
//the main loop
int time,period_ms = 1000;
int current_time;
int init_time = timer_read();
bool time_tick = false;

//initialize all
 stdio_init_all();
 //motors_init();
 init_joystick();
 //init_transmitter();
 lcd_init();
 init_uart();
 ts_lcd_init();
while(1){
     
        sleep_ms(100);
    
     
/*
sl
time_tick = false;
 current_time =timer_read();
  if( timer_elapsed_ms(init_time,current_time)>= period_ms ){
 time_tick = true;
   while (!stdio_usb_connected()) {
        sleep_ms(100); // Optional: wait for terminal
    }
     printf("USB Debug: Joystick polling...\n");
     */
 //

  ts_test();
 //uart_read();
 //execute_joystick();
 //lcd_execute();
 //uart_send();
 transmit();
 //receive();
 //lcd_execute();
 //init_time = current_time;
  }
 }
