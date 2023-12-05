#define LED 2
//int s0 = 1;
//int s1 = 1;
//int s2 = 1;
//int s3 = 1;

const int Line_1_up_r = 32;

const int Line_1_up_g = 33;

const int Line_1_dn_r = 25;

const int Line_1_dn_g = 26;


const int Decoder_input_1 = 17;//LSB
const int Decoder_input_2 = 18;
const int Decoder_input_3 = 19;
const int Decoder_input_4 = 16;//MSB- Enable pin

int Line_1_up_r_State = 0 ;
int Line_1_up_g_State = 0;
int Line_1_dn_r_State = 0;
int Line_1_dn_g_State = 0;


//int error = 0;
//hw_timer_t *My_timer = NULL;
//void IRAM_ATTR onTimer() {
//  digitalWrite(LED, !digitalRead(LED));
//  int Line_1_up_r_State = digitalRead(Line_1_up_r);
//  int Line_1_up_g_State = digitalRead(Line_1_up_g);
//  int Line_1_dn_r_State = digitalRead(Line_1_dn_r);
//  int Line_1_dn_g_State = digitalRead(Line_1_dn_g);
//
// // int  signalCategory ;
// 
//    
//  if (Line_1_up_r_State == 0 && Line_1_up_g_State == 1 && Line_1_dn_r_State == 0 && Line_1_dn_g_State == 1 )
//  { Serial.println("1");
//
//    if ( s0 == 1 && s1 == 1 && s2 == 1 && s3 == 1)
//    { Serial.println("2");
//      digitalWrite(Decoder_input_1, LOW);
//      digitalWrite(Decoder_input_2, LOW);
//      digitalWrite(Decoder_input_3, LOW);
//      digitalWrite(Decoder_input_4, LOW);
//      s0 = 0;
//      s1 = 0;
//      s2 = 0;
//      s3 = 0;
//    }
//  
//#if 1
//  else if ( s0 == 0 && s1 == 0 && s2 == 0 && s3 == 0)
//  { Serial.println("3");
//    digitalWrite(Decoder_input_1, LOW);
//    digitalWrite(Decoder_input_2, HIGH);
//    digitalWrite(Decoder_input_3, LOW);
//    digitalWrite(Decoder_input_4, LOW);
//    s0 = 0;
//    s1 = 1;
//    s2 = 0;
//    s3 = 0;
//  }
//}
//
//else if (Line_1_up_r_State == 1 && Line_1_up_g_State == 0 && Line_1_dn_r_State == 1 && Line_1_dn_g_State == 0 )
//{ Serial.println("4");
//  if ( s0 == 1 && s1 == 1 && s2 == 1 && s3 == 1)
//  { Serial.println("5");
//    digitalWrite(Decoder_input_1, HIGH);
//    digitalWrite(Decoder_input_2, LOW);
//    digitalWrite(Decoder_input_3, LOW);
//    digitalWrite(Decoder_input_4, LOW);
//    s0 = 1;
//    s1 = 0;
//    s2 = 0;
//    s3 = 0;
//  }
//  else if ( s0 == 1 && s1 == 0 && s2 == 0 && s3 == 0)
//  { Serial.println("6");
//    digitalWrite(Decoder_input_1, LOW);
//    digitalWrite(Decoder_input_2, LOW);
//    digitalWrite(Decoder_input_3, LOW);
//    digitalWrite(Decoder_input_4, LOW);
//    s0 = 0;
//    s1 = 0;
//    s2 = 0;
//    s3 = 0;
//  }
//  else if ( s0 == 0 && s1 == 0 && s2 == 0 && s3 == 0)
//  { Serial.println("7");
//    digitalWrite(Decoder_input_1, HIGH);
//    digitalWrite(Decoder_input_2, LOW);
//    digitalWrite(Decoder_input_3, LOW);
//    digitalWrite(Decoder_input_4, LOW);
//    s0 = 1;
//    s1 = 0;
//    s2 = 0;
//    s3 = 0;
//  }
//}
//#endif
//
//}
//





void setup() {
//  My_timer = timerBegin(0, 80, true);
//  timerAttachInterrupt(My_timer, &onTimer, true);
//  timerAlarmWrite(My_timer, 15000, true);
//  timerAlarmEnable(My_timer);


  pinMode(Decoder_input_1, OUTPUT);
  pinMode(Decoder_input_2, OUTPUT);
  pinMode(Decoder_input_3, OUTPUT);
  pinMode(Decoder_input_4, OUTPUT);
  pinMode(Line_1_up_r, INPUT_PULLUP);
  pinMode(Line_1_up_g, INPUT_PULLUP);
  pinMode(Line_1_dn_r, INPUT_PULLUP);
  pinMode(Line_1_dn_g, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
   Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:int Line_1_up_r_State = digitalRead(Line_1_up_r);
  int Line_1_up_r_State = digitalRead(Line_1_up_r); // R1 led - 0000
  int Line_1_up_g_State = digitalRead(Line_1_up_g);// G1 led - 0001

  int Line_1_dn_r_State = digitalRead(Line_1_dn_r);// R2 - 0010
  int Line_1_dn_g_State = digitalRead(Line_1_dn_g); // G2 - 0011

  Serial.println("Red_up=:");
  Serial.println(Line_1_up_r_State);
  Serial.println("green_up=:");
  Serial.println(Line_1_up_g_State);
  Serial.println("Red_dn=:");
  Serial.println(Line_1_dn_r_State);
  Serial.println("green_dn=:");
  Serial.println(Line_1_dn_g_State);
  delay(1000);


//Signals are Active low

// 0101
  if (Line_1_up_r_State == 0 && Line_1_up_g_State == 1 && Line_1_dn_r_State == 0 && Line_1_dn_g_State == 1 )  
  { Serial.println("1");
    Serial.println("Line_1_up_r is on  and Line_1_dn_r is on ");
    
    
      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, LOW);
      digitalWrite(Decoder_input_3, LOW);  //0000 Red up
      digitalWrite(Decoder_input_4, LOW);
       Serial.println(Decoder_input_1);
       Serial.println(Decoder_input_2);
       Serial.println(Decoder_input_3);
       Serial.println(Decoder_input_4);
      delay(1000);

      digitalWrite(Decoder_input_1, LOW);
      digitalWrite(Decoder_input_2, HIGH);
      digitalWrite(Decoder_input_3, LOW);   //0010 Red down
      digitalWrite(Decoder_input_4, LOW);
       Serial.println(Decoder_input_1);
       Serial.println(Decoder_input_2);
       Serial.println(Decoder_input_3);
       Serial.println(Decoder_input_4);
      delay(1000);
  }

// 1010
  if (Line_1_up_r_State == 1 && Line_1_up_g_State == 0 && Line_1_dn_r_State == 1 && Line_1_dn_g_State == 0 )  
  
  { Serial.println("2");
    Serial.println("Line_1_up_g is on  and Line_1_dn_g is on ");
    
    digitalWrite(Decoder_input_1, HIGH);
    digitalWrite(Decoder_input_2, LOW);
    digitalWrite(Decoder_input_3, LOW);   //0001 Green up
    digitalWrite(Decoder_input_4, LOW);
      Serial.println(Decoder_input_1);
      Serial.println(Decoder_input_2);
      Serial.println(Decoder_input_3);
      Serial.println(Decoder_input_4);
    delay(1000);

    digitalWrite(Decoder_input_1, HIGH);
    digitalWrite(Decoder_input_2, HIGH);
    digitalWrite(Decoder_input_3, LOW);  //0011 Green down
    digitalWrite(Decoder_input_4, LOW);
      Serial.println(Decoder_input_1);
      Serial.println(Decoder_input_2);
      Serial.println(Decoder_input_3);
      Serial.println(Decoder_input_4);
    delay(1000);
  }

// 1001
  if (Line_1_up_r_State == 1 && Line_1_up_g_State == 0 && Line_1_dn_r_State == 0 && Line_1_dn_g_State == 1 )  
  
  { Serial.println("3");
    Serial.println("Line_1_up_g is on  and Line_1_dn_r is on ");
    
    digitalWrite(Decoder_input_1, HIGH);
    digitalWrite(Decoder_input_2, LOW);
    digitalWrite(Decoder_input_3, LOW);   //0001 Green up
    digitalWrite(Decoder_input_4, LOW);
      Serial.println(Decoder_input_1);
      Serial.println(Decoder_input_2);
      Serial.println(Decoder_input_3);
      Serial.println(Decoder_input_4);
    delay(1000);

    digitalWrite(Decoder_input_1, LOW);
    digitalWrite(Decoder_input_2, HIGH);
    digitalWrite(Decoder_input_3, LOW);  //0010 Red down
    digitalWrite(Decoder_input_4, LOW);
      Serial.println(Decoder_input_1);
      Serial.println(Decoder_input_2);
      Serial.println(Decoder_input_3);
      Serial.println(Decoder_input_4);
    delay(1000);
  }

// 0110
  if (Line_1_up_r_State == 0 && Line_1_up_g_State == 1 && Line_1_dn_r_State == 1 && Line_1_dn_g_State == 0 )  
  
  { Serial.println("4");
    Serial.println("Line_1_up_r is on  and Line_1_dn_g is on ");
    
    digitalWrite(Decoder_input_1, LOW);
    digitalWrite(Decoder_input_2, LOW);
    digitalWrite(Decoder_input_3, LOW);   //0000 Red up
    digitalWrite(Decoder_input_4, LOW);
      Serial.println(Decoder_input_1);
      Serial.println(Decoder_input_2);
      Serial.println(Decoder_input_3);
      Serial.println(Decoder_input_4);
    delay(1000);

    digitalWrite(Decoder_input_1, HIGH);
    digitalWrite(Decoder_input_2, HIGH);
    digitalWrite(Decoder_input_3, LOW);  //0011 Green down
    digitalWrite(Decoder_input_4, LOW);
      Serial.println(Decoder_input_1);
      Serial.println(Decoder_input_2);
      Serial.println(Decoder_input_3);
      Serial.println(Decoder_input_4);
    delay(1000);
  }



  else 

  {

   Serial.println("no switch pressed  ");
    
    digitalWrite(Decoder_input_1, HIGH);
    digitalWrite(Decoder_input_2, HIGH);
    digitalWrite(Decoder_input_3, HIGH);
    digitalWrite(Decoder_input_4, HIGH);
    delay(1000);
    
  }

  
//  if ((Line_1_up_r_State == 0 && Line_1_up_g_State == 0 && Line_1_dn_r_State == 1 && Line_1_dn_g_State == 1) || (Line_1_up_r_State == 1 && Line_1_up_g_State == 1 && Line_1_dn_r_State == 0 && Line_1_dn_g_State == 0))
//  {
//    
//    digitalWrite(LED, !digitalRead(LED));
//    delay(500);
//    digitalWrite(Decoder_input_1, HIGH);
//    digitalWrite(Decoder_input_2, HIGH);
//    digitalWrite(Decoder_input_3, HIGH);
//    digitalWrite(Decoder_input_4, HIGH);
//    delay(1000);
//    digitalWrite(Decoder_input_1, LOW);
//    digitalWrite(Decoder_input_2, LOW);
//    digitalWrite(Decoder_input_3, LOW);
//    digitalWrite(Decoder_input_4, LOW);
//    Serial.println("Error led on ");
//    delay(1000);
//    Serial.println("8");
//
//    
//    
//  }




}
  
 
 
