/*
	File Name: TemperatureSensor.ino
	Author: Gilberto Romero
	Date: 6/4/19
	Description: 
		This file makes use of HTTP_Publisher.ino and extends it to gather
		data from a TMP36 (analog temperature sensor).
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define LENGTH_OF(x) (sizeof(x) / sizeof(x[0]))

// Replace these constants with applicable values.
const String SSID = "NETWORK_NAME";
const String PASSWORD = "NETWORK_PASSWORD";
const String SERVER = "SERVER_IP";
const String PORT = "SERVER_PORT";

const String APP_KEY = "APPLICATION_KEY";
const String NET_ID = "NETWORK_ID";
const String DEV_ID = "DEVICE_ID";

// Define your inputs for sensor data.
const int DIGITAL_SENSOR_0 = D0;
const int TEMPERATURE_SENSOR = A0;

// Put inputs into a list for easy traversal.
const int DIGITAL_SENSORS[] = {};
const int ANALOG_SENSORS[] = {TEMPERATURE_SENSOR};

// Define the frequency at which data is sent to the server (in milliseconds).
const int PUSH_FREQUENCY = 5000;

// Base URL used for all calls to the API method.
const String URL = "http://" + SERVER + ":" + PORT + "/sensor/collector/" + APP_KEY + "/" + NET_ID + "/" + DEV_ID + "/?ch";

void setup()
{
	// Initialize all digital sensors as inputs.
	for (int i = 0; i < LENGTH_OF(DIGITAL_SENSORS); i++) 
	{
		pinMode(DIGITAL_SENSORS[i], INPUT);
	}

	// Initialize serial monitor.
	Serial.begin(9600);

	// Attempt to connect to the network.
	WiFi.begin(SSID, PASSWORD);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
	}
}

void send(int data, int channel)
{
	HTTPClient http;
	WiFiClient client;

	if (http.begin(client, URL + channel + '=' + data))
	{
		int httpCode = http.GET(); 

		if (httpCode > 0) 
		{
			Serial.printf("OK: %d\n", httpCode);
		}
		else 
		{
			Serial.printf("FAILED: %s\n", http.errorToString(httpCode).c_str());
		}

		http.end();
	}
	else {
		Serial.print("FAILED: no connection\n");
	}
}

void loop()
{
	// Read and send the state of all digital sensors.
	for (int i = 0; i < LENGTH_OF(DIGITAL_SENSORS); i++)
	{	
		const int CHANNEL = i;
		const int SENSOR_ID = DIGITAL_SENSORS[i];

		int sensorValue = digitalRead(SENSOR_ID);

		Serial.printf("Digital[%02d]  Channel[%02d]  State: %d\t", SENSOR_ID, CHANNEL, sensorValue);
		
		if (SENSOR_ID == DIGITAL_SENSOR_0)
		{
			// Code to modify sensor data before sending goes here.
			sensorValue *= 2;
			send(sensorValue, CHANNEL);
		}
		else
		{
			// Send the raw value that the sensor gives.
			send(sensorValue, CHANNEL);
		}
	}

	const int CHANNEL_OFFSET = LENGTH_OF(DIGITAL_SENSORS);

	// Read and send the state of all analog sensors.
	for (int i = 0; i < LENGTH_OF(ANALOG_SENSORS); i++)
	{
		const int CHANNEL = CHANNEL_OFFSET + i;
		const int SENSOR_ID = ANALOG_SENSORS[i];

		int sensorValue = analogRead(SENSOR_ID);

		Serial.printf("Analog [%02d]  Channel[%02d]  State: %d\t", SENSOR_ID, CHANNEL, sensorValue);

		if (SENSOR_ID == TEMPERATURE_SENSOR)
		{
			const double VOLTAGE = sensorValue / 1023.0;
			const double TEMP_C = VOLTAGE * 100;
			const double TEMP_F = (TEMP_C * 1.8) + 32;
			send(TEMP_F, CHANNEL);
		}
		else
		{
			// Send the raw value that the sensor gives.
			send(sensorValue, CHANNEL);
		}
	}
}