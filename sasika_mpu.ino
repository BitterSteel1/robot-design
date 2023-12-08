#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcGyroOffsets();
  Serial.println("Done!\n");
}

void loop() {
  mpu.update();

  Serial.print("Angle - Z: ");
  Serial.println(int(mpu.getAngleZ()));
  
  delay(10);

  // Move the conditional statements inside the loop function
  if (mpu.getAngleZ() > 15) {
    Serial.println("############## Robot is ascending");
    // Perform actions for robot ascending
  } else if (mpu.getAngleZ() < -15) {
    Serial.println("############## Robot is descending");
    // Perform actions for robot descending
  }
}
