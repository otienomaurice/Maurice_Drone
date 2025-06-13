 #ifndef CALCULATOR_FSM.C
 #define CALCULATOR_FSM.C
 #include <stdbool.h>
 #include <stdlib.h>
 //true if C (Clear) is pressed
 bool clear_pressed;
 //true if an error is detected
 bool error;
 //true if operator has been typed in
 bool operator_pressed;
 //true if operand has completly been typed in
 bool operand_pressed;
 //true if the qual sign is pressed
 bool equal_pressed;
 //This fsm is responsible for handling th e states of the calculation
 enum { 
        INITIAL_STATE, 
        INPUT_OPERAND,
        ERROR_STATE,
        RUN_CALCULATION,
        DISPLAY_STATE 
 } 
 CALCULATOR_STATE;

 static CALCULATOR_STATE state;
 //initialize the calculation fsm
 void init_calculation_fsm();
 //advance the fsm by one cycle 
 void tick_calculation_fsm();
 #endif