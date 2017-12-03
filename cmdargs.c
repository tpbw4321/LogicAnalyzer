//
//  cmdargs.c
//  cmdargs
//
//  Created by Barron Wong on 12/1/17.
//  Copyright © 2017 Barron Wong. All rights reserved.
//

#include "cmdargs.h"
#include <string.h>
#include <stdlib.h>

void DisplayAllSettings(const argOptions * options){
    printf("Channels:    %d\n", options->channels);
    printf("Memory Depth: %d\n", options->memDep);
    printf("Sample Freq:   %d\n", options->sampFreq);
    printf("Trigger Direction: %d\n", options->trigDir);
    printf("Trigger Expr: %d\n", options->trigger);
    printf("X-Scale :  %d\n", options->xScale);
}

//parseArguemnts and and calculates equivalents
//returns zero if completed successfully
int ParseArgs(int argc, const char * argv[], argOptions * options){
    char currOption = 0;
    char setting[255];
    char defaultFlag = 0;
    
    SetDefaultOptions(options);
    
    if(argc == 0){
        defaultFlag = 1;
    }
    
    for(int i = 1; i < argc; i=i+2){
        if(*argv[i] == '-'){
            currOption = *(argv[i]+1);
            
            switch(currOption){
                case 'n':
                    strcpy(setting, argv[i+1]);
                    SetNChannels(atoi(setting), options);
                    continue;
                case 't':
                    strcpy(setting, argv[i+1]);
                    SetTrigger(setting, options);
                    continue;
                case 'd':
                    strcpy(setting, argv[i+1]);
                    SetTrigDir(setting, options);
                    continue;
                case 'm':
                    strcpy(setting, argv[i+1]);
                    SetMemDep(atoi(setting), options);
                    continue;
                case 'f':
                    strcpy(setting, argv[i+1]);
                    SetSampFreq(atoi(setting), options);
                    continue;
                case 'x':
                    strcpy(setting, argv[i+1]);
                    SetXScale(atoi(setting), options);
                    continue;
                default:
                    defaultFlag = 1;
                    break;
            }
        }else{
            defaultFlag = 1;
        }
        if(defaultFlag)
            printf("Invalid Settings - Defaults Selected\n");
            SetDefaultOptions(options);
        break;
    }
    
    return 0;
    
}

//sets free run mode returns 1 for freerun and 0 for tiggered
int SetNChannels(int  setting, argOptions * options){
    options->channels = setting;
    return options->channels;
}

//Sets trigger and returns the set trigger value
int SetTrigger(char * setting, argOptions * options){
    return 1;
}

//Sets trigger direction to pos or neg
int SetTrigDir(char* setting, argOptions * options){
    
}


int SetMemDep(int setting, argOptions * options){
    return 1;
}

//Sets Trigger Channel
int SetSampFreq(int setting, argOptions * options){
    return 1;
}

//Sets xScale
int SetXScale(int setting, argOptions * options){

    return 0;
}
//Sets Default Options
void SetDefaultOptions(argOptions * options){
    //No Free Run
    options->channels = 8;
    //Trigger Channel 1
    options->memDep = 5000;
    //Trigger 2.5v
    options->trigger = 0;
    //Trigger Direction Triggers from false to true
    options->trigDir = 0;
    //Trigger Frequency
    options->sampFreq = 10000;
    //xScale 100us
    options->xScale = 100;

}

