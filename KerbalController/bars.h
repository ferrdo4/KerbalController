#ifndef BARS_H
#define BARS_H

//digital pins long
const int dataPin = 8;      //DS - yellow
const int clockPin = 9;     //SH_CP - orange
const int latchPin = 10;    //ST_CP - red

//digital pins short
const int dataPinS = 11;      //DS - brown
const int clockPinS = 12;     //SH_CP - orange
const int latchPinS = 13;    //ST_CP - red

struct ledsS
{
  byte val;
  int on;
  int off;  
};

// ----------========= bar 2
struct bar2
{
  bar2()
  {
    digitalWrite(dataPin, LOW);
    digitalWrite(clockPin, LOW);
    digitalWrite(latchPin, LOW);
    clearData();
    bars[0].val = 0;
    bars[1].val = 0;
  }

  void setNum(int num, int val, int on = -1, int off = -1)
  {
    bars[num].val = val % 20;
    bars[num].on = on;
    bars[num].off = off;
  }

  setNums(byte v1, byte v2)
  {
    bars[0].val = v1 % 20;
    bars[1].val = v2 % 20;
  }

  void up()
  {
    clearData();   
    for (int i = 0; i < 2; ++i)
    {
      int j = (bars[i].val + 24 * i) / 8;
      int k = (bars[i].val + 24 * i) % 8;
      data[j] = (1<<k);    
    }
    writeBus();
  }

private:
  void clearData()
  {
    for(int i=0; i < 6; ++i)
      data[i] = 0;   
  }
  
  void writeBus()
  {
    digitalWrite(latchPin, LOW);
    for(int i = 5; i >= 0; --i)
      shiftOut(dataPin, clockPin, MSBFIRST, data[i]);
    digitalWrite(latchPin, HIGH);
  }
  
  byte data[5]; 
  ledsS bars[2];
};

// ----------========= bar5
struct bar5
{
  bar5()
  {
    digitalWrite(dataPinS, LOW);
    digitalWrite(clockPinS, LOW);
    digitalWrite(latchPinS, LOW);
    clearData();
  }
  
  void setNum(byte num, byte val, int on = -1, int off = -1)
  {
//    bars[num].val = val % 10;
    bars[num].val = 9 - val % 10;
    bars[num].on = on;
    bars[num].off = off;
  }
  
  void setNums(byte v1, byte v2, byte v3, byte v4, byte v5)
  {
//    bars[0].val = v1 % 10;
//    bars[1].val = v2 % 10;
//    bars[2].val = v3 % 10;
//    bars[3].val = v4 % 10;
//    bars[4].val = v5 % 10;  
    bars[0].val = 9 - v1 % 10;
    bars[1].val = 9 - v2 % 10;
    bars[2].val = 9 - v3 % 10;
    bars[3].val = 9 - v4 % 10;
    bars[4].val = 9 - v5 % 10;
  }

  void up()
  {
    int pv1 = 0.1+pow(2, bars[0].val);
    int pv2 = 0.1+pow(2, bars[1].val);
    int pv3 = 0.1+pow(2, bars[2].val);
    int pv4 = 0.1+pow(2, bars[3].val);
    int pv5 = 0.1+pow(2, bars[4].val);

    clearData();    
    data[0] = pv5 >> 8;
    data[1] = pv5;
    data[2] = pv4 >> 2;
    data[3] = (pv4 << 6) | (pv3 >> 4);
    data[4] = (pv3 << 4) | (pv2 >> 6);
    data[5] = (pv2 << 2) | (pv1 >> 8);
    data[6] = pv1;

    writeBus();
  }
  
private:  
  byte data[7];
  ledsS bars[5];

  void clearData()
  {
    for(int i=0; i < 7; ++i)
      data[i] = 0;   
  }

  void writeBus()
  {
    digitalWrite(latchPinS, LOW);
    for(int i = 0; i < 7; ++i)
      shiftOut(dataPinS, clockPinS, MSBFIRST, data[i]); 
    digitalWrite(latchPinS, HIGH);     
  }
};

#endif
