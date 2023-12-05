#include <Arduino.h>
#include <U8x8lib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <WiFiUdp.h>
#include <SPI.h>
#include "WiFi.h"
#include "passwd.h"
#include "motors.h"

unsigned int local_port = 2390;


U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

char packetBuffer[255]; //buffer to hold incoming packet
char ReplyBuffer[] = "acknowledged"; // a string to send back

// based on https://robotzero.one/heltec-wifi-kit-32/

static void sweepNetwork();
static bool connectNetwork(std::string ssid);

WiFiUDP Udp;

void setup() {
  setup_motors();
  Serial.begin(115200);
  WiFi.disconnect();
  Serial.println("Startup");
  WiFi.mode(WIFI_STA);
  // WiFi.config(IPAddress(10, 0, 0, 156), //IP
  //   IPAddress(8,8,8,8), //DNS
  //   IPAddress(192,168,4,255) //GATEWAY
  //   //IPAddress(255,255,255,0) //SUBNET
  //   );
  delay(100);

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.clear();

  u8x8.drawString(0, 0, "Connecting to: ");
  u8x8.drawString(0, 1,  wifi_ssid.c_str());
  u8x8.drawString(0, 2, "Result: ");
  // pass  and wifi_ssid declared as std::string in pass.h, do not commit your password!
  WiFi.begin(wifi_ssid.c_str(), pass.c_str());
  while (WiFi.status() != (int)wl_status_t::WL_CONNECTED) {
    u8x8.drawString(0, 3, std::to_string(WiFi.status()).c_str());
    delay(5000);
  }
  
  Udp.begin(local_port);
}

struct motor motors[] = {{M1_IN_1_CHANNEL, M1_IN_2_CHANNEL}, {M2_IN_1_CHANNEL, M2_IN_2_CHANNEL}};


void loop() {
  Serial.println("Loop");
  forward(&motors[0], 0);
  forward(&motors[1], 0);
  // print networks on OLED
  //sweepNetwork();
  int packetSize = Udp.parsePacket();
  IPAddress remote_ip = Udp.remoteIP();
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.getHostname());
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());
    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.print(ReplyBuffer);
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.endPacket();
  }
  delay(5000);
  u8x8.clear();
  u8x8.drawString(0, 0, "Network: ");
  u8x8.drawString(0, 1, wifi_ssid.c_str());
  u8x8.drawString(0, 2, "Status: ");
  u8x8.drawString(0, 3, std::to_string(WiFi.status()).c_str());
  u8x8.drawString(0, 4, "Remote IP:");
  u8x8.drawString(0, 5, std::to_string(remote_ip).c_str());
  
}


static void sweepNetwork() {
  int n = WiFi.scanNetworks();

  if (n == 0) {
    u8x8.drawString(0, 0, "Searching networks.");
  } 
  else {
    u8x8.drawString(0, 0, "Networks found: ");
    for (int i = 0; i < n; ++i) {
      // Print SSID for each network found
      char currentSSID[64];
      WiFi.SSID(i).toCharArray(currentSSID, 64);
      u8x8.drawString(0, i + 1, currentSSID);
    }
  }

  // Wait a bit before scanning again
  delay(5000);
}