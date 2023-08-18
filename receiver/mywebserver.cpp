#include "ESPAsyncWebServer.h"//You also need Async-tcp lib with that one!
#include "SD.h"

#include "config.h"
#include "mysd.h"

//Webserver-------------------------------------------------------------------------
AsyncWebServer server(80);//Create an Async Web Server on port 80.
AsyncEventSource events("/events");//To automatically display the information on the web server when a new reading arrives, we’ll use Server-Sent Events (SSE).

void initWebServer(){
  //Webserver
  //Get main page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/index.html", "text/html");
  });
  //Get CSS library file
  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/w3.css", "text/css");
  });
  //Get javascript file
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/script.js", "text/javascript");
  });
    
  //Get the photo
  server.on("/img", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, pictureRevSTR, "image/jpeg", false);
  });

  //Get the RSSI percent value
  server.on("/readRSSI", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "N/A");
  });

  //set time from browser
  server.on("/set_time", HTTP_POST, [](AsyncWebServerRequest *request){
    String timeStr = "";
    if (request->hasParam("time", true)) { // true indicates a POST parameter
      AsyncWebParameter* p = request->getParam("time", true);
      timeStr = p->value().c_str();
    } else {
      request->send(200, "application/json", "{\"success\":false,\"error\":\"Parameter time not found\"}");
    }

    size_t msPos = timeStr.length() - 3;

    struct timeval now = {
      timeStr.substring(0, msPos).toInt(),
      timeStr.substring(msPos).toInt() * 1000
    };

    if (now.tv_sec < NOT_BEFORE_TIME) {
      request->send(200, "application/json", "{\"success\":false,\"error\":\"Invalid time value\"}");
    }

    if (settimeofday(&now, NULL) != 0) {
      request->send(200, "application/json", "{\"success\":false,\"error\":\"Failed to set time\"}");
    }
    request->send(200, "application/json", "{\"success\":true}");
  });

  server.begin();
  logToSD("Server active!");
}
