//Define what controls to send to KSP

//check if it is time to send a control packet
void send_control_packet() 
{
  now = millis();
  controlTime = now - controlTimeOld;
  if (controlTime > CONTROLREFRESH) 
  {
    controlTimeOld = now;
    define_control_packet();
  }
}

//define the structure of a control packet
struct ControlPacket 
{
  //the following controls can be sent to the KSPSerialIO plugin (defined by the plugin)
  byte id;
  byte MainControls;                  //SAS RCS Lights Gears Brakes Precision Abort Stage (see enum)
  byte Mode;                          //0 = stage, 1 = docking, 2 = map
  unsigned int ControlGroup;          //action groups 1-10
  byte NavballSASMode;                //AutoPilot mode
  byte AdditionalControlByte1;
  int Pitch;                          //-1000 -> 1000
  int Roll;                           //-1000 -> 1000
  int Yaw;                            //-1000 -> 1000
  int TX;                             //-1000 -> 1000
  int TY;                             //-1000 -> 1000
  int TZ;                             //-1000 -> 1000
  int WheelSteer;                     //-1000 -> 1000
  int Throttle;                       //    0 -> 1000
  int WheelThrottle;                  //    0 -> 1000
};

//Create an instance of a control packet
ControlPacket CPacket;

//macro used to generate the control packet (also used for the handshake packet)
#define details(name) (uint8_t*)&name,sizeof(name)

//Enumeration of MainControls
#define SAS       7
#define RCS       6
#define LIGHTS    5
#define GEARS     4
#define BRAKES    3
#define PRECISION 2
#define ABORT     1
#define STAGE     0

//Main controls uses enum above, e.g. MainControls(SAS,true);
void MainControls(byte n, boolean s) 
{
  (s)?CPacket.MainControls |= (1 << n):CPacket.MainControls &= ~(1 << n);
  // forces nth bit of x to be 1.  all other bits left alone.
  // forces nth bit of x to be 0.  all other bits left alone.
}

//Control groups (action groups) uses an integer to refer to a custom action group, e.g. ControlGroup(5,true);
void ControlGroups(byte n, boolean s) 
{
  (s)?CPacket.ControlGroup |= (1 << n):CPacket.ControlGroup &= ~(1 << n);
  // forces nth bit of x to be 1.  all other bits left alone.
  // forces nth bit of x to be 0.  all other bits left alone.
}

//Enumeration of SAS Modes
#define SMOFF           0
#define SMSAS           1
#define SMPrograde      2
#define SMRetroGrade    3
#define SMNormal        4
#define SMAntinormal    5
#define SMRadialIn      6
#define SMRadialOut     7
#define SMTarget        8
#define SMAntiTarget    9
#define SMManeuverNode  10

//SAS mode uses enum above, e.g. setSASMode(SMPrograde);
void setSASMode(byte m) 
{
  CPacket.NavballSASMode &= B11110000;
  CPacket.NavballSASMode += m;
}

//Enumeration of Navball Target Modes
#define NAVBallIGNORE   0
#define NAVBallORBIT    1
#define NAVBallSURFACE  2
#define NAVBallTARGET   3

//Navball mode uses enum above, e.g. setNavBallMode(NAVBallSURFACE);
void setNavballMode(byte m) 
{
  CPacket.NavballSASMode &= B00001111;
  CPacket.NavballSASMode += m << 4;
}

void define_control_packet() 
{
  if (Connected) 
  {
    //here we define what controls to send when which pins are manipulated
    
    //toggle switches
    (!digitalRead(pSAS))?MainControls(SAS, true):MainControls(SAS, false);
    (!digitalRead(pRCS))?MainControls(RCS, true):MainControls(RCS, false);
    (digitalRead(pABORT))?MainControls(ABORT, true):MainControls(ABORT, false);
  
    //momentary stage button
    (!digitalRead(pSTAGE) && digitalRead(pARM))?MainControls(STAGE, true):MainControls(STAGE, false);
  
    if(digitalRead(pARM))
    {
      now = millis();
      stageLedTime = now - stageLedTimeOld;
      if (stageLedTime > stageLedBlinkTime) 
      {
        stageLedTimeOld = now;
        stage_led_on = !stage_led_on;
      }
    }
    else
    {
      stage_led_on = false;
    }
    digitalWrite(pSTAGELED, stage_led_on);
    
    //toggle buttons
    (digitalRead(pLIGHTS))?MainControls(LIGHTS, true):MainControls(LIGHTS, false);
    (digitalRead(pGEARS))?MainControls(GEARS, true):MainControls(GEARS, false);
    (digitalRead(pBRAKES))?MainControls(BRAKES, true):MainControls(BRAKES, false);
    (digitalRead(pACTION1))?ControlGroups(1, true):ControlGroups(1, false);
    (digitalRead(pACTION2))?ControlGroups(2, true):ControlGroups(2, false);
    (digitalRead(pACTION3))?ControlGroups(3, true):ControlGroups(3, false);
    (digitalRead(pACTION4))?ControlGroups(4, true):ControlGroups(4, false);
    (digitalRead(pLADDER))?ControlGroups(5, true):ControlGroups(5, false);
    (digitalRead(pSOLAR))?ControlGroups(6, true):ControlGroups(6, false);
    (digitalRead(pCHUTES))?ControlGroups(7, true):ControlGroups(7, false);

    //throttle
    CPacket.Throttle = constrain(map(analogRead(pTHROTTLE),30,990,0,1023),0,1000);

    //send the control packet to the KSPSerialIO plugin
    KSPBoardSendData(details(CPacket)); 
  }
}

