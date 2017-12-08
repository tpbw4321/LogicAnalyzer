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
#include <string.h>

void DisplayAllSettings(const argOptions * options){
    printf("Channels:          %d\n", options->channels);
    printf("Memory Depth:      %d\n", options->memDep);
    printf("Sample Freq:       %d\n", options->sampFreq.freq);
    printf("Sample Period:     %d\n", options->sampFreq.period);
    printf("Trigger Direction: %d\n", options->trigDir);
    printf("Trigger Expr:      %s\n", options->trigger);
    printf("X-Scale :          %d\n", options->xScale);
}

//parseArguemnts and and calculates equivalents
//returns zero if completed successfully
int ParseArgs(int argc, const char * argv[], argOptions * options){
    char currOption = 0;
    char setting[255];
    char defaultFlag = 0;
    
    options->trigger = NULL;
    
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
    switch(setting){
        case 2:
        case 4:
        case 8:
            options->channels = setting;
            break;
        default:
            printf("Invalid NChannels - Default Set\n");
            
    }
    return options->channels;
}

//Sets trigger and returns the set trigger value
int SetTrigger(char * setting, argOptions * options){
    options->trigger = (char*)malloc(sizeof(char));
    if(options->trigger){
        strcpy(options->trigger, setting);
        return 1;
    }else{
        options->trigger = NULL;
        printf("No Filename Specified\n");
        return 0;
    }
    
}

//Sets trigger direction to pos or neg
int SetTrigDir(char* setting, argOptions * options){
    if(strcmp(setting, "pos")==0)
        options->trigDir = 1;
    else if(strcmp(setting,"neg")==0)
        options->trigDir = 0;
    else
        printf("Invalid TrigDir - Default Set\n");
    
    return 1;
}


int SetMemDep(int setting, argOptions * options){
    if(setting > 0)
        options->memDep = setting;
    else
        printf("Invalid MemDepth - Default Set\n");
    return 1;
}

//Sets Sampling Frequence
int SetSampFreq(int setting, argOptions * options){
    
    switch(setting){
        case 1:
            options->sampFreq.period = 200;
            options->sampFreq.freq = setting*1000;
            break;
        case 10:
            options->sampFreq.period = 20;
            options->sampFreq.freq = setting*1000;
            break;
        case 50:
            options->sampFreq.period = 10;
            options->sampFreq.freq = setting*1000;
            break;
        case 100:
            options->sampFreq.period = 2;
            options->sampFreq.freq  = setting*1000;
            break;
        default:
            printf("Invalid SampFreq - Default Set\n");
            break;
    }
    return 1;
}

//Sets xScale
int SetXScale(int setting, argOptions * options){
    switch(setting){
        case 1:
        case 5:
        case 10:
        case 100:
        case 500:
        case 1000:
        case 2000:
        case 5000:
        case 10000:
            options->xScale = setting;
            break;
        default:
            printf("Invalid X-Scale - Default Set\n");
            break;
    }
    return 0;
}
//Sets Default Options
void SetDefaultOptions(argOptions * options){
    //8 Channels
    options->channels = 8;
    //5000 Data Points
    options->memDep = 5000;
    //True to False Trigger
    options->trigDir = 1;
    //Sampling Frequency 10Khz
    options->sampFreq.freq = 10000;
    options->sampFreq.period = 20;
    //xScale 100us
    options->xScale = 100;

}

