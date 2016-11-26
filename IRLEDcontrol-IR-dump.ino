
//https://www.youtube.com/user/greatscottlab

#include <IRremote.h>
int bright;
int before;
int out=9; //connect your LED to pin 9 
int steps=5; //dimmer steps, vary those to increase/decrease the steps between full brightness and turned off
int RECV_PIN = 2; //data out of IR receiver connects to pin 11

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup(){
  irrecv.enableIRIn(); // start the receiver
  before=0; //LED is turned off
  bright=255; //brightness value is at maximum (255)
  pinMode(out,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (irrecv.decode(&results)) {
     Serial.print(results.value);
  /*
  if (results.value==0x20DF8D72){ //Code to turn the LED ON/OFF
    if(before==0){ // if the LED was turned off, then we turn it on 
      digitalWrite(out,HIGH);
      before=1; //LED is now turned on
    }
    else{
      digitalWrite(out,LOW); //if the LED was turned on, then we turn it off
      before=0;
      bright=255; 
    }}
  if (results.value==0x20DFF10E && before==1){ //Code to decrease the brightness
    if(bright-255/steps<0){ 
      analogWrite(out,bright);
    }
    else{
    bright=bright-255/steps;
    analogWrite(out,bright);
  }}
  if (results.value==0x20DF718E && before==1){ //Code to increase the brightness
    if(bright+255/steps>255){
      analogWrite(out,bright);
    }
    else{    
    bright=bright+255/steps;
    analogWrite(out,bright);
  }}
  /**/
  
  irrecv.resume();
}}




