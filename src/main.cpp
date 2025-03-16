#include <Arduino.h>
#include "FlowMeter.hpp"

#define FLOW_SENSOR_1_PIN 17
#define FLOW_SENSOR_2_PIN 16
#define CALIBRATION_FACTOR 1684 // Adjust for your flow sensor

FlowMeter flow1(FLOW_SENSOR_1_PIN, CALIBRATION_FACTOR);
FlowMeter flow2(FLOW_SENSOR_2_PIN, CALIBRATION_FACTOR);

void setup() {
    Serial.begin(115200);
    flow1.begin();
    flow2.begin();
}

void loop() {
    float flowRate1 = flow1.getFlow(500); // Measure for 1 second
    float flowRate2 = 0;//flow2.getFlow(500); // Measure for 1 second

    Serial.print("Flow 1 Rate: ");
    Serial.print(flowRate1);
    Serial.println(" GPM");

    //Serial.print("Flow 2 Rate: ");
    //Serial.print(flowRate2);
    //Serial.println(" GPM");

    delay(500); // Wait before next measurement
}
