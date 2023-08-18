#ifndef MYSPIFFS_H
#define MYSPIFFS_H

#include "esp_camera.h"
#include "SPIFFS.h"

void initSPIFFS();
File getPhotoSPIFFS();
void savePhotoSPIFFS(camera_fb_t*);

#endif // MYSPIFFS_H
