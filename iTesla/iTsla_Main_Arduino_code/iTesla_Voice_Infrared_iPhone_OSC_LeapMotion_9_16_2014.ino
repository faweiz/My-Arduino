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
*    1.Avoidance: Use Ultrasonic sensor/ Sharp GP2Y0A02YK0F IR Range Sensor to avidance object.
*
*    2. Infrared: Use infrared controller to control car by using Linksprite Infrared shield;
*
*    3.iPhone:   Use for iphone TouchOSC app to control robot with xbee
*                Download software from here: http://hexler.net/software/touchosc
*
*    4.PC:       Use PC Serial port to control robot by using 
*                机器人助手串口版 V2
*    5. Voice:   Use DFRobot Chinese speech recognition module to control robot motion
*
*    6. LeapMotion: Use leap motion controller to control iTesla movement.
*
*    Designer: Faweiz
*/
 #include <IRremote.h> /* include IRremote library */
 #include <Servo.h>

 #define E1 5 //Left Motor Speed @ pin 5 for DfRobot Cherokey 4WD

 #define E2 6 //Right Motor Speed @ pin 6 for DfRobot Cherokey 4WD

 #define M1 4 //M1=M3=4 Left Motor (from point view of head)

 #define M2 7 //M2=M4=7 Right Motor (from point view of head)

/*Start infrared (红外遥控) Define
 *You could find out Infrared Codes from Serial Monitor
 *红外遥控逻辑代码  自己通过串口监视器可以找出
*/
 #define UP    0xC26BF044    //My remote UP KEY   我的遥控器UP键
 #define DOWN  0xC4FFB646    //My remote DOWN KEY 我的遥控器DOWN键
 #define LEFT  0x758C9D82    //My remote LEFT KEY 我的遥控器LEFT键
 #define RIGHT 0x53801EE8    //My remote RIGHT KEY我的遥控器RIGHT键
 #define STOP  0x8AF13528    //My remote STOP KEY 我的遥控器STOP键
 
/* Diffrent infrared remote controller has diffrent infrared code,
*find out you own infrared code before use infrared control function.
*不同的红外线遥控对应的代码也不一样。
*想使用红外线遥控功能请先解码自己的红外遥控。*/



 int RECV_PIN = 11;    //Linksprite Infrared shield @ pin11 红外接收信号引脚
 IRrecv irrecv(RECV_PIN);//红外pin口
 decode_results results;//定义结构体类型
/*End infrared (红外遥控) Define */

 int val;   //declare val as an "variable" 存储从机接受主机发送的单字节命令的变量
 Servo myservo;         // set a variable to map the servo
 
 int currentRead; //Use for LeapMotion from proceesing port.

/* 初始化 setup the pins, servo and serial port */
 void setup()
{
  // initialize the serial port:
   Serial.begin(19200);//设置串行通信的波特率，启动串行通信   Serial
   Serial1.begin(19200);// 设置串行通信的波特率，启动串行通信 Serial1 (*Use For Leonardo only for xbee communication*)
   myservo.attach(9);  // attaches the servo on pin 9   (Note: In this program I don't use Servo, just define it to let my Ultrasonic face forward,that it :)
   myservo.write(90); //  tells the servo to position at 90-degrees ie. facing forward.
   //红外 Infrared SetUp
   irrecv.enableIRIn(); /* Receiveing Infrared code when remote controller is pressed...
                         * 开始接收 红外
                        */
}
/*
*   http://www.faweiz.com/car
*/
/* moveStop */
//-------------------------------------------------------------------------------------------------------------------------------------
 void moveStop(void) {
   digitalWrite(E1,LOW);
   digitalWrite(E2,LOW);
  }// Stop the motors.
  
/* moveForward */
//-------------------------------------------------------------------------------------------------------------------------------------
 void moveForward() {
   analogWrite (E1,255);      //PWM Speed Control
   digitalWrite(M1,HIGH);
   analogWrite (E2,255);
   digitalWrite(M2,HIGH);
  }// Forward
  
/* moveBackward */
//-------------------------------------------------------------------------------------------------------------------------------------
 void moveBackward() {
   analogWrite (E1,255);
   digitalWrite(M1,LOW);
   analogWrite (E2,255);
   digitalWrite(M2,LOW);
  }// Backward
  
/* turnRight */  
//-------------------------------------------------------------------------------------------------------------------------------------
 void turnRight() {
   analogWrite (E1,255);
   digitalWrite(M1,LOW);
   analogWrite (E2,255);
   digitalWrite(M2,HIGH);
  }// Right
  
/* turnLeft */
//-------------------------------------------------------------------------------------------------------------------------------------
 void turnLeft() {
   analogWrite (E1,255);
   digitalWrite(M1,HIGH);
   analogWrite (E2,255);
   digitalWrite(M2,LOW);
  }// Left
/*
*   http://www.faweiz.com/car
*/
/*Use for iphone TouchOSC app to control robot
* Download from here: http://hexler.net/software/touchosc
*/
//-------------------------------------------------------------------------------------------------------------------------------------
 void moveForwardA() {
   analogWrite (E1,255);      //PWM Speed Control
   digitalWrite(M1,HIGH);
  }

 void moveForwardB() {
    analogWrite (E2,255);
   digitalWrite(M2,HIGH);
  }
 void moveBackwardA() {
   analogWrite (E1,255);
   digitalWrite(M1,LOW);
  }
 void moveBackwardB() {
   analogWrite (E2,255);
   digitalWrite(M2,LOW);
  }
// end TouchOSC App code
//-------------------------------------------------------------------------------------------------------------------------------------
/*
*   http://www.faweiz.com/car
*/
//Start Main Loop 主程序
 void loop()
{
  /* 1. Start Serial1 for Xbee communication using by Voice recognition  */
   if(Serial1.available()>0) //查询串口有无数据 Check to see if there is any data avaiable in Serial1
   {
        val=Serial1.read();//读取主机发送的字符
 // moveForward ---------------------------------------------------------------------------------------------------------------------------------
         if(val=='a')//如果主机发送字符为'a'
        {
           moveForward();  //小车前进 moveForward
           Serial.println("forward");
           Serial1.println("forward");
        }
 // moveBackward -------------------------------------------------------------------------------------------------------------------------------
        if(val=='b')//如果主机发送字符为'b'
        {
           moveBackward();  //小车后退 moveBackward
           Serial.println("back");
           Serial1.println("back");
        }
// turnLeft ------------------------------------------------------------------------------------------------------------------------------------   
        if(val=='c')//如果主机发送字符为'c'
        {
           turnLeft();  //小车左转 turnLeft
           Serial.println("left");
           Serial1.println("left");
        }
// turnRight ----------------------------------------------------------------------------------------------------------------------------------
        if(val=='d')//如果主机发送字符为'd'
        {
           turnRight();  //小车右转 turnRight
           Serial.println("right");
           Serial1.println("right");
        }
// moveStop ------------------------------------------------------------------------------------------------------------------------------------          
        if(val=='s')//如果主机发送字符为's'
        {
          moveStop();  //停止 moveStop (stop forward movement)
          Serial.println("stop");
          Serial1.println("stop");
        }
        if(val=='f')//如果主机发送字符为'f'
        {
           //机械手抓取物体
        }
        if(val=='g')//如果主机发送字符为'g'
        {
            //机械手松开物体
        }
        
/* Start TouchOSC "if" code */
// moveForwardA --------------------------------------------------------------------------------------------------------------------------------
     if(val=='1')//如果主机发送字符为'1'
        {
           moveForwardA(); //小车前进A moveForwardA
           Serial.println("ForwardA");
           Serial1.println("ForwardA");
        }
// moveForwardB --------------------------------------------------------------------------------------------------------------------------------
        if(val=='2')//如果主机发送字符为'2'
        {
           moveForwardB();  //小车前进B moveForwardB
           Serial.println("ForwardB");
           Serial1.println("ForwardB");
        }
// moveBackwardA -------------------------------------------------------------------------------------------------------------------------------
        if(val=='3')//如果主机发送字符为'3'
        {
           moveBackwardA();  //小车后退A moveBackwardA
           Serial.println("BackwardA");
           Serial1.println("BackwardA");
        }
// moveBackwardB -------------------------------------------------------------------------------------------------------------------------------
        if(val=='4')//如果主机发送字符为'4'
        {
           moveBackwardB(); //小车后退B moveBackwardB
           Serial.println("BackwardB");
           Serial1.println("BackwardB");
        }
    //End TouchOSC "if" code
   }/* 1. End Serial1 if for Serial1 Xbee communication */
/*
*   http://www.faweiz.com/car
*/
/* 2. Start Serial else if for PC Serial 串口 */
   else if(Serial.available()>0) //查询串口有无数据 Check to see if there is any data avaiable in Serial
   {
        val=Serial.read();//读取主机发送的字符
 // moveForward ---------------------------------------------------------------------------------------------------------------------------------        
         if(val=='w')//如果主机发送字符为'w'
        {
           moveForward();   //小车前进 moveForward
           Serial.println("forward");
           Serial1.println("forward");
        }
 // moveBackward ---------------------------------------------------------------------------------------------------------------------------------        
        if(val=='s')//如果主机发送字符为's'
        {
           moveBackward();  //小车后退 moveBackward
           Serial.println("back");
           Serial1.println("back");
        }
 // turnLeft ---------------------------------------------------------------------------------------------------------------------------------        
        if(val=='a')//如果主机发送字符为'a'
        {
           turnLeft();  //小车左转 turnLeft
           Serial.println("left");
           Serial1.println("left");
        }
 // turnRight ---------------------------------------------------------------------------------------------------------------------------------        
        if(val=='d')//如果主机发送字符为'd'
        {
           turnRight();  //小车右转 turnRight
           Serial.println("right");
           Serial1.println("right");
        }
 // moveStop ---------------------------------------------------------------------------------------------------------------------------------        
        if(val=='n')//如果主机发送字符为'n'
        {
          moveStop();   // //停止 moveStop (stop forward movement)
          Serial.println("stop");
          Serial1.println("stop");
        }
        
        if(val=='f')//如果主机发送字符为'f'
        {
           //机械手抓取物体
        }
        if(val=='g')//如果主机发送字符为'g'
        {
            //机械手松开物体
        }
   } /* 2. End Serial else if for PC Serial 串口 */
/*
*   http://www.faweiz.com/car
*/
/* 3. Begin irrecv else if for infrared remote 红外遥控 */
   else if (irrecv.decode(&results))
    {
        Serial.println(results.value, HEX);
        switch(results.value)
        {
// moveForward ---------------------------------------------------------------------------------------------------------------------------------        
           case UP://UP 键前进^
           moveForward();
           Serial.println("forward");
           Serial1.println("forward");
           break;
// moveBackward ---------------------------------------------------------------------------------------------------------------------------------        
           case DOWN://DOWN 键后退v
           moveBackward();
           Serial.println("back");
           Serial1.println("back");
           break;
// turnLeft ---------------------------------------------------------------------------------------------------------------------------------        
           case LEFT://LEFT 键左转<-
           turnLeft();
           Serial.println("left");
           Serial1.println("left");
           break;
// turnRight ---------------------------------------------------------------------------------------------------------------------------------        
           case RIGHT://RIGHT 键右转->
           turnRight();
           Serial.println("right");
           Serial1.println("right");
           break;
// moveStop ---------------------------------------------------------------------------------------------------------------------------------        
           case STOP://STOP 键前进
           moveStop();   // stop forward movement
          Serial.println("stop");
          Serial1.println("stop");
          break;
        } //End switch
        irrecv.resume(); // Receive the next value
   } /* 3. End irrecv else if for infrared remote 红外遥控 */
   
/* 4. Start leap motion with processing else if for Serial 手势控制 */   
else if (Serial.available())
{
  currentRead = Serial.read();
// moveForward ---------------------------------------------------------------------------------------------------------------------------------        

  if(currentRead>0 & currentRead<166)
  {
           moveForward();   //小车前进 moveForward
           Serial.println("forward");
           Serial1.println("forward");
  }
// moveBackward ---------------------------------------------------------------------------------------------------------------------------------        
  else if(currentRead>332 & currentRead<500)
  {
           moveBackward();  //小车后退 moveBackward
           Serial.println("back");
           Serial1.println("back");
  }
// turnLeft ---------------------------------------------------------------------------------------------------------------------------------        
  else if(currentRead>0 & currentRead<266)
  {
           turnLeft();  //小车左转 turnLeft
           Serial.println("left");
           Serial1.println("left");
  }
// turnRight ---------------------------------------------------------------------------------------------------------------------------------        
  else if(currentRead>533 & currentRead<800)
  {
           turnRight();  //小车右转 turnRight
           Serial.println("right");
           Serial1.println("right");
  }
// moveStop ---------------------------------------------------------------------------------------------------------------------------------        
  else if(currentRead>266 & currentRead<533 || currentRead>166 & currentRead<332)
  {
          moveStop();   // //停止 moveStop (stop forward movement)
          Serial.println("stop");
          Serial1.println("stop");
  }
}/* 4. End leap motion with processing else if for Serial 手势控制 */  



/* 5. Start leap motion with processing else if for Serial1 Xbee communication 手势控制 */  
else if (Serial1.available())
{
  currentRead = Serial.read();
// moveForward ---------------------------------------------------------------------------------------------------------------------------------        

  if(currentRead>0 && currentRead<166)
  {
           moveForward();   //小车前进 moveForward
           Serial.println("forward");
           Serial1.println("forward");
  }
// moveBackward ---------------------------------------------------------------------------------------------------------------------------------        
  else if(currentRead>332 && currentRead<500)
  {
           moveBackward();  //小车后退 moveBackward
           Serial.println("back");
           Serial1.println("back");
  }
// turnLeft ---------------------------------------------------------------------------------------------------------------------------------        
  else if(currentRead>0 && currentRead<266)
  {
           turnLeft();  //小车左转 turnLeft
           Serial.println("left");
           Serial1.println("left");
  }
// turnRight ---------------------------------------------------------------------------------------------------------------------------------        
  else if(currentRead>533 && currentRead<800)
  {
           turnRight();  //小车右转 turnRight
           Serial.println("right");
           Serial1.println("right");
  }
// moveStop ---------------------------------------------------------------------------------------------------------------------------------        
  else if(currentRead>266 && currentRead<533 || currentRead>166 & currentRead<332)
  {
          moveStop();   // //停止 moveStop (stop forward movement)
          Serial.println("stop");
          Serial1.println("stop");
  }
}/* 5. End leap motion with processing else if for Serial1 Xbee communication 手势控制 */  
   
   
/*
*   http://www.faweiz.com/car
*/
}//end loop
