//debug
bool debug = false;
long debugTime, debugTimeOld;
const int barsRefresh = 80;
int barVal;

//analog pins
const int pTHROTTLE = A0; //slide pot

//lcd pins
const int rs = 6, en = 7, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

//digital pins
const int latchPin = 10;    //ST_CP - green
const int dataPin = 8;      //DS - yellow
const int clockPin = 9;     //SH_CP - blue

const int pSASLED = 22;     //SAS switch LED
const int pSAS = 23;        //SAS switch
const int pRCSLED = 24;     //RCS switch LED
const int pRCS = 25;        //RCS switch
const int pARMLED = 27;     //Arm switch LED(safety switch, active high) 
const int pARM = 27;        //Arm switch (safety switch, active high)
const int pABORTLED = 28;   //Abort switch LED (safety switch, active high)
const int pABORT = 29;      //Abort switch (safety switch, active high)
const int pSOLARLED = 30;   //Solar button LED
const int pSOLAR = 31;      //Solar button (action group 6)
const int pBRAKESLED = 32;  //Brakes button LED
const int pBRAKES = 33;     //Brakes button
const int pCHUTESLED = 34;  //Chutes button LED
const int pCHUTES = 35;     //Chutes button (action group 7)
const int pLIGHTSLED = 36;  //Lights button LED
const int pLIGHTS = 37;     //Lights button
const int pLADDERLED = 38;  //Ladder button LED
const int pLADDER = 39;     //Ladder button (action group 5)
const int pGEARSLED = 40;   //Gears button LED
const int pGEARS = 41;      //Gears button
const int pACTION1LED = 42; //Action Group 1 button LED
const int pACTION1 = 43;    //Action Group 1 button
const int pACTION2LED = 48; //Action Group 2 button LED
const int pACTION2 = 49;    //Action Group 2 button
const int pACTION3LED = 46; //Action Group 3 button LED
const int pACTION3 = 47;    //Action Group 3 button
const int pACTION4LED = 44; //Action Group 4 button LED
const int pACTION4 = 45;    //Action Group 4 button
const int pSTAGELED = 50;   //Stage button LED
const int pSTAGE = 51;      //Stage button

const int pLCD = 53;        //toggle LCD switch
int pLCDState = 0;          //LCD switch state
//const int pMODE = 54;       //mode switch (used for debug mode)
//const int pPOWER = 11;      //power switch

//in-game state used to update button LEDs
bool lights_on = false;
bool ladder_on = false;
bool solar_on = false;
bool gears_on = false;
bool brakes_on = false;
bool chutes_on = false;
bool stage_on = false;
bool action1_on = false;
bool action2_on = false;
bool action3_on = false;
bool action4_on = false;

//stage LED state
bool stage_led_on = false;

//input value variables
int throttle_value;

//variables used for fuel guages
int vSF, vLF, vOX, vEL, vMP;

//timing
const int IDLETIMER = 20000;        //if no message received from KSP for more than 20s, go idle (default 2000)
const int CONTROLREFRESH = 10;      //send control packet every 10 ms (default 25)
const int stageLedBlinkTime = 500;  //blink staging LED when armed every 500 ms

//variables used in timing
unsigned long deadtime, deadtimeOld, controlTime, controlTimeOld, stageLedTime, stageLedTimeOld;
unsigned long now;

//variables used in serial communication
boolean Connected = false;
byte id;

//LCD button
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
const unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

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
  if (digitalRead(pABORT) && digitalRead(pARM))
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
  (digitalRead(pABORT))?l1+="A":l1+="a"; //note abort switch is active high
  (digitalRead(pARM))?l1+="A":l1+="a"; //note arm switch is active high
  (!digitalRead(pSAS))?l1+="S":l1+="s";
  (!digitalRead(pRCS))?l1+="R":l1+="r";
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
    clearNum();
    setNum(barVal, 0);
    setNum(19 - barVal, 1);
    setNum(barVal, 2);
    setNum(19 - barVal, 3);
    updateShiftRegister();

    barVal = (barVal + 1) % 19;
  }
}

