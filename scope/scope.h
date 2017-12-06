#ifndef scope_h
#define scope_h

#define PI 3.14159265
#define SAMP_SIZE 10000
#define DEPTH_MAX 10000

typedef struct{
    int x, y;
} data_point;


// Draw grid lines
void grid(VGfloat x, VGfloat y, // Coordinates of lower left corner
          int nx, int ny, // Number of x and y divisions
          int w, int h); // screen width and height


// Draw the background for the oscilloscope screen
void drawBackground(int w, int h, // width and height of screen
                    int xdiv, int ydiv,// Number of x and y divisions
                    int margin);

// Display x and scale settings
void printScaleSettings(int xscale, int yscale, int xposition, int yposition, VGfloat tcolor[4]);

// Convert waveform samples into screen coordinates
void processSamples(int samples[][DEPTH_MAX],  // sample data
                    int nsamples,    // Number of samples
                    int xstart,      // starting x position of wave
                    int xfinish,     // Ending x position of wave
                    float yscale,    // y scale in pixels per volt
                    int sampleOffset,
                    data_point processedData[][SAMP_SIZE]);



// Plot waveform
void plotWave(data_point *processedData, // sample data
              int nsamples, // Number of samples
              int yoffset, // y offset from bottom of screen
              VGfloat linecolor[4] // Color for the wave
);
//Plots a line at the even location
void plotTriggerEvent(int nsamples, int xstart, int xfinish, int yscale, int triggerLocation);

//Converts data samples to individual bytes
void ConverDataToBytes(queue * rawData, int nsamples, int samples[][DEPTH_MAX]);

//Displays the cursor
void DisplayCursor(int nsamples, int xstart, int xfinish, int yscale, int cursorLocation);
#endif

