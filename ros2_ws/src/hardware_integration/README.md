# Hardware Integration Package

## nodes
### serial_intake 
used to get sensor reading data from esp32 serial bridge, publishes to /rawJson
### json_parser
- used to parse json data received from serial_intake, subscribed to /rawJson
- takes in rawJson string, then goes through every element and recursively publishes every sensor reading in the json string

![RQT graph](rqt_graph.png)
