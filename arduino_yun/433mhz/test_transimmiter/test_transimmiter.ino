 /* 
  RF Blink - Transmit sketch 
     Written by ScottC 17 Jun 2014
     Arduino IDE version 1.0.5
     Website: http://arduinobasics.blogspot.com
     Transmitter: FS1000A/XY-FST
     Description: A simple sketch used to test RF transmission.          
 ------------------------------------------------------------- */

 #define rfTransmitPin 7  //RF Transmitter pin = digital pin 4
 #define ledPin 13        //Onboard LED = digital pin 13

 void setup(){
   pinMode(rfTransmitPin, OUTPUT);     
   pinMode(ledPin, OUTPUT);    
 }

 void loop(){
   for(int i=4000; i>5; i=i-(i/3)){
     digitalWrite(rfTransmitPin, HIGH);     //Transmit a HIGH signal
     digitalWrite(ledPin, HIGH);            //Turn the LED on
     delay(2000);                           //Wait for 1 second
     
     digitalWrite(rfTransmitPin,LOW);      //Transmit a LOW signal
     digitalWrite(ledPin, LOW);            //Turn the LED off
     delay(i);                            //Variable delay
   }
 }
