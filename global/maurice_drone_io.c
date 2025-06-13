
#include "maurice_drone_io.h"
#include "stdbool.h"
//function to initialize gpio pins
void gpio_initialize(uint32_t GPIO_PIN,uint32_t MODE,uint32_t pull_dir){
     //INITIALIZE GPIO PIN
         gpio_init(GPIO_PIN);
     //Set the gpio direction
         gpio_set_dir(GPIO_PIN, 1);
     // set pull direction
switch (pull_dir){
    case 0:
     //do nothing
     break;
     case 1:
     gpio_pull_up(GPIO_PIN);
     break;
     case  2:
     gpio_pull_down(GPIO_PIN);
     break;
}
}
     //function to drive gpio pins
void gpio_set_val(uint32_t GPIO_PIN,uint32_t value){
      gpio_put(GPIO_PIN,value);
}

//function to read gpio
uint32_t gpio_read_val(uint32_t GPIO_PIN){
      uint32_t value = gpio_get(GPIO_PIN);
}

//pwm
void pwm_set_gpio(uint32_t GPIO_PIN,int32_t level,int32_t wrap){
    //
    gpio_set_function(GPIO_PIN,GPIO_FUNC_PWM);
    //find out which pwm slice is connected to to the GPIO
    uint32_t slice_num = pwm_gpio_to_slice_num(GPIO_PIN);
    //
     uint32_t channel = pwm_gpio_to_channel(GPIO_PIN);
    //set the wrap Frequency = system_clock/((wrap+1)*clock divider)
    pwm_set_wrap(slice_num,wrap);
    //set channel A output high for one cycle
    pwm_set_chan_level(slice_num,channel,level);
    //enable pwm running
    pwm_set_enabled(slice_num,true);
}

