void yeelinkData(int num)
{
  while(num =='1'){
/*************************** Get data from yeelink.net (ios,android,web and wechat) ***************************/ 
    // form the string for the API header parameter:
  String apiString = "U-ApiKey: ";
  apiString += APIKEY;
  // form the string for the URL parameter:
  String yeelink_url = "http://api.yeelink.net/v1.1/";
  yeelink_url += "device/";
  yeelink_url += DEVICEID;
  yeelink_url += "/sensor/";
  yeelink_url += SENSORID;
  yeelink_url += "/datapoints";
  /* Send the HTTP Get request 
    It's better to declare the Process here, so when the sendData function finishes the resources are immediately
    released. Declaring it global works too, BTW.
  */
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
  int yyy = yeelink.available();
  Serial.print(yyy);
 
/******************************* Printing data   ********************************/ 
  char yee; String yeelinkState = ""; // yeelink.net
/* If there's incoming data from the net connection, then send it out the Serial */
/******************************* Print the weline data   ********************************/ 
   while (yeelink.available()>0) {
    yee = yeelink.read();
    Serial.print(yee);  // print all message from weline api
    yeelinkState += yee;
  }
  Serial.println();
  Serial.print("yeelinkState: "); Serial.print(yeelinkState);
  Serial.println();
  char y = yeelinkState.charAt(yeelinkState.length() - 75);
  Serial.print("yeelinkState value: ");Serial.print(y);
  Serial.println();

/******************************* Determine the status of the data  ********************************/   
   if( y == '0')
   {  
       Serial.println();
       Serial.print(" y = "); Serial.print(y);
       Serial.println();
       digitalWrite(13,LOW); 
       Serial.print(" LED OFF");
       mySwitch.send(CH1_OFF, 24);
   }
  if( y == '1')
   {
       Serial.println();
       Serial.print(" y = "); Serial.print(y);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
       mySwitch.send(CH1_ON, 24);
   }
/* Ensure the last bit of data is sent */
   Serial.flush(); 
}
}
