// This is the sender code of the test to check remote servo control


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut = 0xE9E8F0F0E1LL;   //IMPORTANT: The same as in the receiver 0xE9E8F0F0E1LL
RF24 radio(7, 8); // select CE,CSN pin

byte pitch;
byte roll;
byte yaw;

struct Signal {
  byte pitch;
  byte roll;
  byte yaw;
};

Signal data;

void ResetData() 
{
  data.pitch = 127; // Center (Signal lost position)
  data.roll = 127; // Center (Signal lost position)
  data.yaw = 127; // Center(Signal lost position)
  }
  
void setup(){
  Serial.begin(9600);
  
  //Start everything up
  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(108);
  radio.setAutoAck(true);
  radio.stopListening(); //start the radio comunication for Transmitter
  
  // Debug arduino connection to radio chip
  Serial.print("Radio is connected? ");
  Serial.println(radio.isChipConnected() ? "Yes" : "No");


  ResetData();
}

// Joystick center and its borders
int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse){
  val = constrain(val, lower, upper);

  if ( val < middle )
  val = map(val, lower, middle, 0, 128);

  else
  val = map(val, middle, upper, 128, 255);

  return ( reverse ? 255 - val : val );
}

void loop(){
  // Control Stick Calibration 
  // Setting may be required for the correct values of the control levers.
  data.roll = mapJoystickValues( analogRead(A2), 12, 524, 1020, true );      // "true" or "false" for servo direction
  data.pitch = mapJoystickValues( analogRead(A3), 12, 524, 1020, true );     // "true" or "false" for servo direction
  data.yaw = mapJoystickValues( analogRead(A1), 12, 524, 1020, true );       // "true" or "false" for servo direction
  
  if(radio.write(&data, sizeof(Signal))){
    Serial.println("==============================");
    Serial.print("Roll: ");
    Serial.println(data.roll);
    Serial.print("Pitch: ");
    Serial.println(data.pitch);
    Serial.println("==============================");
  } else {
    Serial.println("Transmission Failed");
  }

  delay(20);
}
