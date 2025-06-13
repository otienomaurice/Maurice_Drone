#include "calculation_fsm.h"
#include "calculator.h"
void init_calculation_fsm(){
    state = INITIAL_STATE;
    operand_pressed = false;
    equal_pressed = false;
    clear_pressed = false;
    error = false;

}

//advance the fsm by one cycle 
void tick_calculation_fsm(){
    //do the reading stuff and setting required variables
    switch (state)

    {
    case INITIAL_STATE:
        /* code */
        break;
    

    
        case INPUT_OPERAND/:
        /* code */
        break;

    
        case ERROR_STATE:
        /* code */
        break;

    
        case RUN_CALCULATION:
        /* code */
        break;
    
    default:
        break;
    }
}