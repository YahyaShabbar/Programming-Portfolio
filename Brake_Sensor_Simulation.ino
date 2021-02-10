// Yahya Shabbar

int LED = 8;
int edbutton = 9;
int Hbutton = 6;
int Nbutton = 5;
int Lbutton = 4;
int buzzer = 7;
int dsensortrig = 2;
int dsensorecho = 3;

int fcwsystem = 1;
int distalarm = 20;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(edbutton, INPUT);
  pinMode(Hbutton, INPUT);
  pinMode(Nbutton, INPUT);
  pinMode(Lbutton, INPUT);
  pinMode(dsensortrig, OUTPUT);
  pinMode(dsensorecho, INPUT);
  pinMode(buzzer, INPUT);

  digitalWrite(edbutton, HIGH);
  digitalWrite(Hbutton, HIGH);
  digitalWrite(Nbutton, HIGH);
  digitalWrite(Lbutton, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
  int valedbutton;
  int valHbutton;
  int valNbutton;
  int valLbutton;
  float valcm;
  
  valedbutton = digitalRead(edbutton);
  valHbutton = digitalRead(Hbutton);
  valNbutton = digitalRead(Nbutton);
  valLbutton = digitalRead(Lbutton);
  
  // Start of if statement
  if (valedbutton == 0){
    if (fcwsystem == 1){
      fcwsystem = 0;
    }
    else {
      fcwsystem = 1;
    }
    delay(500);
  }

  // Start of second if statement
  if (valHbutton == 0){
      distalarm = 30;                 // Distance sound alarm
      delay(500);
  }                                   // End second if statement

  // Else if
  else if (valNbutton == 0){
      distalarm = 20;
      delay(500);
  }

  // Second Else if
  else if (valLbutton == 0){
    distalarm = 10;
    delay(500); 
  }

  // If statement for FCW system mode
  if (fcwsystem == 1){
    digitalWrite(dsensortrig, LOW);
    delayMicroseconds(2);
    digitalWrite(dsensortrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(dsensortrig, LOW);

    // Read the ultrasonic wave echo
    valcm = pulseIn(dsensorecho, HIGH) / 58.0;      // Value of distance in cm
    valcm = (int(valcm * 100.0)) / 100.0;           // Value of distance in cm
    
    // If statement within If statement
    if (valcm >= 0 && valcm <= distalarm){
      digitalWrite(LED, HIGH);
      tone(buzzer, 2000, 50);
      delay(50);
      digitalWrite(LED, LOW);
      delay(100);

      digitalWrite(LED, HIGH);
      tone(buzzer, 2000, 50);
      delay(50);
      digitalWrite(LED, LOW);
      delay(100);

      digitalWrite(LED, HIGH);
      tone(buzzer, 2000, 50);
      delay(50);
      digitalWrite(LED, LOW);
      delay(100);
      
    } 
  } 
}
