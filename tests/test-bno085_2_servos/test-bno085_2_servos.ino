// This test is to check 2 servo control with roll and pitch of the BNO085 module

#include <Servo.h>
#include <Wire.h>
#include <SparkFun_BNO08x.h>

Servo servo_roll;
int servo_roll_pin = 9;
Servo servo_pitch;
int servo_pitch_pin = 10;

BNO09X myIMU;

void setup() {
  servo_roll.attach(servo_roll_pin);
  servo_pitch.attach(servo_pitch_pin);
  
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
    float pitch = myIMU.getPitch();

    Serial.print("Roll: ");
    Serial.println(roll, 2);
    Serial.print("Pitch: ");
    Serial.println(pitch, 2);

    // Write a mapped value (and also constrained to avoid mechanical issues) into the servo
    servo_roll.write(constrain(map(roll, -90, 90, 0, 180), 0, 180));
    servo_pitch.write(constrain(map(pitch, -90, 90, 0, 180), 0, 180));
  }

  // Small delay to stabilize readings
  delay(50);

}
