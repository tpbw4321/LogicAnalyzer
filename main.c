//
//  main.c
//  LogicAnalyzer
//
//  Created by Barron Wong on 12/3/17.
//  Copyright © 2017 Barron Wong. All rights reserved.
//

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "fontinfo.h"
#include "shapes.h"
#include "queue.h"
#include "scope.h"

#define SAMP_SIZE 250
#define NUM_CHAN 8

static queue rawData;
static queue processedData;

int main(int argc, const char * argv[]) {
    // insert code here...
    int width, height;
    int yscale = 8;
    int pixels_per_volt = 100;
    int * data;
    init(&width, &height);
    VGfloat wavecolor[8][4];
    data_point processedData[NUM_CHAN][SAMP_SIZE];
    
    
    for(int i = 0; i < SAMP_SIZE; i++){
        data = (int*)malloc(sizeof(int));
        *data = rand() % 256;
        Enqueue(&rawData, data);
    }
    
//    processedData = (data_point**)malloc(NUM_CHAN*sizeof(data_point*));
//
//    for(int i = 0; i < NUM_CHAN; i++){
//        processedData[i] = (data_point*)malloc(SAMP_SIZE*sizeof(data_point));
//    }
    
    for(int i = 0; i < NUM_CHAN; i++){
        for(int j = 0; j < 3; j++){
            wavecolor[i][j] = rand() % 255;
        }
        wavecolor[i][3] = 5;
    }
    
    
    printf("%d %d\n", width, height);
    processSamples(&rawData, SAMP_SIZE, 0, width, pixels_per_volt, processedData );
    
    
    Start(width, height);
    Background(50,50,50);
    
    for(int i = 0; i < NUM_CHAN; i++){
        plotWave(processedData[i], SAMP_SIZE, pixels_per_volt  , wavecolor[i]);
        plotWave(processedData[i], SAMP_SIZE, pixels_per_volt*2, wavecolor[i]);
        plotWave(processedData[i], SAMP_SIZE, pixels_per_volt*3, wavecolor[i]);
        plotWave(processedData[i], SAMP_SIZE, pixels_per_volt*4, wavecolor[i]);
    }
    
    End();
    
    while(1);
    
    
}

