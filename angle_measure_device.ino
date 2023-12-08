#include <Wire.h>    //Include wire library 
#include <MPU6050_light.h>  //Include library for MPU communication
// #include <LiquidCrystal_I2C.h>  //Library for LCD Display

MPU6050 mpu(Wire);   //Create object mpu
// LiquidCrystal_I2C lcd(0x20, 16, 2);     //Define LCD address and dimension

unsigned long timer = 0;    

void setup() {

  Serial.begin(9600);    //Start serial communication

  // lcd.begin();     //Start LCD Display
  // lcd.backlight();    

  Wire.begin();     
  mpu.begin();     
  Serial.print(F("MPU6050 status: "));
  Serial.println(F("Calculating offsets, do not move MPU6050"));   
  delay(1000);
  mpu.calcGyroOffsets();     //Calibrate gyroscope
  Serial.println("Done!\n");

}
void loop() {
  mpu.update();    //Get values from MPU

  if ((millis() - timer) > 100) { // print data every 100ms
    timer = millis();
    Serial.print(" Angle: ");
    Serial.print(" X : ");Serial.print(mpu.getAngleX());
    Serial.print(" Y :"); Serial.print(mpu.getAngleY());
    Serial.print(" Z :"); Serial.println(mpu.getAngleZ());

    Serial.print("::::::: Angle ACC: ");
    Serial.print(" X : ");Serial.print(mpu.getAccAngleX());
    Serial.print(" Y :"); Serial.print(mpu.getAccAngleY());
    // Serial.println(" Z :"); Serial.println(mpu.get());
    
        //Print Z angle value on LCD 
    delay(10);
  }
}
