// Code for remote controller

#include <SPI.h>
#include <RF24.h>
#include <RadioLIFT.h>

// TODO: right joystick

#define DEBUG 0

// CE, CSN pins, respectively 
#define CE_PIN  7
#define CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN);

// Struct with sent data
Signal data;

const int rollAxis = A3;
const int pitchAxis = A2;
const int rudderAxis = A4;

/*
  Center servos when no commands are received
*/
void ResetData(){
  data.pitch = 1500;
  data.roll = 1500;
  data.rudder = 1500;
}

/*
  Expo/curves so plane feels more precise around the center 
  by softer stick response near 0 values
*/
float applyExpo(int raw, float expo) {
  float normalized = (raw - 512) / 512.0;
  float curved = expo * pow(normalized, 3) + (1 - expo) * normalized;
  return (curved * 512) + 512;
}

void setup() {
  if(DEBUG) Serial.begin(9600);
  
  if(!radio.begin()){
    if(DEBUG) Serial.println("Radio HW not working");
    while(1);
  }

  radio.openWritingPipe(pipeOut);
  radio.setPALevel(CURRENT_RANGE);
  radio.setDataRate(CURRENT_DATA_RATE);
  radio.setChannel(CURRENT_CHANNEL);
  radio.setAutoAck(true);
  radio.stopListening();

  if(DEBUG){
    Serial.print("Radio is connected? ");
    Serial.println(radio.isChipConnected() ? "Yes" : "No");
  }

  ResetData();
}

void loop() {
  // Map left joystick values from analog interval to servo interval
  data.roll = map((long)applyExpo(analogRead(rollAxis), 0.5), 0, 1023, 1000, 2000);
  data.pitch = map((long)applyExpo(analogRead(pitchAxis), 0.5), 0, 1023, 1000, 2000);
  data.rudder = map((long)applyExpo(analogRead(rudderAxis), 0.5), 0, 1023, 1000, 2000);

  // Send data w/ delay for 50Hz (most servos frequency)
  radio.write(&data, sizeof(Signal));

  if(DEBUG){
    Serial.println("==============================");
    Serial.print("Roll: ");
    Serial.println(data.roll);
    Serial.print("Pitch: ");
    Serial.println(data.pitch);
    Serial.print("Rudder: ");
    Serial.println(data.rudder);
    Serial.println("==============================");
  }

  delay(20);

}
