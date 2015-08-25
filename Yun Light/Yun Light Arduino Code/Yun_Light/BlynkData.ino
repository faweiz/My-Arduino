 void BlynkData(int num)
{
  while(num =='5')
  {
   Blynk.run();
    int b = digitalRead(5);             // Here I used digital 5 as a input button to control the light
    Serial.print(b);
   if( b == 0)
   {
       Serial.println();
       Serial.print(" b = "); Serial.print(b);
       Serial.println();
       digitalWrite(13,LOW); 
       Serial.print(" LED OFF");
       mySwitch.send(CH1_OFF, 24);
   }
  if( b == 1)
   {
       Serial.println();
       Serial.print(" b = "); Serial.print(b);
       Serial.println();
       digitalWrite(13,HIGH); 
       Serial.print(" LED ON");
       mySwitch.send(CH1_ON, 24);
   }
  }  
}
