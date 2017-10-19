#include <SoftwareSerial>

SoftwareSerial BLE(13, 12); // RX, TX  
// Connect HM10      Arduino Uno
//     Pin 1/TXD          Pin 13
//     Pin 2/RXD          Pin 12

boolean NL = true;
char c=' ';

void setup() {  
  Serial.begin(9600);
  // If the baudrate of the HM-10 module has been updated,
  // you may need to change 9600 by another value
  // Once you have found the correct baudrate,
  // you can update it using AT+BAUDx command 
  // e.g. AT+BAUD0 for 9600 bauds
  BLE.begin(9600);
}

void loop() {  
  char c;
  if (Serial.available()) {
    c = Serial.read();
    BLE.print(c);
  }
      
  if (BLE.available()) {
    c = BLE.read(); 
    Serial.print(c);
  } 
    
}