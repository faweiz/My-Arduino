/*   This is an multiple function Arduino R/C car Call iTesla
*    which use DFrobot Cherokey 4WD as platform: http://www.dfrobot.com/wiki/index.php/Cherokey_4WD_Mobile_Platform_(SKU:ROB0102)
*    More information available:
*
*    https://github.com/faweiz
*    https://portfolium.com/faweiz
*    https://www.linkedin.com/in/faweiz
*
*    Project Tutital : https://www.hackster.io/faweiz/arduino_yun_light
*
*    1.Avoidance: Use Ultrasonic sensor/ Sharp GP2Y0A02YK0F IR Range Sensor to avidance object.
*
*    2. Infrared: Use infrared controller to control car by using Linksprite Infrared shield;
*
*    3.iPhone:   Use for iphone TouchOSC app to control robot with xbee
*                Download software from here: http://hexler.net/software/touchosc
*
*    4.PC:       Use PC Serial port to control robot by using 
*                机器人助手串口版 V2
*
*    5. Voice:   Use DFRobot Chinese speech recognition module to control robot motion
*
*    6. LeapMotion: Use leap motion controller to control iTesla movement.
*
*    Designer: Faweiz
*/

//----------- Start Processing code ----------------

import oscP5.*;
import netP5.*;
import processing.serial.*;
Serial arduinoPort;
OscP5 oscP5;

float [] fader = new float [3];

void setup() {
  oscP5 = new OscP5(this,8000);
  arduinoPort = new Serial(this, "COM14", 19200);
}

void oscEvent(OscMessage theOscMessage) {

    String addr = theOscMessage.addrPattern();
    
       if(addr.indexOf("/1/fader") !=-1){ 
       String list[] = split(addr,'/');
     int  xfader = int(list[2].charAt(5) - 0x30);
     if(theOscMessage.get(0).floatValue() !=0){
     fader[xfader]  = theOscMessage.get(0).floatValue();
     }   
    }
}

void draw() {
//---------------------------------Motor A
 if(fader[1] > 0.65){
    arduinoPort.write("1");
  }
   if(fader[1] < 0.35){
    arduinoPort.write("3");
  }
  //--------------------------------Motor B
   if(fader[2] > 0.65){
    arduinoPort.write("2");
  }
   if(fader[2] < 0.35){
    arduinoPort.write("4");
  }
  //----------------------------stop commands
     if(fader[1] < 0.65 && fader[1] > 0.35 ){
    arduinoPort.write("s");
  }
       if(fader[2] < 0.65 && fader[2] > 0.35 ){
    arduinoPort.write("s");
  }
  
}

//---------------------- End Processing Code ------------------
