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

## Naming Conventions

- Class names are capitalized
- Member variables are prefixed with an underscore
- Method names are camel case
- Constants are all caps
- File names are all lowercase (except for README.md)

## JSON Structure

For the JSON object this is what the esp bridge receives, the structure is as follows this can change in the future
Note that the order that the array is in is important. This must match with the config file.
:

```json
{
  "actuators": {
    "thrusters": [
      {
        "speed": 0,
        "id": 1
      },
      {
        "speed": 0,
        "id": 2
      },
      {
        "speed": 0,
        "id": 3
      },
      {
        "speed": 0,
        "id": 4
      }
    ],
    "servos": [
      {
        "angle": 0,
        "id": 5 // note that the id continues from the last id in the previous array this is important
      }
    ],
    "motors": []
  }
}
```

it should be sent to the ESP with no spaces or new lines. You can use this for testing:
{"actuators":{"thrusters":[{"speed":0,"id":1},{"speed":0,"id":2},{"speed":0,"id":3},{"speed":0,"id":4}],"servos":[{"angle":0,"id":5}],"motors":[]}}
