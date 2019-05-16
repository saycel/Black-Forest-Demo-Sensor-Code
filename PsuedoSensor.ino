#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// replace for the SSID  the RPI is connected
const char* ssid = "Towers";
const char* password = "12345678900";
String url = "http://192.168.100.254:2323/sensor/collector/123/456/esp8266/?ch";
int channel = 1;

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 5; // D1     // the number of the pushbutton pin
const int ledPin =  4; // D2      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status


void setup()
{ 
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  
   Serial.begin(115200);
   delay(10);
 
   // Conectar WiFi
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) 
      delay(500);
}

void loop()
{
   HTTPClient http;
   WiFiClient client;

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  char state;
  
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    state = '1';
    // turn LED on:
    //digitalWrite(ledPin, HIGH);
  } else {
    state = '0';
    // turn LED off:
    //digitalWrite(ledPin, LOW);
  }
  
   if (http.begin(client, url + channel + '=' + state))
   {
      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET(); 
 
      if (httpCode > 0) {
         Serial.printf("[HTTP] GET... code: %d\n", httpCode);

         /*
         if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            Serial.println(payload);
         }
         */
      }
      else {
         Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
 
      http.end();
      //chnl++;
   }
   else {
      Serial.printf("[HTTP} Unable to connect\n");
      //chnl++;
   }
   channel++;
 
   delay(1000);
}