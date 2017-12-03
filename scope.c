#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <shapes.h>
#include <fontinfo.h>
#include <math.h>
#include <errno.h>
#include <libusb.h>
#include "queue.h"
#include "cmdargs.h"
#define PI 3.14159265
#define MAX_CHAN 8
#include "scope.h"


// Draw grid lines
void grid(VGfloat x, VGfloat y, // Coordinates of lower left corner
          int nx, int ny, // Number of x and y divisions
          int w, int h) // screen width and height

{
    VGfloat ix, iy;
    Stroke(128, 128, 128, 0.5); // Set color
    StrokeWidth(2); // Set width of lines
    for (ix = x; ix <= x + w; ix += w/nx) {
        Line(ix, y, ix, y + h);
    }
    
    for (iy = y; iy <= y + h; iy += h/ny) {
        Line(x, iy, x + w, iy);
    }
}

// Display x and scale settings
void printScaleSettings(int xscale, int yscale, int xposition, int yposition, VGfloat tcolor[4]) {
    char str[100];
    int y;
    
    y = (yscale * 25)>>4;
    
    setfill(tcolor);
    if (xscale >= 1000)
        sprintf(str, "X scale = %2d ms/div", xscale/1000);
    else
        sprintf(str, "X scale = %2d us/div", xscale);
    Text(xposition, yposition, str, SansTypeface, 18);
    
    if(y >= 1000)
        sprintf(str, "Y scale = %2d  V/div",  y/1000);
    else
        sprintf(str, "Y scale = %2d mV/div",  y);
    Text(xposition, yposition-50, str, SansTypeface, 18);
}

// Convert waveform samples into screen coordinates
void processSamples(queue *rawData,  // sample data
                    int nsamples,    // Number of samples
                    int xstart,      // starting x position of wave
                    int xfinish,     // Ending x position of wave
                    float yscale,    // y scale in pixels per volt
                    queue *processedData){
    VGfloat x1, y1;
    data_point ** p;
    uint8_t * data;
    uint8_t sample;
    uint8_t dataMask = 0x01;
    
    
    for (int i=0; i< nsamples; i++){
        data = (uint8_t*)Dequeue(rawData);
        p = (data_point **) malloc(sizeof(data_point*)*8);
        
        //Separate each bit and process
        for(int j = 0; j < MAX_CHAN; j++){
            p[j] = (data_point*) malloc(sizeof(data_point));
            x1 = xstart + (xfinish-xstart)*i/nsamples;
            sample = (*data & (dataMask<<j))>>j;
            y1 = sample*yscale+(yscale*j);
            p[j]->x = x1;
            p[j]->y = y1;
            printf("%2f %2f %d\n", p[j]->x, p[j]->y, *data);
        }
        free(data);
        Enqueue(processedData, p);
    }
}

int * SplitChannel(queue * rawData){
    
}

// Plot waveform
void plotWave(queue *processedData, // sample data
              int nsamples, // Number of samples
              int yoffset, // y offset from bottom of screen
              VGfloat linecolor[4] // Color for the wave
){
    data_point ** p;
    VGfloat x1[MAX_CHAN], y1[MAX_CHAN], x2[MAX_CHAN], y2[MAX_CHAN];
    Stroke(linecolor[0], linecolor[1], linecolor[2], linecolor[3]);
    StrokeWidth(4);
    
    p = (data_point**)Dequeue(processedData);
    

    for(int i = 0; i < MAX_CHAN; i++){
        x1[i] = p[i]->x;
        y1[i] = p[i]->y + yoffset;
        free(p[i]);
    }
    free(p);
    
    
    for(int i=1; i< nsamples; i++){
        p = (data_point**)Dequeue(processedData);
        for(int j = 0; j < MAX_CHAN; j++){
            x2[j] = p[j]->x;
            y2[j] = p[j]->y;
            free(p[j]);
            Line(x1[j], y1[j], x2[j], y2[j]);
            x1[j] = x2[j];
            y1[j] = y2[j];
        }
        free(p);
    }
}




