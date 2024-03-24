#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>
#include <SPI.h> 
#include <MFRC522.h>
#include <Keypad.h>
#define SS_PIN 53
#define RST_PIN 5



int TRIG=11;
int ECHO=12; 
int BUZZER=13;
int DISTANCE_THRESHOLD = 3;
float duration_us, distance_cm;
int led1=24,led2=26,led3=28,led4=30,led5=32,led6=34,led7=36,led8=38,led9=40;
const int buzzer = 10;
const float songSpeed = 1.0;
const int ROWS = 4; 
const int COLS = 3;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {9,8,7,6};
byte colPins[COLS] = {2,4,3};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
const char* correctPassword = "3815";

#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_B5 988

int notes[] = {
    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
    NOTE_A4, NOTE_G4, NOTE_A4, 0,

    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
    NOTE_A4, NOTE_G4, NOTE_A4, 0,

    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0,
    NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
    NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,

    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_D5, NOTE_E5, NOTE_A4, 0,
    NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
    NOTE_C5, NOTE_A4, NOTE_B4, 0,

    NOTE_A4, NOTE_A4,
    //Repeat of first part
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
    NOTE_A4, NOTE_G4, NOTE_A4, 0,

    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
    NOTE_A4, NOTE_G4, NOTE_A4, 0,

    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0,
    NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
    NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,

    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_D5, NOTE_E5, NOTE_A4, 0,
    NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
    NOTE_C5, NOTE_A4, NOTE_B4, 0,
    //End of Repeat

    NOTE_E5, 0, 0, NOTE_F5, 0, 0,
    NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
    NOTE_D5, 0, 0, NOTE_C5, 0, 0,
    NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4,

    NOTE_E5, 0, 0, NOTE_F5, 0, 0,
    NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
    NOTE_D5, 0, 0, NOTE_C5, 0, 0,
    NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4};



int durations[] = {
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 125,

    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 125,

    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 125, 250, 125,

    125, 125, 250, 125, 125,
    250, 125, 250, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 375,

    250, 125,
    //Rpeat of First Part
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 125,

    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 125,

    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 125, 250, 125,

    125, 125, 250, 125, 125,
    250, 125, 250, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 375,
    //End of Repeat

    250, 125, 375, 250, 125, 375,
    125, 125, 125, 125, 125, 125, 125, 125, 375,
    250, 125, 375, 250, 125, 375,
    125, 125, 125, 125, 125, 500,

    250, 125, 375, 250, 125, 375,
    125, 125, 125, 125, 125, 125, 125, 125, 375,
    250, 125, 375, 250, 125, 375,
    125, 125, 125, 125, 125, 500};


byte readCard[4];
//SCIENTIST SANTHOSH ID  6E E1 BC 10
//PATTU ID 3D D0 30 5F
String MasterTag = "E39192F6";
String tagID = "";
MFRC522 mfrc522(SS_PIN, RST_PIN);



void lightAndSoundIndication()
{
if(distance_cm>=24)
  {
    Serial.print("Water Level is Low Low\n");
    digitalWrite(led1,HIGH);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    digitalWrite(led5,LOW);
    digitalWrite(led6,LOW);
    digitalWrite(led7,LOW);
    digitalWrite(led8,LOW);
    digitalWrite(led9,LOW);
    tone(BUZZER, 2000, 4500); 
  }
  else if(distance_cm<24 && distance_cm>=22)
  {
      Serial.print("Water Level is  Low Medium\n");
    digitalWrite(led1,LOW);
    digitalWrite(led2,HIGH);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    digitalWrite(led5,LOW);
    digitalWrite(led6,LOW);
    digitalWrite(led7,LOW);
    digitalWrite(led8,LOW);
    digitalWrite(led9,LOW);     
  }
  else if(distance_cm<22 && distance_cm>=20)
  {
      Serial.print("Water Level is  Low High\n");
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,HIGH);
    digitalWrite(led4,LOW);
    digitalWrite(led5,LOW);
    digitalWrite(led6,LOW);
    digitalWrite(led7,LOW);
    digitalWrite(led8,LOW);
    digitalWrite(led9,LOW);
  }
  else if(distance_cm<20 && distance_cm>=18)
  {
      Serial.print("Water Level is  Medium Low\n");
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,HIGH);
    digitalWrite(led5,LOW);
    digitalWrite(led6,LOW);
    digitalWrite(led7,LOW);
    digitalWrite(led8,LOW);
    digitalWrite(led9,LOW);
  }
  else if(distance_cm<18 && distance_cm>=16)
  {
      Serial.print("Water Level is  Medium Medium\n");
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    digitalWrite(led5,HIGH);
    digitalWrite(led6,LOW);
    digitalWrite(led7,LOW);
    digitalWrite(led8,LOW);
    digitalWrite(led9,LOW);
  }
  else if(distance_cm<16 && distance_cm>=14)
  {
      Serial.print("Water Level is Medium High\n");
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    digitalWrite(led5,LOW);
    digitalWrite(led6,HIGH);
    digitalWrite(led7,LOW);
    digitalWrite(led8,LOW);
    digitalWrite(led9,LOW);
  }
  else if(distance_cm<14 && distance_cm>=12)
  {
      Serial.print("Water Level is High Low\n");
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    digitalWrite(led5,LOW);
    digitalWrite(led6,LOW);
    digitalWrite(led7,HIGH);
    digitalWrite(led8,LOW);
    digitalWrite(led9,LOW);
  }
  else if(distance_cm<12 && distance_cm>=10)
  {
      Serial.print("Water Level is  High Medium\n");
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    digitalWrite(led5,LOW);
    digitalWrite(led6,LOW);
    digitalWrite(led7,LOW);
    digitalWrite(led8,HIGH);
    digitalWrite(led9,LOW);
  }
  else if(distance_cm<10 && distance_cm>=8)
  {
    Serial.print("Water Level is  High High\n");
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    digitalWrite(led5,LOW);
    digitalWrite(led6,LOW);
    digitalWrite(led7,LOW);
    digitalWrite(led8,LOW);
    digitalWrite(led9,HIGH);
    piratesOfCarribean();
    //tone(BUZZER, 2000, 1500); 
  }
}


void piratesOfCarribean()
{
  const int totalNotes = sizeof(notes) / sizeof(int);
  for (int i = 0; i < totalNotes; i++)
  {
    const int currentNote = notes[i];
    float wait = durations[i] / songSpeed;
    if (currentNote != 0)
    {
      tone(BUZZER, notes[i], wait);
    }
    else
    {
      noTone(BUZZER);
    }
    delay(wait);
  }
} 


boolean getID() 
{
  // Getting ready for Reading PICCs
  //If a new PICC placed to RFID reader continue
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  
  //Since a PICC placed get Serial and continue
  if ( ! mfrc522.PICC_ReadCardSerial()) {
  return false;
  }

  tagID = "";
  // The MIFARE PICCs that we use have 4 byte UID
  for ( uint8_t i = 0; i < 4; i++) {
  //readCard[i] = mfrc522.uid.uidByte[i];
  // Adds the 4 bytes in a single String variable
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;

}


void rfid()
{
  while (getID()) 
  {
    if (tagID == "6EE1BC10" || tagID =="3DD0305F")
    {
      if(tagID == "6EE1BC10")
      {
      Serial.println("Access Granted to SANTHOSH KUMAR!\nTo turn ON the motor.");
      Serial.println("--------------------------");
      noTone(BUZZER);
      }
      else
      {
      if(tagID == "3DD0305F")
      Serial.println("Access Granted to UMA MAGESWARI!\nTo turn ON the motor.");
      Serial.println("--------------------------");
      noTone(BUZZER);
      }
    }
    else
    {
      Serial.println("Access Denied! to turn ON the motor.");
      Serial.println("--------------------------");
      tone(BUZZER, 2000, 2000);
    }
    delay(2000);
    Serial.println(" Access Control ");
    Serial.println("Scan Your Card>>");
  }
}

void numberPad()
{
 static char enteredPassword[5]; // Maximum password length + 1 for null terminator
  static int passwordIndex = 0;
  
  char key = keypad.getKey();

  if (key != NO_KEY) 
  {
    if (key == '#') {
      // User pressed the pound key, check the entered password
      enteredPassword[passwordIndex] = '\0'; // Null-terminate the entered password
      if (strcmp(enteredPassword, correctPassword) == 0) {
        Serial.println("\nAuthentication successful!");
        // Add your code to perform actions after successful authentication
      } else {
        Serial.println("\nAuthentication Denied. Try again.");
      }
      // Reset variables for the next input
      passwordIndex = 0;
      memset(enteredPassword, 0, sizeof(enteredPassword));
    } else {
      // Add the entered digit to the password
      if (passwordIndex < 4) {
        enteredPassword[passwordIndex++] = key;
        Serial.print('*'); // Print '*' for each entered digit
      }
    }
  }
}
void setup() 
{
  Serial.begin (9600);
  SPI.begin();
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(led5,OUTPUT);
  pinMode(led6,OUTPUT);
  pinMode(led7,OUTPUT);
  pinMode(led8,OUTPUT);
  pinMode(led9,OUTPUT);

  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println("--------------------------");
  Serial.println(" Access Control ");
  Serial.println("Scan Your Card>>");
}

void ultrasonicSensor()
{
  digitalWrite(TRIG, HIGH);
  delay(5000);
  digitalWrite(TRIG, LOW);
  duration_us = pulseIn(ECHO, HIGH);
  distance_cm = 0.022 * duration_us;
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");  
  delay(2000);

}
void loop() 
{
  /*digitalWrite(TRIG, HIGH);
  delay(5000);
  digitalWrite(TRIG, LOW);
  duration_us = pulseIn(ECHO, HIGH);
  distance_cm = 0.022 * duration_us;
  

  rfid();
  delay(1500);
  lightAndSoundIndication();
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");  
  delay(2000);


  numberPad();*/
  
  rfid();
  delay(1500);
  lightAndSoundIndication();
  numberPad();
  ultrasonicSensor();



}
