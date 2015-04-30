/*   This is an multiple function Arduino R/C car Call iTesla
*    which use DFrobot Cherokey 4WD as platform: http://www.dfrobot.com/wiki/index.php/Cherokey_4WD_Mobile_Platform_(SKU:ROB0102)
*    More information available:
*
*    http://www.faweiz.com/car
*    http://www.faweiz.com/小车
*    http://www.faweiz.com/itesla
*
*    1.Avoidance: Use Ultrasonic sensor/ Sharp GP2Y0A02YK0F IR Range Sensor to avidance object.
*
*    2. Infrared: Use infrared controller to control car by using Linksprite Infrared shield;
*
*    3.iPhone:   Use for iphone TouchOSC app to control robot with xbee
*                Download software from here: http://hexler.net/software/touchosc
*
*    4.PC:       Use PC Serial port to control robot by using 
*                机器人助手串口版 V2 avaibale for download from here:
*                http://faweiz.com/download/%E6%9C%BA%E5%99%A8%E4%BA%BA%E5%8A%A9%E6%89%8B%E4%B8%B2%E5%8F%A3%E7%89%88.zip
*
*    5. Voice:   Use DFRobot Chinese speech recognition module to control robot motion
*
*    6. LeapMotion: Use leap motion controller to control iTesla movement.
*
*    Designer: Faweiz
*/       
// LEAP libraries
import de.voidplus.leapmotion.*;
LeapMotion leap;

// Serial Port Output
import processing.serial.*; // Serial libraries, send OUTPUT through USB
Serial myPort;

int stop;
int checkStop;

int goback;
int leftright;

int checkGoBack;
int checkLeftRight;


void setup() {
size(800, 500, P3D);
background(255);
noStroke();
fill(50);


// New serial port object
myPort = new Serial(this, "COM14", 19200);


// New leap object
leap = new LeapMotion(this);
}


void draw() {
background(255);

//UP   Orange Color 
  rect(533,166,266,166);
  fill(255, 128, 0);
//DOWN  Green Color
  rect(266,0,266,166);
  fill(0,250,0);
//LEFT  Blue Color
  rect(266,332,266,166);
  fill(0, 0, 255);
//RIGHT  Red Color
  rect(0,166,266,166);
  fill(255, 0, 0);


// Leap magic
int fps = leap.getFrameRate();


// Clean LEAP Hand position
for (Hand hand : leap.getHands()) {

hand.draw();
PVector hand_position = hand.getPosition();

print("X: " + hand.getPosition().x);
print(" y: " + hand.getPosition().y);
print(" z: " + hand.getPosition().z);
println();



// Determine movement(go/back,left/right,stop) according to mapped hand x/y/z position

//Forward/Backward
  checkGoBack = (int) map(hand.getPosition().y,0,500,0,500);
//Left/Right
  checkLeftRight = (int) map(hand.getPosition().x,0,800,0,800);
//Stop movement
  checkStop = (int) map(hand.getPosition().z,-50,100,-50,100);

//Forward/Backward
  if(goback != checkGoBack)
  {
   assignGoback(); 
  }
//Forward/Backward
  else if(leftright !=checkLeftRight)
  {
   assignLeftRight();
  }
//Stop
  else if(stop !=checkStop)
  {
    assignStop();
  }

 }//End for loop

}//End draw()


void assignGoback(){
  goback = checkGoBack;
  myPort.write(goback);
  println("GoBack: "+goback);
}
void assignLeftRight(){
  leftright = checkLeftRight;
  myPort.write(leftright);
  println("LeftRight: "+leftright);
}


//Stop
  void assignStop(){
    stop = checkStop;
// Send hand position value through port
    myPort.write(stop);
    println("Stop"+stop);
  }


void leapOnInit() {
 //println(“Leap Motion Init”);
}
void leapOnConnect() {
// println(“Leap Motion Connect”);
}
void leapOnFrame() {
// println(“Leap Motion Frame”);
}
void leapOnDisconnect() {
// println(“Leap Motion Disconnect”);
}
void leapOnExit() {
// println(“Leap Motion Exit”);
}



