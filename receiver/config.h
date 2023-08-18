#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Version number of firmwarle
#define VERSION_STR "0.1"

// Minimal unix time for clock to be considered valid.
#define NOT_BEFORE_TIME 1568184212

//led pin
#define REDLED 33

//wifi ap setup
#define AP_CHANNEL 1
#define AP_SSID "willFree"
#define AP_PASS "nowifinocry"

extern String pictureswitchSTR;
extern String pictureRevSTR;

#endif
