
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

const int Line_1_red = 36;
const int Line_1_green = 39;
const int Line_2_red = 34;
const int Line_2_green = 35;
const int Line_3_red = 32;
const int Line_3_green = 33;
const int Line_4_red = 25;
const int Line_4_green = 26;


const int Decoder_input_1 = 17;//LSB
const int Decoder_input_2 = 18;
const int Decoder_input_3 = 19;
const int Decoder_input_4 = 16;//MSB- Enable pin


int Line_1_red_State = 0;
int Line_1_green_State = 0;
int Line_2_red_State = 0;
int Line_2_green_State = 0;
int Line_3_red_State = 0;
int Line_3_green_State = 0;
int Line_4_red_State = 0;
int Line_4_green_State = 0;


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

  pinMode(Decoder_input_1, OUTPUT);
  pinMode(Decoder_input_2, OUTPUT);
  pinMode(Decoder_input_3, OUTPUT);
  pinMode(Decoder_input_4, OUTPUT);

  pinMode(Line_1_red, INPUT_PULLUP);
  pinMode(Line_1_green, INPUT_PULLUP);
  pinMode(Line_2_red, INPUT_PULLUP);
  pinMode(Line_2_green, INPUT_PULLUP);
  pinMode(Line_3_red, INPUT_PULLUP);
  pinMode(Line_3_green, INPUT_PULLUP);
  pinMode(Line_4_red, INPUT_PULLUP);
  pinMode(Line_4_green, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
   Serial.begin(115200);
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
  // put your main code here, to run repeatedly:int Line_1_red_State = digitalRead(Line_1_red);
  int Line_1_red_State = digitalRead(Line_1_red); // R1 led - 0000
  int Line_1_green_State = digitalRead(Line_1_green);// G1 led - 0001
  int Line_2_red_State = digitalRead(Line_2_red);// R2 - 0010
  int Line_2_green_State = digitalRead(Line_2_green); // G2 - 0011
  int Line_3_red_State = digitalRead(Line_3_red); // R3 led - 0100
  int Line_3_green_State = digitalRead(Line_3_green);// G3 led - 0101
  int Line_4_red_State = digitalRead(Line_4_red);// R4 - 0110
  int Line_4_green_State = digitalRead(Line_4_green); // G4 - 0111

  Serial.println("Red_1=:");
  Serial.println(Line_1_red_State);
  Serial.println("green_1=:");
  Serial.println(Line_1_green_State);
  Serial.println("Red_2=:");
  Serial.println(Line_2_red_State);
  Serial.println("green_2=:");
  Serial.println(Line_2_green_State);

  Serial.println("Red_3=:");
  Serial.println(Line_3_red_State);
  Serial.println("green_3=:");
  Serial.println(Line_3_green_State);
  Serial.println("Red_4=:");
  Serial.println(Line_4_red_State);
  Serial.println("green_4=:");
  Serial.println(Line_4_green_State);
  delay(1000);

//Signals are Active low
// 0 = HIGH
// 1 = LOW

// 10101010
  if (Line_1_red_State == 1 && Line_1_green_State == 0 && Line_2_red_State == 1 && Line_2_green_State == 0 && Line_3_red_State == 1 && Line_3_green_State == 0 && Line_4_red_State == 1 && Line_4_green_State == 0 )  
  { Serial.println("1");
    Serial.println("Line_1_green is on | Line_2_green is on | Line_3_green is on | Line_4_green is on");
    
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0001 green 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0011 green 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0101 green 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0111 green 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }

// 01010101
  if (Line_1_red_State == 0 && Line_1_green_State == 1 && Line_2_red_State == 0 && Line_2_green_State == 1 && Line_3_red_State == 0 && Line_3_green_State == 1 && Line_4_red_State == 0 && Line_4_green_State == 1 )  
  { Serial.println("2");
    Serial.println("Line_1_red is on | Line_2_red is on | Line_3_red is on | Line_4_red  is on");
    
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 red 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0010 red 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0100 red 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0110 red 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }

// 10101001
  if (Line_1_red_State == 1 && Line_1_green_State == 0 && Line_2_red_State == 1 && Line_2_green_State == 0 && Line_3_red_State == 1 && Line_3_green_State == 0 && Line_4_red_State == 0 && Line_4_green_State == 1 )  
  { Serial.println("3");
    Serial.println("Line_1_green is on | Line_2_green is on | Line_3_green is on | Line_4_red is on");
    
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0001 green 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0011 green 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0101 green 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0110 red 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }
// 10100110
  if (Line_1_red_State == 1 && Line_1_green_State == 0 && Line_2_red_State == 1 && Line_2_green_State == 0 && Line_3_red_State == 0 && Line_3_green_State == 1 && Line_4_red_State == 1 && Line_4_green_State == 0 )  
  { Serial.println("4");
    Serial.println("Line_1_green is on | Line_2_green is on | Line_3_red is on | Line_4_green is on");
     
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0001 green 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0011 green 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0100 red 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0111 green 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }
// 10011010
  if (Line_1_red_State == 1 && Line_1_green_State == 0 && Line_2_red_State == 0 && Line_2_green_State == 1 && Line_3_red_State == 1 && Line_3_green_State == 0 && Line_4_red_State == 1 && Line_4_green_State == 0 )  
  { Serial.println("5");
    Serial.println("Line_1_green is on | Line_2_red is on | Line_3_green is on | Line_4_green is on");
    
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0001 green 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0010 red 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0101 green 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0111 green 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }
// 01101010
  if (Line_1_red_State == 0 && Line_1_green_State == 1 && Line_2_red_State == 1 && Line_2_green_State == 0 && Line_3_red_State == 1 && Line_3_green_State == 0 && Line_4_red_State == 1 && Line_4_green_State == 0 )  
  { Serial.println("6");
    Serial.println("Line_1_red is on | Line_2_green is on | Line_3_green is on | Line_4_green is on");
    
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 red 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0011 green 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0101 green 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0111 green 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }
// 01011010
  if (Line_1_red_State == 0 && Line_1_green_State == 1 && Line_2_red_State == 0 && Line_2_green_State == 1 && Line_3_red_State == 1 && Line_3_green_State == 0 && Line_4_red_State == 1 && Line_4_green_State == 0 )  
  { Serial.println("7");
    Serial.println("Line_1_red is on | Line_2_red is on | Line_3_green is on | Line_4_green is on");
    
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 red 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0010 red 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0101 green 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0111 green 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }
// 01100110
  if (Line_1_red_State == 0 && Line_1_green_State == 1 && Line_2_red_State == 1 && Line_2_green_State == 0 && Line_3_red_State == 0 && Line_3_green_State == 1 && Line_4_red_State == 1 && Line_4_green_State == 0 )  
  { Serial.println("8");
    Serial.println("Line_1_red is on | Line_2_green is on | Line_3_red is on | Line_4_green is on");
    
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 red 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0011 green 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0100 red 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0111 green 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }
// 01101001
  if (Line_1_red_State == 0 && Line_1_green_State == 1 && Line_2_red_State == 1 && Line_2_green_State == 0 && Line_3_red_State == 1 && Line_3_green_State == 0 && Line_4_red_State == 0 && Line_4_green_State == 1 )  
  { Serial.println("9");
    Serial.println("Line_1_red is on | Line_2_green is on | Line_3_green is on | Line_4_red is on");
    
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 red 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0011 green 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0101 green 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0110 red 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }
// 10010110
  if (Line_1_red_State == 1 && Line_1_green_State == 0 && Line_2_red_State == 0 && Line_2_green_State == 1 && Line_3_red_State == 0 && Line_3_green_State == 1 && Line_4_red_State == 1 && Line_4_green_State == 0 )  
  { Serial.println("10");
    Serial.println("Line_1_green is on | Line_2_red is on | Line_3_red is on | Line_4_green is on");
    
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0001 green 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0010 red 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0100 red 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0111 green 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }
// 10011001
  if (Line_1_red_State == 1 && Line_1_green_State == 0 && Line_2_red_State == 0 && Line_2_green_State == 1 && Line_3_red_State == 1 && Line_3_green_State == 0 && Line_4_red_State == 0 && Line_4_green_State == 1 )  
  { Serial.println("11");
    Serial.println("Line_1_green is on | Line_2_red is on | Line_3_green is on | Line_4_red is on");
    
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0001 green 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0010 red 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0101 green 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0110 red 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }
// 10100101
  if (Line_1_red_State == 1 && Line_1_green_State == 0 && Line_2_red_State == 1 && Line_2_green_State == 0 && Line_3_red_State == 0 && Line_3_green_State == 1 && Line_4_red_State == 0 && Line_4_green_State == 1 )  
  { Serial.println("12");
    Serial.println("Line_1_green is on | Line_2_green is on | Line_3_redn is on | Line_4_red is on");
    
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0001 green 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0011 green 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0100 red 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0110 red 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }
// 01010110
  if (Line_1_red_State == 0 && Line_1_green_State == 1 && Line_2_red_State == 0 && Line_2_green_State == 1 && Line_3_red_State == 0 && Line_3_green_State == 1 && Line_4_red_State == 1 && Line_4_green_State == 0 )  
  { Serial.println("13");
    Serial.println("Line_1_red is on | Line_2_red is on | Line_3_red is on | Line_4_green  is on");
    
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 red 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0010 red 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0100 red 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0111 green 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }
// 01011001
  if (Line_1_red_State == 0 && Line_1_green_State == 1 && Line_2_red_State == 0 && Line_2_green_State == 1 && Line_3_red_State == 1 && Line_3_green_State == 0 && Line_4_red_State == 0 && Line_4_green_State == 1 )  
  { Serial.println("14");
    Serial.println("Line_1_red is on | Line_2_red is on | Line_3_green is on | Line_4_red  is on");
    
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 red 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0010 red 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0101 green 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0110 red 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }
// 01100101
  if (Line_1_red_State == 0 && Line_1_green_State == 1 && Line_2_red_State == 1 && Line_2_green_State == 0 && Line_3_red_State == 0 && Line_3_green_State == 1 && Line_4_red_State == 0 && Line_4_green_State == 1 )  
  { Serial.println("15");
    Serial.println("Line_1_red is on | Line_2_green is on | Line_3_red is on | Line_4_red  is on");
    
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 red 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0011 green 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0100 red 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0110 red 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }
// 10010101
  if (Line_1_red_State == 1 && Line_1_green_State == 0 && Line_2_red_State == 0 && Line_2_green_State == 1 && Line_3_red_State == 0 && Line_3_green_State == 1 && Line_4_red_State == 0 && Line_4_green_State == 1 )  
  { Serial.println("16");
    Serial.println("Line_1_green is on | Line_2_red is on | Line_3_red is on | Line_4_red  is on");
    
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0001 green 1 
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);   //0010 red 2
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0100 red 3
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);

      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);   //0110 red 4
      digitalWrite(Decoder_input_4, HIGH);
      delay(500);
  }

  // Construct the 8-digit code
  String eightDigitCode = String(Line_1_red_State) + String(Line_1_green_State) +
                          String(Line_2_red_State) + String(Line_2_green_State) +
                          String(Line_3_red_State) + String(Line_3_green_State) +
                          String(Line_4_red_State) + String(Line_4_green_State);
 String message = "Code: " + eightDigitCode + " Station: " + stationCode + " Time: " + getDateTime();
  sendSMS(message);

}
String getDateTime() {
  Serial.println("AT+CCLK?");   // Query clock
  String response = Serial.readString();

  // Check if response is in correct format
  if(!response.startsWith("+CCLK:")) {
    return "";
  }
  // Extract date and time string
  String dateTime = response.substring(7,24);  

  // Extract individual values
  String year = dateTime.substring(0,2);  
  String month = dateTime.substring(3,5);
  String day = dateTime.substring(6,8);

  String hours = dateTime.substring(9,11);
  String minutes = dateTime.substring(12,14);  
  String seconds = dateTime.substring(15,17);

  // Construct full date time string
  String current = day + "/" + month + "/" + year + " " + hours + ":" + minutes + ":" + seconds;
  return current;
}