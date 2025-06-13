#include <global/global.h>
#include <pico/stdlib.h>
#include "global/maurice_drone_io.h"
#include "Motors/Motors.h"
bool time_tick;
//set the ouputs
void set_motor_outputs(uint32_t FL_level, uint32_t FR_level,uint32_t BL_level,uint32_t BR_level ){
         pwm_set_gpio(MOTOR_FL,FL_level,wrap);
         pwm_set_gpio(MOTOR_FR,FR_level,wrap);
         pwm_set_gpio(MOTOR_BL,BL_level,wrap);
         pwm_set_gpio(MOTOR_BR,BR_level,wrap);

}
void init_motor_outputs(uint32_t pwm_level){
         pwm_set_gpio(MOTOR_FL,pwm_level,wrap);
         sleep_ms(100);
         pwm_set_gpio(MOTOR_FL,0,wrap);
         pwm_set_gpio(MOTOR_FR,pwm_level,wrap);
         sleep_ms(100);
         pwm_set_gpio(MOTOR_FR,0,wrap);
         pwm_set_gpio(MOTOR_BL,pwm_level,wrap);
         sleep_ms(100);
         pwm_set_gpio(MOTOR_BL,0,wrap);
         pwm_set_gpio(MOTOR_BR,pwm_level,wrap);
         sleep_ms(100);
         pwm_set_gpio(MOTOR_BR,0,wrap);

}
//initialize motor set_up
void motors_init(){
    state = INITIAL_STATE;
    timer_counter = 0;
    period_elapsed = false;
    gpio_initialize(1,1,0);
}
void tick_motor_fsm(){
    //update time
     if(time_tick){
        timer_counter++;
         gpio_set_val(1,1);
        
}
    if(timer_counter == periods){
        timer_counter = 0;
        period_elapsed = true;
    }

    switch(state){
        case  INITIAL_STATE:
         init_motor_outputs(0xffff);
        
        // if (period_elapsed) {
         state = SAMPLE_STATE;
        // }
         //else{
          //  state = INITIAL_STATE;
         //}
         
         break;
         case SAMPLE_STATE:
          set_motor_outputs(level,level,level,level);
          sleep_ms(1000);
          set_motor_outputs(0,0,0,0);
            gpio_set_val(1,0);
         // if (period_elapsed) {
         state = INITIAL_STATE;
         //}
         //else{
          //  state = SAMPLE_STATE;
        // }
         break;

    }
    //reset timer
    period_elapsed = false;
}
