#include <inttypes.h>
#include <stdbool.h>
#ifndef CALCULATOR_H
#define CALCULATOR_H

 typedef struct 
{
    int32_t operand1;
    int32_t operand2;
    int32_t result;
    enum OPPERATOR operator;
    char display[30];
    bool hasError;
}Calculator;
enum OPPERATOR
{
    ADD,
    SUB,
    MUL,
    DIV,
    CLEAR,
    EQUAL,
};

int32_t add( Calculator *calc);
int32_t sub(Calculator *calc);
int32_t mult(Calculator *calc );
double div(Calculator *calc );
void calculate ( int (*operation)( Calculator *calc ),Calculator *calc);

#endif