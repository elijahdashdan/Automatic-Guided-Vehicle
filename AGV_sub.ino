#include <genieArduino.h>  // MODIFIED new genieArduino library
#include <Wire.h>// FOR RGB
#include "Adafruit_TCS34725.h"//FOR RGB
#include <SoftwareSerial.h>

//This is the wireless transmitter
const int bSize = 20; 
char sBuffer[bSize];  // Serial buffer
char Data[20];       // ditto for data size
int ByteCount;

const byte HC12RxdPin = 5;                  // Recieve Pin on HC12
const byte HC12TxdPin = 4;                  // Transmit Pin on HC12
SoftwareSerial HC12(HC12TxdPin,HC12RxdPin); // Create Software Serial Port

//This is the LCD Screen
//Genie genie;
//#define RESETLINE 4  // Change this if you are not using an Arduino Adaptor Shield Version 2 (see code below)

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X);// RGB Sensor Setting
SoftwareSerial mySerial(9, 8); // RX, TX Receive from Main Arduino

//Ultrasonic
int trig = 0;
int echo = 0;

//LOGIC
int counter = 0;
int colorstate = 0;
int previoustate = 0;
double black_r = 12;
double black_g = 12;
double black_b = 9;
double yellow_r = 140;
double yellow_g = 135;
double yellow_b = 64;
String colorstate2 = "";
String previoustate2 = "";

int collisionflag =13;

void setup() {
  Serial.begin(9600);//LCD Serial
  HC12.begin(9600);                         // Open serial port to HC12
  mySerial.begin(9600);
  pinMode(7, OUTPUT); // Sets the trigPin as an Output 
  pinMode(12, INPUT); // Sets the echoPin as an Input 
  pinMode(11, OUTPUT); // Sets the trigPin as an Output 
  pinMode(10, INPUT); // Sets the echoPin as an Input 
  pinMode(3, OUTPUT); // Sets the trigPin as an Output 
  pinMode(2, INPUT); // Sets the echoPin as an Input 
  pinMode(13, OUTPUT); // Sets the echoPin as an Input 
  
/*This is the LCD Setup
  Serial1.begin(115200);//LCD Serial
  genie.Begin(Serial3);   // Use Serial0 for talking to the Genie Library, and to the 4D Systems display
  pinMode(RESETLINE, OUTPUT);  // Set D4 on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
  digitalWrite(RESETLINE, 1);  // Reset the Display via D4
  delay(100);
  digitalWrite(RESETLINE, 0);  // unReset the Display via D4
  delay (3500); //let the display start up after the reset (This is important)
  resetDisplay();
 */
 }

 void loop() {
  //genie.DoEvents();// Genie Event Handle
  
int ultraleft = read_ultra(7,12);//trig,echo
int ultramid = read_ultra(11,10);
int ultraright = read_ultra(3,2);
 
String Ultra = String(ultraleft) +","+ String(ultramid) +","+ String(ultraright);

if ((ultraleft<40)||(ultramid<40)||(ultraright<40)){
  digitalWrite(collisionflag,HIGH);
}
else{
  digitalWrite(collisionflag,LOW);
}
 Serial.println(Ultra);
 /*
uint16_t r, g, b, c, colorTemp, lux;
tcs.getRawData(&r, &g, &b, &c);
//String Color = "R2-" + String(r) + "," + "G2-" + String(g) + "," + "B2-" + String(b);


if ((r >= (black_r-black_r*0.1)) && (r <= (black_r+black_r*0.1)) && (g >= (black_g-black_g*0.1)) && (g <= (black_g+black_g*0.1)) && (b >= (black_b-black_b*0.1)) && (b <= (black_b+black_b*0.1)) )
{
colorstate = 1;
    colorstate2 = "Black";
    HC12.println(colorstate2);
}
else if ( (r >= (yellow_r-yellow_r*0.1)) && (r <= (yellow_r+yellow_r*0.1)) && (g >= (yellow_g-yellow_g*0.1)) && (g <= (yellow_g+yellow_g*0.1))  && (b >= (yellow_b-yellow_b*0.1)) && (b <= (yellow_b+yellow_b*0.1)) )
{
colorstate = 2;
    colorstate2 = "Yellow";
    HC12.println(colorstate2);
}


if (previoustate!=colorstate)
{
  if (colorstate==1){
    colorstate2 = "Black";
    HC12.println(colorstate2);
  }
  else if (colorstate==2){
    colorstate2 = "Yellow";
    HC12.println(colorstate2);
  }
}
previoustate = colorstate;


while (mySerial.available()>0) {
 ByteCount = -1;
 ByteCount =  mySerial.readBytesUntil('\n',sBuffer,bSize);  
    if (ByteCount  > 0) {
      HC12.println(sBuffer);
   }
    HC12.flush();
  memset(sBuffer, 0, sizeof(sBuffer));   // Clear contents of Buffer
}   
*/
}
 /*static void resetDisplay(void)
{
  for (int z = 0;z <16;z++)
  {
      genie.WriteStr(z, 00.00);
  }
      genie.WriteObject(GENIE_OBJ_SCOPE, 0, 0);
}
*/

int read_ultra(int trig,int echo){
//Collision Sensor

  
digitalWrite(trig, LOW); 
delayMicroseconds(2); 
digitalWrite(trig, HIGH); 
delayMicroseconds(10); 
digitalWrite(trig, LOW); 
long duration = pulseIn(echo, HIGH); 
int distance= duration*0.034/2; 
return distance;
}
