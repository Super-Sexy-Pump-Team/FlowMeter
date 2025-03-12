#include "FlowMeter.hpp"

FlowMeter* FlowMeter::instances[40] = {nullptr}; // Store instance pointers
volatile uint8_t FlowMeter::currentDevice = 0; // Track the current device

/**
 * @brief Construct a new Flow Meter object
 * @param pin GPIO pin connected to the flow meter
 * @param K Calibration factor for the flow meter
**/
FlowMeter::FlowMeter(uint8_t pin, float K) {
    flowPin = pin;
    factor = K;
    pulseCount = 0;
}

/**
 * @brief Initialize the flow meter
 * @note Call this function in the setup() function
**/
void FlowMeter::begin() {
    pinMode(flowPin, INPUT);
    instances[flowPin] = this; // Register instance
}

/**
 * @brief Global ISR to forward to instance-specific ISR
**/
void IRAM_ATTR FlowMeter::globalISR0() {
    Serial.println("Global ISR triggered");
    instances[currentDevice]->pulseISR(); // Forward to instance
}

/**
 * @brief Instance-specific ISR to count pulses
 * @note This function is called by the global ISR
**/
void IRAM_ATTR FlowMeter::pulseISR() {
    Serial.println("Instance ISR triggered");
    pulseCount++;
}

/**
 * @brief Measure the flow rate
 * @param sampleTimeMs Time to measure flow in milliseconds
 * @return Flow rate in GPM
**/
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
