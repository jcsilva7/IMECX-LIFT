// This is the receiver code of the test to check remote servo control

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

int ch_width_1 = 0;
int ch_width_2 = 0;

Servo ch1;
Servo ch2;

short int ch1_pin = 3;
short int ch2_pin = 6;

struct Signal {
  byte pitch;
  byte roll;
} __attribute__((packed));

Signal data;

const uint64_t pipeIn = 0xE9E8F0F0E1LL;

RF24 radio(7, 8); 

void ResetData()
{
  // Define the inicial value of each data input.
  // The middle position for Potenciometers. (254/2=127)
  data.pitch = 127;  // Center
  data.roll = 127;   // Center
}

void setup()
{
  Serial.begin(9600);
  //Set the pins for each PWM signal
  ch1.attach(ch1_pin);
  ch2.attach(ch2_pin);
  //Configure the NRF24 module
  ResetData();
  radio.begin();
  radio.openReadingPipe(0 ,pipeIn);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  
  radio.startListening(); //start the radio comunication for receiver

  Serial.print("Radio chip connected? ");
  Serial.println(radio.isChipConnected() ? "Yes" : "No");


  Serial.println("Receiver Ready");
}

unsigned long lastRecvTime = 0;

void recvData()
{
  while ( radio.available() ) {
    radio.read(&data, sizeof(Signal));
    lastRecvTime = millis();   // receive the data

    Serial.print("Roll: ");
    Serial.println(data.roll);
    Serial.print("Pitch: ");
    Serial.println(data.pitch);
  }
}

void loop()
{
  recvData(); 
  
  unsigned long now = millis();
  
  if ( now - lastRecvTime > 1000 ) {
    ResetData(); // Signal lost.. Reset data 
    Serial.println("Signal Lost");
  }
  
  ch_width_1 = map(data.pitch,    0, 255, 1000, 2000);     // pin D3 (PWM signal)
  ch_width_2 = map(data.roll,     0, 255, 1000, 2000);     // pin D4 (PWM signal)
  
  // Write the PWM signal
  ch1.writeMicroseconds(ch_width_1);
  ch2.writeMicroseconds(ch_width_2);

  delay(10);
  
}
