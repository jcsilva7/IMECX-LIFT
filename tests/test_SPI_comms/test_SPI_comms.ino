#include <SPI.h>

void setup() {
  Serial.begin(9600);
  SPI.begin();

  pinMode(8, OUTPUT);  // CSN
  digitalWrite(8, HIGH);

  delay(100);

  digitalWrite(8, LOW);
  byte response = SPI.transfer(0xFF);  // send dummy
  digitalWrite(8, HIGH);

  Serial.print("SPI response: 0x");
  Serial.println(response, HEX);
}

void loop() {}

// 0x0E or 0x0F good
// 0x00 or 0xFF bad, wiring mistake or dead module