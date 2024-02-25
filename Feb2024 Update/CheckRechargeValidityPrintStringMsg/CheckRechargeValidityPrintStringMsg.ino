// Check Recharge Validity using AT command...print message after executing on serial monitor
// // Delete all existing SMS messages
  // SerialAT.println("AT+CMGD=1,4");
  // delay(1000);

#define TINY_GSM_MODEM_SIM7600
#include <TinyGsmClient.h>

#define SerialAT Serial1
#define SerialMon Serial
#define TINY_GSM_USE_GPRS true

char a, b;

const char apn[] = ""; // APN automatically detects for 4G SIM, NO NEED TO ENTER, KEEP IT BLANK

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
TinyGsmClient client(modem);

void checkBalance() {
  SerialAT.println("AT+CUSD=1,\"*123#\",15"); // Replace *123# with your operator's USSD code for balance check
  delay(2000);

  while (SerialAT.available()) {
    char c = SerialAT.read();
    SerialMon.write(c);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  delay(100);
  digitalWrite(4, HIGH);
  delay(1000);
  digitalWrite(4, LOW);

  Serial.println("\nConfiguring VVM501 Module. Please wait...");

  delay(10000);

  SerialAT.begin(115200, SERIAL_8N1, 27, 26);

  DBG("Initializing modem...");
  if (!modem.init()) {
    DBG("Failed to restart modem, delaying 10s and retrying");
    return;
  }

  DBG("Initializing modem...");
  if (!modem.restart()) {
    DBG("Failed to restart modem, delaying 10s and retrying");
    return;
  }

  String name = modem.getModemName();
  DBG("Modem Name:", name);

  String modemInfo = modem.getModemInfo();
  DBG("Modem Info:", modemInfo);

  Serial.println("Waiting for network...");
  if (!modem.waitForNetwork()) {
    Serial.println("Fail");
    delay(10000);
    return;
  }
  Serial.println("Success");

  if (modem.isNetworkConnected()) {
    Serial.println("Network connected");
  }

  Serial.print(F("Connecting to "));
  Serial.print(apn);
  if (!modem.gprsConnect(apn)) {
    Serial.println("Fail");
    delay(10000);
    return;
  }
  Serial.println("Success");

  if (modem.isGprsConnected()) {
    Serial.println("LTE module connected");
  }

  Serial.println("Enter Standard AT commands like AT, AT+CPIN?, AT+CCLK?, etc.");
  Serial.println("Select Serial Port Monitor as \"Both NL & CR\" to view command response correctly if you are using Arduino IDE");
  Serial.println("Refer to the A7600 series datasheet for the entire list of commands");
  Serial.println("Understand the AT Commands properly");
  Serial.println("Incorrect AT commands can corrupt the 4G module memory!!!");

  // MQTT Broker setup
  // mqtt.setServer(broker, 1883);
  // mqtt.setCallback(callback);
}

void loop() {
  if (Serial.available() > 0) {
    a = Serial.read();
    SerialAT.write(a);
  }

  if (SerialAT.available() > 0) {
    b = SerialAT.read();
    Serial.write(b);
  }

  checkBalance();
}