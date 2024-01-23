#define TINY_GSM_MODEM_SIM7600
#define LED 2
#define RXD2 27
#define TXD2 26
#define powerPin 4
#define SerialMon Serial
#define TINY_GSM_USE_GPRS true


#include <Arduino.h>
#include <TinyGsmClient.h>
#include <Wire.h>
#include <FirebaseESP32.h>

//Provide the token generation process info.
#include <addons/TokenHelper.h>
//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

int rx = -1;
#define SerialAT Serial1
String rxString;
int _timeout;
String _buffer;

char apn[] = "";
char user[] = "";
char pass[] = "";

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
#define ALERT_PHONE_NUMBER "+919960491371"
String stationCode = "Vasai-01";
String number = "+919960491371";  // Replace with the recipient's phone number
#define API_KEY "AIzaSyDbAKHfK0o7Dz3TZqQwuYIdeC4XffpuecY"
#define DATABASE_URL "https://railway-rakshak-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define USER_EMAIL "Shaista.khan@vcet.edu.in"
#define USER_PASSWORD "shafin15"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;


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

void setup() {
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, LOW);
  Serial.begin(115200);
  delay(100);
  SerialAT.begin(115200, SERIAL_8N1, RXD2, TXD2);
  delay(10000);

  Serial.println("Modem Reset, please wait");
  SerialAT.println("AT+CRESET");
  delay(10000);
  SerialAT.println("AT+CRESET");
  delay(10000);  // WAITING FOR SOME TIME TO CONFIGURE MODEM

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
  Serial.println("Type s to send an SMS, r to receive an SMS, and c to make a call");

  // Initialize Firebase
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  delay(5000);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Check if Firebase is ready before sending data
  if (Firebase.ready()) {
    // Your Firebase-related code here

    // Example: Writing a string to Firebase
    String data = "Hello, Firebase!";
    if (Firebase.setString(fbdo, "examplePath/exampleNode", data)) {
      Serial.println("Data successfully written to Firebase!");
    } else {
      Serial.println("Failed to write data to Firebase. Reason: " + fbdo.errorReason());
    }
  } else {
    Serial.println("Firebase not ready, retrying...");
    delay(2000);
  }
}
