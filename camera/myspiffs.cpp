#include "SPIFFS.h"
#include "esp_camera.h"
#include "config.h"

void initSPIFFS(){
  //start spiffs (added from receiver sketch)
  if (!SPIFFS.begin()){Serial.println(F("ERROR: File System Mount Failed!"));}
  else{Serial.println(F("success init spifss"));}
}

File getPhotoSPIFFS(){
  return SPIFFS.open(FILE_PHOTO, FILE_READ); 
}

void savePhotoSPIFFS(camera_fb_t *fb){
    // Photo file name
    Serial.printf("Picture file name: %s\n", FILE_PHOTO);
    File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);

    // Insert the data in the photo file
    if (!file) {
      Serial.println("Failed to open file in writing mode");
    }
    else {
      file.write(fb->buf, fb->len); // payload (image), payload length
      Serial.print("The picture has been saved in ");
      Serial.print(FILE_PHOTO);
      Serial.print(" - Size: ");
      Serial.print(file.size());
      Serial.println(" bytes");
    }
    // Close the file
    file.close();
}
