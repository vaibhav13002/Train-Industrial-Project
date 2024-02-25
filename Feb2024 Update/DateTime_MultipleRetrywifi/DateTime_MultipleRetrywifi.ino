// if error encountered it will try multiple time and and will in same loop until time gets come
#include <WiFi.h>
#include "time.h"

const char* ssid = "Bega";
const char* password = "begabega";
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 5 * 3600 + 30 * 60;  // Indian Standard Time
const int daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
    attempts++;
    if (attempts > 20) {
      Serial.println("\nFailed to connect to WiFi. Please check your credentials");
      while (1) {
        delay(1000);
      }
    }
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

  // Configure NTP server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time. Retrying...");
    delay(1000);
    return;
  }

  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  delay(1000);
}
