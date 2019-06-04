#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR_SSID";                   //Nombre de la RED
const char* password = "YOUR_PASSWORD";           //Password de la RED
const char* mqtt_server = "MQTT_SERVER_IP";   //Dirección servidor

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
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
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("forest"); // THIS IS WHERE YOU SETUP TE TOPIC
      
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

  if (!client.connected()) {
    reconnect();
  }
  
}

void loop() {
  client.loop();
int random_value = random(219,220);  // here is where you put the mensure from the sensor

  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    ++value;

    snprintf (msg, 50, "%f", random_value);
    client.publish("forest", msg);

  }
}
