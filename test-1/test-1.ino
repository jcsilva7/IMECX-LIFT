  #include <Servo.h>
#include <Wire.h>
#include <SparkFun_BNO08x.h>

Servo testes;
int testes_pin = 9;

void setup() {
  testes.attach(testes_pin)
  Serial.begin(115200);

}

void loop() {
  for (int i = 0; i < 360; i++){
    if(i < 180) testes.write(i);
    else testes.write(180-i);
    delay(50);
  }

}
