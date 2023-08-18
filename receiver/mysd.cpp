#include "config.h"
#include "SD.h"

void logToSD(String message) {
  time_t now = time(NULL);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  char timeStr[25];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S: ", &timeinfo);
  Serial.println(String(timeStr) + message);
  File logFile = SD.open("/log.txt", FILE_WRITE);
  if (!logFile) {
    Serial.println("Failed to open log.txt for writing");
    return;
  }
  logFile.println(String(timeStr) + message);
  logFile.close();
}

void initSD(){
  //start sd
  if (!SD.begin(5)){Serial.println(F("ERROR: File System Mount Failed!"));}
  else{logToSD("success init SD");}
}

void copyFile(){
  logToSD("Starting copying files");
  File sourceFile = SD.open("/picture.jpg", FILE_READ);
  if (!sourceFile) {
    logToSD("Failed to open source file for reading");
    return;
  }

  // Generate filename
  time_t now = time(NULL);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);

  char filename[sizeof("/pics") + 15 + 4 + 1];
  size_t capture_path_len = strlen("/pics");
  strcpy(filename, "/pics");
  strftime(&filename[capture_path_len], sizeof(filename) - capture_path_len,
             "/%Y%m%d_%H%M%S.jpg", &timeinfo);

  logToSD(String("Copying to: ") + filename);

  File destFile = SD.open(filename, FILE_WRITE);
  if (!destFile) {
    logToSD("Failed to open destination file for writing");
    sourceFile.close();
    return;
  }

  byte buffer[512];
  while (sourceFile.available()) {
    int bytesRead = sourceFile.read(buffer, sizeof(buffer));
    destFile.write(buffer, bytesRead);
  }

  sourceFile.close();
  destFile.close();
  logToSD("Ended copying files");
}


