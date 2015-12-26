int BUTTON = 2;
int ACK_led = 12;

void setup()
{
  pinMode(BUTTON, INPUT);
  // initialize the digital pin as an output.
  pinMode(ACK_led, OUTPUT);     
  Serial.begin(9600);
}

void loop(){
  // SEND
  // send a capital D over the serial port if the button is pressed
  if(digitalRead(BUTTON)==HIGH)
  {
    Serial.print("D");
    delay(300); // prevents overwhelming the serial port
  }
  
  // RECEIVE
  if (Serial.available()>0)
  {
    if (Serial.read() == 'A')
    {
      digitalWrite(ACK_led, HIGH);
      delay(500);
    } 
    digitalWrite(ACK_led, LOW);
  }
}
