// The purpose of this test is to try and control a servo based on the BNO085 module roll

#include <Servo.h>
#include <Wire.h>
#include <SparkFun_BNO08x.h>

Servo servo;
int servo_pin = 9;

BNO09X myIMU;

void setup() {
  servo.attach(servo_pin);
  
  Serial.begin(115200);

  // For testing I²C is fine 
  Wire.begin();

  if (!myIMU.begin()){
    Serial.println("BNO085 not detected. Connect or end.");
    while (1);
  }

  // Enable output in Euler angles
  myIMU.enableRotationVector();
  myIMU.enableEulerAngles();
  
  Serial.println("BNO085 Initialized!");
  
}

void loop() {
  if(myIMU.dataAvailable()) {
    float roll = myIMU.getRoll();

    Serial.print("Roll: ");
    Serial.println(roll, 2);

    // Write a mapped value (and also constrained to avoid mechanical issues) into the servo
    servo.write(constrain(map(roll, -90, 90, 0, 180), 0, 180));
  }

  // Small delay to stabilize readings
  delay(50);


}
