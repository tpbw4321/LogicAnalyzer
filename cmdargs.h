//
//  cmdargs.h
//  cmdargs
//
//  Created by Barron Wong on 12/1/17.
//  Copyright © 2017 Barron Wong. All rights reserved.
//

#ifndef cmdargs_h
#define cmdargs_h
#define POS "pos"
#define NEG "neg"
#define FREE "free"
#define TRIGGER "trigger"
#include <stdio.h>

typedef struct sampleData{
    int period;
    int freq;
    int time;
}sampleData;

typedef struct argOptions{
    int channels;
    char * trigger;
    int trigDir;
    int memDep;
    sampleData sampFreq;
    int xScale;
}argOptions;



//parseArguemnts and and calculates equivalents
int ParseArgs(int argc, const char * argv[], argOptions * options);

//sets free run mode
int SetNChannels(int setting, argOptions * options);

//Sets trigger
int SetTrigger(char * setting, argOptions * options);

int SetTrigDir(char* setting, argOptions * options);

//Sets trigger to pos or neg slope
int SetMemDep(int setting, argOptions * options);

//Sets Trigger Channel
int SetSampFreq(int setting, argOptions * options);

//Sets xScale
int SetXScale(int setting, argOptions * options);

//Sets all defaults
void SetDefaultOptions(argOptions * options);

void DisplayAllSettings(const argOptions * options);

#endif /* cmdargs_h */
