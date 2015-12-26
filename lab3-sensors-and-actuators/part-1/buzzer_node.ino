// Pin where the buzzer is connected
const int BELL = 5;

void setup()
{
  pinMode(BELL, OUTPUT);
  Serial.begin(9600); 
}

void loop()
{
  if (Serial.available() > 0)
  {
    if (Serial.read() == 'D')
    { 
      // Send 'A' to tell the other node that
      //  it is playing a beep in the buzzer
      Serial.print("A");
      
      analogWrite(BELL, 240);
      delay(10);
      analogWrite(BELL, 0); 
    }
  } 
}
