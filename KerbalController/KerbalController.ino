#include "init.h"

void setup() 
{
  Serial.begin(38400);  //KSPSerialIO connection
 
  initLCD();
  writeLCD("KerbalController");
  jumpToLineTwo();
  writeLCD("booting...");
  delay(100);

  //Initialize
  controlsInit();   //set all pin modes
  InitTxPackets();  //initialize the serial communication
  if (!digitalRead(pABORT) && !digitalRead(pARM))
  {
    debug = true;
    barVal = 0;
  }
}

void loop() 
{
  // debounce issue
  bool LCDPressed = !digitalRead(pLCD);
  if (LCDPressed != lastButtonState)
  {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (LCDPressed != buttonState)
    {
      buttonState = LCDPressed;
      if (buttonState == HIGH)
      {
        pLCDState = (pLCDState + 1) % 8;
      }
    }
  }
  lastButtonState = LCDPressed;
  
  if(debug)
  {
    doDebug();
  }
  else 
  {
    get_vessel_data();
    send_control_packet();
  }
}
