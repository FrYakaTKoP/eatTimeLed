/*
  RF 433Mhz Keyfob controlled "come eat signal Led"

  based on following libaries:
  https://github.com/sui77/rc-switch/
  http://fastled.io/
  

  Creator: FrYakaTKoP / 2017

  Version:
  1.0 initial release
      - working
      - does not use interuppt for ack button


  
 


*/

#include <RCSwitch.h>
#include "FastLED.h"

// define settings:
#define NUM_LEDS 4
#define LED_DATA_PIN 4 // Pin D4
#define BUTTON_PIN 3 // Pin D3 interuppt 1 on uno

#define RX_IQR 0 // Receiver on interrupt 0 => that is pin D2 on uno

// serial debug true/false
boolean debug = true;

// Keyfob Keys (dez):
// read the values using serial monitor after enabling debug
unsigned long keyfobKeys[] =
{
  5592512,
  5592332,
  5592368,
  5592323,
  5594371
};

// blink time 
const int blinktime = 1; // sec


volatile int sysState = 0;
RCSwitch rfRx = RCSwitch();
CRGB leds[NUM_LEDS];

void setup() {
  if(debug)
  {    
    Serial.begin(9600); // enable serial
  }
  // enable rf Receiver
  rfRx.enableReceive(RX_IQR);  

  // LED  
  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);

  // Button
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), processButton, FALLING);
}

void loop() {
  if (rfRx.available()) {    
    processRx();
  }
  processState();
  if(digitalRead(BUTTON_PIN) == 0)
  {
    processButton();
  }
}

void processButton()
{
  sysState = 0;
  fill_solid(leds, NUM_LEDS, CRGB::Black); // Set all off
  FastLED.show();
  // enable rf Receiver
  //rfRx.enableReceive(RX_IQR);
  //rfRx.resetAvailable();
}

void processRx()
{
  unsigned long rxValue = rfRx.getReceivedValue();
  bool knownValue = false;
  
  if (rxValue == 0) 
  {
    // value received not valid 
    if(debug)
    {       
      Serial.println("no valid message");
    }
  }
  else
  {  
    for(int i=0; i < sizeof(keyfobKeys); i++ )
    {
      if(rxValue == keyfobKeys[i])
      {
        if(debug)
        {
          // known value received
          Serial.print(F("Button "));
          Serial.print(i+10, HEX );
          Serial.println(F(" pressed"));
        }          
        knownValue = true;        
        sysState = i + 1;  
        if(sysState == 1)
        {
          //rfRx.disableReceive();  // disable Rx since we don't need it in sysState 1
        }
        break; // exit for loop no need to scan further  
      }
    }
  }  
  if(debug && knownValue == false)
  {
    Serial.print(F("Received unkown Value"));
    Serial.print(rfRx.getReceivedValue());
    Serial.print(F(" / "));
    Serial.print( rfRx.getReceivedBitlength());
    Serial.print(F("bit / "));
    Serial.print(F("Protocol: "));
    Serial.println(rfRx.getReceivedProtocol());
  }    
  rfRx.resetAvailable();
}

void processState()
{
  if(sysState == 1)
  {
    // Indicate eat time until reset with HW button  
    fill_solid(leds, NUM_LEDS, CRGB::Red); // Set all to red.
    FastLED.show();      
    delay(blinktime*1000/2);
    fill_solid(leds, NUM_LEDS, CRGB::Black); // Set all off
    FastLED.show();
    delay(blinktime*1000/2);
  }
  else if(sysState >= 2)
  {
    // only temporary state turn on leds then return to state 0
    // turn leds off short time to signal a 2nd press of the low priority buttons
    fill_solid(leds, NUM_LEDS, CRGB::Black); // Set all off
    FastLED.show();
    delay(blinktime*1000/2);
    // indicate low priority ring
    fill_solid(leds, NUM_LEDS, CRGB::Green); // Set all to red.
    FastLED.show();  
    sysState = 0;
  }
}

