// The purpose of this test is to verify the readings from the BNO085 module

#include <Wire.h>
#define BNO08X_ADDR 0x4A  // Default I2C address for BNO085
#define CMD_REPORT_ROTATION_VECTOR 0x05  // Report ID for rotation vector

int servo_pin = 9;

// Quaternion data structure
struct Quaternion {
  float w;
  float x;
  float y;
  float z;
};

// Function to read rotation vector (quaternion) from BNO085
bool readRotationVector(Quaternion &q) {
  Wire.beginTransmission(BNO08X_ADDR);
  if (Wire.endTransmission() != 0) return false;  // Check connection

  Wire.write(0xFA); // SYS-EX read command
  Wire.endTransmission(false);

  Wire.requestFrom(BNO08X_ADDR, 20); // request 20 bytes

  if (Wire.available() < 20) return false;

  int16_t qw = Wire.read() | (Wire.read() << 8);
  int16_t qx = Wire.read() | (Wire.read() << 8);
  int16_t qy = Wire.read() | (Wire.read() << 8);
  int16_t qz = Wire.read() | (Wire.read() << 8);
  int16_t acc = Wire.read() | (Wire.read() << 8); // ignore acceleration data
  for (int i = 0; i < 8; i++) Wire.read();       // skip remaining bytes

  // Convert from int16_t to float (-1..1)
  q.w = qw / 16384.0;
  q.x = qx / 16384.0;
  q.y = qy / 16384.0;
  q.z = qz / 16384.0;

  return true;
}

// Convert quaternion to Euler angles (degrees)
void quaternionToEuler(const Quaternion &q, float &roll, float &pitch, float &yaw) {
  roll  = atan2(2*(q.w*q.x + q.y*q.z), 1 - 2*(q.x*q.x + q.y*q.y)) * 57.2958;
  pitch = asin(2*(q.w*q.y - q.z*q.x)) * 57.2958;
  yaw   = atan2(2*(q.w*q.z + q.x*q.y), 1 - 2*(q.y*q.y + q.z*q.z)) * 57.2958;
}

// Check if BNO085 is connected
bool isBNOConnected() {
  Wire.beginTransmission(BNO08X_ADDR);
  return (Wire.endTransmission() == 0);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(100);

  if (isBNOConnected()) {
    Serial.println("BNO085 detected!");
  } else {
    Serial.println("BNO085 NOT detected!");
  }
}

void loop() {
  Quaternion q;
  float roll, pitch, yaw;

  if (isBNOConnected()) {
    if (readRotationVector(q)) {
      quaternionToEuler(q, roll, pitch, yaw);

      Serial.print("Roll: "); Serial.print(roll);
      Serial.print(" Pitch: "); Serial.print(pitch);
      Serial.print(" Yaw: "); Serial.println(yaw);
    }
  } else {
    Serial.println("BNO085 disconnected!");
  }

  delay(500); // slower loop for status readability
}
