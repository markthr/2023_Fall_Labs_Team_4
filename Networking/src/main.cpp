#include <Arduino.h>
#include <U8x8lib.h>
#include <fstream>
#include <iostream>
#include <string>
#include "WiFi.h"
#include "passwd.h"

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// based on https://robotzero.one/heltec-wifi-kit-32/

static void sweepNetwork();
static bool connectNetwork(std::string ssid);


void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.clear();

  u8x8.drawString(0, 0, "Connecting to: ");
  u8x8.drawString(0, 1,  wifi_ssid.c_str());
  u8x8.drawString(0, 2, "Result: ");
  // pass  and wifi_ssid declared as std::string in pass.h, do not commit your password!
  WiFi.begin(wifi_ssid.c_str(), pass.c_str());
  u8x8.drawString(0, 3, std::to_string(WiFi.status()).c_str());

  delay(5000);
}


void loop() {
  // print networks on OLED
  //sweepNetwork();
  delay(5000);
  u8x8.clear();
  u8x8.drawString(0, 0, "Network: ");
  u8x8.drawString(0, 1, wifi_ssid.c_str());
  u8x8.drawString(0, 2, "Status: ");
  u8x8.drawString(0, 3, std::to_string(WiFi.status()).c_str());
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