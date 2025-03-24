#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>


#define SSID_NAME "MyAccessPoint"     // WiFi SSID
#define PASSWORD "12345678"      // WiFi Password (Minimum 8 characters)
#define CHANNEL 1                // WiFi Channel
#define MAX_CONNECTIONS 4        // Max clients allowed

void setup() {
  Serial.begin(115200);
  Serial.println("Starting ESP32 Access Point...");

  // Set up WiFi as an Access Point
  WiFi.softAP(SSID_NAME, PASSWORD, CHANNEL, 0, MAX_CONNECTIONS);

  // Display Access Point details
  Serial.println("WiFi AP Started!");
  Serial.print("SSID: ");
  Serial.println(SSID_NAME);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}
void loop() {
  Serial.print("Connected Clients: ");
  Serial.println(WiFi.softAPgetStationNum()); // Print number of connected clients
  delay(5000);
}
