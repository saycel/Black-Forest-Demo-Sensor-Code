
## HTTP_Publisher.ino

### About
This Arduino file connects to a local area network wirelessly then pushes data to the Raspberry Pi via the HTTP API that it exposes. Said data are raw sensor values that it picks up from the digital and analog pins defined in the code.

### Dependencies
[ESP8266 Wifi Library](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)

### Running
While in the Arduino IDE, on the top left go to File > Open, then select the HTTP_Publisher.ino file. After making the necessary code changes to the file, connect your WeMos board through USB. From there, go to Tools > Port and select whichever one is available. After that, you can upload the code by pressing the right-facing arrow at the top left.

## MQTT_Publisher.ino

### About
This Arduino file was written by [Germán Martinez](https://gitlab.com/german.martinez). It connects to a network and pushes random data to the Raspberry Pi. It does so by making use of the MQTT protocol that the store-forward API supports. Compared to HTTP, it's better to use as it's designed to be more lightweight and reliable.

### Dependencies
[ESP8266 Wifi Library](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)
[Arduino Client for MQTT](https://github.com/knolleary/pubsubclient)

### Running
While in the Arduino IDE, on the top left go to File > Open, then select the MQTT_Publisher.ino file. After making the necessary code changes to the file, connect your WeMos board through USB. From there, go to Tools > Port and select whichever one is available. After that, you can upload the code by pressing the right-facing arrow at the top left.
