#include <SPI.h>
#include <RF24.h>
#include <Servo.h>
#include <RadioLIFT.h>

// TODO: phase 2
// TODO: throttle
// TODO: pid w/ control module

// Servo's pins
#define LEFT_WING_PIN   3
#define RIGHT_WING_PIN  6
#define RUDDER_PIN      9

// Receiver pins
#define CE_PIN  7
#define CSN_PIN 8
// #define THROTTLE_PIN 9

int left_wing_pos = 0;
int right_wing_pos = 0;
int rudder_pos = 0;

Servo left_wing;
Servo right_wing; 
Servo rudder;

Signal data;

RF24 radio(CE_PIN, CSN_PIN);

void ResetData(){
  data.pitch = 1500;
  data.roll = 1500;
  data.rudder = 1500;
}

void setup(){
    left_wing.attach(LEFT_WING_PIN);
    right_wing.attach(RIGHT_WING_PIN);
    rudder.attach(RUDDER_PIN);

    ResetData();

    radio.begin();
    radio.openReadingPipe(1, pipeOut);
    radio.setPALevel(CURRENT_RANGE);
    radio.setDataRate(CURRENT_DATA_RATE);
    radio.setChannel(CURRENT_CHANNEL);

    radio.startListening();

}

unsigned long lastRcvTime = 0;

void loop(){
    while(radio.available()){
        radio.read(&data, sizeof(Signal));
        lastRcvTime = millis();
    }

    unsigned long now = millis();

    // Check if conn is lost
    if (now - lastRcvTime > 1000) {
        ResetData();
    }

    left_wing.writeMicroseconds(constrain(data.pitch + data.roll - 1500, 1000, 2000));
    right_wing.writeMicroseconds(constrain(data.pitch - data.roll + 1500, 1000, 2000));
    // Map rudder to around 90º w/ full stick motion
    int rudder_cmd = map(data.rudder, 1000, 2000, 1250, 1750);
    rudder.writeMicroseconds(constrain(rudder_cmd, 1250, 1750));    
    delay(10);
}
