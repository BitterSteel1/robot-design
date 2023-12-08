#include <Wire.h>
#include <Adafruit_TCS34725.h>

// Create an Adafruit_TCS34725 instance
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup() {
  Serial.begin(9600);
  
  
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

bool isRedDetected() {
  uint16_t clear, red, green, blue;

  // Read the RGB values from the sensor
  tcs.getRawData(&red, &green, &blue, &clear);

  // Calculate the total color value
  uint32_t sum = red;
  sum += green;
  sum += blue;

  // Calculate the ratio of red to the total color value
  float redRatio = (float)red / sum;

  // Set a threshold for detecting red (adjust as needed)
  float redThreshold = 0.5; // Example threshold, you may adjust this value

  // Check if the detected color is predominantly red
  if (redRatio > redThreshold) {
    return true;
  }

  return false;
}

void loop() {
    uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
  
  if (isRedDetected()) {
    Serial.println("######################################################################## Red color detected!");
    // Perform actions when red is detected
    // You can add additional actions here
  }

  delay(20); // Adjust delay as needed for your application
}
