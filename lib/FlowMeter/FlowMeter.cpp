#include "FlowMeter.hpp"

FlowMeter* FlowMeter::instances[40] = {nullptr}; // Store instance pointers
volatile uint8_t FlowMeter::currentDevice = 0; // Track the current device
volatile bool FlowMeter::samplesComplete = false; // Flag to indicate when samples are complete
volatile uint32_t FlowMeter::lastTime = 0; // Last time a pulse was detected
volatile uint32_t FlowMeter::periodSamples[50] = {0}; // Store the time between pulses
volatile uint8_t FlowMeter::sampleIndex = 0; // Index for the periodSamples array

/**
 * @brief Construct a new Flow Meter object
 * @param pin GPIO pin connected to the flow meter
 * @param K Calibration factor for the flow meter
**/
FlowMeter::FlowMeter(uint8_t pin, float K) {
    flowPin = pin;
    factor = K;
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
 * @brief Global pulse detecting ISR
**/
void IRAM_ATTR FlowMeter::pulseISR() {
    if (!samplesComplete) {
        uint32_t currentTime = millis();
        periodSamples[sampleIndex] = currentTime - lastTime;
        lastTime = currentTime;
        sampleIndex++;
        if (sampleIndex >= 50) {
            samplesComplete = true;
            detachInterrupt(digitalPinToInterrupt(currentDevice));
        }
    }
}



/**
 * @brief Measure the flow rate
 * @param sampleTimeMs Time to measure flow in milliseconds
 * @return Flow rate in GPM
**/
float FlowMeter::getFlow(uint16_t sampleTimeMs) {

    // Attach the global ISR, which maps to the correct instance
    samplesComplete = false;
    sampleIndex = 0;
    lastTime = millis();
    currentDevice = flowPin;

    attachInterrupt(digitalPinToInterrupt(flowPin), pulseISR, RISING);

    while (!samplesComplete) {}

    uint64_t periodSum = 0;
    for (uint8_t i = 0; i < 50; i++) {
        periodSum += periodSamples[i];
    }

    float periodAvg = periodSum / 50.0;
    Serial.println(periodAvg);

    float flowRate = 1 / factor * 60 * periodAvg; // Convert to GPM

    return flowRate;
}
