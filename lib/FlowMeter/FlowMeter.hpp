#ifndef FLOWMETER_H
#define FLOWMETER_H

#include <Arduino.h>

class FlowMeter {
public:
    FlowMeter(uint8_t pin, float calibrationFactor);
    void begin();
    float getFlow(uint16_t sampleTimeMs = 1000); // Measure flow for sampleTimeMs milliseconds

private:
    uint8_t flowPin;
    float factor;
    static FlowMeter* instances[40]; // Support up to 40 GPIOs on ESP32

    // ISR variables
    static void IRAM_ATTR pulseISR();               // Static interrupt handler
    static volatile uint8_t currentDevice;          // Track the current device
    static volatile bool samplesComplete;           // Flag to indicate when samples are complete
    static volatile uint32_t lastTime;              // Last time a pulse was detected
    static volatile uint32_t periodSamples[50];     // Store the time between pulses
    static volatile uint8_t sampleIndex;            // Index for the periodSamples array
};

#endif // FLOWMETER_H
