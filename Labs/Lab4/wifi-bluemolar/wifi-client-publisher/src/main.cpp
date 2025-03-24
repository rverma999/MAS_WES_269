#include <Arduino.h>
#include <WiFi.h>
#include "TinyMqtt.h"   // MQTT client/broker
#include "TinyConsole.h" // MQTT
#include "TinyStreaming.h" // MQTT

const uint16_t PORT = 1883;
const std::string IP_STR = "192.168.1.67"; // IP of our broker ESP32

const char* ssid = "Batman";
const char* password = "Basil@222";

//std::string topic="numbers/primes";

std::string publish_topic = "numbers/primes";
std::string subscribe_topic = "numbers/primes"; // Subscribe to the same topic

//MqttBroker broker(1883);
//MqttClient mqtt_a(nullptr);
//MqttClient mqtt_b(nullptr);
MqttClient mqtt_client(nullptr);


//void onPublishA(const MqttClient* /* srce */, const Topic& topic, const char* payload, size_t /* length */)
//{ Serial << "--> Client A received msg on topic " << topic.c_str() << ", " << payload << endl; }

//void onPublishB(const MqttClient* /* srce */, const Topic& topic, const char* payload, size_t /* length */)
//{ Serial << "--> Client B received msg on topic " << topic.c_str() << ", " << payload << endl; }


// Callback when a message is received
void onPublish(const MqttClient* /* srce */, const Topic& topic, const char* payload, size_t /* length */)
{ 
  Serial << "--> Received msg on topic " << topic.c_str() << ", " << payload << endl; 
}

void setup()
{
    Serial.begin(115200);

    // Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {   
        Serial << '-'; 
        delay(500); 
    }

    Serial << "Connected to " << ssid << endl << "IP address: " << WiFi.localIP() << endl;

    // Connect to the broker
    Serial.printf("Connecting to broker at %s:%d...\n", IP_STR.c_str(), PORT);
    mqtt_client.connect(IP_STR, PORT);
    
    while(!mqtt_client.connected()) {
        delay(100);
        Serial.print(".");
    }
    Serial.println("Connected to broker!");

    // Set up callback and subscribe
    mqtt_client.setCallback(onPublish);
    mqtt_client.subscribe(subscribe_topic);
    
    Serial.println("Setup complete - now acting as both publisher and subscriber");
}

void loop() {
    mqtt_client.loop();

    // Publish periodically
    static const int interval = 5000; // 5 seconds between messages
    static uint32_t timer = millis() + interval;

    if (millis() > timer) {
        String message = "Value: " + String(millis());
        Serial << "Publishing to " << publish_topic.c_str() << ": " << message.c_str() << endl;
        timer += interval;
        mqtt_client.publish(publish_topic, message.c_str());
    }
}
