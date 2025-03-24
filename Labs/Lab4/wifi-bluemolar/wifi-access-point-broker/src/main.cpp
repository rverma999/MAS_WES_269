#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include "TinyMqtt.h"   // MQTT client/broker
#include "TinyConsole.h" // MQTT
#include "TinyStreaming.h" // MQTT

const uint16_t PORT = 1883;
const uint8_t  RETAIN = 10;  // Max retained messages

MqttBroker broker(PORT);
const char* ssid = "Batman";
const char* password = "Basil@222";

void setup()
{
    Serial.begin(115200);


	if (strlen(ssid)==0)
		Console << TinyConsole::red << "****** PLEASE MODIFY ssid/password *************" << endl;

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        Serial << '.';
        delay(500);
    }
    Console << TinyConsole::green << "Connected to " << ssid << "IP address: " << WiFi.localIP() << endl;

    broker.begin();
    Console << "Broker ready : " << WiFi.localIP() << " on port " << PORT << endl;
}

void loop()
{
    broker.loop();
}

