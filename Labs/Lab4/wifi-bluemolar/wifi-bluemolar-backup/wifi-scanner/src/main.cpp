#include <Arduino.h> // Required for all code
#include <WiFi.h> // Required for all wifi code

void setup()
{
    Serial.begin(115200);

    Serial.println("Setting up WiFi");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    Serial.println("Setup done");
}

void loop()
{
  Serial.println("Scanning");
  int num_networks = WiFi.scanNetworks();
  Serial.println("Scanned");
  
  if (num_networks == 0){
    Serial.println("no networks found :(");
  } else {
    Serial.print(num_networks);
    Serial.println(" networks found");
    Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
    for (int i = 0; i < num_networks; ++i) {
      // Print SSID and RSSI for each network found
      Serial.printf("%2d", i + 1);
      Serial.print(" | ");
      Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
      Serial.print(" | ");
      Serial.printf("%4ld", WiFi.RSSI(i));
      Serial.print(" | ");
      Serial.printf("%2ld", WiFi.channel(i));
      Serial.print(" | ");
      switch (WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN:            Serial.print("open"); break;
        case WIFI_AUTH_WEP:             Serial.print("WEP"); break;
        case WIFI_AUTH_WPA_PSK:         Serial.print("WPA"); break;
        case WIFI_AUTH_WPA2_PSK:        Serial.print("WPA2"); break;
        case WIFI_AUTH_WPA_WPA2_PSK:    Serial.print("WPA+WPA2"); break;
        case WIFI_AUTH_WPA2_ENTERPRISE: Serial.print("WPA2-EAP"); break;
        case WIFI_AUTH_WPA3_PSK:        Serial.print("WPA3"); break;
        case WIFI_AUTH_WPA2_WPA3_PSK:   Serial.print("WPA2+WPA3"); break;
        case WIFI_AUTH_WAPI_PSK:        Serial.print("WAPI"); break;
        default:                        Serial.print("unknown");
      }
      Serial.println();
      delay(10);
    }
  }

  Serial.println("");

  // Delete the scan result to free memory for code below.
  WiFi.scanDelete();

  // Wait a bit before scanning again.
  delay(5000);
}

