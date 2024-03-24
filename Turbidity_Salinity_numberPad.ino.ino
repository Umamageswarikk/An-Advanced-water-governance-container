#include <Keypad.h>
#include <GravityTDS.h>
#include <EEPROM.h>
#define TdsSensorPin A1
int sensorPin = A0;
GravityTDS gravityTds;
float temperature = 25,tdsValue = 0;
const int ROWS = 4; // define the number of rows on the keypad
const int COLS = 3; // define the number of columns on the keypad

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3};    // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const char* correctPassword = "3815"; // Change this to your desired password

void setup() {
  Serial.begin(9600);
  gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();  //initialization
}



void loop() {
  static char enteredPassword[5]; // Maximum password length + 1 for null terminator
  static int passwordIndex = 0;
  
  char key = keypad.getKey();

  if (key != NO_KEY) {
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
  
      //temperature = readTemperature();  //add your temperature sensor and read it
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue();  // then get the value
    Serial.print(tdsValue,0);
    Serial.println("ppm");
    delay(2000);


  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  int turbidity = map(sensorValue, 0, 750, 100, 0);
  delay(100);

  if (turbidity < 500 && turbidity >250) {
    /*digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);*/
    Serial.println("Water is DIRTY");
    //Serial.println("It's CLEAR");
  }
  else if (turbidity >= 200 && turbidity < 250) {
    /*digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);*/
    Serial.println("It's CLOUDY");
  }
  else if (turbidity <100) {
    /*digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);*/
    Serial.println("Water is CLEAN");
  }

}

 




