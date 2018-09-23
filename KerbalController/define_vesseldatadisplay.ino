//grab info from KSP here (VData object) and write out results to the Arduino pins

//connect to the KSPSerialIO plugin and receive data
int get_vessel_data() 
{
  int returnValue = -1;
  now = millis();

  if (KSPBoardReceiveData())
  {
    //data is being received
    deadtimeOld = now;
    returnValue = id;
    switch(id) 
    {
    case 0: //First packet is a handshake packet
    {
      Handshake();
      clearLCD();
      writeLCD("KerbalController");
      writeLCD("handshake...");
      break;
    }
    case 1:
    {
      //subsequent packets are data from the plugin
      define_vessel_data_display(); //define what to do with the data below
      break;
    }
    }
    Connected = true;
  }
  else
  { //if no message received for a while, go idle
    deadtime = now - deadtimeOld; 
    if (deadtime > IDLETIMER)
    {
      deadtimeOld = now;
      Connected = false;
      clearLCD();
      writeLCD("KerbalController");
      writeLCD("idle...");
    }    
  }
  return returnValue;
}

//define the structure of a VesseData packet
struct VesselData
{
    //the following data is available in the packet received from the plugin (defined by the KSPSerialIO plugin)
    byte id;                //1   packet id
    float AP;               //2   apoapsis (m to sea level)
    float PE;               //3   periapsis (m to sea level)
    float SemiMajorAxis;    //4   orbit semi major axis (m)
    float SemiMinorAxis;    //5   orbit semi minor axis (m)
    float VVI;              //6   vertical velocity (m/s)
    float e;                //7   orbital eccentricity (unitless, 0 = circular, > 1 = escape)
    float inc;              //8   orbital inclination (degrees)
    float G;                //9   acceleration (Gees)
    long TAp;               //10  time to AP (seconds)
    long TPe;               //11  time to Pe (seconds)
    float TrueAnomaly;      //12  orbital true anomaly (degrees)
    float Density;          //13  air density (presumably kg/m^3, 1.225 at sea level)
    long period;            //14  orbital period (seconds)
    float RAlt;             //15  radar altitude (m)
    float Alt;              //16  altitude above sea level (m)
    float Vsurf;            //17  surface velocity (m/s)
    float Lat;              //18  Latitude (degree)
    float Lon;              //19  Longitude (degree)
    float LiquidFuelTot;    //20  Liquid Fuel Total
    float LiquidFuel;       //21  Liquid Fuel remaining
    float OxidizerTot;      //22  Oxidizer Total
    float Oxidizer;         //23  Oxidizer remaining
    float EChargeTot;       //24  Electric Charge Total
    float ECharge;          //25  Electric Charge remaining
    float MonoPropTot;      //26  Mono Propellant Total
    float MonoProp;         //27  Mono Propellant remaining
    float IntakeAirTot;     //28  Intake Air Total
    float IntakeAir;        //29  Intake Air remaining
    float SolidFuelTot;     //30  Solid Fuel Total
    float SolidFuel;        //31  Solid Fuel remaining
    float XenonGasTot;      //32  Xenon Gas Total
    float XenonGas;         //33  Xenon Gas remaining
    float LiquidFuelTotS;   //34  Liquid Fuel Total (stage)
    float LiquidFuelS;      //35  Liquid Fuel remaining (stage)
    float OxidizerTotS;     //36  Oxidizer Total (stage)
    float OxidizerS;        //37  Oxidizer remaining (stage)
    uint32_t MissionTime;   //38  Time since launch (s)
    float deltaTime;        //39  Time since last packet (s)
    float VOrbit;           //40  Orbital speed (m/s)
    uint32_t MNTime;        //41  Time to next node (s) [0 when no node]
    float MNDeltaV;         //42  Delta V for next node (m/s) [0 when no node]
    float Pitch;            //43  Vessel Pitch relative to surface (degrees)
    float Roll;             //44  Vessel Roll relative to surface (degrees)
    float Heading;          //45  Vessel Heading relative to surface (degrees)
    uint16_t ActionGroups;  //46  status bit order:SAS, RCS, Light, Gears, Brakes, Abort, Custom01 - 10
    byte SOINumber;         //47  SOI Number (decimal format: sun-planet-moon e.g. 130 = kerbin, 131 = mun)
    byte MaxOverHeat;       //48  Max part overheat (% percent)
    float MachNumber;       //49  Mach number
    float IAS;              //50  Indicated Air Speed
    byte CurrentStage;      //51  Current stage number
    byte TotalStage;        //52  TotalNumber of stages
    float TargetDist;       //53  Distance to targeted vessel (m)
    float TargetV;          //54  Target vessel relative velocity
    byte NavballSASMode;    //55  Combined byte for navball target mode and SAS mode
                                // First four bits indicate AutoPilot mode:
                                // 0 SAS is off  //1 = Regular Stability Assist //2 = Prograde
                                // 3 = RetroGrade //4 = Normal //5 = Antinormal //6 = Radial In
                                // 7 = Radial Out //8 = Target //9 = Anti-Target //10 = Maneuver node
                                // Last 4 bits set navball mode. (0=ignore,1=ORBIT,2=SURFACE,3=TARGET)
};

//create an instance of a VesselData object
VesselData VData;

//Enumeration of ActionGroups (includes main controls and custom action groups)
#define AGSAS      0
#define AGRCS      1
#define AGLight    2
#define AGGears    3
#define AGBrakes   4
#define AGAbort    5
#define AGCustom01 6
#define AGCustom02 7
#define AGCustom03 8
#define AGCustom04 9
#define AGCustom05 10
#define AGCustom06 11
#define AGCustom07 12
#define AGCustom08 13
#define AGCustom09 14
#define AGCustom10 15

//get the current state of main controls and custom action groups using enumeration above, e.g. ControlStatus(AGBrakes);
byte ControlStatus(byte n)
{
  return ((VData.ActionGroups >> n) & 1) == 1;
}

//get the current SAS Mode. Can be compared to enum values, e.g. if(getSASMode() == SMPrograde){//do stuff}
byte getSASMode() 
{
  return VData.NavballSASMode & B00001111; // leaves alone the lower 4 bits of; all higher bits set to 0.
}

//get the current navball mode. Can be compared to enum values, e.g. if (getNavballMode() == NAVBallTARGET){//do stuff}
byte getNavballMode() 
{
  return VData.NavballSASMode >> 4; // leaves alone the higher 4 bits of; all lower bits set to 0.
}

//define what to do with the vessel data here, e.g. turn on LED's, display text on the LCD
void define_vessel_data_display() 
{
  //Fuel Gauges
  vSF = 10 - ceil(10 * VData.SolidFuel / VData.SolidFuelTot); //percentage of solid fuel remaining
  vLF = 10 - ceil(10 * VData.LiquidFuelS / VData.LiquidFuelTotS); //percentage of liquid fuel remaining in current stage
  vOX = 10 - ceil(10 * VData.OxidizerS / VData.OxidizerTotS); //percentage of oxidized remaining in current stage
  vEL = 10 - ceil(10 * VData.ECharge / VData.EChargeTot); //percentage of electric charge remaining
  vMP = 10 - ceil(10 * VData.MonoProp / VData.MonoPropTot); //percentage of monopropellant remaining
  vA = 20 - ceil(VData.Density * 16);
  vA = (vA == 0) ? 19: vA;
  vG = 15 - ceil(VData.G);

  char line1[17];
  char line2[17];
  switch (pLCDState)
  {
  case 0:
  {
    String strVsurf = "Vsurf: ";
    strVsurf += String(VData.Vsurf, 0);
    strVsurf += " m/s";
    strVsurf.toCharArray(line1,17);
    //Acceleration (G)
    char bufferGee[17];
    String strGee = "Accel: ";
    strGee += String(VData.G, 0);
    strGee += " G";
    strGee.toCharArray(line2,17);
    break;    
  }
  case 1:
  {
    //Apoapsis
    String strApo = "AP: ";
    if (VData.AP < 10000 && VData.AP > -10000) 
    {
      strApo += String(VData.AP,0);
      strApo += "m ";
    } 
    else if ((VData.AP >= 10000 && VData.AP < 10000000) || (VData.AP <= -10000 && VData.AP > -10000000)) 
    {
      strApo += String((VData.AP / 1000),0);
      strApo += "km ";
    } 
    else if ((VData.AP >= 10000000 && VData.AP < 10000000000) || (VData.AP <= -10000000 && VData.AP > -10000000000)) 
    {
      strApo += String((VData.AP / 1000000),0);
      strApo += "Mm ";
    } 
    else 
    {
      strApo += String((VData.AP / 1000000000),0);
      strApo += "Gm ";
    }
    strApo += String(VData.TAp); //time to apoapsis
    strApo += "s";
    strApo.toCharArray(line1, 17);
    
    //Periapsis
    String strPeri = "PE: ";
    if (VData.PE < 10000 && VData.PE > -10000) 
    {
      strPeri += String(VData.PE,0);
      strPeri += "m ";
    } 
    else if ((VData.PE >= 10000 && VData.PE < 10000000) || (VData.PE <= -10000 && VData.PE > -10000000)) 
    {
      strPeri += String((VData.PE / 1000.0),0);
      strPeri += "km ";
    } 
    else if ((VData.PE >= 10000000 && VData.PE < 10000000000) || (VData.PE <= -10000000 && VData.PE > -10000000000)) 
    {
      strPeri += String((VData.PE / 1000000.0),0);
      strPeri += "Mm ";
    }
    else 
    {
      strPeri += String((VData.PE / 1000000000.0),0);
      strPeri += "Gm ";
    }
    strPeri += String(VData.TPe); //time to periapsis
    strPeri += "s";
    strPeri.toCharArray(line2, 17);
    break;    
  }
  case 2:
  {
    String l1 = "Tnode: ";
    char t[10];
    dtostrf(VData.MNTime, 8, 0, t);
    l1 += String(t);
    l1 += "s";
    l1.toCharArray(line1, 17);
    
    //MNDeltaV
    String strMNDeltaV = "dV: ";
    strMNDeltaV += String(VData.MNDeltaV, 0);
    strMNDeltaV += " m/s";
    strMNDeltaV.toCharArray(line2, 17);
    break;
  }
  case 3:
  {
    String strTargetDist = "TDist: ";
    strTargetDist += String(VData.TargetDist, 0);
    strTargetDist += " m";
    strTargetDist.toCharArray(line1, 17);

    //Target Velocity
    String strTargetV = "TVel: ";
    strTargetV += String(VData.TargetV, 0);
    strTargetV += " m/s";
    strTargetV.toCharArray(line2, 17);
    break;
  }
  case 4:
  {
    String l1 = "Heat: ";
    char t[3];
    dtostrf(VData.MaxOverHeat, 3, 0, t);
    l1 += String(t);
    l1 += "%";
    l1.toCharArray(line1, 17);
    
    //Acceleration (G)
    String strGee = "Decel: ";
    strGee += String(VData.G, 0);
    strGee += " G";
    strGee.toCharArray(line2, 17);
    break;
  }
  case 5:
  {
    String strAlt = "Alt:  ";
    strAlt += String(VData.Alt, 0);
    strAlt += " m/s";
    strAlt.toCharArray(line1, 17);

    //Mach Number
    String strMach = "Mach:";
    strMach += String(VData.G, 0);
    strMach.toCharArray(line2, 17);
    break;
  }
  case 6:
  {
    String strRAlt = "RAlt: ";
    strRAlt += String(VData.RAlt, 0);
    strRAlt += " m/s";
    strRAlt.toCharArray(line1,17);

    //Vertical Velocity
    String strVVI = "VVI:  ";
    strVVI += String(float(vA), 0);
    strVVI += " m/s";
    strVVI.toCharArray(line2,17);
    break;
  }
  case 7:
  default:
  {
    String l1 = "KerbalController";
    l1.toCharArray(line1,17);
    char s[2];
    dtostrf(vSF, 2, 0, s);
    String l2 = String(s);
    l2 += "S ";
    dtostrf(vLF, 2, 0, s);
    l2 += String(s);    
    l2 += "L ";
    dtostrf(vEL, 2, 0, s);
    l2 += String(s);    
    l2 += "E ";
    dtostrf(vMP, 2, 0, s);
    l2 += String(s);    
    l2 += "M ";
    l2.toCharArray(line2,17);
    break;
  }
  }

  clearLCD();
  writeLCD(line1);
  jumpToLineTwo();
  writeLCD(line2);
  
  //get in-game status for updating the LED statuses on the controller  
  rcs_on = ControlStatus(AGRCS);
  sas_on = ControlStatus(AGSAS);
  lights_on = ControlStatus(AGLight);
  gears_on = ControlStatus(AGGears);
  brakes_on = ControlStatus(AGBrakes);
  action1_on = ControlStatus(AGCustom01);
  action2_on = ControlStatus(AGCustom02);
  action3_on = ControlStatus(AGCustom03);
  action4_on = ControlStatus(AGCustom04);
  ladder_on = ControlStatus(AGCustom05);
  solar_on = ControlStatus(AGCustom06);
  chutes_on = ControlStatus(AGCustom07);

  //Fuel Gauges
  barsS.setNums(vSF, vLF, vOX, vEL, vMP);
  barsS.up();

  //G+A
  bars.setNums(vA , vG);
  bars.up();
}
