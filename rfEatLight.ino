/*
  Simple example for receiving
  
  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>
#include "FastLED.h"

// LED 
#define NUM_LEDS 4
#define DATA_PIN 44
CRGB leds[NUM_LEDS];

// RF
RCSwitch mySwitch = RCSwitch();

int blinktime = 2; // sec

void setup() {

  Serial.begin(9600);
  
  // RF
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2

  //LED  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  if (mySwitch.available()) {    
    unsigned long value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      //not valid message
    } else if (value == 5592512) {
      lightled(0,blinktime,100,0,0);
    } else if (value == 5592332) {
      lightled(1,blinktime,0,100,0);
    } else if (value == 5592368) {
      lightled(2,blinktime,0,0,100);
    } else if (value == 5592323) {
      lightled(3,blinktime,100,100,100);
    } else if (value == 5594371) {
      lightled(1,5,100,100,100);
    }
     

      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
    
    mySwitch.resetAvailable();
  }
}

void lightled(int led, int t, int r, int g, int b)
{
  Serial.println("blink");
  leds[led].r = r;
  leds[led].g = g;
  leds[led].b = b;  
  FastLED.show();
  delay(t/2*1000);
  // Now turn the LED off, then pause
  leds[led] = CRGB::Black;
  FastLED.show();
  delay(t/2*1000);
}

