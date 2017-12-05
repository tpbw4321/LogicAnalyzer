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

#define EP1 (0x80|0x01)
#define SAMP_SIZE 10000
#define NUM_CHAN 8
#define MAX_PACKET 1000
#define MICRO 1e-6
static struct libusb_transfer * iso = NULL;
static libusb_device_handle * dev = NULL;
static queue rawData;
static queue processedData;
static void SelectWaveColors();
static uint8_t packetBuffer[MAX_PACKET];
static VGfloat wavecolor[8][4];
static argOptions options;


static void LIBUSB_CALL ReadBufferData(struct libusb_transfer *transfer){
    struct libusb_iso_packet_descriptor *ipd = transfer->iso_packet_desc;
    char * item;
    
    if(ipd->status == LIBUSB_TRANSFER_COMPLETED){
        *(int *)transfer->user_data = ipd->actual_length;
        for(int i = 0; i < ipd->actual_length; i++){
            item = (char*)malloc(sizeof(char));
            *item = packetBuffer[i];
            Enqueue(&rawData, item);
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
    
    init(&width, &height);
    
    uint8_t * sample;
    
    for(int i = 0; i < SAMP_SIZE; i++){
        sample = (uint8_t*)malloc(sizeof(uint8_t));
        *sample = i%256;
        Enqueue(&rawData, sample);
    }
    
//    SetDefaultOptions(&options);
//
//    dev = SetupDevHandle(0x04B4, 0x8051);
//    iso = SetupIsoTransfer(dev, EP1, packetBuffer, 1, ReadBufferData, &data);
//
//    int packetSize = libusb_get_max_iso_packet_size(libusb_get_device(dev), EP1);
//    printf("Packet Size: %d", packetSize);
    int frequency = 1000;
    int xscale = 100;           //in microseconds
    int samples_per_screen = frequency*(xscale*MICRO)+1;
    
    
    ConverDataToBytes(&rawData, SAMP_SIZE, sampleData);
    
    processSamples(sampleData, samples_per_screen, 0, width, pixels_per_volt, processedData );
    
    Start(width, height);
    Background(50,50,50);
    SelectWaveColors();
    
    for(int i = 0; i < NUM_CHAN; i++){
        plotWave(processedData[i], samples_per_screen, pixels_per_volt*i, wavecolor[i]);
    }
    
    End();
    while(1){
        //PacketTransfer(dev, iso, EP1, NULL, &data, LIBUSB_TRANSFER_TYPE_ISOCHRONOUS);
    }
    
}
void SelectWaveColors(){
    for(int i = 0; i < NUM_CHAN; i++){
        for(int j = 0; j < 3; j++){
            wavecolor[i][j] = rand() % 255;
        }
        wavecolor[i][3] = 5;
    }
}

