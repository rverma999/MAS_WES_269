#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const char* SSID     = "tock_tutorial"; // MUST update this
const char* PASSWORD = ""; // MUST update this

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup()
{
    Serial.begin(115200);
    // We start by connecting to a WiFi network

  Serial.println();
  Serial.println("******************************************************");
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("- IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("- Wifi Channel: ");
  Serial.println(WiFi.channel());

  // Print my mac addr
  uint8_t mac[6];
  WiFi.macAddress(mac);
  Serial.printf("- My MAC Addr: %02x:%02x:%02x:%02x:%02x:%02x\n",
      mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);


  timeClient.begin();
  Serial.println("Time client begin");
  timeClient.update();
  Serial.println(timeClient.getFormattedDate());

}


void loop()
{
  // forces the timeclient to requery the net every second
  timeClient.forceUpdate();

  Serial.println(timeClient.getFormattedTime());

  delay(1000);
}

