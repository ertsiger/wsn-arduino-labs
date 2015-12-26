// named constants for the switch and motor pins
const int STATE_PIN = 2; // the number of the State pin
const int  FAN_PIN =  5; // the number of the motor pin

int DETstate = 0;  // variable for reading the state detector status

void setup() {
  // initialize the motor pin as an output:
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW); 
  // initialize the switch pin as an input:
  pinMode(DETstate, INPUT);
  Serial.begin(9600); 
}

void loop(){
  // read the state of the Threshold value:
  DETstate = digitalRead(STATE_PIN);
  // check if the activationg motor order is rx
  if (DETstate == HIGH) {     
    // turn fan on:    
    digitalWrite(FAN_PIN, HIGH);  
  } 
  else {
    // turn fan off:
    digitalWrite(FAN_PIN, LOW); 
  }
  delay(10);
}



