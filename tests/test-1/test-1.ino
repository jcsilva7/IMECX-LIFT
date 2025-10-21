// The purpose of this test is to verify the readings from the BNO085 module

#include <Servo.h>
#include <Wire.h>
#include <SparkFun_BNO08x.h>

Servo servo;
BNO09X myIMU;
int servo_pin = 9;

void setup() {
  servo.attach(servo_pin);
  
  Serial.begin(115200);

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
    float roll  = myIMU.getRoll();
    float pitch = myIMU.getPitch();
    float yaw   = myIMU.getYaw();

    Serial.print("Roll: "); Serial.print(roll);
    Serial.print(" Pitch: "); Serial.print(pitch);
    Serial.print(" Yaw: "); Serial.println(yaw);
  }

  // Small delay to stabilize readings
  delay(50);

}
