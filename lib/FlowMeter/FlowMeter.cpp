#include "FlowMeter.hpp"

FlowMeter* FlowMeter::instances[40] = {nullptr}; // Store instance pointers
volatile uint8_t FlowMeter::currentDevice = 0; // Track the current device

FlowMeter::FlowMeter(uint8_t pin, float K) {
    flowPin = pin;
    factor = K;
    pulseCount = 0;
}

void FlowMeter::begin() {
    pinMode(flowPin, INPUT);
    instances[flowPin] = this; // Register instance
}

void IRAM_ATTR FlowMeter::globalISR0() {
    Serial.println("Global ISR triggered");
    instances[currentDevice]->pulseISR(); // Forward to instance
}

void IRAM_ATTR FlowMeter::pulseISR() {
    Serial.println("Instance ISR triggered");
    pulseCount++;
}

float FlowMeter::getFlow(uint16_t sampleTimeMs) {
    pulseCount = 0; // Reset pulse count

    // Attach the global ISR, which maps to the correct instance
    currentDevice = flowPin;
    attachInterrupt(digitalPinToInterrupt(flowPin), globalISR0, RISING);

    delayMicroseconds(sampleTimeMs); // Wait for the sampling period

    detachInterrupt(digitalPinToInterrupt(flowPin)); // Stop counting pulses

    float frequency = (pulseCount / (sampleTimeMs / 1000.0)); // Pulses per second
    float flowRate = frequency / factor * 60; // Convert to GPM

    return flowRate;
}
