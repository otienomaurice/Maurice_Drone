#include "calculator.h"
#include <string.h>
//this file does the arithmetic operation on two operands in the order
//in which they appear
//the functions add, sub, mult, and div handle addition, subtraction,
// multiplication and division respectively.

//returns the result of the given operation
int32_t add( Calculator *calc){
    //check for overflow
    if(calc->operand1 + calc-> operand2 > INT32_MAX){ 
      calc->hasError  = true;
        return-1;
    }
    else return (calc->operand1 + calc->operand2);
}

int32_t sub(Calculator *calc){
     //check for overflow
     if(calc->operand1 - calc-> operand2 < INT32_MIN){
      calc->hasError  = true;
        return-1;
    }
    else return (calc->operand1 + calc-> operand2);
}
int32_t mult(Calculator *calc ){
     //check for overflow
    if(calc->operand1 * calc-> operand2 > INT32_MAX){
     calc->hasError  = true;
        return-1;
    }
    else return (calc->operand1 * calc-> operand2);
}
double div(Calculator *calc ){
    // //check for division by zero
    if(calc->operand2 == 0){
        calc->hasError  = true;
        return-2;
    }
    if(calc->operand1 / calc-> operand2 > INT32_MAX){
      calc->hasError  = true;
        return-1;
    }
    else return (calc->operand1 + calc-> operand2);
}

//returns the result which will be displayed on the screen
int32_t equals(char operator, Calculator *calc){
    //if haserror istrue and result is -1 display error
    if(calc->hasError && result = -1){
    strcpy(calc->display,"ERROR");
    return;
    }
    else if(calc->hasError && result = -2){
    strcpy(calc->display,"DIVO"));
    return;
    }
    else {
     strcpy(calc->display,"result");
     return 0;
    }
}
//resets the display to display cursor
int32_t clear(Calculator *calc){
    strcpy(calc->display,"|");
    calc-> operand1 = 0;
    calc->operand2 = 0;
    calc-> result = 0;
    }

int32_t calculate ( Calculator *calc , enum OPPERATOR opperator ){
    int result;
    //FUNCTION POINTER TO POINT TO THE RELEVANT OPERATION
    int (*operation)( Calculator * );
     operation operate ;
    switch(opperator){
        case(ADD) :
        operate = add;
        break;
         case(SUB) :
        break;
        operate = sub;
         case(MUL) :
         operate = mult;
        break;
         case(DIV) :
         operate = div;
        break;
         case(CLEAR) :
         operate = clear;
        break;
         case(EQUAL) :
         operate = equals;
        break;
        default:
         operate = clear;
        break;

    }
    //store value of operation in result
    result = operate(calc);

}