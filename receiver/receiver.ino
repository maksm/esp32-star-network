#include "config.h"
#include "mywifi.h"
#include "myespnow.h"
#include "mysd.h"
#include "mywebserver.h"

void setup() {
  pinMode(REDLED, OUTPUT);//red led
  Serial.begin(115200);
  Serial.println("ESP32CAM, Esp-Now receiver");

  initSD();
  configDeviceAP();
  InitESPNow();
  initWebServer(); 
}

void loop(){
  wifiLoop();
}

