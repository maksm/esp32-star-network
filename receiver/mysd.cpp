#include "config.h"
#include "SD.h"

void initSD(){
  //start sd
  if (!SD.begin(5)){Serial.println(F("ERROR: File System Mount Failed!"));}
  else{Serial.println(F("success init SD"));}
}

void copyFile(){
  Serial.println("Starting copying files");
  File sourceFile = SD.open("/picture.jpg", FILE_READ);
  if (!sourceFile) {
    Serial.println("Failed to open source file for reading");
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

  Serial.print("Copying to: ");
  Serial.println(filename);

  File destFile = SD.open(filename, FILE_WRITE);
  if (!destFile) {
    Serial.println("Failed to open destination file for writing");
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
  Serial.println("Ended copying files");
}
