#include "esp_now.h"

void startTransmitSPIFFS();
void sendNextPackage();
void sendData(uint8_t * dataArray, uint8_t dataArrayLength);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void initESPNow();
void ScanAndConnectToSlave();
bool manageSlave();
void deletePeer();
