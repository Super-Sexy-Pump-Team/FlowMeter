#include <Arduino.h>

#define FLOW_SENSOR_PIN 42  // Use a safe GPIO pin

volatile int pulseCount = 0;

void IRAM_ATTR pulseISR() {
    pulseCount++;
}

void setup() {
    Serial.begin(115200);
    pinMode(FLOW_SENSOR_PIN, INPUT); // Ensure correct mode
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseISR, RISING); 
}

void loop() {
    Serial.print("Pulse Count: ");
    Serial.println(pulseCount);
    delay(1000); // Print count every second
}
