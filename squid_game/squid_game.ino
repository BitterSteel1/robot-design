#include "Filter.h"

// Define necessary parameters
#define MIC_PIN A0
#define NOISE 780
#define TOP 255
#define LED_PIN 13
#define NUM_SAMPLES 40

// Variables for audio levels
int maxLevel = 0;
int averageLevel = 0;

// Filter instantiation for smoothing the raw audio signal
ExponentialFilter<long> ADCFilter(5, 0);

void setup() {
    Serial.begin(9600);
    
    // Calculate max and average levels
    for (int i = 0; i < NUM_SAMPLES; i++) {
        int temp = getSoundLevel();
        averageLevel += temp;
        if (temp > maxLevel) {
            maxLevel = temp;
        }
        delay(100);
    }
    
    averageLevel /= NUM_SAMPLES;
    maxLevel += averageLevel + 5;
    
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    delay(2000);
    digitalWrite(LED_PIN, LOW);
}

void loop() {
    int result = getSoundLevel();
    Serial.println(result);
    
    if (result >= maxLevel) {
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }
    // Other loop logic
}

int getSoundLevel() {
    int n, height;
    n = analogRead(MIC_PIN);
    n = abs(1023 - n);
    n = (n <= NOISE) ? 0 : abs(n - NOISE);
    ADCFilter.Filter(n);
    int lvl = ADCFilter.Current();
    
    height = TOP * (lvl - 0) / (long)(50);
    height = constrain(height, 0, TOP);
    int finalHeight = height - maxLevel;
    
    return finalHeight;
}
