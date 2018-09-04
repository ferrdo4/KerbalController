#define barleds 20
#define controlers 12 //displays * barleds / 8
byte leds[controlers];

void initShiftRegister()
{
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, LOW);
  digitalWrite(latchPin, LOW);
}

void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   for (int i = controlers; i >= 0; i--)
   {
      shiftOut(dataPin, clockPin, MSBFIRST, leds[i]);
   }
   digitalWrite(latchPin, HIGH);
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
    Data[i] = 0;
  }
  int pv1 = 0.1+pow(2, v1);
  int pv2 = 0.1+pow(2, v2);
  int pv3 = 0.1+pow(2, v3);
  int pv4 = 0.1+pow(2, v4);
  int pv5 = 0.1+pow(2, v5);

  Data[0] = pv5 >> 8;
  Data[1] = pv5;
  Data[2] = pv4 >> 2;
  Data[3] = (pv4 << 6) | (pv3 >> 4);
  Data[4] = (pv3 << 4) | (pv2 >> 6);
  Data[5] = (pv2 << 2) | (pv1 >> 8);
  Data[6] = pv1;
}
