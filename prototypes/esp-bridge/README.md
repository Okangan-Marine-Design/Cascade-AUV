# ESP Bridge

This code is the bridge between the electronics and the Jetson Orin Nano. It passes data 2 ways.

## Features

- Logging ✅ Implemented
- Sensor Interface ✅ Implemented
- Wifi Debug Portal ❌ Not yet implemented
- Serial Communication ❌ Not yet implemented
- Temperature Sensor ⚠️ Half implemented (sends fake data)
- Leak Sensor ⚠️ Half implemented (sends fake data)
- Current Sensor ❌ Not yet implemented
- Hydrophone ❌ Not yet implemented
- Kill Switch ⚠️ Half implemented (sends fake data)
- Motor Control ❌ Not yet implemented
- LED Status ❌ Not yet implemented
- LED Control ❌ Not yet implemented

## General Program Structure

Program Start

1. Initialize Serial Communication

2. Initialize Logging

3. Initialize Sensors
   Sensors are contained in individual files that are imported into the main program. Each sensor is a derived class of the main sensor class. The general structure of the sensor class is as follows:

```c++
// This is the base class for all sensors SwitchSensor
class Sensor
{
public:
    Sensor(int pin, String name) : _pin(pin), name(name) // Constructor
    {
        pinMode(_pin, INPUT); // Set the pin mode to input
    }

    template <typename T>
    T getReading(); // Sensor reading method with a template so we can return different types of data

    String name; // The name of the sensor

protected:
    int _pin; // The pin the sensor is connected to
};
```

We use polymorphism to create a common interface for all sensors. This allows us to easily add more sensors in the future.

4. Build the JSON Object
   The JSON object is built from the sensor data. The JSON object is then sent to the Jetson Orin Nano.
   This means that we can add more sensors easily by adding more data to the JSON object.

5. Send the JSON Object
   The JSON object is sent to the Jetson Orin Nano over serial communication.

6. Receive the JSON Object
   The JSON object is received from the Jetson Orin Nano over serial communication.

7. Parse the JSON Object
   The JSON object is parsed and the data is used to control the motors and LEDs.

8. Heartbeat
   The program checks to see when the last time it received a JSON object from the Jetson Orin Nano. If it has been too long, the program will trigger an emergency mode.

## Naming Conventions

- Class names are capitalized
- Member variables are prefixed with an underscore
- Method names are camel case
- Constants are all caps
- File names are all lowercase (except for README.md)
- For sensor files, the file name is the same as the sensor name without the word "sensor" (e.g. temperature.cpp) or (e.g. leak.cpp)
- For sensor classes, if there is a generic type type of sensor we should try to make the code as generic as possible. For example, the kill switch sensor is a switch sensor, so we create a base class called SwitchSensor then we can instantiate a switch sensor with a pin and a name. This allows us to easily add more sensors in the future. This is much better than creating a new class for each sensor type.

## JSON Structure

For the JSON object this is what the esp bridge receives, the structure is as follows this can change in the future:

```json
{
  "actuators": {
    // These are the main actuators that we can control
    "thrusters": [
      {
        "id": "1",
        "speed": 0
      },
      {
        "id": "2",
        "speed": 0
      },
      {
        "id": "3",
        "speed": 0
      },
      {
        "id": "4",
        "speed": 0
      }
    ],
    "servos": [
      // This is an array of servos not used now but if we add servos in the future we can add them here
      {
        "id": "1",
        "angle": 0
      }
    ],
    "motors": [] // This is an empty array for now if we add motors in the future we can add them here
  }
}
```

it should be sent to the ESP with no spaces or new lines. You can use this for testing:
{"actuators":{"thrusters":[{"id":"1","speed":0},{"id":"2","speed":0},{"id":"3","speed":0},{"id":"4","speed":0}],"servos":[{"id":"1","angle":0}],"motors":[]}}
