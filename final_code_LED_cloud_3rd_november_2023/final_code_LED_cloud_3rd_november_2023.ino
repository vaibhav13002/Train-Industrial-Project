
#define BLYNK_TEMPLATE_ID "TMPL3d-H1h95L"
#define BLYNK_TEMPLATE_NAME "DHT11"
#define BLYNK_AUTH_TOKEN "gEVjLoZZVr5xy1e9b3h-yrbeK1yA3IVY"

#define RXD2 27
#define TXD2 26
#define powerPin 4
#define TINY_GSM_MODEM_SIM7600
#include <Arduino.h>
#include <Wire.h> 
#include <BlynkSimpleTinyGSM.h>


#define LED 2
int s0 = 1;
int s1 = 1;
int s2 = 1;
int s3 = 1;

const int Line_1_up_r = 36;
const int ledPin_1 = V1;
const int Line_1_up_g = 39;
const int ledPin_2 = V2;
const int Line_1_dn_r = 34;
const int ledPin_3 = V3;
const int Line_1_dn_g = 35;
const int ledPin_4 = V6;

const int Decoder_input_1 = 17;//LSB
const int Decoder_input_2 = 18;
const int Decoder_input_3 = 19;
const int Decoder_input_4 = 16;//MSB- Enable pin

int Line_1_up_r_State = 0 ;
int Line_1_up_g_State = 0;
int Line_1_dn_r_State = 0;
int Line_1_dn_g_State = 0;

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

//void IRAM_ATTR onTimer1() {
// timer_1_minute = 1;
//  Code for sending data to cloud 
//}


int error = 0;
hw_timer_t *My_timer = NULL;
void IRAM_ATTR onTimer() {
  digitalWrite(LED, !digitalRead(LED));
  int Line_1_up_r_State = digitalRead(Line_1_up_r);
  int Line_1_up_g_State = digitalRead(Line_1_up_g);
  int Line_1_dn_r_State = digitalRead(Line_1_dn_r);
  int Line_1_dn_g_State = digitalRead(Line_1_dn_g);
  
  if (Line_1_up_r_State == 0 && Line_1_up_g_State == 1 && Line_1_dn_r_State == 0 && Line_1_dn_g_State == 1)
  { Serial.println("1");

    if ( s0 == 1 && s1 == 1 && s2 == 1 && s3 == 1)
    { Serial.println("2");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);
      digitalWrite(Decoder_input_4, LOW);
      s0 = 0;
      s1 = 0;
      s2 = 0;
      s3 = 0;
    }
  
#if 1
  else if ( s0 == 0 && s1 == 0 && s2 == 0 && s3 == 0)
  { Serial.println("3");
    digitalWrite(Decoder_input_1, LOW);
    digitalWrite(Decoder_input_2, HIGH);
    digitalWrite(Decoder_input_3, LOW);
    digitalWrite(Decoder_input_4, LOW);
    s0 = 0;
    s1 = 1;
    s2 = 0;
    s3 = 0;
  }
}

else if (Line_1_up_r_State == 1 && Line_1_up_g_State == 0 && Line_1_dn_r_State == 1 && Line_1_dn_g_State == 0)
{ Serial.println("4");
  if ( s0 == 1 && s1 == 1 && s2 == 1 && s3 == 1)
  { Serial.println("5");
    digitalWrite(Decoder_input_1, HIGH);
    digitalWrite(Decoder_input_2, LOW);
    digitalWrite(Decoder_input_3, LOW);
    digitalWrite(Decoder_input_4, LOW);
    s0 = 1;
    s1 = 0;
    s2 = 0;
    s3 = 0;
  }
  else if ( s0 == 1 && s1 == 0 && s2 == 0 && s3 == 0)
  { Serial.println("6");
    digitalWrite(Decoder_input_1, LOW);
    digitalWrite(Decoder_input_2, LOW);
    digitalWrite(Decoder_input_3, LOW);
    digitalWrite(Decoder_input_4, LOW);
    s0 = 0;
    s1 = 0;
    s2 = 0;
    s3 = 0;
  }
  else if ( s0 == 0 && s1 == 0 && s2 == 0 && s3 == 0)
  { Serial.println("7");
    digitalWrite(Decoder_input_1, HIGH);
    digitalWrite(Decoder_input_2, LOW);
    digitalWrite(Decoder_input_3, LOW);
    digitalWrite(Decoder_input_4, LOW);
    s0 = 1;
    s1 = 0;
    s2 = 0;
    s3 = 0;
  }
}
#endif

}

void setup() {
  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
  timerAlarmWrite(My_timer, 5000, true);
  timerAlarmEnable(My_timer);

//  My_timer_1 = timerBegin(0, 80, true);
//  timerAttachInterrupt(My_timer_1, &onTimer1, true);  // timer for 1 minute 
//  timerAlarmWrite(My_timer_1, 60000000, true);
//  timerAlarmEnable(My_timer_1);

  pinMode(Decoder_input_1, OUTPUT);
  pinMode(Decoder_input_2, OUTPUT);
  pinMode(Decoder_input_3, OUTPUT);
  pinMode(Decoder_input_4, OUTPUT);
  pinMode(Line_1_up_r, INPUT_PULLUP);
  pinMode(Line_1_up_g, INPUT_PULLUP);
  pinMode(Line_1_dn_r, INPUT_PULLUP);
  pinMode(Line_1_dn_g, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

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

  Blynk.begin(auth, modem, apn, user, pass);
  
}

void loop() {

  Blynk.run();
  // put your main code here, to run repeatedly:
  int Line_1_up_r_State = digitalRead(Line_1_up_r);
  int Line_1_up_g_State = digitalRead(Line_1_up_g);
  int Line_1_dn_r_State = digitalRead(Line_1_dn_r);
  int Line_1_dn_g_State = digitalRead(Line_1_dn_g);

  if ((Line_1_up_r_State == 0 && Line_1_up_g_State == 0 && Line_1_dn_r_State == 1 && Line_1_dn_g_State == 1) || (Line_1_up_r_State == 1 && Line_1_up_g_State == 1 && Line_1_dn_r_State == 0 && Line_1_dn_g_State == 0))
  {
    timerStop(My_timer);
    digitalWrite(LED, !digitalRead(LED));
    delay(500);
    digitalWrite(Decoder_input_1, HIGH);
    digitalWrite(Decoder_input_2, HIGH);
    digitalWrite(Decoder_input_3, HIGH);
    digitalWrite(Decoder_input_4, HIGH);
    delay(1000);
    digitalWrite(Decoder_input_1, LOW);
    digitalWrite(Decoder_input_2, LOW);
    digitalWrite(Decoder_input_3, LOW);
    digitalWrite(Decoder_input_4, LOW);
    Serial.println("Error led on ");
    Serial.println("8");

    Blynk.virtualWrite(ledPin_4, HIGH);  // Turn on the LED on the Blynk app
    Blynk.virtualWrite(ledPin_2, HIGH);  // Turn off the LED on the Blynk app
    Blynk.virtualWrite(ledPin_3, HIGH);  // Turn off the LED on the Blynk app
    Blynk.virtualWrite(ledPin_1, HIGH);  // Turn off the LED on the Blynk app

    error = 1;
  }
  else if (error == 1)
  { Serial.println("9");
    timerStart(My_timer);
  }

  if (Line_1_up_r_State == 0 && Line_1_up_g_State == 1 && Line_1_dn_r_State == 0 && Line_1_dn_g_State == 1)
  { Serial.println("10");
    Blynk.virtualWrite(ledPin_4, LOW);  // Turn on the LED on the Blynk app
    Blynk.virtualWrite(ledPin_3, HIGH);  // Turn off the LED on the Blynk app
    Blynk.virtualWrite(ledPin_2, LOW);  // Turn off the LED on the Blynk app
    Blynk.virtualWrite(ledPin_1, HIGH);  // Turn off the LED on the Blynk app

    Serial.println("Line_1_up_r is on  and Line_1_dn_r is on ");

  } 
  if (Line_1_up_r_State == 1 && Line_1_up_g_State == 0 && Line_1_dn_r_State == 1 && Line_1_dn_g_State == 0)
  { Serial.println("11");
    Blynk.virtualWrite(ledPin_4, HIGH);  // Turn on the LED on the Blynk app
    Blynk.virtualWrite(ledPin_3, LOW);  // Turn off the LED on the Blynk app
    Blynk.virtualWrite(ledPin_2, HIGH);  // Turn off the LED on the Blynk app
    Blynk.virtualWrite(ledPin_1, LOW);  // Turn off the LED on the Blynk app

    Serial.println("Line_1_up_g is on  and Line_1_dn_g is on ");
  } 
}
