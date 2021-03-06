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

#endif
