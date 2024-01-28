#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <DHT.h>
#include <EEPROM.h>
const int EEPROM_ADDRESS = 0; // EEPROM address to start writing data
// const String FIREBASE_HOST  = "https://railway-rakshak-default-rtdb.asia-southeast1.firebasedatabase.app/";
// const String FIREBASE_SECRET  = "dtgQBVf1j2vbYl6s8P1CBLFQJDZVRQ5p6LRemtx9";
const String FIREBASE_HOST  = "https://bega2-38a04-default-rtdb.firebaseio.com/";
const String FIREBASE_SECRET  = "sCFIQou2jRCB1Q9ooM4y5hXtvO8TQxTgSOvZmWgs";


#define USE_SSL true
#define DELAY_MS 500

#define DEBUG true
#define RXD2 27    //VVM501 MODULE RXD INTERNALLY CONNECTED
#define TXD2 26    //VVM501 MODULE TXD INTERNALLY CONNECTED
#define powerPin 4 //VVM501 MODULE ESP32 PIN D4 CONNECTED TO POWER PIN OF A7670C CHIPSET, INTERNALLY CONNECTED
#define Sensor_PIN 15  //D15-DHT22 CONNECT EXTERNALLY

DHT  dht(Sensor_PIN, DHT22);
String get_temprature();
void post_to_firebase(String data);
boolean waitResponse(String expected_answer="OK", unsigned int timeout=2000);
int rx = -1;
#define SerialAT Serial1
String rxString;
int _timeout;
String _buffer;

void setup()
{
  EEPROM.begin(512);
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, LOW);
  Serial.begin(115200);
  delay(100);
  SerialAT.begin(115200, SERIAL_8N1, RXD2, TXD2);
  delay(10000);  //WAITING FOR MODEM RESET
  dht.begin();
  delay(2000);
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
  Serial.println("4G HTTP Test Begin!");

  dht.begin();
  delay(1000);
  Serial.begin(115200);
}

void loop()
{
 
 
  String data1 = get_temprature();
  post_to_firebase(data1);
  saveDataToEEPROM(data1); // Save data to EEPROM
  delay(5000);

  // Load and print data from EEPROM
  String loadedData = loadDataFromEEPROM();
  Serial.println("Data loaded from EEPROM: " + loadedData);

  delay(5000);
}
void saveDataToEEPROM(String data) {
  int length = data.length();
  int eepromIndex = 0;

  // Find the end of existing data in EEPROM
  while (EEPROM.read(EEPROM_ADDRESS + eepromIndex) != '\0') {
    eepromIndex++;
  }

  // Write new data starting from the end of existing data
  for (int i = 0; i < length; ++i) {
    EEPROM.write(EEPROM_ADDRESS + eepromIndex + i, data[i]);
  }

  // Null-terminate the string
  EEPROM.write(EEPROM_ADDRESS + eepromIndex + length, '\0');
  EEPROM.commit(); // Save changes to EEPROM
}

String loadDataFromEEPROM() {
  String loadedData;
  char c;
  int eepromIndex = 0;

  Serial.println("Reading data from EEPROM:");

  // Iterate through EEPROM and read characters until null character is encountered
  while ((c = EEPROM.read(EEPROM_ADDRESS + eepromIndex)) != '\0') {
    Serial.print(c);
    loadedData += c;
    eepromIndex++;
  }

  Serial.println(); // Add a newline for better readability
  Serial.println("End of data from EEPROM");

  return loadedData;
}

String get_temprature() {
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  // float humidityValue = dht.readHumidity();
  // float temperatureValue = dht.readTemperature();

  float randomHumidity = random(0, 100);  // Random humidity between 0 and 100
  float randomTemperature = random(-50, 50); // Random temperature between -50 and 50

  String humidityString = String(randomHumidity, 2);  // 2 decimal places
  String temperatureString = String(randomTemperature, 2);  // 2 decimal places

  Serial.print("Temperature = ");
  Serial.print(temperatureString);
  Serial.println(" °C");
  Serial.print("Humidity = ");
  Serial.print(humidityString);
  Serial.println(" %");

  String Data = "{";

  Data += "\"temperature\":\"" + temperatureString + "\",";
  Data += "\"humidity\":\"" + humidityString + "\"";

  Data += "}";

  return Data;
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Function: post_to_firebase() start
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void post_to_firebase(String data1)
{
  
 //-----------Firebase---------------------
//String firebase_str = "AT+HTTPPARA=\"URL\", \"" + FIREBASE_HOST + ".json?auth=" + FIREBASE_SECRET + "\"";
String firebase_url = FIREBASE_HOST + ".json?auth=" + FIREBASE_SECRET;


Serial.println(firebase_url);
SerialAT.println("AT+HTTPINIT");
waitResponse();
delay(2000);
//SerialAT.println(firebase_str);
SerialAT.println("AT+HTTPPARA=\"URL\",\"" + firebase_url + "\"");
waitResponse();
delay(3000);
// Set the HTTP data size
SerialAT.println("AT+HTTPDATA=" + String(data1.length()) + ",10000");
waitResponse();
delay(1000);


SerialAT.println(data1);
waitResponse();
delay(3000);
SerialAT.println("AT+HTTPACTION=1");
 for (uint32_t start = millis(); millis() - start < 20000;){
    while(!SerialAT.available());
    String response = SerialAT.readString();
    if(response.indexOf("+HTTPACTION:") > 0)
    {
      Serial.println(response);
      break;
    }
  }
  delay(3000);
  //SerialAT.println("AT+HTTPREAD");
 // waitResponse("OK");
 // delay(1000);

SerialAT.println("AT+HTTPTERM");
waitResponse();

}

boolean waitResponse(String expected_answer, unsigned int timeout) //uncomment if syntax error (esp8266)
{
  uint8_t x=0, answer=0;
  String response;
  unsigned long previous;
    
  //Clean the input buffer
  while( SerialAT.available() > 0) SerialAT.read();
  
  
  previous = millis();
  do{
    //if data in UART INPUT BUFFER, reads it
    if(SerialAT.available() != 0){
        char c = SerialAT.read();
        response.concat(c);
        x++;
        //checks if the (response == expected_answer)
        if(response.indexOf(expected_answer) > 0){
            answer = 1;
        }
    }
  }while((answer == 0) && ((millis() - previous) < timeout));
  
  
  Serial.println(response);
  return answer;
}


