#include "config.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "myspiffs.h"
#include "FS.h"

// Global copy of slave
esp_now_peer_info_t slave;

uint8_t ReceiverMacID[6] = {0xB0, 0xA7, 0x32, 0x15, 0xD9, 0x95};//Modify this according to the sender board MAC adress (example here for MAC address 10:52:1C:63:3F:B4)
unsigned long sendStartTime;
 
void sendData(uint8_t * dataArray, uint8_t dataArrayLength) {
  const uint8_t *peer_addr = slave.peer_addr;
  esp_err_t result = esp_now_send(peer_addr, dataArray, dataArrayLength);
  if (result == ESP_OK) {
    //Serial.println("Success");
  } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW not Init.");
  } else if (result == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("Internal Error");
  } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
  } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }
}

void startTransmitSPIFFS()//modification to send from SPIFFS, this function open file and display data
{
  Serial.println("Starting transmit");//inform about the current task
  File file = getPhotoSPIFFS();//open the file to send from spiff
  Serial.print(FILE_PHOTO); Serial.print(" - Size: "); Serial.print(file.size()); Serial.println(" bytes");//display name and size
  
  int fileSize = file.size();//make variable containing the file size
  file.close();//close the file
  currentTransmitCurrentPosition = 0;
  currentTransmitTotalPackages = ceil(fileSize / fileDatainMessage);
  Serial.println(currentTransmitTotalPackages);
  uint8_t message[] = {0x01, currentTransmitTotalPackages >> 8, (byte) currentTransmitTotalPackages};
  sendStartTime = millis();
  sendData(message, sizeof(message));
}

void sendNextPackage(){
    // clear the flag
  sendNextPackageFlag = 0;

  // if got to AFTER the last package
  if (currentTransmitCurrentPosition == currentTransmitTotalPackages)
  {
    currentTransmitCurrentPosition = 0;
    currentTransmitTotalPackages = 0;
    Serial.println("Done submiting files");
    Serial.println();
    lastEndTrans = millis();//set last end of transmission
    takeNextPhotoFlag = 1;//ask next photo
    return;
  } //end if

  File file = getPhotoSPIFFS();//open the file to send from spiff
  if (!file) {
    Serial.println("Failed to open file in writing mode");
    return;
  }

  // set array size.
  int fileDataSize = fileDatainMessage;
  // if its the last package - we adjust the size !!!
  if (currentTransmitCurrentPosition == currentTransmitTotalPackages - 1)
  {
    Serial.println("*************************");
    Serial.println(file.size());
    Serial.println(currentTransmitTotalPackages - 1);
    Serial.println((currentTransmitTotalPackages - 1)*fileDatainMessage);
    fileDataSize = file.size() - ((currentTransmitTotalPackages - 1) * fileDatainMessage);
  }

  //Serial.println("fileDataSize=" + String(fileDataSize));

  // define message array
  uint8_t messageArray[fileDataSize + 3];
  messageArray[0] = 0x02;

  file.seek(currentTransmitCurrentPosition * fileDatainMessage);
  currentTransmitCurrentPosition++; // set to current (after seek!!!)
  //Serial.println("PACKAGE - " + String(currentTransmitCurrentPosition));

  messageArray[1] = currentTransmitCurrentPosition >> 8;
  messageArray[2] = (byte) currentTransmitCurrentPosition;
  for (int i = 0; i < fileDataSize; i++)
  {
    if (file.available())
    {
      messageArray[3 + i] = file.read();
    } //end if available
    else
    {
      Serial.println("END !!!");
      break;
    }
  } //end for

  sendData(messageArray, sizeof(messageArray));
  file.close();
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (currentTransmitTotalPackages)
  {
    sendNextPackageFlag = 1;
    // if nto suecess 0 resent the last one
    if (status != ESP_NOW_SEND_SUCCESS)
    {
      // Check for timeout
      if (millis() - sendStartTime > SEND_TIMEOUT) {
        Serial.println("Send operation timed out!");
        currentTransmitCurrentPosition = 0;
        currentTransmitTotalPackages = 0;
        sendNextPackageFlag = 0;
        lastEndTrans = millis();
        return;
      }
      currentTransmitCurrentPosition--;
    }
  } //end if
}

void initESPNow() {
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
}

void deletePeer() {
  const esp_now_peer_info_t *peer = &slave;
  const uint8_t *peer_addr = slave.peer_addr;
  esp_err_t delStatus = esp_now_del_peer(peer_addr);
  Serial.print("Slave Delete Status: ");
  if (delStatus == ESP_OK) {
    // Delete success
    Serial.println("Success");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW Not Init");
  } else if (delStatus == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }
}

bool manageSlave() {
  if (slave.channel == AP_CHANNEL) {
    if (DELETEBEFOREPAIR) {
      deletePeer();
    }

    Serial.print("Slave Status: ");
    const esp_now_peer_info_t *peer = &slave;
    const uint8_t *peer_addr = slave.peer_addr;
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(peer_addr);
    if ( exists) {
      // Slave already paired.
      Serial.println("Already Paired");
      return true;
    } else {
      // Slave not paired, attempt pair
      esp_err_t addStatus = esp_now_add_peer(peer);
      if (addStatus == ESP_OK) {
        // Pair success
        Serial.println("Pair success");
        return true;
      } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        Serial.println("ESPNOW Not Init");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        Serial.println("Peer list full");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("Out of memory");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        Serial.println("Peer Exists");
        return true;
      } else {
        Serial.println("Not sure what happened");
        return false;
      }
    }
  } else {
    // No slave found to process
    Serial.println("No Slave found to process");
    return false;
  }
}

void ScanAndConnectToSlave() {
  memcpy(slave.peer_addr, ReceiverMacID, sizeof(ReceiverMacID));
  slave.channel = AP_CHANNEL; // pick a channel
  slave.encrypt = 0; // no encryption
  // Add slave as peer if it has not been added already
  isPaired = manageSlave();
  if (isPaired) {
    Serial.println("Slave pair success!");
  } else {
    // slave pair failed
    Serial.println("Slave pair failed!");
  }
} //end functin
