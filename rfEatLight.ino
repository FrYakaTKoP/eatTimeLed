/*
  RF 433Mhz Keyfob controlled "come eat signal Led"

  based on following libaries:
  https://github.com/sui77/rc-switch/
  (TBD) add fastled link

  Creator: FrYakaTKoP / 2017


 NOT WORKING! WORK IN PROGRESS VERSION!
 


*/

#include <RCSwitch.h>
#include "FastLED.h"

// define LED settings:
#define NUM_LEDS 4
#define DATA_PIN 44

// serial debug true/false
boolean debug = true;

// Keyfob Keys (dez):
// read the values using serial monitor after enabling debug
const unsigned long keyfobKeys[] PROGMEM =
{
  5592512,
  5592332,
  5592368,
  5592323,
  5594371
}

// blink time 
int blinktime = 2; // sec


RCSwitch rfRx = RCSwitch();
CRGB leds[NUM_LEDS];

void setup() {
  if(debug)
  {
    // enable serial
    Serial.Begin(9600);
  }
  // enable rf Receiver
  rfRx.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2 on most arduinos

  //LED  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  if (mySwitch.available()) {    
    unsigned long rxValue = rfRx.getReceivedValue();
    bool knowValue = false;
    
    if (rxValue == 0) 
    {
      if(debug)
      {
        // value received not valid 
        Serial.println("no valid message");
      }
    }
    }
    else
    {  
      for(i=0; i < sizeof(keyfobValues) - 1; i++ )
      {
        if(rxValue == keyfobValues[i])
        {
          if(debug)
          {
            // known value received
            Serial.print("Button ");
            Serial.print(i+11, HEX );
            Serial.println(" pressed");
          }          
          knownValue = true;
          setLedState(i);
          break; // exit for loop no need to scan further  
        }
      }
    }
  
    if(debug && knownValue == false)
    {
      Serial.print("Received unkown Value");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit / ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
    }
    
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

