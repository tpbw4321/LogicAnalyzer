//
//  main.c
//  scope
//
//  Created by Barron Wong on 12/5/17.
//  Copyright © 2017 Barron Wong. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <shapes.h>
#include <fontinfo.h>
#include <math.h>
#include <errno.h>
#include "queue.h"
#define PI 3.14159265
#define MAX_CHAN 8
#include "scope.h"

static VGfloat wavecolor[8][4];
void SelectWaveColors(){
    for(int i = 0; i < MAX_CHAN; i++){
        for(int j = 0; j < 3; j++){
            wavecolor[i][j] = rand() % 255;
        }
        wavecolor[i][3] = 5;
    }
}
int main(int argc, const char * argv[]) {
    // insert code here...
    int width, height;
    int yscale = 8;
    int pixels_per_volt = 1000;
    int data = 0;
    data_point processedData[MAX_CHAN][SAMP_SIZE];
    queue rawData;
    int * item;
    int samples[8][10000];
    
    init(&width, &height);
    
    for(int i = 0; i < 20; i++){
        item = (int*) malloc(sizeof(int));
        *item = i+5;
        Enqueue(&rawData, item);
    }
    SelectWaveColors();
    ConverDataToBytes(&rawData, 20, samples);
    
    processSamples(samples, 20, 0, width, yscale, 0, processedData);
    
    //perror("made");
    
    Start(width, height);
    Background(50,50,50);
    for(int i = 0; i < MAX_CHAN; i++){
        plotWave(processedData[i], 20, pixels_per_volt*i, wavecolor[i]);
    }
    End();
    while(1);
    
    
    return 0;
}

