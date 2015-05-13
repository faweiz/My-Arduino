/*
 
 This sketch connects an 415 MHz to control light using the Yeelink (iOS,Andriod app and web), wechat, ifttt(twitter)
 // https://github.com/faweiz
 // https://portfolium.com/faweiz
 // https://www.linkedin.com/in/faweiz
 V1.0 created 11, May, 2015 to support Yeelink
 V1.1 update 12 May, 2015 to support wechat(weline)
 V1.2 update 13 May 2015 to support Temboo with twitter to read a tweet trigger by IFTTT when we push a DO button. 
 
 by Fawei Zhang
   
 */
 

// include all Libraries needed:
#include <Process.h>
#include <Bridge.h>
#include <Temboo.h>
#include "password.h"

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

  // Is better to declare the Process here, so when the
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
  
//  int i = yeelink.available();
//  Serial.print(i);

  
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
  
 // int j = weline.available();
 // Serial.print(j);
 
 /******************************* Get data from Temboo(Twitter) and IFTTT ********************************/ 
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
 // Set Choreo inputs
    UserTimelineChoreo.addInput("ScreenName", ScreenName);
    UserTimelineChoreo.addInput("AccessToken", AccessToken);
    UserTimelineChoreo.addInput("AccessTokenSecret", AccessTokenSecret);
    UserTimelineChoreo.addInput("ConsumerSecret", ConsumerSecret);
    UserTimelineChoreo.addInput("ConsumerKey", ConsumerKey);
 // Identify the Choreo to run
    UserTimelineChoreo.setChoreo("/Library/Twitter/Timelines/UserTimeline");
 // we want the text of the tweet
    UserTimelineChoreo.addOutputFilter("tweet", "/[1]/text", "Response");
 // and the name of the author
    UserTimelineChoreo.addOutputFilter("author", "/[1]/user/screen_name", "Response");   
 // Run the Choreo; when results are available, print them to serial
    UserTimelineChoreo.run();  
 // Declare 2 string variable to store the content of tweet posted.   
    String author; // a String to hold the tweet author's name
    String tweet; // a String to hold the text of the tweet  
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

/******************************* Printing data   ********************************/ 

  char yee,we;
  String yeelinkState = "";
  String welineState = "";
  
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
 
  /////////////////////////// Print the twitter data  /////////////////////////// 
  Serial.print("Twitter Value: @" + author + " - " + tweet);
  Serial.println();
  char t = tweet.charAt(tweet.length() - 1);
  Serial.print("Twitter value: "); Serial.print(t);
  Serial.println();
 
 
 
/******************************* Determine the status of the data    ********************************/
   if(y == '0'&& w == '0' && t == '0' )
   {  
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,LOW); 
       Serial.print(" LED OFF");
   }
  if(y == '0'&& w == '0' && t =='1')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
   }
   if(y == '0'&& w == '1' && t =='0')
   {  
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
   }
   
   // The controller can't be control the light from both of 2 platform.....
   // You could control, but it will doing the backward. 
   // Such as, when y = 1; the w = 0 the light up.and when y = 1, w = 1, the light down etc...
   if(y == '0'&& w == '1' && t =='1')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
   }
   if(y == '1'&& w == '0' && t =='0')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
   }
   if(y == '1'&& w == '0' && t =='1')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
   }
   if(y == '1'&& w == '1' && t =='0')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
   }
   if(y == '1'&& w == '1' && t =='1')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,LOW); 
       Serial.print(" LED OFF");
   }
   
/******************************* Determine the status of the data    ********************************/
/*
   if(y == '0'&& w == '0' && )
   {  
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,LOW); 
       Serial.print(" LED OFF");
   }
  if(y == '0'&& w == '1')
   {
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
   }
   if(y == '1'&& w == '0')
   {  
       Serial.println();
       Serial.print("y = "); Serial.print(y);Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
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
   }
   
 */
   // Ensure the last bit of data is sent.
   Serial.flush(); 
}

