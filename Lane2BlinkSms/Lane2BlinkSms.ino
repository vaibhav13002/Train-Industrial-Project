#define BLYNK_TEMPLATE_ID "TMPL3-ySfBtB2"
#define BLYNK_TEMPLATE_NAME "Railwaysignals"
#define BLYNK_AUTH_TOKEN "jSaZhXF91noyLoEJKDLjoma3TRldsLs-"

#define RXD2 27
#define TXD2 26
#define powerPin 4
#define TINY_GSM_MODEM_SIM7600
#include <Arduino.h>
#include <Wire.h> 
#include <BlynkSimpleTinyGSM.h>

#define ALERT_PHONE_NUMBER "+919960491371" // Replace with the recipient's phone number


const int Line_1_up_r = 36;
//const int ledPin_1 = V1;
const int Line_1_up_g = 39;
//const int ledPin_2 = V2;
const int Line_1_dn_r = 34;
//const int ledPin_3 = V3;
const int Line_1_dn_g = 35;
//const int ledPin_4 = V6;

const int output_red = 17;//LSB
const int output_green = 18;


int Line_1_up_r_State = 0 ;
int Line_1_up_g_State = 0;

int rx = -1;
#define SerialAT Serial1
String rxString;
int _timeout;
String _buffer;

BlynkTimer timer;
char auth[] = BLYNK_AUTH_TOKEN;
char apn[] = "";
char user[] = "";
char pass[] = "";

TinyGsm modem(SerialAT);

void sendAlert(String message) {
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



void setup() {
  pinMode(output_red, OUTPUT);
  pinMode(output_green, OUTPUT);
  
  pinMode(Line_1_up_r, INPUT_PULLUP);
  pinMode(Line_1_up_g, INPUT_PULLUP);

      // Serial.begin(115200);
 // pinMode(LED, OUTPUT);

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

  // SMS initialization code
  SerialAT.println("AT+CNMI=2,2,0,0,0"); // Configure SMS notification
  delay(500);

  SerialAT.flush();
  sendAlert("Device is online!");

 Blynk.begin(auth, modem, apn, user, pass);
delay(1000);

}

void loop() {

 Blynk.run();
delay(1000);

  Serial.println("main");
  // put your main code here, to run repeatedly:
//  digitalWrite(LED, !digitalRead(LED));
   Line_1_up_r_State = digitalRead(Line_1_up_r);
   Line_1_up_g_State = digitalRead(Line_1_up_g);
  delay(1000);
  
  Serial.println("Input");
  Serial.println(Line_1_up_r_State);
  Serial.println(Line_1_up_g_State);
  delay(1000);

  if (Line_1_up_r_State == 0 )
  {
    
    digitalWrite(output_red, HIGH);
    

    
//    Blynk.virtualWrite(ledPin_1, HIGH);  // Turn off the LED on the Blynk app
    Serial.println("1");
   }


if (Line_1_up_r_State == 1 )
  {
    
    digitalWrite(output_red, LOW);
   
    Serial.println("2");

    
//    Blynk.virtualWrite(ledPin_1, LOW);  // Turn off the LED on the Blynk app

   }

if (Line_1_up_g_State == 0 )
  {
    
   
    digitalWrite(output_green, HIGH);
    
   Serial.println("3");
    
//    Blynk.virtualWrite(ledPin_2, HIGH);  // Turn off the LED on the Blynk app

   }

if (Line_1_up_g_State == 1 )
  {
    
   
    digitalWrite(output_green, LOW);
    
   Serial.println("4");
    
//    Blynk.virtualWrite(ledPin_2, LOW);  // Turn off the LED on the Blynk app

   }
  

}