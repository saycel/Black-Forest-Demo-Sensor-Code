
# Black Forest Demo Sensor Code

This repository contains various code and scripts to help test and debug our forest network and store-forward API. Most, if not all the code here is barebones and require further additions to have it send data that's more significant than just random or incremented numbers. However, they should work as-is if the intent is just to test our existing solutions.



## PsuedoSensor.ino

### About

PsuedoSensor.ino is a C++ file that's meant to run on a ESP8266. All it does is connect to a network then push data to a Raspberry Pi via the HTTP API that it exposes. Said data are raw sensor values that it picks up from the digital and analog pins defined in the code.

### Prerequisites
[Arduino IDE](https://www.arduino.cc/en/Main/Software)
[ESP8266 Wifi Library](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)

### Running

Load PsuedoSensor.ino into the Arduino IDE after installing the ESP8266 Wifi Library. After making the necessary code changes to the file, connect your ESP8266 and upload the code.


## SensorEmulatorSkeleton.py

### About

SensorEmulatorSkeleton.py is a python file written by Jeremy that also connects to a network and pushes random data to the Raspberry Pi. However, it has a one-line edit by me to get it working with the HTTP API that the Raspberry Pi has.

### Prerequisites

[Python](https://www.python.org/downloads/) >= 3.0

### Running

Run the file from a terminal after making the necessary changes to it.



## firmware.ino

### About

firmware.ino is a C++ file written by Germán that's also meant to run on an ESP8266. It connects to a network and pushes random data to the Raspberry Pi. However, it does so using MQTT as opposed to HTTP, the former designed to be more lightweight and reliable than the latter.

### Prerequisites

[Arduino IDE](https://www.arduino.cc/en/Main/Software)
[ESP8266 Wifi Library](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)
[Arduino Client for MQTT](https://github.com/knolleary/pubsubclient)

### Running

After installing the ESP8266 Wifi Library and Arduino Client for MQTT, load firmware.ino into the Arduino IDE and make changes as necessary. From there, plug in an ESP8266 and upload the code.
