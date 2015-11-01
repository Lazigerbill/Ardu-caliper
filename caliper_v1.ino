#include "ClickButton.h"

// the Button
const int buttonPin1 = 7;
ClickButton button1(buttonPin1, HIGH, CLICKBTN_PULLUP);

// the Caliper
int i;
int sign;
long value;
float result;
int clockpin = 4;  
int datapin = 5;
unsigned long tempmicros;

void setup()
{ 
  Serial.begin(115200);
  pinMode(clockpin, INPUT);
  pinMode(datapin, INPUT);
  delay(5000);
  Serial.println("ready!");
  
  // Setup button timers (all in milliseconds / ms), These are default if not set, but changeable for convenience
  button1.debounceTime   = 20;   // Debounce timer in ms
  button1.multiclickTime = 250;  // Time limit for multi clicks
  button1.longClickTime  = 1000; // time until "held-down clicks" register
}


void loop()
{
  // Update button state
  button1.Update();

  if(button1.clicks == 1) {
    Serial.println("Single Click!");
    while (digitalRead(clockpin)==HIGH) {} //if clock is LOW wait until it turns to HIGH
    tempmicros=micros();
    while (digitalRead(clockpin)==LOW) {} //wait for the end of the HIGH pulse
    if ((micros()-tempmicros)>500) { //if the HIGH pulse was longer than 500 micros we are at the start of a new bit sequence
      decode(); //decode the bit sequence
  }
  }
  if(button1.clicks == 2) Serial.println("Double Click!");
  if(button1.clicks == -1) Serial.println("Button Hold!");

 
}

void decode() {
  sign=1;
  value=0;
  for (i=0;i<23;i++) {
    while (digitalRead(clockpin)==HIGH) { } //wait until clock returns to HIGH- the first bit is not needed
    while (digitalRead(clockpin)==LOW) {} //wait until clock returns to LOW
    if (digitalRead(datapin)==LOW) {
      if (i<20) {
        value|= 1<<i;
      }
      if (i==20) {
        sign=-1;
      }
    }
  }
  result=(value*sign)/100.00;    
  Serial.println(result,2); //print result with 2 decimals
}
