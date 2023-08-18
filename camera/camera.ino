#include "soc/soc.h"           // Disable brownour problems included as standard
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems included as standard

#include "config.h"
#include "myspiffs.h"
#include "mywifi.h"
#include "mycamera.h"
#include "myespnow.h"
#include "SPIFFS.h"

int PDATrans = 10000;//photo delay after transmission

void setup() {
  // NEEDED ????
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  // start serial
  Serial.begin(115200);
  Serial.println("CAMERA ESP-NOW MASTER STARTED");
  // init the led's
  pinMode(WHITELED, OUTPUT); pinMode(REDLED, OUTPUT);
  digitalWrite(WHITELED, LOW); digitalWrite(REDLED, HIGH);

  initCamera();
  initSPIFFS();
  initWiFiTransmitter();
  initESPNow();
}

void loop() {
  // if we are:
  // 2. NOT PARIED
  // 3. OUR LAST CONNECT ATTMEPT WAS OVER DUE
  if (!isPaired  && lastConnectNowAttempt + nextConnectNowGap < millis())
  {
    Serial.println("NOT CONNECTED -> TRY TO CONNECT");
    ScanAndConnectToSlave();
    // if we connected
    if (isPaired)
    {
      blinkIt(150, 2);
    }
    else
    {
      //nextConnectNowGap *= 2; // dbl the gap (disabled)
      blinkIt(150, 3); // blink 3 times
    }
    // save last attmpe
    lastConnectNowAttempt = millis();
  }

  // if we are :
  // 2. The button is LOW (pressed)
  // 3. we are currently not have currentTransmitTotalPackages set
  // 4. the sendNextPackageFlag is not set.

  if (!currentTransmitTotalPackages && !sendNextPackageFlag )
    takeNextPhotoFlag = 1;

  // if the sendNextPackageFlag is set
  if (sendNextPackageFlag)
    sendNextPackage();

  // if takeNextPhotoFlag is set
  if (takeNextPhotoFlag == 1 && millis() >= (lastEndTrans+PDATrans))//if photo is requested and idle time exceeded take new photo and send.
    capturePhotoSaveSpiffs();
  }


/* ***************************************************************** */
/*                  HELPERS RELATED FUNCTIONS                        */
/* ***************************************************************** */
void blinkIt(int delayTime, int times)
{
  for (int i = 0; i < times; i++)
  {
    if(UseFlashLED == 1){digitalWrite(WHITELED, LOW);}else{digitalWrite(REDLED, HIGH);}
    delay(delayTime);
    if(UseFlashLED == 1){digitalWrite(WHITELED, HIGH);}else{digitalWrite(REDLED, LOW);}
    delay(delayTime);
  }
}
