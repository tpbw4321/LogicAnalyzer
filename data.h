//
//  data.h
//  Data Processor
//
//  Created by Barron Wong on 12/3/17.
//  Copyright © 2017 Barron Wong. All rights reserved.
//

#ifndef data_h
#define data_h

#include "stdint.h"
#define BUFF 100
#define FALSE 0
#define TRUE 1
#define MAXNUMOPS 8
#define TRIGGERMAX 256



typedef enum Operands{
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H
}Operands;

//typedef enum Operands{
//    H,
//    G,
//    F,
//    E,
//    D,
//    C,
//    B,
//    A
//}Operands;

typedef struct exprs{
    int * expressions;
    int length;
}exprs;

#include <stdio.h>


//Checks to see if item is an operand
int DataIsOperand(char item);
//Converts an infix  statement to postfix
int InfixToPostfix(char * expression);
//Returns the precendence of the operator
int GetPrecedence(char operator);

//Evaluates a PostFix expression
int EvalPosExpr(int values, char* expr);

//Converts operands to binary equvialent
int ConvertOperands(char operand);

//Generates all triggers given an expression
int GenerateTriggers(char * fileName, uint8_t * triggers);

//Checks an event to see if its is a triggering event
int CheckTriggerEvent(uint8_t * triggers, int triggerCount, uint8_t event);

//Takes a file name and returns an expression
int GetExpression(char * fileName, char * expr);


#endif

