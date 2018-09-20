#ifndef INIT_H
#define INIT_H

#include "bars.h"
#include "lcd.h"
#include "vals.h"

void controlsInit() 
{
  pinMode(pTHROTTLE, INPUT);
  pinMode(pLCD, INPUT_PULLUP);
  pinMode(pSAS, INPUT_PULLUP);
  pinMode(pRCS, INPUT_PULLUP);
  pinMode(pABORT, INPUT_PULLUP);
  pinMode(pARM, INPUT_PULLUP);
  pinMode(pSTAGE, INPUT_PULLUP);
  pinMode(pSTAGELED, OUTPUT);
  pinMode(pLIGHTS, INPUT_PULLUP);
  pinMode(pLIGHTSLED, OUTPUT);
  pinMode(pLADDER, INPUT_PULLUP);
  pinMode(pLADDERLED, OUTPUT);
  pinMode(pSOLAR, INPUT_PULLUP);
  pinMode(pSOLARLED, OUTPUT);
  pinMode(pCHUTES, INPUT_PULLUP);
  pinMode(pCHUTESLED, OUTPUT);
  pinMode(pGEARS, INPUT_PULLUP);
  pinMode(pGEARSLED, OUTPUT);
  pinMode(pBRAKES, INPUT_PULLUP);
  pinMode(pBRAKESLED, OUTPUT);
  pinMode(pACTION1, INPUT_PULLUP);
  pinMode(pACTION1LED, OUTPUT);
  pinMode(pACTION2, INPUT_PULLUP);
  pinMode(pACTION2LED, OUTPUT);
  pinMode(pACTION3, INPUT_PULLUP);
  pinMode(pACTION3LED, OUTPUT);
  pinMode(pACTION4, INPUT_PULLUP);
  pinMode(pACTION4LED, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPinS, OUTPUT);
  pinMode(clockPinS, OUTPUT);
  pinMode(dataPinS, OUTPUT);
  pLCDState = 0;
}

void testLEDS(int testdelay){
  digitalWrite(pSTAGELED,HIGH);
  delay(testdelay);
  digitalWrite(pSTAGELED,LOW);
  digitalWrite(pLIGHTSLED,HIGH);
  delay(testdelay);
  digitalWrite(pLIGHTSLED,LOW);
  digitalWrite(pLADDERLED,HIGH);
  delay(testdelay);
  digitalWrite(pLADDERLED,LOW);
  digitalWrite(pSOLARLED,HIGH);
  delay(testdelay);
  digitalWrite(pSOLARLED,LOW);
  digitalWrite(pCHUTESLED,HIGH);
  delay(testdelay);
  digitalWrite(pCHUTESLED,LOW);
  digitalWrite(pGEARSLED,HIGH);
  delay(testdelay);
  digitalWrite(pGEARSLED,LOW);
  digitalWrite(pBRAKESLED,HIGH);
  delay(testdelay);
  digitalWrite(pBRAKESLED,LOW);
  digitalWrite(pACTION1LED,HIGH);
  delay(testdelay);
  digitalWrite(pACTION1LED,LOW);
  digitalWrite(pACTION2LED,HIGH);
  delay(testdelay);
  digitalWrite(pACTION2LED,LOW);
  digitalWrite(pACTION3LED,HIGH);
  delay(testdelay);
  digitalWrite(pACTION3LED,LOW);
  digitalWrite(pACTION4LED,HIGH);
  delay(testdelay);
  digitalWrite(pACTION4LED,LOW);
}

void doDebug()
{
  String l1 = "";
  (digitalRead(pABORT))?l1+="A":l1+="a"; //note abort switch is active high
  (digitalRead(pARM))?l1+="A":l1+="a"; //note arm switch is active high
  (!digitalRead(pSAS))?l1+="S":l1+="s";
  digitalWrite(pSASLED, digitalRead(pSAS));
  (!digitalRead(pRCS))?l1+="R":l1+="r";
  digitalWrite(pRCSLED, digitalRead(pRCS));
  (digitalRead(pSTAGE))?l1+="S":l1+="s";
  digitalWrite(pSTAGELED, digitalRead(pSTAGE));
  (digitalRead(pSOLAR))?l1+="S":l1+="s";
  digitalWrite(pSOLARLED, digitalRead(pSOLAR));
  (digitalRead(pLIGHTS))?l1+="L":l1+="l";
  digitalWrite(pLIGHTSLED, digitalRead(pLIGHTS));   
  (digitalRead(pLADDER))?l1+="L":l1+="l";
  digitalWrite(pLADDERLED, digitalRead(pLADDER));
  (digitalRead(pBRAKES))?l1+="B":l1+="b";
  digitalWrite(pBRAKESLED, digitalRead(pBRAKES));
  (digitalRead(pCHUTES))?l1+="C":l1+="c";
  digitalWrite(pCHUTESLED, digitalRead(pCHUTES));
  (digitalRead(pGEARS))?l1+="G":l1+="g";
  digitalWrite(pGEARSLED, digitalRead(pGEARS));
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

  now = millis();
  debugTime = now - debugTimeOld;

  if (debugTime > barsRefresh) 
  {
    clearLCD();  
    writeLCD(line);
    jumpToLineTwo();
    writeLCD(throttle_char);
    debugTimeOld = now;

    bars.setNums(barVal, barVal+10);
    bars.up();

    barsS.setNums(barVal, barVal+2, barVal+4, barVal+6, barVal+8);
    barsS.up();
    
    ++barVal;
  }
}
#endif
