void DeviceHubData(int num)
{
  while(num =='6')
  {
/******************************* Get data from DeviceHub.net  ********************************/ 
   // form the string for the API header parameter
  String XApiKey = "X-ApiKey: ";
  XApiKey += DeviceApiKey;  
  // form the string for the URL parameter:
  String DeviceHub_url = "http://api.devicehub.net/v2/project/";
  DeviceHub_url += PROJECT_ID;
  DeviceHub_url += "/device/";
  DeviceHub_url += DEVICE_UUID;
  DeviceHub_url += "/actuator/";
  DeviceHub_url += ACTUATOR_NAME;
  DeviceHub_url += "/state";
  /* Send the HTTP Get request 
    It's better to declare the Process here, so when the sendData function finishes the resources are immediately
    released. Declaring it global works too, BTW.
  */
  Process DeviceHub;
  Serial.print("\nSending DeviceHub data... ");
  DeviceHub.begin("curl");
  DeviceHub.addParameter("--request");
  DeviceHub.addParameter("GET");
  DeviceHub.addParameter("--data-binary");
  DeviceHub.addParameter(dataStringTemp);
  DeviceHub.addParameter("--header");
  DeviceHub.addParameter(XApiKey);
  DeviceHub.addParameter(DeviceHub_url);
  DeviceHub.run();
  Serial.println("Done! DeviceHub");
  Serial.println(DeviceHub_url);
  int Dev = DeviceHub.available();
  Serial.print(Dev);
 
/******************************* Printing data   ********************************/ 
  char de; String DeviceHubState = ""; // DeviceHub.net
/* If there's incoming data from the net connection, then send it out the Serial */
/////////////////////////// Print the DeviceHub data  /////////////////////////// 
   while (DeviceHub.available()>0) {
    de = DeviceHub.read();
   Serial.print(de); // print all message from weline api
    DeviceHubState += de;
  }
  Serial.println();
  Serial.print("DeviceHubState: "); Serial.print(DeviceHubState);
  Serial.println();
  char d = DeviceHubState.charAt(DeviceHubState.length()- 48);
  Serial.print("DeviceHubState value: "); Serial.print(d);
  Serial.println();

/******************************* Determine the status of the data  ********************************/   
   if( d == '0')
   {  
       Serial.println();
       Serial.print(" d = "); Serial.print(d);
       Serial.println();
       digitalWrite(13,LOW); 
       Serial.print(" LED OFF");
       mySwitch.send(CH1_OFF, 24);
   }
  if( d == '1')
   {
       Serial.println();
       Serial.print(" d = "); Serial.print(d);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
       mySwitch.send(CH1_ON, 24);
   }
/* Ensure the last bit of data is sent */
   Serial.flush(); 
   
  }
}
