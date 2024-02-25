// Finally decided to go with RTC module for realtime date and time (Coz AT+CCLK is giving incorrect time (approx near time in case of Jio Sim...issue is with jio sim not the case with Airtel sim))
// Time from Ntp server and inbult library also failed coz of jio sim
// 10/feb/2024  update

#include <stdio.h>
#include <string.h>
#include <Wire.h>
#include <RTClib.h>

#define TINY_GSM_MODEM_SIM7600  // SIM7600 AT instruction is compatible with A7670
#define SerialAT Serial1
#define SerialMon Serial
#define TINY_GSM_USE_GPRS true
#include <TinyGsmClient.h>
#include <esp32-hal-timer.h>
#define RXD2 27    //VVM501 MODULE RXD INTERNALLY CONNECTED
#define TXD2 26    //VVM501 MODULE TXD INTERNALLY CONNECTED
#define powerPin 4 ////VVM501 MODULE ESP32 PIN D4 CONNECTED TO POWER PIN OF A7670C CHIPSET, INTERNALLY CONNECTED
#define USE_SSL true
#define DELAY_MS 500

#define DEBUG true

RTC_DS1307 rtc;


const String FIREBASE_HOST  = "https://railway-rakshak-default-rtdb.asia-southeast1.firebasedatabase.app/Stations/virar";
const String FIREBASE_SECRET  = "dtgQBVf1j2vbYl6s8P1CBLFQJDZVRQ5p6LRemtx9";


boolean waitResponse(String expected_answer="OK", unsigned int timeout=2000);
int rx = -1;
#define SerialAT Serial1
#define ALERT_PHONE_NUMBER "+919960491371"
String rxString;
int _timeout;
String _buffer;
String number = "+919321562213"; //REPLACE WITH YOUR NUMBER

int  signalCategory_1 =0;
int Failure = 0;
 int RECEIVE = 0;
 int TRANSMIT = 0;

char a, b;
const char apn[] = "";

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

//hw_timer_t *timer = NULL;
//volatile int ledState = 0; // 0: Red, 1: Green, 2: Yellow
//
//void toggleLED() {
//  // Toggle the LED state
//  ledState = (ledState + 1) % 3; // Cycle between 0, 1, and 2
//}

int getSignalStrength() {
  SerialAT.println("AT+CSQ");
  delay(10);
  int signalStrength = 0;

  if (SerialAT.find("+CSQ: ")) {
    signalStrength = SerialAT.parseInt();
  }

  int dBmSignalStrength = -113 + 2 * signalStrength;
  SerialAT.readStringUntil('\n');
  SerialAT.println(dBmSignalStrength);
  return dBmSignalStrength;
}

const int Main_up_r_1 = 25;

const int Main_up_g_1 = 13;

const int Main_dn_r_2 = 32;

const int Main_dn_g_2 = 33;


const int Third_up_r_3 = 34;

const int Third_up_g_3 = 35;

const int Third_dn_r_4 = 36;

const int Third_dn_g_4 = 39;


const int Decoder_input_1 = 17;//LSB
const int Decoder_input_2 = 18;
const int Decoder_input_3 = 19;
const int Decoder_input_4 = 16;//MSB- Enable pin
const int data_decoder = 2;

int Main_up_r_1_State = 0 ;
int Main_up_g_1__State = 0;
int Main_dn_r_2_State = 0 ;
int Main_dn_g_2__State = 0;

int Third_up_r_3_State = 0;
int Third_up_g_3_State = 0;
int Third_dn_r_4_State = 0;
int Third_dn_g_4_State = 0;

String Previous_codeword = "00000000";

int R1=0;
int G1=0;
int R2=0;
int G2=0;

int R3=0;
int G3=0;
int R4=0;
int G4=0;

String station_name = "Virar";
String station_address = "123456";


void setup() {

  pinMode(powerPin, OUTPUT);

  // put your setup code here, to run once:
  pinMode(Decoder_input_1, OUTPUT);
  pinMode(Decoder_input_2, OUTPUT);
  pinMode(Decoder_input_3, OUTPUT);
  pinMode(Decoder_input_4, OUTPUT);
  pinMode(data_decoder, OUTPUT);

  pinMode(Main_up_r_1, INPUT_PULLUP);
  pinMode(Main_up_g_1, INPUT_PULLUP);
  pinMode(Main_dn_r_2, INPUT_PULLUP);
  pinMode(Main_dn_g_2, INPUT_PULLUP);

  pinMode(Third_up_r_3, INPUT_PULLUP);
  pinMode(Third_up_g_3, INPUT_PULLUP);
  pinMode(Third_dn_r_4, INPUT_PULLUP);
  pinMode(Third_dn_g_4, INPUT_PULLUP);
  
  Serial.begin(115200);

      
  
  digitalWrite(powerPin, LOW);
  delay(100);
  digitalWrite(powerPin, HIGH);
  delay(1000);
  digitalWrite(powerPin, LOW);

  Serial.println("\nConfiguring VVM501 Module. Please wait...");
  delay(10000);
  SerialAT.begin(115200, SERIAL_8N1, RXD2, TXD2);

  

  if (!modem.init()) {
    Serial.println("Failed to initialize modem. Please check the connection.");
    return;
  }

  if (!modem.restart()) {
    Serial.println("Failed to restart modem. Please check the connection.");
    return;
  }

  String name = modem.getModemName();
  Serial.println("Modem Name: " + name);
  String modemInfo = modem.getModemInfo();
  Serial.println("Modem Info: " + modemInfo);
  Serial.println("Waiting for network...");

  if (!modem.waitForNetwork()) {
    Serial.println("Network registration failed. Please check the SIM card and signal.");
    delay(10000);
    return;
  }

  Serial.println("Network registration successful.");

  if (modem.isNetworkConnected()) {
    Serial.println("Network connected.");
  }

  Serial.print(F("Connecting to "));
  Serial.print(apn);

  if (!modem.gprsConnect(apn)) {
    Serial.println("GPRS connection failed. Please check the APN settings.");
    delay(10000);
    return;
  }

  Serial.println("GPRS connection successful.");

  if (modem.isGprsConnected()) {
    Serial.println("LTE module connected.");
  }

  Serial.println("Enter standard AT commands like AT, AT+CPIN?, AT+CCLK?, etc.");
  Serial.println("Select Serial Port Monitor as \"Both NL & CR\" to view command responses correctly if you are using Arduino IDE.");
  Serial.println("Refer to the A7600 series datasheet for the entire list of commands.");
  Serial.println("Understand the AT Commands properly. Incorrect AT commands can corrupt the 4G module memory!");


 Serial.println("Modem Reset, please wait");
  SerialAT.println("AT+CRESET");
  delay(1000);
  SerialAT.println("AT+CRESET");
  delay(20000);  // WAITING FOR SOME TIME TO CONFIGURE MODEM

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
  Serial.println("4G HTTP Test Begin!");


  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  
  delay(1000);

  
  digitalWrite(data_decoder, LOW);
      
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      delay(10);
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      delay(10);
      
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      delay(10);
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      delay(10);
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      delay(10);
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      delay(10);
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      delay(10);
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      delay(10);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      delay(10);
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      delay(10);
      
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      delay(10);
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      delay(10);
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      delay(10);
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      delay(10);
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      delay(10);
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      delay(10);

}


void loop() 
{ 
   Serial.println("Previous_codeword");
   Serial.println(Previous_codeword);
  // delay(1000);
int Main_up_r_1_State = digitalRead(Main_up_r_1); // R1 led - 0000
int Main_up_g_1__State = digitalRead(Main_up_g_1);// G1 led - 0001
int Main_dn_r_2_State = digitalRead(Main_dn_r_2); // R1 led - 0000
int Main_dn_g_2__State = digitalRead(Main_dn_g_2);// G1 led - 0001

int Third_up_r_3_State = digitalRead(Third_up_r_3); // R1 led - 0000
int Third_up_g_3_State = digitalRead(Third_up_g_3);// G1 led - 0001
int Third_dn_r_4_State = digitalRead(Third_dn_r_4); // R1 led - 0000
int Third_dn_g_4_State = digitalRead(Third_dn_g_4);// G1 led - 0001
  
  // Serial.println("Main_up_r_1=:");
  // Serial.println(Main_up_r_1_State);
  // Serial.println("Main_up_g_1=:");
  // Serial.println(Main_up_g_1__State);
  // Serial.println("Main_dn_r_2=:");
  // Serial.println(Main_dn_r_2_State);
  // Serial.println("Main_dn_g_2=:");
  // Serial.println(Main_dn_g_2__State);


  // Serial.println("Third_up_r_3=:");
  // Serial.println(Third_up_r_3_State);
  // Serial.println("Third_up_g_3=:");
  // Serial.println(Third_up_g_3_State);
  // Serial.println("Third_dn_r_4=:");
  // Serial.println(Third_dn_r_4_State);
  // Serial.println("Third_dn_g_4=:");
  // Serial.println(Third_dn_g_4_State);


  

  delay(5);
  
  int Signal_Strength=getSignalStrength();

// Serial.println("Signal_strength_value");
// Serial.println(Signal_Strength);



// Serial.println("Signal_strength");
// Serial.println(signalCategory_1);


  if (Signal_Strength > -85) 
    { signalCategory_1 = 1;
    }

    else if (Signal_Strength > -95)
    { signalCategory_1 = 2;
    }

    else 
     { signalCategory_1 = 3;
    }


String eightDigitCode = String(Main_up_r_1_State) + String(Main_up_g_1__State) +
                          String(Main_dn_r_2_State) + String(Main_dn_g_2__State)+
                          String(Third_up_r_3_State) + String(Third_up_g_3_State) +
                          String(Third_dn_r_4_State) + String(Third_dn_g_4_State) ;
// Serial.println("ALL_states");                         
// Serial.println(eightDigitCode);

if (Main_up_r_1_State == 0 ) 
   
  {// { Serial.println("1");
    
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
    //  delay(100);
  }

  if (Main_up_r_1_State == 1 ) 
   
  {// { Serial.println("2");
   
    // Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
     // delay(100);
  }


  if (Main_up_g_1__State == 0 ) 
   
  {// { Serial.println("3");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
     // delay(100);
  }

  if (Main_up_g_1__State == 1 ) 
   
  {// { Serial.println("4");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
     // delay(100);
  }

if (Main_dn_g_2__State == 0 ) 
   
  {// { Serial.println("5");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");
// 
        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
     // delay(100);
  }

  if (Main_dn_g_2__State == 1 ) 
   
  {// { Serial.println("6");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
  //    delay(100);
  }


  if (Main_dn_r_2_State == 0 ) 
   
  {// { Serial.println("7");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
    //  delay(100);
  }

  if (Main_dn_r_2_State == 1 ) 
   
  {// { Serial.println("8");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
      //delay(100);
  }

if (Third_dn_r_4_State == 0 ) 
   
  {// { Serial.println("9");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
      //delay(100);
  }

  if (Third_dn_r_4_State == 1 ) 
   
  {// { Serial.println("10");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
      //delay(100);
  }

 if (Third_dn_g_4_State == 0 ) 
   
  {// { Serial.println("11");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
      //delay(100);
  }

  if (Third_dn_g_4_State == 1 ) 
   
 { // { Serial.println("12");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
      //delay(100);
  }
  
if (Third_up_g_3_State == 0 ) 
   
  {// { Serial.println("13");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
     // delay(100);
  }

  if (Third_up_g_3_State == 1 ) 
   
  {// { Serial.println("14");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
      //delay(100);
  }


if (Third_up_r_3_State == 0 ) 
   
  {// { Serial.println("15");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
      //delay(100);
  }

  if (Third_up_r_3_State == 1 ) 
   
  {// { Serial.println("16");
  //   Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
      //delay(100);
  }

if (signalCategory_1 == 2)

 { // {   Serial.println("23");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, HIGH);
     // delay(100);
    
  }

 if (signalCategory_1 != 2)

  {// {   Serial.println("24");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
      //delay(100);
    
  }

  if (signalCategory_1 == 3)

  {// {   Serial.println("25");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, HIGH);
      //delay(100);
    
  }

 if (signalCategory_1 != 3)

  {// {   Serial.println("26");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
      //delay(100);
    
  }

if (RECEIVE == 1)

  {// {   Serial.println("29");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, HIGH);
      //delay(100);
    
  }

 if (RECEIVE == 0)

  {// {   Serial.println("30");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
     // delay(100);
    
  }

if (TRANSMIT == 1)

  {// {   Serial.println("27");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, HIGH);
     // delay(100);
    
  }

 if (TRANSMIT == 0)

  {// {   Serial.println("28");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
     // delay(100);
    
  }

if (Failure == 1)

  {// {   Serial.println("19");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, HIGH);
     // delay(100);
    
  }

 if (Failure == 0)

  {// {   Serial.println("20");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
      //delay(100);
    
  }


if (signalCategory_1 == 1)

 { // {   Serial.println("21");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, HIGH);
      //delay(100);
    
  }

 if (signalCategory_1 != 1)

  {// {   Serial.println("22");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
      //delay(100);
    
  }

{
  // {     Serial.println("18");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
     // delay(100);
      
  }

{// {  Serial.println("17");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
      //delay(100);
      
  }
if(Previous_codeword != eightDigitCode)
    {
     

     String timestamp= getCurrentDateTime();
      
 

  
    String message = "Station_name: " + station_name +" Station_address: " + station_address +"Code: " + eightDigitCode +  " Time: " + timestamp ;
    // String message = "Code: " + eightDigitCode + " Station: " + stationCode+ " Time: " + timestamp ;
    Serial.println("Codeword");                         
    Serial.println(message);
    // RECEIVE = 1;


    
    // if (SerialAT.available() > 0)
    // Serial.write(SerialAT.read());
    // Serial.println("Main message");
   
   // sendSMS(message); 
    String Data1 = "{";

  Data1 += "\"data\":\"" + message + "\"";
  

  Data1 += "}";
    //  String data1 = get_data();
  post_to_firebase(Data1);
  
  delay(500);
 
    
   }

 



 Previous_codeword= eightDigitCode;











}




String _readSerial() {
  _timeout = 0;
  while  (!SerialAT.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (SerialAT.available()) {
    return SerialAT.readString();
  }
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


String getCurrentDateTime() {
  // Get the current date and time
  DateTime now = rtc.now();

  // Construct the date and time string
  String dateTimeString = String(now.year(), DEC) + '/' +
                          String(now.month(), DEC) + '/' +
                          String(now.day(), DEC) + ' ' +
                          String(now.hour(), DEC) + ':' +
                          String(now.minute(), DEC) + ':' +
                          String(now.second(), DEC);

  // Return the date and time string
  return dateTimeString;
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
delay(10);
//SerialAT.println(firebase_str);
SerialAT.println("AT+HTTPPARA=\"URL\",\"" + firebase_url + "\"");
waitResponse();
delay(10);
// Set the HTTP data size
SerialAT.println("AT+HTTPDATA=" + String(data1.length()) + ",10000");
waitResponse();
delay(10);


SerialAT.println(data1);
waitResponse();
delay(10);
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
  delay(10);
  //SerialAT.println("AT+HTTPREAD");
 // waitResponse("OK");
 // delay(500);

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

