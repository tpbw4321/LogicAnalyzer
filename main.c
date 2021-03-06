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
#include "usbcomm.h"
#include "libusb.h"
#include "cmdargs.h"
#include "data.h"

#define EP1 (0x80|0x01)
#define EP2 (0x80|0x02)
#define EP3 (0x00|0x03)
#define SAMP_SIZE 10000
#define NUM_CHAN 8
#define MAX_PACKET 1000
#define MICRO 1e-6
#define POTMAX 255
#define TRIGMAX 256
#define SET 0
#define PERIOD 1
#define BUFFER 100000

static libusb_device_handle * dev = NULL;
static queue rawData;
static void SelectWaveColors();
static uint8_t packetBuffer[MAX_PACKET];
static VGfloat wavecolor[8][4];
static uint8_t triggerEvents[TRIGMAX];
static int triggerCount= 0;
static uint8_t triggerFlag=0;
static int sampRemaining = 0;
static int eventLocation = 0;

int main(int argc, const char * argv[]) {
    
    
    int width, height;
    int yscale = 8;
    int pixels_per_volt = 80;
    int data = 0;
    argOptions options;
    data_point processedData[NUM_CHAN][SAMP_SIZE];
    int sampleData[NUM_CHAN][SAMP_SIZE];
    unsigned char potReading[2]= {0,0};
    uint8_t * sample;
    
    
    SetDefaultOptions(&options);
    
    dev = SetupDevHandle(0x04B4, 0x8051);
    
    int frequency = 100;
    int xscale = 10000;           //in microseconds
    int samples_per_screen = frequency*(xscale*MICRO);
    int samplecount = 0;
    int offset = 0;
    int cursorScale = width/POTMAX;
    int shiftScale = 0;
    int shiftPrevious = 500;
    int cursorPrevious = 500;
    int cursScale = 0;
    int xdivisions = 0;
    unsigned char buffer[BUFFER];
    int * item;
    float secPerSample;
    unsigned char control[2];
    uint8_t negFlag = 0;
    int offChannel[DEPTH_MAX];
    
    ParseArgs(argc, argv, &options);
    DisplayAllSettings(&options);
    triggerCount = GenerateTriggers(options.trigger, triggerEvents);
    if(triggerCount < 0){
        printf("Invalid Trigger\n");
        return -1;
    }
    
    control[SET] = 1;
    control[PERIOD] = options.sampFreq.period;
    
    int sent_bytes = 0;
    int rcvd_bytes = 0;
    
    if(libusb_interrupt_transfer(dev, EP3, control, 2, &sent_bytes, 0) !=0 )
        return -1;
    
    sampRemaining = options.memDep/2;
    
    SelectWaveColors();
    init(&width, &height);
    
    //Gather Samples
    while(sampRemaining>0){
        if(libusb_bulk_transfer(dev, EP1, buffer, BUFFER, &rcvd_bytes, 0)!=0){
            perror("Incoming Data");
            return -1;
        }
        else{
            for(int i = 0; i < rcvd_bytes; i++){
                item = (int*)malloc(sizeof(int));
                *item = buffer[i];
                if(options.trigDir == 1){
                    if(!triggerFlag && CheckTriggerEvent(triggerEvents, triggerCount, *item) && rawData.count > sampRemaining){
                        triggerFlag = 1;
                        eventLocation = rawData.count;
                    }
                    if(triggerFlag)
                        sampRemaining--;
                    
                    else if(rawData.count > sampRemaining)
                        Dequeue(&rawData);
                    
                    if(sampRemaining > 0){
                        Enqueue(&rawData, item);
                    }
                }
                else{
                    if(!negFlag && CheckTriggerEvent(triggerEvents, triggerCount, *item)&& !triggerFlag && rawData.count > sampRemaining){
                        negFlag = 1;
                    }
                    if(negFlag && !CheckTriggerEvent(triggerEvents, triggerCount, *item)&& !triggerFlag){
                        triggerFlag = 1;
                        eventLocation = rawData.count;
                    }
                    if(triggerFlag)
                        sampRemaining--;
                    else if(rawData.count >= sampRemaining)
                        Dequeue(&rawData);
                    if(sampRemaining > 0)
                        Enqueue(&rawData, item);
                }
            }
        }
    }
    printf("Count: %d\n", rawData.count);
    
    
        control[SET] = 2;
    
        if(libusb_interrupt_transfer(dev, EP3, control, 2, &sent_bytes, 0) !=0 )
           return -1;
    
        secPerSample = 1.0/options.sampFreq.freq;
        samples_per_screen = options.sampFreq.freq*(options.xScale*MICRO)*10;
    
        samplecount = rawData.count;
    
        shiftScale = (samplecount/POTMAX) - 1;
        cursScale = width/POTMAX;
    
        int startPos;
    
        ConverDataToBytes(&rawData, rawData.count, sampleData);
    
    
        processSamples(sampleData, samples_per_screen, 0, width, pixels_per_volt, offset,processedData );
        while(1){
            PacketTransfer(dev, NULL, EP2, potReading, NULL, LIBUSB_TRANSFER_TYPE_INTERRUPT);
            if(shiftPrevious != potReading[0] || cursorPrevious != potReading[1]){
                startPos = shiftScale*potReading[0];
                Start(width, height);
                Background(50,50,50);
                //grid(0,0,pixels_per_volt,width,height);
                processSamples(sampleData, samples_per_screen, 0, width, pixels_per_volt, startPos,processedData );
    
                for(int i = 0; i < options.channels; i++){
                    plotWave(processedData[i], samples_per_screen, pixels_per_volt*i, wavecolor[i]);
                }
                if(startPos <= eventLocation && eventLocation < samples_per_screen + startPos){
                    plotTriggerEvent(samples_per_screen, 0, width, pixels_per_volt, eventLocation-startPos);
                }
                DisplayCursor(samples_per_screen, 0, width, pixels_per_volt, cursScale*(POTMAX-potReading[1]));
                DisplayTime(startPos, width, secPerSample, eventLocation, samples_per_screen, cursScale*(POTMAX-potReading[0]));
                End();
                shiftPrevious = potReading[0];
                cursorPrevious = potReading[1];
            }
        }
    printf("Target Found");
    return 0;
    
}
void SelectWaveColors(){
    for(int i = 0; i < NUM_CHAN; i++){
        for(int j = 0; j < 3; j++){
            wavecolor[i][j] = rand() % 255;
        }
        wavecolor[i][3] = 5;
    }
}


