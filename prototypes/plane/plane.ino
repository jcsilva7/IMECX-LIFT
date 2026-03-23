#include <SPI.h>
#include <RF24.h>
#include <Servo.h>
#include <RadioLIFT.h>

// TODO: throttle
// TODO: pid w/ control module

// Servo's pins
#define LEFT_WING_PIN   3
#define RIGHT_WING_PIN  6

// Receiver pins
#define CE_PIN  7
#define CSN_PIN 8
// #define THROTTLE_PIN 9

// Show debug prints in serial console or not
#define DEBUG 0

int left_wing_pos = 0;
int right_wing_pos = 0;
int throttle_pos = 0;

Servo left_wing;
Servo right_wing;
// Servo throttle;

Signal data;

RF24 radio(CE_PIN, CSN_PIN);

void ResetData(){
  data.pitch = 1500;
  data.roll = 1500;
  data.throttle = 0;
}

void setup(){
    if(DEBUG) Serial.begin(9600);

    left_wing.attach(LEFT_WING_PIN);
    right_wing.attach(RIGHT_WING_PIN);

    ResetData();

    radio.begin();
    radio.openReadingPipe(1, pipeOut);
    radio.setPALevel(CURRENT_RANGE);
    radio.setDataRate(CURRENT_DATA_RATE);
    radio.setChannel(CURRENT_CHANNEL);

    radio.startListening();

    if(DEBUG){
        Serial.print("Radio chip connected? ");
        Serial.println(radio.isChipConnected() ? "Yes" : "No");
        
        Serial.println("Receiver Ready");
    }
}

unsigned long lastRcvTime = 0;

void loop(){
    while(radio.available()){
        radio.read(&data, sizeof(Signal));
        lastRcvTime = millis();
    }

    if(DEBUG){
        Serial.print("Roll: ");
        Serial.println(data.roll);
        Serial.print("Pitch: ");
        Serial.println(data.pitch);
        Serial.print("Throttle: ");
        Serial.println(data.throttle);
    }

    unsigned long now = millis();

    // Check if conn is lost
    if (now - lastRcvTime > 1000) {
        ResetData();
        if(DEBUG) Serial.println("Signal Lost");
    }

    left_wing.writeMicroseconds(constrain(data.pitch + data.roll - 1500, 1000, 2000));
    right_wing.writeMicroseconds(constrain(data.pitch - data.roll + 1500, 1000, 2000));
    
    delay(10);
}