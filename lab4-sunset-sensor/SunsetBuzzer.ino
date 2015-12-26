
// Pin where the buzzer is connected
const int BELL = 5;
const int PIN_STATE = 2;
// Light detector state
int DETstate = 0;
// Tone variables
float sinVal;
int toneVal;
void setup()
{
  pinMode(BELL, OUTPUT);
  Serial.begin(9600); 
}

void loop()
{
  DETstate = digitalRead(PIN_STATE);
  if (DETstate == HIGH){
    for (int x=0; x<180; x++) {
      // convert degrees to radians then obtain sin value
      sinVal = (sin(x*(3.1412/180)));
      // generate a frequency from the sin value
      toneVal = 2000+(int(sinVal*1000));
      tone(BELL, toneVal);
       
    }
    delay(10);
    noTone(BELL);
    //analogWrite(BELL, 0); 
  }


}


