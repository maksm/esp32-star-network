#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Version number of firmwarle
#define VERSION_STR "0.1"

// Minimal unix time for clock to be considered valid.
#define NOT_BEFORE_TIME 1568184212

//led pin
#define WHITELED 4 //flash LED
#define REDLED 33 //Red LED
#define RXPIN 3 //Button trigger pin

//wifi ap setup
#define AP_CHANNEL 1

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define fileDatainMessage 240.0

#define PRINTSCANRESULTS 1
#define DELETEBEFOREPAIR 1

// Photo File Name to save in SPIFFS
#define FILE_PHOTO "/photo.jpg"

// for esp now connect
extern unsigned long lastConnectNowAttempt;
extern unsigned long nextConnectNowGap;
extern bool isPaired;

// for photo transmit
extern int currentTransmitCurrentPosition;
extern int currentTransmitTotalPackages;
extern byte sendNextPackageFlag;

//BL mod
extern unsigned long lastEndTrans;//last end of transmission
extern unsigned long lastphototime;
extern byte takeNextPhotoFlag;
extern boolean UseFlashLED;

#endif
