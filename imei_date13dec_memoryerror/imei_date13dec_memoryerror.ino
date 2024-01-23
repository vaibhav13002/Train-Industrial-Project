#define LED 2
#define RXD2 27
#define TXD2 26
#define powerPin 4

#define TINY_GSM_MODEM_SIM7600

#include <Arduino.h>
#include <TinyGsmClient.h>
#include <Wire.h>

int rx = -1;
#define SerialAT Serial1
String rxString;
int _timeout;
String _buffer;

char apn[] = "";
char user[] = "";
char pass[] = "";

TinyGsm modem(SerialAT);
#define ALERT_PHONE_NUMBER "+919960491371"
String stationCode = "Vasai-01";
String number = "+919960491371";  // Replace with the recipient's phone number

void SendMessage() {
  SerialAT.println("AT+CMGF=1");    // Sets the GSM Module in Text Mode
  delay(1000);
  SerialAT.println("AT+CMGS=\"" + number + "\"\r"); // Mobile phone number to send message
  delay(1000);

  // Construct message with station code, IMEI, and date-time
  String message = "Station Code: " + stationCode + "\nIMEI: " + getIMEI() + "\nDate-Time: " + getDateTime();

  SerialAT.println(message);
  delay(100);
  SerialAT.println((char)26);  // ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
  Serial.println("Message Sent!");
}

String getIMEI() {
  SerialAT.println("AT+CGSN");
  delay(500);
  return _readSerial();
}

String _readSerial() {
  _timeout = 0;
  while (!SerialAT.available() && _timeout < 12000) {
    delay(13);
    _timeout++;
  }
  if (SerialAT.available()) {
    return SerialAT.readString();
  }
  return "";
}

String getDateTime() {
 String timestamp;
SerialAT.println("AT+CCLK?");
// delay(1000);
delayMicroseconds(100000); // Replace delay(1000) in getDateTime function


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

void sendSMS(String message) {
  SerialAT.println("AT+CMGF=1");
  delay(500);

  SerialAT.print("AT+CMGS=\"");
  SerialAT.print(ALERT_PHONE_NUMBER);
  SerialAT.println("\"");

  delay(1000);
  SerialAT.print(message);
  SerialAT.write(26);
  delay(1000);
}

void setup() {
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
  SerialAT.println("ATE0");
  delay(1000);
  SerialAT.println("ATE0");
  delay(1000);

  Serial.println("SIM card check");
  SerialAT.println("AT+CPIN?");
  delay(5000);

  String name = modem.getModemName();
  delay(500);
  Serial.println("Modem Name: " + name);

  // Send SMS
  // SendMessage();
getDateTime();
getIMEI();
}

void loop() {
  // Your loop code
}
