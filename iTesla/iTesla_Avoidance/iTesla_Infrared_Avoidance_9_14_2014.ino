/*   This is an multiple function Arduino R/C car Call iTesla
*    which use DFrobot Cherokey 4WD as platform: http://www.dfrobot.com/wiki/index.php/Cherokey_4WD_Mobile_Platform_(SKU:ROB0102)
*    More information available:
*
*    https://github.com/faweiz
*    https://portfolium.com/faweiz
*    https://www.linkedin.com/in/faweiz
*
*    Project Tutital : https://www.hackster.io/faweiz/arduino-smart-car
*
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
*
*    5. Voice:   Use DFRobot Chinese speech recognition module to control robot motion
*
*    6. LeapMotion: Use leap motion controller to control iTesla movement.
*/    

#include <Servo.h>

#define E1 5 //Left Motor Speed

#define E2 6 //Right Motor Speed

#define M1 4 //M1=M3=4 Left Motor (from point view of head)

#define M2 7 //M2=M4=7 Right Motor (from point view of head)

Servo myservo;         // set a variable to map the servo

int IRpin = A3;                // analog pin for reading the IR sensor

int leftDistance, rightDistance;//distances on either side
int curDist=0;

/* setup the pins, servo and serial port */
void setup() { 
  myservo.attach(9);  // attaches the servo on pin 9
  myservo.write(90); // tells the servo to position at 90-degrees ie. facing forward.
  // initialize the serial port:
  Serial.begin(9600);
} 

//calculate the distance
int readInfrared(){ 
float volts = analogRead(IRpin)*0.0048828125;          // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3

float distance = 65*pow(volts,-1.10);                  // worked out from graph 65 = theretical distance / (1/Volts)

return distance;
}

void loop() {
  myservo.write(90);  // move eyes forward
  delay(1000);

curDist = readInfrared();  //read distance

  Serial.print("Current Distance: ");
  Serial.print(curDist);
  Serial.println("cm");

if (curDist <50 & curDist > 10)
{
  
  moveStop();
  delay(500);
  Serial.println("stop"); //this "stop" is use for check the left and right side
  
  myservo.write(0);  //check distance to the right @ 0 degree
  delay(500);
  rightDistance = readInfrared(); //set right distance
  delay(300);
  
  Serial.print("right Distance: ");
  Serial.print(rightDistance);
  Serial.println("cm");

  myservo.write(90); //return to center
  delay(300);
  
  myservo.write(180);  //check distance to the left @ 180 degree
  delay(500);
  leftDistance = readInfrared();  //set left dstance
  delay(300);
  
  Serial.print("left Distance: ");
  Serial.print(leftDistance);
  Serial.println("cm");
  
  myservo.write(90); //return to center
  delay(300);
  
  
  if (leftDistance>rightDistance)
{
  moveBackward();
  delay(500);
  turnLeft();
  delay(3000);
  Serial.println("Turn left");
}

else if (rightDistance>leftDistance)
{
  moveBackward();
  delay(500);
  turnRight();
  delay(3000);
  Serial.println("Turn Right");
}

else if (rightDistance = leftDistance)
{
  moveBackward();//backward
  delay(1000);
  Serial.println("back");
}

}//end if


else 
{
  moveForward();
  delay(1500);
  moveStop();   //stop forward movement
  
  Serial.println("Go Forward");
}

}//end loop


//-------------------------------------------------------------------------------------------------------------------------------------
void moveStop(void) {

  digitalWrite(E1,LOW);  
  digitalWrite(E2,LOW); 

}  // stop the motors
//-------------------------------------------------------------------------------------------------------------------------------------
void moveForward() {   
  analogWrite (E1,255);      //PWM Speed Control
  digitalWrite(M1,HIGH);   
  analogWrite (E2,255);      //PWM Speed Control
  digitalWrite(M2,HIGH);
  }

//-------------------------------------------------------------------------------------------------------------------------------------
void moveBackward() { 
  analogWrite (E1,255);       //PWM Speed Control
  digitalWrite(M1,LOW);  
  analogWrite (E2,255);       //PWM Speed Control
  digitalWrite(M2,LOW);
  }
//-------------------------------------------------------------------------------------------------------------------------------------
void turnRight() {
  analogWrite (E1,255);
  digitalWrite(M1,LOW);   
  analogWrite (E2,255);   
  digitalWrite(M2,HIGH);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void turnLeft() {
  analogWrite (E1,255);
  digitalWrite(M1,HIGH);   
  analogWrite (E2,255);   
  digitalWrite(M2,LOW);  
}
