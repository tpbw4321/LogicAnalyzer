//
//  stack.c
//  Data Processor
//
//  Created by Barron Wong on 12/4/17.
//  Copyright © 2017 Barron Wong. All rights reserved.
//

#include "stack.h"

//Initalizes stack with size
Stack * StartStack(int size){
    Stack * stack = (Stack *)malloc(sizeof(Stack));
    
    if(!stack)
        return 0;
    
    stack->items = (char*)malloc(size*sizeof(char));
    stack->top = -1;
    return stack;
}

//Checks to see if stack is empty
int StackIsEmpty(Stack * stack){
    if(stack->top == -1)
        return 1;
    else
        return 0;
}

//Returns top item of stack without popping
char StackTop(Stack * stack){
    if(!StackIsEmpty(stack))
        return stack->items[stack->top];
    return -1;
}


//Pops the first item on stack
char StackPop(Stack * stack){
    if(!StackIsEmpty(stack))
        return stack->items[stack->top--];
    return -1;
}

//Pushes item to stack
void StackPush(Stack* stack, char item){
    stack->items[++stack->top] = item;
}
