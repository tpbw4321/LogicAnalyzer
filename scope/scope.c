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
#define TEST perror("TEST")


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

void ConverDataToBytes(queue * rawData, int nsamples, int samples[][DEPTH_MAX]){
    int * data;
    int dataMask = 1;
    
    for (int i=0; i < nsamples; i++){
        data = (int*)Dequeue(rawData);
        for(int j = 0; j < MAX_CHAN; j++){
            samples[j][i] = (*data & (dataMask<<j))>>j;
        }
        free(data);
    }
    
}
// Convert waveform samples into screen coordinates
void processSamples(int samples[][DEPTH_MAX],  // sample data
                    int nsamples,    // Number of samples
                    int xstart,      // starting x position of wave
                    int xfinish,     // Ending x position of wave
                    float yscale,    // y scale in pixels per volt
                    int sampleOffset,
                    data_point  processedData[][DEPTH_MAX]){
    int x1, y1;
    
    for (int i=0; i < nsamples; i++){
        for(int j = 0; j < MAX_CHAN; j++){
            x1 = xstart + (xfinish-xstart)*i/nsamples;
            y1 = samples[j][i+sampleOffset]*yscale+(yscale*j)+8*j;
            processedData[j][i].x = x1;
            processedData[j][i].y = y1;
        }
    }
}

// Plot waveform
void plotWave(data_point * processedData, // sample data
              int nsamples, // Number of samples
              int yoffset, // y offset from bottom of screen
              VGfloat linecolor[4] // Color for the wave
){
    int x1, y1, x2, y2;
    Stroke(linecolor[0], linecolor[1], linecolor[2], linecolor[3]);
    StrokeWidth(4);
    
    x1 = processedData[0].x;
    y1 = processedData[0].y;
    
    
    for(int i=1; i< nsamples; i++){
        x2 = processedData[i].x;
        y2 = processedData[i].y;
        Line(x1, y1, x2, y1);
        Line(x2, y1, x2, y2);
        x1 = x2;
        y1 = y2;
    }
}
//Plots a line at the even location
void plotTriggerEvent(int nsamples, int xstart, int xfinish, int yscale, int triggerLocation){
    int x1,x2,y1,y2;
    
    Stroke(0,0,0,5);
    StrokeWidth(4);
    
    x1 = xstart + (xfinish-xstart)*triggerLocation/nsamples;
    x1 = 500;
    y1 = 0;
    x2 = x1;
    y2 = 8*yscale;
    Line(x1,y1,x2,y2);
    perror("wemadeit");
}

//Displays the cursor
void DisplayCursor(int nsamples, int xstart, int xfinish, int yscale,int cursorLocation){
    int x1,x2,y1,y2;
    x1 = xstart + (xfinish-xstart)*cursorLocation/nsamples;
    y1 = 0;
    x2 = x1;
    y2 = 8*yscale;
    Line(x1,y1,x2,y2);
    
    
}



