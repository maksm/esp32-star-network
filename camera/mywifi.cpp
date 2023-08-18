#include "config.h"
#include "WiFi.h"

void initWiFiTransmitter(){
  //Set device in STA mode to begin with
  WiFi.mode(WIFI_STA);
  // This is the mac address of the Master in Station Mode
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
}
