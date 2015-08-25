void wechatData(int num)
{
  while(num =='2')
  {
/******************************* Get data from weline (wechat) ********************************/ 
   // form the string for the API header parameter:
  String openID = "Openid: ";
  openID += openAPI;
  
  // form the string for the URL parameter:
  //String weline_url = "http://api.weline.top/index.php/switch/getstatus/id/";
  String weline_url = "http://115.28.174.156/api1.0/index.php?s=/switch/getstatus/id/";
  weline_url += welineDEVICEID;
  /* Send the HTTP Get request 
    It's better to declare the Process here, so when the sendData function finishes the resources are immediately
    released. Declaring it global works too, BTW.
  */
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
  int wwww = weline.available();
  Serial.print(wwww);
 
/******************************* Printing data   ********************************/ 
  char we; String welineState = ""; // weline(wechat)微信
/* If there's incoming data from the net connection, then send it out the Serial */
/******************************* Print the weline data   ********************************/ 
   while (weline.available()>0) {
    we = weline.read();
   Serial.print(we); // print all message from weline api
    welineState += we;
  }
  Serial.println();
  Serial.print("welineState: "); Serial.print(welineState);
  Serial.println();
  char w = welineState.charAt(welineState.length()- 3);
  Serial.print("welineState value: "); Serial.print(w);
  Serial.println();

/******************************* Determine the status of the data  ********************************/   
   if( w == '0')
   {
       Serial.println();
       Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,LOW); 
       Serial.print(" LED OFF");
       mySwitch.send(CH1_OFF, 24);
   }
  if( w == '1')
   {
       Serial.println();
       Serial.print(" w = "); Serial.print(w);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
       mySwitch.send(CH1_ON, 24);
   }
/* Ensure the last bit of data is sent */
   Serial.flush(); 
   
  }
}
