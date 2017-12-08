//
//  data.c
//  Data Processor
//
//  Created by Barron Wong on 12/3/17.
//  Copyright © 2017 Barron Wong. All rights reserved.
//

#include "data.h"
#include <stdio.h>
#include <string.h>
#include "stack.h"

int GetExpression(char * fileName, char * expr){
    FILE * fp;
    fp = fopen(fileName, "r");
    
    if(!fp){
        printf("Unable to Open \"%s\"\n", fileName);
        return 0;
    }
    
    fscanf(fp,"%s", expr);
    return 1;
}
//Checks to see if item is an operand
int DataIsOperand(char item){
    return (item >= 'A' && item <= 'H') || (item >= 'a' && item <= 'h');
}
int ConvertOperands(char operand){
    switch(operand){
        case 'A':  //Top Bit
        case 'a':
            return A;
            break;
        case 'B':
        case 'b':
            return B;
            break;
        case 'C':
        case 'c':
            return C;
            break;
        case 'D':
        case 'd':
            return D;
            break;
        case 'E':
        case 'e':
            return E;
            break;
        case 'F':
        case 'f':
            return F;
            break;
        case 'G':
        case 'g':
            return G;
            break;
        case 'H':
        case 'h':
            return H; //Bottom Bit
            break;
        default:
            return -1;
    }
}
//Converts an infix  statement to postfix
int InfixToPostfix(char * expression){
    
    int length = (int) strlen(expression);
    int k = 0;
    char output[length];
    char curr;
    Stack * stack = StartStack(length);
    for(int i = 0; i < length; i++){
        curr = expression[i];
        //Check for White spaces
        if(expression[i] == ' ')
            continue;
        
        //If operand add to output
        else if(DataIsOperand(expression[i]))
            output[k++] = expression[i];
        
        //If ( encountered push to stack
        else if(expression[i] == '(')
            StackPush(stack, expression[i]);
        
        //If ')' enountered pop until '(' is found
        else if(expression[i] == ')'){
            while(!StackIsEmpty(stack) && StackTop(stack)!= '(')
                output[k++] = StackPop(stack);
            if(!StackIsEmpty(stack) && StackTop(stack)!= '(')
                return -1;
            else
                StackPop(stack);
        }
        else{
            while(!StackIsEmpty(stack) && (GetPrecedence(expression[i]) <= GetPrecedence(StackTop(stack))))
                output[k++] = StackPop(stack);
            StackPush(stack, expression[i]);
        }
    }
    while(!StackIsEmpty(stack)){
        output[k++] = StackPop(stack);
    }
    output[k] = '\0';
    strcpy(expression, output);
    printf("%s\n",output);
    return 1;
}
int GetPrecedence(char operator){
    switch(operator){
        case '~':
            return 3;
        case '&':
            return 2;
        case '|':
            return 1;
        default:
            return 0;
    }
}

int EvalPosExpr(int values, char* expr){
    
    int operands[MAXNUMOPS];
    int mask = 1;
    int length = strlen(expr);
    int index;
    int op1, op2;
    
    Stack * stack = StartStack(length);
    
    
    for(int i = 0; i < MAXNUMOPS; i++){
        operands[i] = (values & mask<<i)>>i;
    }
    
    for(int i = 0; i < length; i++){
        if(DataIsOperand(expr[i])){
            index = ConvertOperands(expr[i]);
            if(index == -1){
                return -1;
            }
            StackPush(stack, operands[index]);
        }
        else{
            switch(expr[i]){
                case '~':
                    op1 = !StackPop(stack);
                    StackPush(stack, op1);
                    continue;
                case '|':
                    op1 = StackPop(stack);
                    op2 = StackPop(stack);
                    StackPush(stack, (op1|op2));
                    continue;
                case '&':
                    op1 = StackPop(stack);
                    op2 = StackPop(stack);
                    StackPush(stack, (op1 & op2));
                    continue;
                default:
                    return -1;
            }
        }
    }
    
    return StackPop(stack);
}
//Generates tigger events in a given filename
//Returns the number of trigger events
int GenerateTriggers(char * fileName, uint8_t * triggers){
    
    char expr[TRIGGERMAX];
    
    if(!GetExpression(fileName, expr)){
        return -1;
    }
    
    int j = 0;
    if( InfixToPostfix(expr) == -1){
        return 0;
    }
    
    
    for(int i = 0; i < TRIGGERMAX; i++){
        if(EvalPosExpr(i, expr))
            triggers[j++] = i;
    }
    
    return j;
}
int CheckTriggerEvent(uint8_t * triggers, int triggerCount, uint8_t event){
    for(int i = 0; i < triggerCount; i++)
        if(triggers[i] == event)
            return 1;
    return 0;
}
