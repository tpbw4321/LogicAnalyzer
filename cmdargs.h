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
#define TRIG_MAX 5000.00
#define FREE "free"
#define TRIGGER "trigger"
#include <stdio.h>

typedef struct xSettings{
    int period;
    int samples;
    int time;
}xSettings;

typedef struct argOptions{
    int channels;
    int trigger;
    int trigDir;
    int memDep;
    int sampFreq;
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
