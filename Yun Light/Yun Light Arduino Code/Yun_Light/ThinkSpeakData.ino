 #include <HttpClient.h>
 void ThinkSpeakData(int num)
{
  while(num =='4')
  {  
/******************************* Get data from ThingSpeak (IFTTT/Do button/ twitter) ********************************/ 
  // form the string for the URL parameter:
  String thingSpeak_url = "http://api.thingspeak.com/";
  thingSpeak_url += "channels/";
  thingSpeak_url += CHANEELID;
  thingSpeak_url += "/feed/last?key="; // Read "Get Last Entry in a Channel Feed" https://thingspeak.com/docs/channels#get_feed
  thingSpeak_url += thingSpeakAPI;
  /* Send the HTTP Get request 
    It's better to declare the Process here, so when the sendData function finishes the resources are immediately
    released. Declaring it global works too, BTW.
  */
  HttpClient thingSpeak;
  Serial.print("\nSending thingSpeak data... ");
  thingSpeak.get(thingSpeak_url); // Make a HTTP request:
  int ttt = thingSpeak.available();
  Serial.print(ttt);
  
/******************************* Printing data   ********************************/ 
  char thsp; String thingSpeak_State =""; // thingSpeak
/* If there's incoming data from the net connection, then send it out the Serial */

/******************************* Print the thingSpeak data   ********************************/ 
  while (thingSpeak.available()) {
    thsp = thingSpeak.read();
    Serial.print(thsp); // print all message from ThingSpeak api
    thingSpeak_State += thsp;
  }
  Serial.println();
  Serial.print("thingSpeak State: "); Serial.print(thingSpeak_State);
  Serial.println();
  char t = thingSpeak_State.charAt(thingSpeak_State.length()-3);
  Serial.print("thingSpeak State value: "); Serial.print(t);
  Serial.println();
 
/******************************* Determine the status of the data  ********************************/   
   if( t == '0')
   {  
       Serial.println();
       Serial.print(" t = "); Serial.print(t);
       Serial.println();
       digitalWrite(13,LOW); 
       Serial.print(" LED OFF");
       mySwitch.send(CH1_OFF, 24);
   }
  if( t == '1')
   {
       Serial.println();
       Serial.print(" t = "); Serial.print(t);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
       mySwitch.send(CH1_ON, 24);
   }
/* Ensure the last bit of data is sent */
   Serial.flush(); 
  }
}
