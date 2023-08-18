#include "config.h"
#include "WiFi.h"
#include <DNSServer.h>

static const byte DNS_PORT = 53;
static const IPAddress SERVER_IP(192, 168, 1, 1);
static const IPAddress GATEWAY_IP(0, 0, 0, 0);
static const IPAddress SERVER_NETMASK(255, 255, 255, 0);

DNSServer dnsServer;

// config AP SSID------------------------------------------------------------------
void configDeviceAP() {
  WiFi.softAPConfig(SERVER_IP, GATEWAY_IP, SERVER_NETMASK);
  bool result = WiFi.softAP(AP_SSID, AP_PASS, AP_CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: willFree");
  }
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  Serial.println("Staring DNS server");
  dnsServer.start(DNS_PORT, "*", SERVER_IP);

}//end of esp now slave init function

void wifiLoop(){
  dnsServer.processNextRequest();
}
