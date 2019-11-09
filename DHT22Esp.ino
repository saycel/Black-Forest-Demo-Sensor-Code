#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <DHT.h>
#include "DHTesp.h"
#define MQTT_MAX_PACKET_SIZE = 4096



//#define DHTPIN 4
//#define DHTTYPE DHT11

//DHT dht(DHTPIN, DHTTYPE);
DHTesp dht;

const char* ssid = "NETWORK_SSID";                   //Nombre de la RED
const char* password = "NETWORK_PASSWORD";           //Password de la RED
const char* mqtt_server = "157.230.15.139";   //Dirección servidor

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[1000];
int value = 0;

int sensorPin = A0;

  void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
  
void callback(char* topic, byte* payload, unsigned int length) {

  String pay_load = "";
  for( int i = 0; i < length; i++ ){
    pay_load = pay_load + (char)payload[i];
    }  
  Serial.println(pay_load); // HERE IS WHERE THE MENSSAGE FROM MQTT ARRIVE
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      //client.subscribe("forest"); // THIS IS WHERE YOU SETUP TE TOPIC
      
      } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  dht.setup(D2, DHTesp::DHT22);

  if (!client.connected()) {
    reconnect();
  }
  
}

void loop() {
  client.loop();

  long now = millis();
  if (now%5000 == 0) {
    String humidity = String(dht.getHumidity(), 2);//String(dht.readHumidity(), 2);
    String temperature = String(dht.getTemperature(), 2);//String(dht.readTemperature(), 2);

    Serial.print("Temp: " + temperature + "\tHumidity: " + humidity + "\n");
  
    String buf;
    buf += "{\"unit\":\"%\",\"net_key\":\"net\",\"app_key\":\"app\",\"device_id\":\"Esp2\",\"channels\":{ \"humidity\":\"";
    buf += humidity;
    buf += "\"}}";
    
    if (!client.publish("forest",  buf.c_str())){
        client.publish("debug","esp8266 messsage for humidity is too large");
    }
    
   buf += "{\"unit\":\"°C\",\"net_key\":\"net\",\"app_key\":\"app\",\"device_id\":\"Esp2\",\"channels\":{ \"temperature\":\"";
    buf += temperature;
    buf += "\"}}";
    
    
    if (!client.publish("forest",  buf.c_str())){
        client.publish("debug","esp8266 messsage for temperature is too large");
    }

  }
}