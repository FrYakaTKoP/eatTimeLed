/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void setup() {

  Serial.begin(9600);
    // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(42);

  // Optional set pulse length.
  mySwitch.setPulseLength(290);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(3);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) { 
    mySwitch.send("010101010101110100000011");
    Serial.println("sent");
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}


/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


