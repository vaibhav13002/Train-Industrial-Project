
#define LED 2
#define RXD2 27    //VVM501 MODULE RXD INTERNALLY CONNECTED
#define TXD2 26    //VVM501 MODULE TXD INTERNALLY CONNECTED
#define powerPin 4 ////VVM501 MODULE ESP32 PIN D4 CONNECTED TO POWER PIN OF A7670C CHIPSET, INTERNALLY CONNECTED

// Select your modem:
#define TINY_GSM_MODEM_SIM7600   //SIMA7670 Compatible with SIM7600 AT instructions

#include <Arduino.h>
#include <TinyGsmClient.h> // Include TinyGSM library for GSM connectivity
#include <Wire.h>
int rx = -1;
#define SerialAT Serial1
String rxString;
int _timeout;
String _buffer;


// Your GPRS credentials
// Leave empty, if missing user or pass
char apn[]  = "";
char user[] = "";
char pass[] = "";

// bool reply = false;
TinyGsm modem(SerialAT);
#define ALERT_PHONE_NUMBER "+919960491371" // Replace with the recipient's phone number
String stationCode = "123456";




void setup() {    //modem setup isko same rkhna h iske last me add krna h jo chalu krna h
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, LOW);
  Serial.begin(115200);
  delay(100);
  SerialAT.begin(115200, SERIAL_8N1, RXD2, TXD2);
  delay(10000);
  Serial.println("Modem Reset, Please Wait");
  SerialAT.println("AT+CRESET");
  delay(1000);
  SerialAT.println("AT+CRESET");
  delay(20000);
  SerialAT.flush();

  Serial.println("Echo Off");
  SerialAT.println("ATE0");   //120s
  delay(1000);
  SerialAT.println("ATE0");   //120s
  rxString = SerialAT.readString();
  Serial.print("Got: ");
  Serial.println(rxString);
  rx = rxString.indexOf("OK");
  if (rx != -1)
    Serial.println("Modem Ready");
  delay(1000);

  Serial.println("SIM card check");
  SerialAT.println("AT+CPIN?"); //9s
  rxString = SerialAT.readString();
  Serial.print("Got: ");
  Serial.println(rxString);
  rx = rxString.indexOf("+CPIN: READY");
  if (rx != -1)
    Serial.println("SIM Card Ready");
  delay(1000);

  String name = modem.getModemName();
  delay(500);
  Serial.println("Modem Name: " + name);


// Print IMEI number and MAC address
  SerialAT.println("AT+CGSN"); // Get IMEI number
  delay(500);
  Serial.println("IMEI: " + SerialAT.readString());

}

void sendSMS(String message) {
  SerialAT.println("AT+CMGF=1"); // Set SMS mode to text
  delay(500);

  SerialAT.print("AT+CMGS=\"");
  SerialAT.print(ALERT_PHONE_NUMBER);
  SerialAT.println("\"");

  delay(1000);
  SerialAT.print(message);
  SerialAT.write(26); // End of message character
  delay(1000);
}

void loop() {

}

//a method me h so jab call hoga tab he run hogha
String getDateTime() {
 String timestamp;
SerialAT.println("AT+CCLK?");
delay(1000);

if (SerialAT.available()) {
  String response = SerialAT.readStringUntil('\n'); // Read until a newline character

  int index = response.indexOf("+CCLK: \"");
  if (index != -1) {
    String dateTimeStr = response.substring(index + 8, index + 26); // extract date and time string

    // dateTimeStr should have the format YY/MM/DD,hh:mm:ss+TZ
    int year = (dateTimeStr.substring(0, 2).toInt() + 2000); // extract year and add 2000 to get the full year
    int month = dateTimeStr.substring(3, 5).toInt(); // extract month
    int day = dateTimeStr.substring(6, 8).toInt(); // extract day
    int hour = dateTimeStr.substring(9, 11).toInt(); // extract hour
    int minute = dateTimeStr.substring(12, 14).toInt(); // extract minute
    int second = dateTimeStr.substring(15, 17).toInt(); // extract second
    String timeZone = dateTimeStr.substring(18, 21); // extract timezone

    timestamp = String(day) + "-" + String(month) + "-" + String(year) + " " + String(hour) + ":" + String(minute) + ":" + String(second); // construct date and time string in day-month-year hour:minutes:seconds format
    Serial.println(timestamp);
  }
}

}


////Separate Serial print function agr upr wala nhi chala tab

// String getIMEI()
// {
//   SerialAT.println("AT+CGSN"); // Get IMEI number
//   delay(500);
//   return _readSerial();
// }
// ////////MAC address
// String getMAC()
// {
//   SerialAT.println("AT+CIPSTAMAC?"); // Get MAC address
//   delay(500);
//   return _readSerial();
// }
// /////Serial Print
// String _readSerial()
// {
//   _timeout = 0;
//   while (!SerialAT.available() && _timeout < 12000)
//   {
//     delay(13);
//     _timeout++;
//   }
//   if (SerialAT.available())
//   {
//     return SerialAT.readString();
//   }
//   return "";
// }