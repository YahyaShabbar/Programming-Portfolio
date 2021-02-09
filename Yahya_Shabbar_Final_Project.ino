// Yahya Shabbar
// Final Project

#include<Keypad.h>
#include<Servo.h>

void clearInputKeys(void);
void clearUserKeys(void);

int compareInputWithMasterKey(int keyNum);      // Ask double
void slideInputKeys(char newKey);               // Ask why this is a void

// Declare Constant Global Variables
const byte ROWS = 4;
const byte COLS = 4;

// Map 4x4
char keys[ROWS][COLS] = {
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};  //Row pins    // Might have to remove ROWS and COLS
byte colPins[COLS] = {5, 4, 3, 2};  //Column pins

// Declare Constant Global Variables
const int servoPin = 10;
const int ledPin = 11;

// Keypad
Keypad kp = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);   // Ask professor why keypad is orange and kp isn't
                                                                      // kp might be keypad

// Servo
Servo myservo;

// Factory Key
const char factoryKey[5] = {'9', '3', '9', '1', '3'};

char lockKey1 = '8';
char lockKey2 = '0';

char holdKey1;
char holdKey2;

int keyCount = 0;
char inputKey[5] = {' ', ' ', ' ', ' ', ' '};
char userKey[5] = {'z', 'z', 'z', 'z', 'z'};
char savedPersonalKey[5] = {'z', 'z', 'z', 'z', 'z'};
int correctFactoryKey = 0;
int personalKeyEntryMode = 0;
int personalKeyCount = 0;
unsigned long currentTime = 0;
unsigned long keyTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  myservo.attach(11);
  myservo.write(10);     // Might be 10 or 0
}

void loop() {

  int difference = 5;
  if (kp.getKeys()){           // kp might be keypad
    keyTime = millis();
    digitalWrite(ledPin, HIGH);
    holdKey1 = kp.key[0].kchar;
    holdKey2 = kp.key[1].kchar;

    // Optional Debug Printing
    Serial.print("Holding the first key = ");
    Serial.println(holdKey1);
    Serial.print("Holding the second key = ");
    Serial.println(holdKey2);

    // Door Locked Anytime
    // Still inside first if statement

    if ((holdKey1 == lockKey1 && holdKey2 == lockKey2)||(holdKey1 == lockKey2 && holdKey2 == lockKey1)){
      Serial.println("Lock the door.");
      myservo.write(10);
      clearInputKeys();        
      keyCount = 0;
      correctFactoryKey = 0;
      clearUserKeys();          
      personalKeyEntryMode = 0;
      personalKeyCount = 0;
      // end of if, locking the door with ‘8’ + ‘0’
     }
    else if (kp.key[0].kstate == PRESSED){
      if (correctFactoryKey == 1 && holdKey1 == '1'){
        Serial.println("Set up personal key.");
        clearInputKeys();
        keyCount = 0;
        personalKeyEntryMode = 1;
        personalKeyCount = 0;
        // end of if, after correct factory key is entered and getting a ‘1’
      }
    else if (personalKeyEntryMode == 1 && personalKeyCount < 5){
      //holdKey1 = userKey[personalKeyCount];                      // Double Check 
      userKey[personalKeyCount] = holdKey1;
      personalKeyCount++;      
      // end of else if, receiving personal key code
      }
    else if (personalKeyEntryMode == 1 && personalKeyCount >= 5){
      if (holdKey1 == '1'){
          Serial.println("Personal Pin Saved.");
          //savedPersonalKey[userKey[0]];       
          //savedPersonalKey[userKey[1]];
          //savedPersonalKey[userKey[2]];
          //savedPersonalKey[userKey[3]];
          //savedPersonalKey[userKey[4]];

          savedPersonalKey[0] = userKey[0];
          savedPersonalKey[1] = userKey[1];
          savedPersonalKey[2] = userKey[2];
          savedPersonalKey[3] = userKey[3];
          savedPersonalKey[4] = userKey[4];

          //userKey[0] = savedPersonalKey[0];
          //userKey[1] = savedPersonalKey[1];
          //userKey[2] = savedPersonalKey[2];
          //userKey[3] = savedPersonalKey[3];
          //userKey[4] = savedPersonalKey[4];

          myservo.write(10);
          delay(200);
          myservo.write(170);          
        }
        
      else {
          clearUserKeys();                   
        }                                      // end of else, receiving a not-‘1’ key after 5 personal key
          personalKeyEntryMode = 0;
          personalKeyCount = 0;
          // end of else if, after receiving 5 personal keys, getting the next key
      }
    else if (keyCount < 5){
      //holdKey1 = inputKey[keyCount];                    // Ask Professor (This might be wrong)
      inputKey[keyCount] = holdKey1;
      personalKeyCount++;                                         // Check to make sure increment is right
      // end of else if, less than 5 key press  
      }
      else {
        slideInputKeys(holdKey1);            // *I removed char infront of slideInput Keys*
      // end of else, more than 5 keys
      }

      // Debug Printing
      Serial.println("Input Keys: ");
      Serial.print(inputKey[0]);
      Serial.print(inputKey[1]);
      Serial.print(inputKey[2]);
      Serial.print(inputKey[3]);
      Serial.print(inputKey[4]);
      Serial.println("Personal Keys: ");
      Serial.print(userKey[0]);
      Serial.print(userKey[1]);
      Serial.print(userKey[2]);
      Serial.print(userKey[3]);
      Serial.print(userKey[4]);
      Serial.println("Saved Personal Keys: ");
      Serial.print(savedPersonalKey[0]);
      Serial.print(savedPersonalKey[1]);
      Serial.print(savedPersonalKey[2]);
      Serial.print(savedPersonalKey[3]);
      Serial.print(savedPersonalKey[4]);
      correctFactoryKey = 0;

      // Call the progammer-defined function
      difference = compareInputWithMasterKey(0);   
            
      if (difference == 0){
         Serial.println("Unlock the Door.");
         myservo.write(170);
         clearInputKeys();                  
         keyCount = 0;
         correctFactoryKey = 1;         
      }

      difference = compareInputWithMasterKey(1);

      if (difference == 0){
        Serial.println("Unlock the door. ");
        myservo.write(170);
        clearInputKeys();
        keyCount = 0;
        correctFactoryKey = 1;
      }
    }                                   // end of else if, getting a key press
  }                                     // end of if getting keys

  currentTime = millis();

  if ((currentTime - keyTime) > 5000){
    clearInputKeys();              
    keyCount = 0;
    correctFactoryKey = 0;
    clearUserKeys();
    personalKeyEntryMode = 0;
    personalKeyCount = 0;

    digitalWrite(ledPin, LOW);
  }                                     // end of if, over 5 seconds without key press
}                                       // end of loop forever function


void clearInputKeys() {

    int i;
    for (i = 0; i < 5; i++)
      {
        inputKey[i] = ' ';
      }
}

void clearUserKeys() {

    int i;
    for (i = 0; i < 5; i++)
      {
        userKey[i] = 'z';
      }
  
}

int compareInputWithMasterKey(int keyNum) {
    
    int i;
    int wrong = 0;
    for (i = 0; i < 5; i++)
    {
    if(keyNum == 0 && inputKey[i] != factoryKey[i]){
      wrong++;
      } 
    else if(keyNum == 1 && inputKey[i] != savedPersonalKey[i]){
      wrong++;
      }
   }
   return wrong;
}

void slideInputKeys(char newKey){

    int i;
    for (i = 0; i < 4; i++)
    {
      inputKey[i+1] = inputKey[i];
      newKey = inputKey[4];
    }
}
