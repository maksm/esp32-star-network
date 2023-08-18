#include "config.h"

// for esp now connect
unsigned long lastConnectNowAttempt = 0;
unsigned long nextConnectNowGap = 3000; // 1000 at origin
bool isPaired = 0;

// for photo transmit
int currentTransmitCurrentPosition = 0;
int currentTransmitTotalPackages = 0;
byte sendNextPackageFlag = 0;

//BL mod
unsigned long lastEndTrans = 0;
unsigned long lastphototime = 0;
byte takeNextPhotoFlag = 1;
boolean UseFlashLED = 0;
