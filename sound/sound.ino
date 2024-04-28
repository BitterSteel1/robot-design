#include "arduinoFFT.h" // Ensure this library is available

arduinoFFT FFT;

// Define constants
#define CHANNEL A0
const uint16_t samples = 64;
const double samplingFrequency = 2000;

unsigned int sampling_period_us;
float heighestf = 0; // Initialize to avoid potential issues
unsigned long microseconds;

double vReal[samples];
double vImag[samples];

void setup() {
  pinMode(13, OUTPUT);
  sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Ready");
}

void checkfft() {
  microseconds = micros();
  for (int i = 0; i < samples; i++) {
    vReal[i] = analogRead(CHANNEL);
    vImag[i] = 0;
    while (micros() - microseconds < sampling_period_us) {
      // Empty loop
    }
    microseconds += sampling_period_us;
  }
  
  FFT = arduinoFFT(vReal, vImag, samples, samplingFrequency);
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();
  heighestf = FFT.MajorPeak();
  Serial.println(heighestf, 6);
  delay(100);
}

void loop() {
  checkfft();
  if (heighestf > 950 && heighestf < 1050) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}
