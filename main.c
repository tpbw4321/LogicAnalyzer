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
    VGfloat wavecolor[4] = {0, 0, 0, 0.5};
    data_point **sample;
    data_point *dataPoint;
    

    for(int i = 0; i < SAMP_SIZE; i++){
        data = (int*)malloc(sizeof(int));
        *data = rand() % 256;
        Enqueue(&rawData, data);
    }
    printf("%d %d\n", width, height);
    
    processSamples(&rawData, SAMP_SIZE, 0, width, pixels_per_volt, &processedData);
    
    
    
    Start(width, height);
    Background(255,255,255);
    plotWave(&processedData, SAMP_SIZE, pixels_per_volt, wavecolor);
    End();
    
    while(1);
    
    
}
