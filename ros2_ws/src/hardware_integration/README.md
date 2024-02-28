# Hardware Integration Package

## nodes
---
### serial_intake 
used to get sensor reading data from esp32 serial bridge, publishes to /rawJson
### json_parser
used to parse json data received from serial_intake, subscribed to /rawJson
