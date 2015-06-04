#include <RCSwitch.h>

#define CH1_ON  4478259
#define CH1_OFF 4478268
#define CH2_ON  4478403
#define CH2_OFF 4478412
#define CH3_ON  4478723
#define CH3_OFF 4478732

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #7  
  mySwitch.enableTransmit(7);
  
  // Optional set pulse length.
  mySwitch.setPulseLength(176);
}

void loop() {
  mySwitch.send(CH1_ON, 24);
  delay(2000);  
//  mySwitch.send(CH1_OFF, 24);
//  delay(2000);  

}
