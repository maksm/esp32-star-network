#include "esp_now.h"
#include "esp_wifi.h"
#include "SD.h"

#include "config.h"
#include "mysd.h"

int currentTransmitCurrentPosition = 0;
int currentTransmitTotalPackages = 0;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  float picture;
  unsigned int readingId;
} struct_message;

// callback when data is recv from Master-------------------------------------------
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  switch (*data++)
  {
    case 0x01:
      Serial.println("Data from: ");
      for (int i = 0; i < 6; i++) {
        Serial.print(mac_addr[i], HEX);
        if (i < 5) Serial.print(":");
      }
      Serial.println();

     logToSD("Start of new file transmit");
      currentTransmitCurrentPosition = 0;
      currentTransmitTotalPackages = (*data++) << 8 | *data;
      logToSD("currentTransmitTotalPackages = " + String(currentTransmitTotalPackages));
      SD.remove(pictureswitchSTR);
      break;
    case 0x02:
      //Serial.println("chunk of file transmit");
      currentTransmitCurrentPosition = (*data++) << 8 | *data++;
      //Serial.println("chunk NUMBER = " + String(currentTransmitCurrentPosition));
      File file = SD.open(pictureswitchSTR,FILE_APPEND);
      if (!file)
        logToSD("Error opening file ...");
        
      for (int i=0; i < (data_len-3); i++)
      {
        //byte dat = *data++;
        //Serial.println(dat);
        file.write(*data++);
      }
      file.close();

      if (currentTransmitCurrentPosition == currentTransmitTotalPackages)
      {
        logToSD("done file transfer");
        SD.open(pictureswitchSTR);
        file.close();
        logToSD("File saved and closed.");

        if(pictureswitchSTR == "/picture.jpg"){pictureswitchSTR = "/picturetwo.jpg"; pictureRevSTR = "/picture.jpg";}
          else{ pictureswitchSTR = "/picture.jpg"; pictureRevSTR = "/picturetwo.jpg";}
        copyFile();
      }      
      break;
  } //end case 
} //end of OnDataRecv function


// Init ESP Now with fallback------------------------------------------------------
void InitESPNow() {
  if (esp_now_init() == ESP_OK) {
    logToSD("ESPNow Init Success");
  }
  else {
    logToSD("ESPNow Init Failed");
    ESP.restart();
  }
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_recv_cb(OnDataRecv);//function to receive data
  

}//end of esp now init function

