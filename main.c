//
//  main.c
//  LogicAnalyzer
//
//  Created by Barron Wong on 12/3/17.
//  Copyright © 2017 Barron Wong. All rights reserved.
//

#include <stdio.h>
#define SAMP_SIZE 5000
int main(int argc, const char * argv[]) {
    // insert code here...
    uint8_t data[5000];
    
    for(int i = 0; i < SAMP_SIZE; i++){
        data[i] = i % 256;
    }
    
}
