#include <sensor.h>
#include <Arduino.h>

class TemperatureSensor : public Sensor
{
public:
    using Sensor::Sensor; // Constructor
    double getReading();  // Method
};

// set up getReading method
#include <cmath>

double TemperatureSensor::getReading()
{
    // for now, we don't have a temperature sensor, so we'll just return a number based on the sin of the current time
    double reading = abs(10 * sin(millis() / 10000.0)) + 10;
    return reading;
}