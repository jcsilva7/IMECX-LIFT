// Code for remote controller

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Data.h>

#define DEBUG 0

// CE, CSN pins, respectively 
const int CE_pin = 7;
const int CSN_pin = 8;

RF24 radio(CE_pin, CSN_pin);

// Struct with sent data
Signal data;

const int rollAxis = A2;
const int pitchAxis = A3;
const int throttleAxis = A4;

/*
  Center servos when no commands are received
*/
void ResetData(){
  data.pitch = 1500;
  data.roll = 1500;
}

void setup() {
  if(DEBUG) Serial.begin(9600);
  
  if(!radio.begin()){
    if(DEBUG) Serial.println("Radio HW not working");
    while(1);
  }

  radio.openWritingPipe(pipeOut);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.setAutoAck(true);
  radio.stopListening();

  if(DEBUG){
    Serial.print("Radio is connected? ");
    Serial.println(radio.isChipConnected() ? "Yes" : "No");
  }

  ResetData();
  data.throttle = 0;
}

void loop() {
  // Map left joystick values from analog interval to servo interval
  data.roll = map(analogRead(rollAxis), 0, 1023, 1000, 2000);
  data.pitch = map(analogRead(pitchAxis), 0, 1023, 1000, 2000);
  // Map right joystick values to small change values for smoother throttle control
  int throttleStick = map(analogRead(throttleAxis), 0, 1023, -10, 10);
  
  data.throttle = (abs(throttleStick) > 2) ? throttleStick : 0;

  // Send data w/ delay for 50Hz (most servos frequency)
  radio.write(&data, sizeof(Signal));

  if(DEBUG){
    Serial.println("==============================");
    Serial.print("Roll: ");
    Serial.println(data.roll);
    Serial.print("Pitch: ");
    Serial.println(data.pitch);
    Serial.print("Throttle: ");
    Serial.println(data.throttle);
    Serial.println("==============================");
  }

  delay(20);

}
