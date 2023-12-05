#define LED 2
//int s0 = 1;
//int s1 = 1;
//int s2 = 1;
//int s3 = 1;

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


void setup() {
//  My_timer = timerBegin(0, 80, true);
//  timerAttachInterrupt(My_timer, &onTimer, true);
//  timerAlarmWrite(My_timer, 15000, true);
//  timerAlarmEnable(My_timer);

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
}
  
 
 
