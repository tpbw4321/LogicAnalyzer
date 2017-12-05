//
//  stack.h
//  Data Processor
//
//  Created by Barron Wong on 12/4/17.
//  Copyright © 2017 Barron Wong. All rights reserved.
//

#ifndef stack_h
#define stack_h

#include <stdio.h>
#include <stdlib.h>


typedef struct Stack{
    int top;
    char * items;
}Stack;

//Initalizes stack with size
Stack * StartStack(int size);

//Checks to see if stack is empty
int StackIsEmpty(Stack * stack);

//Returns top item of stack without popping
char StackTop(Stack * stack);

//Pops the first item on stack
char StackPop(Stack * stack);

//Pushes item to stack
void StackPush(Stack* stack, char item);


#endif /* stack_h */
