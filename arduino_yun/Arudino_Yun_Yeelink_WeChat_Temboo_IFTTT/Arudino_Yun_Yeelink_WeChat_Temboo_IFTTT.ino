/*
 Control light or any other applicace from Yeelink, Wechat,Temboo/ThingSpeak of IFTTT by using arduino yun
 
 This sketch connects an 415 MHz to control light using the Yeelink (iOS,Andriod app and web), wechat, ifttt(twitter)
 // https://github.com/faweiz
 // https://portfolium.com/faweiz
 // https://www.linkedin.com/in/faweiz
 
 Version change
 V1.0 created 11, May, 2015 to support Yeelink
 V1.1 update 12 May, 2015 to support wechat(weline)
 V1.2 update 13 May 2015 to support Temboo with twitter to read a tweet trigger by IFTTT when we push a DO button. 
 V1.3 update 14 May 2015 to support ThingSpeak's App "Tweet Control" to read a tweet trigger by IFTTT when we push a DO button. 
 V.14 update 3 June 2015 to support 443 MHz RF module
 
 Designer: Fawei Zhang
   
 */
 
// include all Libraries needed:
#include <Process.h>
#include <Bridge.h>
#include <Temboo.h>
#include <RCSwitch.h>
#include "password.h" // Contains Yeelink/Weline/Temboo/Thingspeak account info

// Define the 433 MHz RF Remote conttoller demical code
#define CH1_ON  4478259
#define CH1_OFF 4478268
#define CH2_ON  4478403
#define CH2_OFF 4478412
#define CH3_ON  4478723
#define CH3_OFF 4478732

RCSwitch mySwitch = RCSwitch();

// set up net client info:
const unsigned long postingInterval = 1000;  //delay between updates to emoncms.com
unsigned long lastRequest = 0;      // when you last made a request
String dataStringTemp = "";;

void setup() {
  // start serial port:
  pinMode(13, OUTPUT);
  Bridge.begin();
  Serial.begin(9600);

  while(!Serial);    // wait for Network Serial to open
  Serial.println("Connecting....");
  // Do a first function update immediately
  updateData();
  sendData_yeelink_weline_temboo();     // get data from yeelink (ios,android,web, wechat,temboo/IFTTT)
  lastRequest = millis();
  
// 433 MHz RF module:
  // Transmitter is connected to Arduino Pin #7  
  mySwitch.enableTransmit(7);
  // Optional set pulse length.
  mySwitch.setPulseLength(176);
}

void loop() {
  // get a timestamp so you can calculate reading and sending intervals:
  long now = millis();
  
  // if the sending interval has passed since your
  // last connection, then connect again and send data:
  if (now - lastRequest >= postingInterval) {
    updateData();
    sendData_yeelink_weline_temboo(); // Main function to get the data
    lastRequest = now;
  }
}

void updateData() {
  // convert the readings to a String to send it:
  dataStringTemp = "{\"value\":";
  dataStringTemp += random(10) + 20;
  dataStringTemp += "}";
}  

/*************************** Get data from yeelink (ios,android,web and wechat) ***************************/ 
void sendData_yeelink_weline_temboo()
{
/*************************** Get data from yeelink (ios,android,web) ***************************/ 
   // form the string for the API header parameter:
  String apiString = "U-ApiKey: ";
  apiString += APIKEY;
  // form the string for the URL parameter:
  String yeelink_url = "http://api.yeelink.net/v1.0/";
  yeelink_url += "device/";
  yeelink_url += DEVICEID;
  yeelink_url += "/sensor/";
  yeelink_url += SENSORID;
  yeelink_url += "/datapoints";
  // Send the HTTP GET request

  // It's better to declare the Process here, so when the
  // sendData function finishes the resources are immediately
  // released. Declaring it global works too, BTW.
  Process yeelink;
  Serial.print("\nSending Yeelink data... ");
  yeelink.begin("curl");
  yeelink.addParameter("--request");
  yeelink.addParameter("GET");
  yeelink.addParameter("--data-binary");
  yeelink.addParameter(dataStringTemp);
  yeelink.addParameter("--header");
  yeelink.addParameter(apiString);
  yeelink.addParameter(yeelink_url);
  yeelink.run();
  Serial.println("Done! Yeelink");
  Serial.println(yeelink_url);

/******************************* Get data from weline (wechat) ********************************/ 
   // form the string for the API header parameter:
  String openID = "Openid: ";
  openID += openAPI;
  
  // form the string for the URL parameter:
  String weline_url = "http://api.weline.top/";
  weline_url += "index.php/switch/getstatus/id/";
  weline_url += welineDEVICEID;
  // Send the HTTP PUT request
  
  // Is better to declare the Process here, so when the
  // sendData function finishes the resources are immediately
  // released. Declaring it global works too, BTW.
  Process weline;
  Serial.print("\nSending wechat data... ");
  weline.begin("curl");
  weline.addParameter("--request");
  weline.addParameter("GET");
  weline.addParameter("--data-binary");
  weline.addParameter(dataStringTemp);
  weline.addParameter("--header");
  weline.addParameter(openID);
  weline.addParameter(weline_url);
  weline.run();
  Serial.println("Done! WeChat");
  Serial.println(weline_url);
 
 /******************************* Get data from Temboo(Twitter) and IFTTT ********************************/ 
 /* 
 // limit the number of call for using temboo
   int numRuns = 1; // Execution count, so this doesn't run forever
   int maxRuns = 5; // Maximum number of times the Choreo should be executed
   // Declare 2 string variable to store the content of tweet posted.   
    String author; // a String to hold the tweet author's name
    String tweet; // a String to hold the text of the tweet  
   if(numRuns <= maxRuns)
 {
 // Learn more from  https://temboo.com/arduino/yun/read-a-tweet
    Serial.print("\nReading Twitter User Timeline... ");
    TembooChoreo UserTimelineChoreo; // Use the temboo twitter's user time line choreo
 // invoke the Temboo client.
 // NOTE that the client must be reinvoked, and repopulated with
 // appropriate arguments, each time its run() method is called.
    UserTimelineChoreo.begin();
 // Set Temboo account credentials
    UserTimelineChoreo.setAccountName(TEMBOO_ACCOUNT);
    UserTimelineChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    UserTimelineChoreo.setAppKey(TEMBOO_APP_KEY);
 */ 
    
// skip this, In order to reduece the memory usage, I using a profile to store 
// my Twitter credemtials on Temboo. This means that I can simply reference them 
// from my sketch without including all the String data that consumes memory
// Learn more : https://temboo.com/videos#profiles
// https://temboo.com/library/Library/Twitter/Timelines/UserTimeline/
/* // Set Choreo inputs
    UserTimelineChoreo.addInput("ScreenName", ScreenName);
    UserTimelineChoreo.addInput("AccessToken", AccessToken);
    UserTimelineChoreo.addInput("AccessTokenSecret", AccessTokenSecret);
    UserTimelineChoreo.addInput("ConsumerSecret", ConsumerSecret);
    UserTimelineChoreo.addInput("ConsumerKey", ConsumerKey);
*/ 
/*
 // Set profile to use for execution
    UserTimelineChoreo.setProfile("twitter");
 // Identify the Choreo to run
    UserTimelineChoreo.setChoreo("/Library/Twitter/Timelines/UserTimeline");
 // we want the text of the tweet
    UserTimelineChoreo.addOutputFilter("tweet", "/[1]/text", "Response");
 // and the name of the author
    UserTimelineChoreo.addOutputFilter("author", "/[1]/user/screen_name", "Response");   
 // Run the Choreo; when results are available, print them to serial
    UserTimelineChoreo.run();  
 // choreo outputs are returned as key/value pairs, delimited with 
 // newlines and record/field terminator characters, for example:
 // Name1\n\x1F
 // Value1\n\x1E
 // Name2\n\x1F
 // Value2\n\x1E  
 // see the examples at http://www.temboo.com/arduino for more detail
 // we can read this format into separate variables, as follows: 
    while(UserTimelineChoreo.available()) 
    {
        // read the name of the output item
        String name = UserTimelineChoreo.readStringUntil('\x1F');
        name.trim();

        // read the value of the output item
        String data = UserTimelineChoreo.readStringUntil('\x1E');
        data.trim();

    // assign the value to the appropriate String
        if (name == "tweet") {
          tweet = data;
        } else if (name == "author") {
          author = data;
        }
     }
     Serial.println("Done! Twitter");
     Serial.println("@" + author + " - " + tweet);
     UserTimelineChoreo.close();
 } 
 */
 // Since the temboo only allow us to use 1000 Choreo call per month for free plan; 
 // therefore, I try to use alternative serivice ThingSpeak's app "Tweet Control"
 // Learn more: https://thingspeak.com/docs/tweetcontrol
 
 /******************************* Get data from ThingSpeak (ifttt/do button/ twitter) ********************************/ 
   // form the string for the API header parameter:
  String thingSpeak_api = " ThingSpeak API: ";
  thingSpeak_api += thingSpeakAPI;
  
  // form the string for the URL parameter:
  String thingSpeak_url = "http://api.thingspeak.com/";
  thingSpeak_url += "channels/";
  thingSpeak_url += CHANEELID;
  thingSpeak_url += "/feed/last?key="; // "Get Last Entry in a Channel Feed" https://thingspeak.com/docs/channels#get_feed
  thingSpeak_url += thingSpeakAPI;
  // Send the HTTP PUT request
    
  // Is better to declare the Process here, so when the
  // sendData function finishes the resources are immediately
  // released. Declaring it global works too, BTW.
  Process thingSpeak;
  Serial.print("\nSending thingSpeak data... ");
  thingSpeak.begin("curl");
  thingSpeak.addParameter("--request");
  thingSpeak.addParameter("GET");
  thingSpeak.addParameter("--data-binary");
  thingSpeak.addParameter(dataStringTemp);
  thingSpeak.addParameter("--header");
  thingSpeak.addParameter(thingSpeak_api);
  thingSpeak.addParameter(thingSpeak_url);
  thingSpeak.run();
  Serial.println("Done! thingSpeak");
  Serial.println(thingSpeak_url);
  
 // int T = thingSpeak.available();
 // Serial.print(T);
 
/******************************* Printing data   ********************************/ 
  char yee,we,thsp;
  String yeelinkState = "";
  String welineState = "";
  String thingSpeak_Twitter_State ="";
  
  // If there's incoming data from the net connection,
  // send it out the Serial:
  /////////////////////////// Print the yeelink data  /////////////////////////// 
   while (yeelink.available()>0) {
    yee = yeelink.read();
    yeelinkState += yee;
  }
  Serial.println();
  Serial.print("yeelinkState: "); Serial.print(yeelinkState);
  Serial.println();
  char y = yeelinkState.charAt(yeelinkState.length() - 2);
  Serial.print("yeelinkState value: ");Serial.print(y);
  Serial.println();
  
  /////////////////////////// Print the weline data  /////////////////////////// 
   while (weline.available()>0) {
    we = weline.read();
    welineState += we;
  }
  Serial.print("welineState: "); Serial.print(welineState);
  Serial.println();
  char w = welineState.charAt(welineState.length()- 3);
  Serial.print("welineState value: "); Serial.print(w);
  Serial.println();
/* 
  /////////////////////////// Print the twitter data from temboo  /////////////////////////// 
  Serial.print("Twitter Value: @" + author + " - " + tweet);
  Serial.println();
  char t = tweet.charAt(tweet.length() - 1);
  Serial.print("Twitter value: "); Serial.print(t);
  Serial.println();
*/ 

  /////////////////////////// Print the thingSpeak data  /////////////////////////// 
  while (thingSpeak.available()) {
    thsp = thingSpeak.read();
    thingSpeak_Twitter_State += thsp;
  }
  Serial.print("thingSpeak_Twitter State: "); Serial.print(thingSpeak_Twitter_State);
  Serial.println();
  char t = thingSpeak_Twitter_State.charAt(thingSpeak_Twitter_State.length()- 3);
  Serial.print("thingSpeak_Twitter State value: "); Serial.print(t);
  Serial.println();
  
/******************************* Determine the status of the data  (yeelink and wechat only)  ********************************/
/*
   if(y == '0'&& w == '0')
   {  
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,LOW); 
       Serial.print(" LED OFF");
       mySwitch.send(CH1_OFF, 24);
   }
  if(y == '0'&& w == '1')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
       mySwitch.send(CH1_ON, 24);
   }
   if(y == '1'&& w == '0')
   {  
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
       mySwitch.send(CH1_ON, 24);
   }
   
   // The controller can't be control the light from both of 2 platform.....
   // You could control, but it will doing the backward. 
   // Such as, when y = 1; the w = 0 the light up.and when y = 1, w = 1, the light down etc...
   if(y == '1'&& w == '1')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,LOW); 
       Serial.print(" LED OFF");
       mySwitch.send(CH1_OFF, 24);
   }
 */  
/********************** Determine the status of the data (yeelink,wechat,(Temboo or Thingspeak),twitter,ifttt) **********************/

   if(y == '0'&& w == '0' && t == '0' )
   {  
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);Serial.print(" t = "); Serial.print(t);
       Serial.println();
       digitalWrite(13,LOW); 
       Serial.print(" LED OFF");
       mySwitch.send(CH1_OFF, 24);
   }
  if(y == '0'&& w == '0' && t =='1')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);Serial.print(" t = "); Serial.print(t);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
       mySwitch.send(CH1_ON, 24);
   }
   if(y == '0'&& w == '1' && t =='0')
   {  
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);Serial.print(" t = "); Serial.print(t);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
       mySwitch.send(CH1_ON, 24);
   }
   
   // The controller can't be control the light from both of 2 platform.....
   // You could control, but it will doing the backward. 
   // Such as, when y = 1; the w = 0 the light up.and when y = 1, w = 1, the light down etc...
   if(y == '0'&& w == '1' && t =='1')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);Serial.print(" t = "); Serial.print(t);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
       mySwitch.send(CH1_ON, 24);
   }
   if(y == '1'&& w == '0' && t =='0')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);Serial.print(" t = "); Serial.print(t);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
       mySwitch.send(CH1_ON, 24);
   }
   if(y == '1'&& w == '0' && t =='1')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);Serial.print(" t = "); Serial.print(t);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
       mySwitch.send(CH1_ON, 24);
   }
   if(y == '1'&& w == '1' && t =='0')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);Serial.print(" t = "); Serial.print(t);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
       mySwitch.send(CH1_ON, 24);
   }
   if(y == '1'&& w == '1' && t =='1')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);Serial.print(" t = "); Serial.print(t);
       Serial.println();
       digitalWrite(13,LOW); 
       Serial.print(" LED OFF");
       mySwitch.send(CH1_OFF, 24);
   }


   // Ensure the last bit of data is sent.
   Serial.flush(); 
}

