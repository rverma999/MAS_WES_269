#include <Arduino.h> // Required for all code
#include <WiFi.h> // Required for all wifi code
#include <esp_wifi.h>
#include <esp_wifi_types.h>

#define CHANNEL 5
#define ADDR_LENGTH 6

//uint8_t CLIENT_MAC[6] = {0x34, 0xcd, 0xb0, 0x3b, 0xe6, 0x9c};
uint8_t CLIENT_MAC[6] ;//= {0x34, 0xcd, 0xb0, 0x3b, 0xe6, 0x9c};
uint8_t stored_channel;

typedef struct {
  unsigned frame_ctrl:16;
  unsigned duration_id:16;
  uint8_t addr1[6]; /* receiver address */
  uint8_t addr2[6]; /* sender address */
  uint8_t addr3[6]; /* filtering address */
  unsigned sequence_ctrl:16;
  uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct {
  wifi_ieee80211_mac_hdr_t hdr;
  uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;

const char* wifi_sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type)
{
  switch(type) {
    case WIFI_PKT_MGMT: return "MGMT";
    case WIFI_PKT_DATA: return "DATA";
    default:  
      case WIFI_PKT_MISC: return "MISC";
  }
}

void sniffer_callback(void* buf, wifi_promiscuous_pkt_type_t type) {
    // data!
    
    static int counter;

    const wifi_promiscuous_pkt_t* raw = (wifi_promiscuous_pkt_t*)buf;
    const wifi_ieee80211_packet_t* pkt = (wifi_ieee80211_packet_t*)raw->payload;
    const wifi_ieee80211_mac_hdr_t* hdr = &(pkt->hdr);


    counter = counter + 1;
 
    //if(counter>=0) 
    if(0){
      // Add these new packet details
      Serial.println("\n=== Packet Details ===");
      Serial.printf("Frame Control: 0x%04x\n", hdr->frame_ctrl);
      Serial.printf("Duration: %d μs\n", hdr->duration_id);
      Serial.printf("Sequence Control: 0x%04x\n", hdr->sequence_ctrl);
      
      // Print RX control information
      Serial.println("\n=== RX Control ===");
      Serial.printf("RSSI: %d dBm\n", raw->rx_ctrl.rssi);
      Serial.printf("Rate: %d Mbps\n", raw->rx_ctrl.rate);
      Serial.printf("Timestamp: %u μs\n", raw->rx_ctrl.timestamp);
      Serial.printf("Signal Mode: %d\n", raw->rx_ctrl.sig_mode);
      Serial.printf("MCS: %d\n", raw->rx_ctrl.mcs);
      Serial.printf("FEC Coding: %d\n", raw->rx_ctrl.fec_coding);
      Serial.printf("Aggregation: %d\n", raw->rx_ctrl.aggregation);
      
      // Print packet size
      Serial.printf("Packet Size: %d bytes\n", raw->rx_ctrl.sig_len);
      
      Serial.println("==================\n");
    
      // ... rest of existing code ...
  

    //if ((memcmp(hdr->addr1, MAC_client, ADDR_LENGTH) == 0) || 
    //    (memcmp(hdr->addr2, MAC_client, ADDR_LENGTH) == 0))
    //    if(1){
      printf("PACKET TYPE=%s, CHAN=%02d, RSSI=%02d,"
        " ADDR1=%02x:%02x:%02x:%02x:%02x:%02x,"
        " ADDR2=%02x:%02x:%02x:%02x:%02x:%02x,"
        " ADDR3=%02x:%02x:%02x:%02x:%02x:%02x\n",
        wifi_sniffer_packet_type2str(type),
        raw->rx_ctrl.channel,
        raw->rx_ctrl.rssi,
        /* ADDR1 */
        hdr->addr1[0],hdr->addr1[1],hdr->addr1[2],
        hdr->addr1[3],hdr->addr1[4],hdr->addr1[5],
        /* ADDR2 */
        hdr->addr2[0],hdr->addr2[1],hdr->addr2[2],
        hdr->addr2[3],hdr->addr2[4],hdr->addr2[5],
        /* ADDR3 */
        hdr->addr3[0],hdr->addr3[1],hdr->addr3[2],
        hdr->addr3[3],hdr->addr3[4],hdr->addr3[5]
      );

      /** sig_mode < 0: non HT(11bg) packet; 1: HT(11n) packet; 3: VHT(11ac) packet */
      if (raw->rx_ctrl.sig_mode <= 0) {
        Serial.println("SIG_MODE: non HT(11bg)");
      } else if (raw->rx_ctrl.sig_mode == 1) {
        Serial.println("SIG_MODE: HT(11n)");
      } else if (raw->rx_ctrl.sig_mode == 1) {
        Serial.println("SIG_MODE: VHT(11ac)");
      } else {
        Serial.printf("SIG_MODE: ERROR -> %d\n", raw->rx_ctrl.sig_mode);
      }

      /**< Channel Bandwidth of the packet. 0: 20MHz; 1: 40MHz */
      Serial.printf("CHANNEL BANDWIDTH: %s\n", raw->rx_ctrl.cwb ? "40MHZ" : "20MHZ");
    }



    

}


void setup()
{
    Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    // channel 1
    //MAC Address: 33:34:3A:43:44:3A

    Serial.printf("Operating on channel: %d\n", WiFi.channel());
    Serial.printf("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
    WiFi.macAddress()[0], WiFi.macAddress()[1], WiFi.macAddress()[2],
    WiFi.macAddress()[3], WiFi.macAddress()[4], WiFi.macAddress()[5]);
    stored_channel = WiFi.channel();
    for(int i=0;i<6;i++){
       CLIENT_MAC[i] = WiFi.macAddress()[i] ;
       Serial.printf("Stored CLINET_MAC=%02X\n",CLIENT_MAC[i]);
    } 
    
    //, WiFi.macAddress()[1], WiFi.macAddress()[2],
    //WiFi.macAddress()[3], WiFi.macAddress()[4], WiFi.macAddress()[5]);
    // Pick the channel to scan on
    // Set to channel 1 by default, you should change this though
    printf("Connecting to %d", CHANNEL);
    esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);

    // Start promiscuous scanning. Callback function will be called for each packet received
    esp_wifi_set_promiscuous_rx_cb(&sniffer_callback); 
    esp_wifi_set_promiscuous(true); 

    Serial.println("Setup done");
}

void loop()
{
  /*
  //Serial.print("inside loop");
  delay(1000); // wait for a second
  
  if (digitalRead(LED_GPIO_PIN) == LOW)
    digitalWrite(LED_GPIO_PIN, HIGH);
  else
    digitalWrite(LED_GPIO_PIN, LOW);
  vTaskDelay(WIFI_CHANNEL_SWITCH_INTERVAL / portTICK_PERIOD_MS);
  wifi_sniffer_set_channel(channel);
  channel = (channel % WIFI_CHANNEL_MAX) + 1;
  */
}

