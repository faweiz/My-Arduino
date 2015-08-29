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
*
*    5. Voice:   Use DFRobot Chinese speech recognition module to control robot motion
*
*    6. LeapMotion: Use leap motion controller to control iTesla movement.
*
*    Designer: Faweiz
*/

#include "Voice.h" //申明语音识别库文件
#define SUM 7 //定义待识别的单词数量
uint8  nAsrStatus=0; //申明语音模块工作状态
//定义识别单词“拼音”内容的数组
char sRecog[SUM][80] = 
{"qian jin","hou tui","zuo zhuan","you zhuan","zhua qi","fang xia","ting zhi"};
char flag;//定义向上位机发送字符命令的变量
//根据单词数组的元素号，执行相应命令
void finally (unsigned char n)
{
   switch(n) 
{
    case 0:
     flag='a';//小车前进
             Serial.print(flag);   
              Serial.print(" qian jin ");     
    break;
    
    case 1:
     flag='b'; //小车后退
              Serial.print(flag); 
              Serial.print(" hou tui "); 
     break;
     
        case 2:
     flag=='b'; //小车左转
              Serial.print(flag);
              Serial.print(" zuo zhuan "); 
     break;
               
        case 3:
     flag=='d'; //小车右转
              Serial.print(flag);
              Serial.print(" you zhuan ");
             
     break;
     
        case 4:
     flag='f';//机械手抓起 
              Serial.print(flag);
      break;
      
        case 5:
     flag='g';//机械手放下 
              Serial.print(flag);
      break;
               
        case 6:
     flag='s'; //小车停止
              Serial.print(flag);
              Serial.print(" stop ");
      break;
      
      default:         
              flag='e'; 
              Serial.print(flag);
                break;        
}
}
//识别到声音，产生一个触发中断
void ExtInt0Handler ()
{
  Voice.ProcessInt0();//执行中断服务子程序 
}
//初始化
void setup()
{
  Serial.begin(19200);//串行通信启动
  Voice.Initialise(MIC,VoiceRecognitionV1);//初始化语音模块
  
  //Initialise mode MIC or MONO,default is MIC
  //VoiceRecognitionV1 is VoiceRecognitionV1.0 shield
  //VoiceRecognitionV2 is VoiceRecognitionV2.1 module
  
  
  attachInterrupt(0,ExtInt0Handler,LOW);  
}
//主程序
void loop()
{  
        uint8 nAsrRes;
nAsrStatus = LD_ASR_NONE;
while(1)
{
//当语音模块处于各状态，执行相应工作
 switch(nAsrStatus)
{
case LD_ASR_RUNING:
case LD_ASR_ERROR: 
break;
case LD_ASR_NONE:
{
                            nAsrStatus=LD_ASR_RUNING;
                            if (Voice.RunASR(SUM,80,sRecog)==0) 
                            {      
                                  nAsrStatus= LD_ASR_ERROR;
                                       // Serial.println( "ASR_ERROR"); 
                             }
                                //Serial.println( "ASR_RUNING.....");
                              break;
}
//识别到单词是设定单词
 case LD_ASR_FOUNDOK:
{
                                //Serial.print( "ASR_FOUN ONE: ");
                               //提取所识别到的单词是数组的第几个元素
                                nAsrRes =Voice. LD_GetResult();
                                //根据数组元素号，执行相应命令
                                finally(nAsrRes); 
                              nAsrStatus = LD_ASR_NONE;
                               break;
}
 //识别到单词“不是”设定单词
case LD_ASR_FOUNDZERO:
                        {
                          //Serial.println( "ASR_FOUND ZERO");
                          nAsrStatus = LD_ASR_NONE;
                          break;
                        }
default:
                      {
                        nAsrStatus = LD_ASR_NONE;
                        break;
                        }
}// switch
   delay(10);
}// while
}
