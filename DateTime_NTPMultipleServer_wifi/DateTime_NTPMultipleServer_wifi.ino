#include <WiFi.h>
#include "time.h"

const char* ntpServers[] = {"0.pool.ntp.org","1.pool.ntp.org","2.pool.ntp.org"}; 

const char* ssid = "Bega";
const char* password = "begabega";  

const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

struct tm timeinfo; // declare timeinfo

void setup() {

  Serial.begin(115200);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

}

void loop() {
  for (int i = 0; i < 3; i++) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServers[i]);
    
    if (getLocalTime(&timeinfo)) {
      break;
    }
    
    delay(1000);
  }

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }  

  char formattedTime[20]; // Buffer to store the formatted time

  // Format the time according to "dd/MM/yyyy HH:mm:ss"
  strftime(formattedTime, sizeof(formattedTime), "%d/%m/%Y %H:%M:%S", &timeinfo);

  // Concatenate date and time into a single string
  char dateTimeString[25]; // Adjust the size based on your needs
  snprintf(dateTimeString, sizeof(dateTimeString), "%s", formattedTime);

  Serial.println(dateTimeString);

  delay(1000);
}
