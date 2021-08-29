#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

#define I0 A8// Button Pin
#define I1 A9// Potentiometer

//pin used, D13,D12,D11,D10
//front sensor D22,D24,D26,D28,D30,D32,D34

Servo driver;  // create servo object to control a servo
int pos = 86;    // variable to store the servo position
int pos2 = 0;    // variable to store the servo position
int max_left_angle = 69;//60 max
int max_right_angle = 101;//110 max
int min_left_angle = 69;
int min_right_angle = 101;
int center_angle = 86;

int delayslow = 15;
int delayfast = 15;
int traveldelay = 3000;

//This is the Ultrasonic Sensor
const int trigPin = 11; 
const int echoPin = 12; 
long duration; 
int distance; 
String printedval = "hey";
int collisionflag = 0;

//This is the Line Sensor
int S1_val = 0;
int S2_val = 0;
int S3_val = 0;
int S4_val = 0;
int S5_val = 0;
int CLP_val = 0;
int Near_val = 0;
const int S1 = 22; 
const int S2 = 24; 
const int S3 = 26; 
const int S4 = 28; 
const int S5 = 30; 
const int CLP = 32; 
const int Near = 34; 

//This is Button
 int val = 0;
 int startme = 0;

//This is the RGB
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X);
double red_r = 92;
double red_g = 34;
double red_b = 34;
double yellow_r = 190;
double yellow_g = 194;
double yellow_b = 90;
int color_flag = 0;

//Motor
 String Speedme = "";
 String Speedme2 = "";
 int reversespeed = 0;
 int mapspeed =  0;
 int mapspeed2= 0;

 int l1 = 0;
 int l2 = 0;
 int l3 = 0;
 int l4 = 0;
 int l5 = 0;

void setup() {
  driver.attach(10);  
  Serial.begin(9600);//Main Serial Port
  Serial1.begin(9600);//Right Motor
  Serial2.begin(9600);//Left Motor
  Serial3.begin(9600);//SErial to Sub Arduino
  
  driver.write(pos);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output 
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input 
 
  Serial1.println("Y");
  Serial2.println("Y");
  Serial1.println("S0");
  Serial2.println("S0");
}

void loop() {

//Collision Sensor
digitalWrite(trigPin, LOW); 
delayMicroseconds(2); 
// Sets the trigPin on HIGH state for 10 micro seconds 
digitalWrite(trigPin, HIGH); 
delayMicroseconds(10); 
digitalWrite(trigPin, LOW); 
// Reads the echoPin, returns the sound wave travel time in microseconds 
duration = pulseIn(echoPin, HIGH); 
// Calculating the distance 
distance= duration*0.034/2; 


 val = analogRead(I0);
 reversespeed = analogRead(I1);
 mapspeed = map(reversespeed, 9, 888, 0, 255);
 mapspeed2 = mapspeed - 0;
 
int S1_val = digitalRead(S1);
int S2_val = digitalRead(S2);
int S3_val = digitalRead(S3);
int S4_val = digitalRead(S4);
int S5_val = digitalRead(S5);
int CLP_val = digitalRead(CLP);
int Near_val = digitalRead(Near);

 
   if (val > 1000){
      startme = 1;
   }

uint16_t r, g, b, c, colorTemp, lux;
tcs.getRawData(&r, &g, &b, &c);
String Color = "R-" + String(r) + "," + "G-" + String(g) + "," + "B-" + String(b);

if ((r >= (red_r-red_r*0.1)) && (r <= (red_r+red_r*0.1)) && (g >= (red_g-red_g*0.1)) && (g <= (red_g+red_g*0.1)) && (b >= (red_b-red_b*0.1)) && (b <= (red_b+red_b*0.1)) )
{
  if (color_flag==0){
    Serial3.println("BLOCK RED");
//CHANGE to 3
    Serial3.flush();
    color_flag = 1;
  }
}
else if  ( (r >= (yellow_r-yellow_r*0.1)) && (r <= (yellow_r+yellow_r*0.1)) && (g >= (yellow_g-yellow_g*0.1)) && (g <= (yellow_g+yellow_g*0.1))  && (b >= (yellow_b-yellow_b*0.1)) && (b <= (yellow_b+yellow_b*0.1)) )
{
  if (color_flag==0){
    Serial3.println("BLOCK YELLOW");
//CHANGE to 3
    Serial3.flush();
    color_flag = 1;
  }
}
else {
  color_flag = 0;
}

//printedval = String(S1_val) +","+ String(S2_val) +","+ String(S3_val) +","+ String(S4_val) +","+ String(S5_val) +","+ String(CLP_val) +","+ String(Near_val);//Line Sensor Values
//Speedme = "S" + String(mapspeed);
//Serial.println(Speedme); 
//Serial.flush();

//Stop from Collission
if (distance<40) {
      collisionflag=1;
      off();
}
else {
      collisionflag=0;
}


//MOTOR DRIVER CONDITION


  if(S1_val==0&&S2_val==0&&S3_val==0&&S4_val==0&&S5_val==0&&startme==1){//AUTO-CORRECTION OFF
     S1_val = l1; 
     S2_val = l2; 
     S3_val = l3; 
     S4_val = l4; 
     S5_val = l5; 
  }

  if (S3_val == 0&&S1_val==1&&S2_val==1&&S4_val==1&&S5_val==1&&startme==1&&collisionflag==0){//center
      straightlang();
        }
  else if(S1_val==1&&S2_val==0&&S4_val==1&&S5_val==1&&startme==1&&collisionflag==0){//min left
left();
    do {  
        pos-=1;
       driver.write(pos);                   // tell servo to go to position in variable 'pos'
      delay(delayslow);                            // waits 15ms for the servo to reach the positioN
      S3_val = digitalRead(S3);
    }while (pos>min_left_angle&&S3_val==1);  
   
off();
    do {  
              pos+=1;
             driver.write(pos);                   // tell servo to go to position in variable 'pos'
            delay(delayslow);                            // waits 15ms for the servo to reach the positioN
          }while (pos<=center_angle);  

straight();
    
  }
  else if(S4_val==0&&S5_val==1&&S1_val==1&&S2_val==1&&startme==1&&collisionflag==0){//min right
right();
    do {  
        pos+=1;
       driver.write(pos);                   // tell servo to go to position in variable 'pos'
       delay(delayslow);                            // waits 15ms for the servo to reach the positioN
       S3_val = digitalRead(S3);
    } while (pos<min_right_angle&&S3_val==1);  

off();
    do {  
              pos-=1;
             driver.write(pos);                   // tell servo to go to position in variable 'pos'
            delay(delayslow);                            // waits 15ms for the servo to reach the positioN
          }while (pos>=center_angle);  

straight();
    
  }
  else if(S1_val==0&&S2_val==1&&S4_val==1&&S5_val==1&&startme==1&&collisionflag==0){//max left
left();
    do {  
        pos-=1;
       driver.write(pos);                   // tell servo to go to position in variable 'pos'
      delay(delayfast);                            // waits 15ms for the servo to reach the positioN
      S3_val = digitalRead(S3);
    }while (pos>max_left_angle&&S3_val==1);  
   
off();
    do {  
              pos+=1;
             driver.write(pos);                   // tell servo to go to position in variable 'pos'
            delay(delayfast);                            // waits 15ms for the servo to reach the positioN
          }while (pos<=center_angle);  

straight();
    
  }
  else if(S4_val==1&&S5_val==0&&S1_val==1&&S2_val==1&&startme==1&&collisionflag==0){//max right
right();
    do {  
        pos+=1;
       driver.write(pos);                   // tell servo to go to position in variable 'pos'
       delay(delayfast);                            // waits 15ms for the servo to reach the positioN
       S3_val = digitalRead(S3);
    } while (pos<max_right_angle&&S3_val==1);  

off();
    do {  
              pos-=1;
             driver.write(pos);                   // tell servo to go to position in variable 'pos'
            delay(delayfast);                            // waits 15ms for the servo to reach the positioN
          }while (pos>=center_angle);  

straight(); 
  }
     l1 = S1_val; 
     l2 = S2_val; 
     l3 = S3_val; 
     l4 = S4_val; 
     l5 = S5_val; 

}

void straight(){
  Speedme = "S" + String(mapspeed);
  Speedme2 = "S-" + String(mapspeed2);
      Serial1.println(Speedme);
      Serial2.println(Speedme2);//lesser orig
}

void straightlang(){
      Serial1.println("S255");
      Serial2.println("S-255");
}

void left(){
  Speedme = "S" + String(mapspeed);
  Speedme2 = "S" + String(mapspeed2);
      Serial1.println(Speedme);
      Serial2.println(Speedme2);//lesser orig
}

void right(){
  Speedme = "S-" + String(mapspeed);
  Speedme2 = "S-" + String(mapspeed2);
      Serial1.println(Speedme);//lesser orig
      Serial2.println(Speedme2);
}

void off(){
      Serial1.println("S0");
      Serial2.println("S0");  
}



