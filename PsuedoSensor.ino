/*
	File Name: PsuedoSensor.ino
	Author: Gilberto Romero
	Date: 5/29/19
	Description: 
		This file contains the bare minimum code required to connect
		an ESP8266-compatible device to a Raspberry Pi running the 
		store-forward node API. It also contains skeleton code to allow
		easily connecting sensors to the device and sending the data
		they output to the server. Though, note that said data are the
		raw values and any further processing of them needs to be
		programmed in.
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
const int DIGITAL_SENSORS[] = {D0, D1};
const int ANALOG_SENSORS[] = {A0};

// Define the frequency at which data is sent to the server (in milliseconds).
const int FREQUENCY = 1000;

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
		const int DIGITAL_IN = digitalRead(DIGITAL_SENSORS[i]);
		Serial.printf("Digital[%02d]  Channel[%02d]  State: %d\t", DIGITAL_SENSORS[i], i, DIGITAL_IN);
		send(DIGITAL_IN, i);
	}

	const int CHANNEL_OFFSET = LENGTH_OF(DIGITAL_SENSORS);

	// Read and send the state of all analog sensors.
	for (int i = 0; i < LENGTH_OF(ANALOG_SENSORS); i++)
	{
		const int ANALOG_IN = analogRead(ANALOG_SENSORS[i]);
		const int CHANNEL = CHANNEL_OFFSET + i;
		Serial.printf("Analog [%02d]  Channel[%02d]  State: %d\t", ANALOG_SENSORS[i], CHANNEL, ANALOG_IN);
		send(ANALOG_IN, CHANNEL);
	}

	delay(FREQUENCY);
}