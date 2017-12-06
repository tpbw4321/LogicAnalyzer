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
#define SAMP_SIZE 10000
#define NUM_CHAN 8
#define MAX_PACKET 1000
#define MICRO 1e-6
#define POTMAX 255
#define TRIGMAX 256
static struct libusb_transfer * iso = NULL;
static libusb_device_handle * dev = NULL;
static queue rawData;
static queue processedData;
static void SelectWaveColors();
static uint8_t packetBuffer[MAX_PACKET];
static VGfloat wavecolor[8][4];
static argOptions options;
static uint8_t triggerEvents[TRIGMAX];
static int triggerCount= 0;
static uint8_t triggerFlag=0;
static int sampRemaining = 0;
static int evenLocation = 0;

static void LIBUSB_CALL ReadBufferData(struct libusb_transfer *transfer){
    struct libusb_iso_packet_descriptor *ipd = transfer->iso_packet_desc;
    char * item;
    
    if(ipd->status == LIBUSB_TRANSFER_COMPLETED){
        *(int *)transfer->user_data = ipd->actual_length;
        for(int i = 0; i < ipd->actual_length; i++){
            item = (char*)malloc(sizeof(char));
            *item = packetBuffer[i];
        }
    }
    else{
        perror("Failed");
        *(int *)transfer->user_data = -1;
        
    }
}

int main(int argc, const char * argv[]) {
    
    
    int width, height;
    int yscale = 8;
    int pixels_per_volt = 100;
    int data = 0;
    argOptions options;
    data_point processedData[NUM_CHAN][SAMP_SIZE];
    int sampleData[NUM_CHAN][SAMP_SIZE];
    unsigned char potReading[2]= {0,0};
    uint8_t * sample;
    
    
    //    SetDefaultOptions(&options);
    
    dev = SetupDevHandle(0x04B4, 0x8051);
    iso = SetupIsoTransfer(dev, EP1, packetBuffer, 1, ReadBufferData, &data);
    
    int frequency = 10000;
    int xscale = 10000;           //in microseconds
    int samples_per_screen = frequency*(xscale*MICRO);
    int samplecount = SAMP_SIZE;
    int processMax = samplecount/samples_per_screen;
    int offset = 0;
    int cursorScale = width/POTMAX;
    int shiftScale = (samplecount/POTMAX)-10;
    int potPrevious = 0;
    unsigned char buffer[64];
    int * item;
    
    char filename[] = "express";
    
    triggerCount = GenerateTriggers(filename, triggerEvents);
    sampRemaining = SAMP_SIZE/2;
    
    printf("Samples Remaining: %d\n", sampRemaining);
    
    
    
    SelectWaveColors();
    
    
    while(sampRemaining>0){
        if(PacketTransfer(dev, NULL, EP1, buffer , NULL, LIBUSB_TRANSFER_TYPE_BULK)){
            for(int i = 0; i < 64; i++){
                item = (int*)malloc(sizeof(int));
                *item = buffer[i];
                if(!triggerFlag && CheckTriggerEvent(triggerEvents, triggerCount, *item)){
                    triggerFlag = 1;
                    evenLocation = rawData.count;
                }
                if(triggerFlag)
                    sampRemaining--;
                
                else if(rawData.count > sampRemaining)
                    Dequeue(&rawData);
                
                if(sampRemaining > 0){
                    //printf("%3d ", *item);
                    Enqueue(&rawData, item);
                }
            }
        }
    }
    
    
    init(&width, &height);
    ConverDataToBytes(&rawData, rawData.count, sampleData);
    processSamples(sampleData, samples_per_screen, 0, width, pixels_per_volt, offset,processedData );
    while(1){
        PacketTransfer(dev, NULL, EP2, potReading, NULL, LIBUSB_TRANSFER_TYPE_INTERRUPT);
        if(potPrevious != potReading[0]){
            Start(width, height);
            Background(50,50,50);
            processSamples(sampleData, samples_per_screen, 0, width, pixels_per_volt, shiftScale*potReading[0],processedData );
            for(int i = 0; i < NUM_CHAN; i++){
                plotWave(processedData[i], samples_per_screen, pixels_per_volt*i, wavecolor[i]);
            }
            End();
            potPrevious = potReading[0];
        }
    }
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

