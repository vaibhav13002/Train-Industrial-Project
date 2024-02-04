#define TINY_GSM_MODEM_SIM7600  // SIM7600 AT instruction is compatible with A7670
#define SerialAT Serial1
#define SerialMon Serial
#define TINY_GSM_USE_GPRS true
#include <TinyGsmClient.h>
#include <esp32-hal-timer.h>
#define RXD2 27    //VVM501 MODULE RXD INTERNALLY CONNECTED
#define TXD2 26    //VVM501 MODULE TXD INTERNALLY CONNECTED
#define powerPin 4 ////VVM501 MODULE ESP32 PIN D4 CONNECTED TO POWER PIN OF A7670C CHIPSET, INTERNALLY CONNECTED

int rx = -1;
#define SerialAT Serial1
#define ALERT_PHONE_NUMBER "+919321562213"
String rxString;
int _timeout;
String _buffer;
String number = "+919321562213"; //REPLACE WITH YOUR NUMBER

int  signalCategory_1 =0;
int Failure = 0;
 int RECEIVE = 1;
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


int prev_Main_up_r_1_State=-1;
int prev_Main_up_g_1__State=-1;
int prev_Main_dn_r_2_State=-1;
int prev_Main_dn_g_2__State=-1;

int prev_Third_up_r_3_State=-1;
int prev_Third_up_g_3_State=-1;
int prev_Third_dn_r_4_State=-1;
int prev_Third_dn_g_4_State=-1;


int R1=0;
int G1=0;
int R2=0;
int G2=0;

int R3=0;
int G3=0;
int R4=0;
int G4=0;

String stationCode = "123456";

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
int Main_up_r_1_State = digitalRead(Main_up_r_1); // R1 led - 0000
int Main_up_g_1__State = digitalRead(Main_up_g_1);// G1 led - 0001
int Main_dn_r_2_State = digitalRead(Main_dn_r_2); // R1 led - 0000
int Main_dn_g_2__State = digitalRead(Main_dn_g_2);// G1 led - 0001

int Third_up_r_3_State = digitalRead(Third_up_r_3); // R1 led - 0000
int Third_up_g_3_State = digitalRead(Third_up_g_3);// G1 led - 0001
int Third_dn_r_4_State = digitalRead(Third_dn_r_4); // R1 led - 0000
int Third_dn_g_4_State = digitalRead(Third_dn_g_4);// G1 led - 0001
  
  Serial.println("Main_up_r_1=:");
  Serial.println(Main_up_r_1_State);
  Serial.println("Main_up_g_1=:");
  Serial.println(Main_up_g_1__State);
  Serial.println("Main_dn_r_2=:");
  Serial.println(Main_dn_r_2_State);
  Serial.println("Main_dn_g_2=:");
  Serial.println(Main_dn_g_2__State);


  Serial.println("Third_up_r_3=:");
  Serial.println(Third_up_r_3_State);
  Serial.println("Third_up_g_3=:");
  Serial.println(Third_up_g_3_State);
  Serial.println("Third_dn_r_4=:");
  Serial.println(Third_dn_r_4_State);
  Serial.println("Third_dn_g_4=:");
  Serial.println(Third_dn_g_4_State);



  Serial.println("prev_Main_up_r_1=:");
  Serial.println(prev_Main_up_r_1_State);
  Serial.println("prev_Main_up_g_1=:");
  Serial.println(prev_Main_up_g_1__State);
  Serial.println("prev_Main_dn_r_2=:");
  Serial.println(prev_Main_dn_r_2_State);
  Serial.println("prev_Main_dn_g_2=:");
  Serial.println(prev_Main_dn_g_2__State);


  Serial.println("prev_Third_up_r_3=:");
  Serial.println(prev_Third_up_r_3_State);
  Serial.println("prev_Third_up_g_3=:");
  Serial.println(prev_Third_up_g_3_State);
  Serial.println("prev_Third_dn_r_4=:");
  Serial.println(prev_Third_dn_r_4_State);
  Serial.println("prev_Third_dn_g_4=:");
  Serial.println(prev_Third_dn_g_4_State);


  delay(5);
  
  int Signal_Strength=getSignalStrength();

Serial.println("Signal_strength_value");
Serial.println(Signal_Strength);



Serial.println("Signal_strength");
Serial.println(signalCategory_1);


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
Serial.println("ALL_states");                         
Serial.println(eightDigitCode);


String timestamp;
  SerialAT.println("AT+CCLK?");
  delay(10);
  if (SerialAT.available()) {
    String response = SerialAT.readString();
    int index = response.indexOf("+CCLK: \"");
    if (index != -1) {
      String dateTimeStr = response.substring(index + 8, index + 26); // extract date and time string
      // dateTimeStr should have the format YY/MM/DD,hh:mm:ss+TZ
      int year = dateTimeStr.substring(0, 2).toInt() + 2000; // extract year and add 2000 to get the full year
      int month = dateTimeStr.substring(3, 5).toInt(); // extract month
      int day = dateTimeStr.substring(6, 8).toInt(); // extract day
      int hour = dateTimeStr.substring(9, 11).toInt(); // extract hour
      int minute = dateTimeStr.substring(12, 14).toInt(); // extract minute
      int second = dateTimeStr.substring(15, 17).toInt(); // extract second
      String timeZone = dateTimeStr.substring(18, 21); // extract timezone
      timestamp = String(day) + "-" + String(month) + "-" + String(year) + " " + String(hour) + ":" + String(minute) + ":" + String(second); // construct date and time string in day-month-year hour:minutes:seconds format
      Serial.println(timestamp);
//String message = "Code: " + eightDigitCode + " Station: " + stationCode;
    }
  }

    if ((Main_up_r_1_State != prev_Main_up_r_1_State)||(Main_up_g_1_State != prev_Main_up_g_1State)||(Main_dn_g_2State != prev_Main_dn_g_2_State)||(Main_dn_r_2_State != prev_Main_dn_r_2_State)||(Third_dn_r_4_State != prev_Third_dn_r_4_State)||(Third_dn_g_4_State != prev_Third_dn_g_4_State)||(Third_up_g_3_State != prev_Third_up_g_3_State)||(Third_up_r_3_State != prev_Third_up_r_3_State))
    {
    String message = "Code: " + eightDigitCode + " Station: " + stationCode+ " Time: " + timestamp ;
    Serial.println("Codeword");                         
    Serial.println(message);
    }

//String timestamp =  getTimeString();
  

//if (Serial.available() > 0)
//    {
//      Serial.println("Test message");
//    SendMessage();   //YOU CAN SEND MESSAGE FROM SIM TO THE MENTIONED PHONE NUMBER
//    }

//String message = "Code: " + eightDigitCode + " Station: " + stationCode + " Time: " + getDateTime();

//if (SerialAT.available() > 0)
//    Serial.write(SerialAT.read());
//    Serial.println("Main message");
////    String message = "Code: " + eightDigitCode + " Station: " + stationCode+ " Time: " + getDateTime() ;
////    Serial.println("Codeword");                         
////    Serial.println(message);
//    sendSMS(message);  


if (Main_up_r_1_State != prev_Main_up_r_1_State) {
    // State has changed
  7  prev_Main_up_r_1_State = Main_up_r_1_State;

  
if (Main_up_r_1_State == 0 ) 
   
  { Serial.println("1");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
    //  delay(100);

    Main_up_g_1();
  }

  if (Main_up_r_1_State == 1 ) 
   
  { Serial.println("2");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
     // delay(100);

     Main_up_g_1();
  }
  
}

if (Main_up_g_1_State != prev_Main_up_g_1_State) {
    // State has changed
    prev_Main_up_g_1_State = Main_up_g_1_State;

  if (Main_up_g_1__State == 0 ) 
   
  { Serial.println("3");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
     // delay(100);
  }

  

  if (Main_up_g_1__State == 1 ) 
   
  { Serial.println("4");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
     // delay(100);
  }
}

if (Main_dn_g_2_State != prev_Main_dn_g_2_State) {
    // State has changed
    prev_Main_dn_g_2_State = Main_dn_g_2_State;


if (Main_dn_g_2__State == 0 ) 
   
  { Serial.println("5");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
     // delay(100);
  }

  if (Main_dn_g_2__State == 1 ) 
   
  { Serial.println("6");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
  //    delay(100);
  }

}

if (Main_dn_r_2_State != prev_Main_dn_r_2_State) {
    // State has changed
    prev_Main_dn_r_2_State = Main_dn_r_2_State;

  if (Main_dn_r_2_State == 0 ) 
   
  { Serial.println("7");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
    //  delay(100);
  }

  if (Main_dn_r_2_State == 1 ) 
   
  { Serial.println("8");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
      //delay(100);
  }

  if (Third_dn_r_4_State != prev_Third_dn_r_4_State) {
    // State has changed
    prev_Third_dn_r_4_State = Third_dn_r_4_State;


if (Third_dn_r_4_State == 0 ) 
   
  { Serial.println("9");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
      //delay(100);
  }



  if (Third_dn_r_4_State == 1 ) 
   
  { Serial.println("10");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
      //delay(100);
  }
  }

if (Third_dn_g_4_State != prev_Third_dn_g_4_State) {
    // State has changed
    prev_Third_dn_g_4_State = Third_dn_g_4_State;

  
 if (Third_dn_g_4_State == 0 ) 
   
  { Serial.println("11");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
      //delay(100);
  }



  if (Third_dn_g_4_State == 1 ) 
   
  { Serial.println("12");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
      //delay(100);
  }
}

if (Third_up_g_3_State != prev_Third_up_g_3_State) {
    // State has changed
    prev_Third_up_g_3_State = Third_up_g_3_State;
  
if (Third_up_g_3_State == 0 ) 
   
  { Serial.println("13");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
     // delay(100);
  }


  if (Third_up_g_3_State == 1 ) 
   
  { Serial.println("14");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
      //delay(100);
  }
}

if (Third_up_r_3_State != prev_Third_up_r_3_State) {
    // State has changed
    prev_Third_up_r_3_State = Third_up_r_3_State;
  

if (Third_up_r_3_State == 0 ) 
   
  { Serial.println("15");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
      //delay(100);
  }

  if (Third_up_r_3_State == 1 ) 
   
  { Serial.println("16");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
      //delay(100);
  }
}
if (signalCategory_1 == 2)

  {   Serial.println("23");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, HIGH);
     // delay(100);
    
  }

 if (signalCategory_1 != 2)

  {   Serial.println("24");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
      //delay(100);
    
  }

  if (signalCategory_1 == 3)

  {   Serial.println("25");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, HIGH);
      //delay(100);
    
  }

 if (signalCategory_1 != 3)

  {   Serial.println("26");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
      //delay(100);
    
  }

if (RECEIVE == 1)

  {   Serial.println("29");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, HIGH);
      //delay(100);
    
  }

 if (RECEIVE == 0)

  {   Serial.println("30");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
     // delay(100);
    
  }

if (TRANSMIT == 1)

  {   Serial.println("27");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, HIGH);
     // delay(100);
    
  }

 if (TRANSMIT == 0)

  {   Serial.println("28");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, HIGH);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
     // delay(100);
    
  }

if (Failure == 1)

  {   Serial.println("19");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, HIGH);
     // delay(100);
    
  }

 if (Failure == 0)

  {   Serial.println("20");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
      //delay(100);
    
  }


if (signalCategory_1 == 1)

  {   Serial.println("21");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, HIGH);
      //delay(100);
    
  }

 if (signalCategory_1 != 1)

  {   Serial.println("22");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
      //delay(100);
    
  }

{     Serial.println("18");
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
     // delay(100);
      
  }

{  Serial.println("17");
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, HIGH);
      digitalWrite(data_decoder, LOW);
      //delay(100);
      
  }



  }
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
//void SendMessage()
//{
//  //Serial.println ("Sending Message");
//  SerialAT.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
//  delay(1000);
//  //Serial.println ("Set SMS Number");
//  SerialAT.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
//  delay(1000);
//  String SMS = "MESSAGE FROM VVM501 ESP32 4G LTE MODULE";
//  SerialAT.println(SMS);
//  delay(100);
//  SerialAT.println((char)26);// ASCII code of CTRL+Z
//  delay(1000);
//  _buffer = _readSerial();
//}
//void sendSMS(String message) {
//  SerialAT.println("AT+CMGF=1"); // Set SMS mode to text
//  delay(500);
//
//  SerialAT.print("AT+CMGS=\"");
//  SerialAT.print(ALERT_PHONE_NUMBER);
//  SerialAT.println("\"");
//
//  delay(1000);
//  SerialAT.print(message);
//  SerialAT.write(26); // End of message character
//  delay(1000);
//}
void Main_up_g_1()
{
if (Main_up_g_1__State == 0 ) 
   
  { Serial.println("3_F");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, HIGH);
     // delay(100);
  }

  

  if (Main_up_g_1__State == 1 ) 
   
  { Serial.println("4_F");
    Serial.println("Main Line_up_r_1 is on  and Main Line_up_g_1 is off ");

        
      digitalWrite(Decoder_input_1, HIGH);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
      digitalWrite(data_decoder, LOW);
     // delay(100);
  }
}