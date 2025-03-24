#include <Arduino.h>
#include <WiFi.h>
#include "TinyMqtt.h"   // MQTT client/broker
#include "TinyConsole.h" // MQTT
#include "TinyStreaming.h" // MQTT


const uint16_t PORT = 1883;
const std::string IP_STR = "192.168.8.107";

const char* ssid = "tock_tutorial";
const char* password = "";

std::string topic="numbers/primes";

//MqttBroker broker(1883);
MqttClient mqtt_a(nullptr);
MqttClient mqtt_b(nullptr);

void onPublishA(const MqttClient* /* srce */, const Topic& topic, const char* payload, size_t /* length */)
{ Serial << "--> Client A received msg on topic " << topic.c_str() << ", " << payload << endl; }

void onPublishB(const MqttClient* /* srce */, const Topic& topic, const char* payload, size_t /* length */)
{ Serial << "--> Client B received msg on topic " << topic.c_str() << ", " << payload << endl; }


void setup()
{
    Serial.begin(115200);

    //== TODO: connect to wifi, set up broker?
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {   Serial << '-'; delay(500); }

    Serial << "Connected to " << ssid << endl << "IP address: " << WiFi.localIP() << endl;



    Serial.printf("connecting to %s : %d...\n", IP_STR.c_str(), PORT);
    mqtt_a.connect(IP_STR, PORT);
    while(!mqtt_a.connected()) {
        delay(100);
        Serial.printf(".");
    }
    Serial.println("Connected!");

    //Serial.printf("connecting b to %s : %d...\n", IP_STR.c_str(), PORT);
    //mqtt_b.connect(IP_STR, PORT);
    //while(!mqtt_b.connected()) {
    //    delay(100);
    //    Serial.printf(".");
    //}
    //Serial.println("Connected!");

    mqtt_a.setCallback(onPublishA);
    //mqtt_a.subscribe(topic);

    //mqtt_b.setCallback(onPublishB);
    //mqtt_b.subscribe(topic);
}

void loop() {
    //broker.loop();	// Don't forget to call loop() for all brokers and clients
    mqtt_a.loop();
    //mqtt_b.loop();

    // ============= client A publish ================
    static const int intervalA = 5000;
    static uint32_t timerA = millis() + intervalA;


    static int i = 5;

    if (millis() > timerA) {
        Serial << "A is publishing " << topic.c_str() << endl;
        timerA += intervalA;
        char buff[32];
        snprintf(buff, 32, "your number is %d", i);
        i +=5;
        mqtt_a.publish(topic, buff);
    }

    // ============= client B publish ================
    // static const int intervalB = 3000; // will send topic each 5000 ms
    // static uint32_t timerB = millis() + intervalB;

    // if (millis() > timerB) {
    //     Serial << "B is publishing " << topic.c_str() << endl;
    //     timerB += intervalB;
    //     mqtt_b.publish(topic, "sent by B");
    // }
}
