#include <Arduino.h>
#include <U8x8lib.h>
#include "WiFi.h"

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// based on https://robotzero.one/heltec-wifi-kit-32/

static void sweepNetwork();

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
}

static void sweepNetwork()
{
  int n = WiFi.scanNetworks();

  if (n == 0) {
    u8x8.drawString(0, 0, "Searching networks.");
  } else {
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


void loop()
{
  sweepNetwork();
}
