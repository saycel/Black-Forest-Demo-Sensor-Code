
## TemperatureSensor.ino

### About
Building upon HTTP_Publisher.ino, this file extends it to send temperature data to a Raspberry Pi every five seconds.

### Prerequisites
You'll need to have bought a [TMP36](https://www.adafruit.com/product/165) and have hooked it up to the WeMos in the following configuration:
![An image of the WeMos connected to the TMP36](https://i.imgur.com/3b2CkcR.png)

### Running

While in the Arduino IDE, on the top left go to File > Open, then select the TemperatureSensor.ino file. After making the necessary code changes to the file, connect your WeMos board through USB. From there, go to Tools > Port and select whichever one is available. After that, you can upload the code by pressing the right-facing arrow at the top left.
