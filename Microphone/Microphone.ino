#include "Line_Following_Tested.h"

const int sampleWindow = 10;  // Sample window width in mS (50 mS = 20Hz)
int const AMP_PIN = A7;       // Preamp output pin connected to A0
unsigned int sample;

void setup()
{
  initializeMotors();
  initializeIRSensors();
  Serial.begin(9600);
}

void loop()
{ 
  Serial.print(sample);
  Serial.print("  ");
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS and then plot data
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(AMP_PIN);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  Serial.println(peakToPeak);
  delay(500);
  //double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
  //Serial.println(volts);

  if (peakToPeak>250){
    Rotate_Wheels(0,0);
  }

  else{
    pid_line_following();
  }
}

