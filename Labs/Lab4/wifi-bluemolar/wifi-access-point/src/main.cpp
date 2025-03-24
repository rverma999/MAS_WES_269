#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <esp_wifi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

// Set these to your desired credentials.
const char *ssid = "StrawberryJamnet";
const char *password = "EvenStephen";
WiFiServer server(80);

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("Configuring access point...");

    // You can remove the password parameter if you want the AP to be open.
    // a valid password must have more than 7 characters
    if (!WiFi.softAP(ssid, password)) {
        log_e("Soft AP creation failed.");
        while (1);
    }
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.begin();

    Serial.println("Server started");
}

void get_client_details() {
    wifi_sta_list_t clients;
    esp_wifi_ap_get_sta_list(&clients);
    Serial.printf("Connected clients: %d\n", clients.num);

    // typedef struct {
    //     uint8_t mac[6];  /**< mac address */
    //     int8_t  rssi;    /**< current average rssi of sta connected */
    //     uint32_t phy_11b:1;      /**< bit: 0 flag to identify if 11b mode is enabled or not */
    //     uint32_t phy_11g:1;      /**< bit: 1 flag to identify if 11g mode is enabled or not */
    //     uint32_t phy_11n:1;      /**< bit: 2 flag to identify if 11n mode is enabled or not */
    //     uint32_t phy_lr:1;       /**< bit: 3 flag to identify if low rate is enabled or not */
    //     uint32_t is_mesh_child:1;/**< bit: 4 flag to identify mesh child */
    //     uint32_t reserved:27;    /**< bit: 5..31 reserved */
    // } wifi_sta_info_t;

    for (int i = 0; i < clients.num; i++){
        wifi_sta_info_t client = clients.sta[i];
        printf("\n+++CLIENT: %d+++", i);
        printf("MAC %02x:%02x:%02x:%02x:%02x:%02x\n", client.mac[0],client.mac[1],client.mac[2],client.mac[3],client.mac[4],client.mac[5]);
        printf("RSSI: %d\n", client.rssi);
        printf("PHY_B %d, PHY_g %d, PHY_n %d, PHY_r %d\n", client.phy_11b, client.phy_11g, client.phy_11n, client.phy_lr);
    }
}

void loop()
{
    get_client_details();
    delay(1000);
}

