#ifndef FLOWMETER_H
#define FLOWMETER_H

#include <Arduino.h>

class FlowMeter {
public:
    FlowMeter(uint8_t pin, float calibrationFactor);
    void begin();
    float getFlow(uint16_t sampleTimeMs = 1000); // Measure flow for sampleTimeMs milliseconds

private:
    static void IRAM_ATTR globalISR0(); // Static interrupt handler
    void IRAM_ATTR pulseISR();          // Instance-specific ISR
    volatile uint32_t pulseCount;
    static volatile uint8_t currentDevice;
    uint8_t flowPin;
    float factor;
    static FlowMeter* instances[40]; // Support up to 40 GPIOs on ESP32
};

#endif // FLOWMETER_H
