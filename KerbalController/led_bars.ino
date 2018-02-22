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

