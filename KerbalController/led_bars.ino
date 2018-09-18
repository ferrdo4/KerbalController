#define barleds 20
#define controlers 12 //displays * barleds / 8
byte leds[controlers];
byte ledsShort[7];

void initShiftRegister()
{
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, LOW);
  digitalWrite(latchPin, LOW);

  digitalWrite(dataPinS, LOW);
  digitalWrite(clockPinS, LOW);
  digitalWrite(latchPinS, LOW);
}

void updateShiftRegister()
{
  digitalWrite(latchPin, LOW);
  digitalWrite(latchPinS, LOW);
  for (int i = controlers; i >= 0; i--)
    shiftOut(dataPin, clockPin, MSBFIRST, leds[i]);
  for(int i = 0; i < 7; ++i)
    shiftOut(dataPinS, clockPinS, MSBFIRST, ledsShort[i]); 
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPinS, HIGH);
}

void clearNum()
{
   for (int i = controlers; i >= 0; i--)
   {
      leds[i] = 0;
   }
}

void setNum(int num, int disp)
{
    int j = (num + 24 * disp) / 8;
    int k = (num + 24 * disp) % 8;
    leds[j] = (1<<k);
}

void setValues(byte v1, byte v2, byte v3, byte v4, byte v5)
{
  for (int i = 0; i < 7; ++i)
  {
    ledsShort[i] = 0;
  }
  int pv1 = 0.1+pow(2, v1);
  int pv2 = 0.1+pow(2, v2);
  int pv3 = 0.1+pow(2, v3);
  int pv4 = 0.1+pow(2, v4);
  int pv5 = 0.1+pow(2, v5);

  ledsShort[0] = pv5 >> 8;
  ledsShort[1] = pv5;
  ledsShort[2] = pv4 >> 2;
  ledsShort[3] = (pv4 << 6) | (pv3 >> 4);
  ledsShort[4] = (pv3 << 4) | (pv2 >> 6);
  ledsShort[5] = (pv2 << 2) | (pv1 >> 8);
  ledsShort[6] = pv1;
}
