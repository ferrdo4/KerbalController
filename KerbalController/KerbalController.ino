#include "init.h"

bar5 barsS;
bar2 bars;

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

void doDebug()
{
  String l1 = "";
  (!digitalRead(pABORT))?l1+="A":l1+="a"; //note abort switch is active high
  (!digitalRead(pARM))?l1+="A":l1+="a"; //note arm switch is active high
  (!digitalRead(pSTAGE))?l1+="S":l1+="s";
  digitalWrite(pSTAGELED, digitalRead(pSTAGE));
  (digitalRead(pRCS))?l1+="R":l1+="r";
  digitalWrite(pRCSLED, digitalRead(pRCS));
  (digitalRead(pSAS))?l1+="S":l1+="s";
  digitalWrite(pSASLED, digitalRead(pSAS));
  (digitalRead(pBRAKES))?l1+="B":l1+="b";
  digitalWrite(pBRAKESLED, digitalRead(pBRAKES));
  (digitalRead(pGEARS))?l1+="G":l1+="g";
  digitalWrite(pGEARSLED, digitalRead(pGEARS));
  (digitalRead(pSOLAR))?l1+="S":l1+="s";
  digitalWrite(pSOLARLED, digitalRead(pSOLAR));
  (digitalRead(pCHUTES))?l1+="C":l1+="c";
  digitalWrite(pCHUTESLED, digitalRead(pCHUTES));
  (digitalRead(pLIGHTS))?l1+="L":l1+="l";
  digitalWrite(pLIGHTSLED, digitalRead(pLIGHTS));   
  (digitalRead(pLADDER))?l1+="L":l1+="l";
  digitalWrite(pLADDERLED, digitalRead(pLADDER));
  (digitalRead(pACTION1))?l1+="1":l1+="a";
  digitalWrite(pACTION1LED, digitalRead(pACTION1));
  (digitalRead(pACTION2))?l1+="2":l1+="a";
  digitalWrite(pACTION2LED, digitalRead(pACTION2));
  (digitalRead(pACTION3))?l1+="3":l1+="a";
  digitalWrite(pACTION3LED, digitalRead(pACTION3));
  (digitalRead(pACTION4))?l1+="4":l1+="a";
  digitalWrite(pACTION4LED, digitalRead(pACTION4));
  char cLCDState[1];
  sprintf(cLCDState, "%i", pLCDState);
  l1+=String(cLCDState);
  char line[17];
  l1.toCharArray(line, 17);
  throttle_value = analogRead(pTHROTTLE);
  char throttle_char[5];
  itoa(throttle_value, throttle_char, 10);

  barsRefresh = throttle_value;

  now = millis();
  debugTime = now - debugTimeOld;

  if (debugTime > barsRefresh) 
  {
    clearLCD();  
    writeLCD(line);
    jumpToLineTwo();
    writeLCD(throttle_char);
    debugTimeOld = now;

    bars.setNums(barVal, barVal + 10);
    bars.up();

    barsS.setNums(barVal, barVal+2, barVal+4, barVal+6, barVal+8);
    barsS.up();
    
    ++barVal;
  }
}
