#include <SoftwareSerial.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

const byte HC12RxdPin = 5;                  // Recieve Pin on HC12
const byte HC12TxdPin = 4;                  // Transmit Pin on HC12

SoftwareSerial HC12(HC12TxdPin,HC12RxdPin); // Create Software Serial Port
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X);
void setup() {
  digitalWrite(13,HIGH);
  Serial.begin(9600);                       // Open serial port to computer
  HC12.begin(9600);                         // Open serial port to HC12
}

void loop(void) {
uint16_t r, g, b, c, colorTemp, lux;
 
/*
int X= analogRead(A0);
int Y= analogRead(A1);

if (X>800&&Y==520){
   delay(1000);
   HC12.write("SP");              // Send that data to serial
}
else if (X==521&&Y<100){
   delay(1000);
   HC12.write("LP");              // Send that data to serial
}
else if (X==521&&Y>800){
   delay(1000);
   HC12.write("RP");              // Send that data to serial
}
else if (X<100&&Y==520){
   delay(1000);
   HC12.write("STOP");              // Send that data to serial
}
*/


 
tcs.getRawData(&r, &g, &b, &c);

String Color = "R-" + String(r) + "," + "G-" + String(g) + "," + "B-" + String(b);
HC12.println(Color);



  if(HC12.available()){                     // If Arduino's HC12 rx buffer has data
    String Serialread = HC12.readString();              // Send the data to the computer
    Serial.println(Serialread);              // Send the data to the computer
    }
}

