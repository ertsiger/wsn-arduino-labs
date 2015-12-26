int LED_NIGHT = 10;
int LED_SUNSET = 11;
int LED_DAY = 12;
int debugLED = 13;
int analogValue = 0;
// Upper boundaries
int DAYup = 750;
int SUNSETup = 950;

int remoteIndicator = false; // keeps track of the desired remote on/off state
int lastRemoteIndicator = false; // record of prior remote state

unsigned long lastSent = 0; // records last time the remote was re-set to keep it in sync

void setup() {
  pinMode(LED_DAY,OUTPUT);
  pinMode(LED_SUNSET,OUTPUT);
  pinMode(LED_NIGHT,OUTPUT);
  pinMode(debugLED,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(LED_NIGHT, LOW);
  digitalWrite(LED_SUNSET, LOW);
  digitalWrite(LED_DAY, LOW);
  // make sure everything we need is in the buffer
  if (Serial.available() >= 23) {
    // look for the start byte
    if (Serial.read() == 0x7E) {
      //blink debug LED to indicate when data is received
      digitalWrite(debugLED, HIGH);
      delay(10);
      digitalWrite(debugLED, LOW);
      // read the variables that we're not using out of the buffer
      for (int i = 0; i<20; i++) {
        byte discard = Serial.read();
      }
      int analogHigh = Serial.read();
      int analogLow = Serial.read();
      analogValue = analogLow + (analogHigh * 256);
      Serial.print("Analog value ");
      Serial.println(analogValue);
    }
  }

  // DAY TIME
  if (analogValue > 0 && analogValue <= DAYup) {
    digitalWrite(LED_DAY, HIGH);
    delay(10);
    digitalWrite(LED_DAY, LOW);
    remoteIndicator = false;
  }

  // SUNSET TIME
  if (analogValue > DAYup && analogValue <= SUNSETup) {
    digitalWrite(LED_SUNSET, HIGH);
    delay(10);
    digitalWrite(LED_SUNSET, LOW);
    remoteIndicator = false;
  }

  // NIGHT TIME
  if (analogValue > SUNSETup && analogValue <= 1023) {
    digitalWrite(LED_NIGHT, HIGH);
    delay(10);
    digitalWrite(LED_NIGHT, LOW);
    remoteIndicator = true;
  }

  // set the indicator immediately when there's a state change
  if (remoteIndicator != lastRemoteIndicator) {
    if (remoteIndicator==false) setRemoteState(0x4);
    if (remoteIndicator==true) setRemoteState(0x5);
    lastRemoteIndicator = remoteIndicator;
  }
  // re-set the indicator occasionally in case it's out of sync
  if (millis() - lastSent > 10000 ) {
    if (remoteIndicator==false) setRemoteState(0x4);
    if (remoteIndicator==true) setRemoteState(0x5);
    lastSent = millis();
  }

}


void setRemoteState(int value) {
  Serial.write(0x7e);//Start byte
  Serial.write((byte)0x0);//Length
  Serial.write(0x10);//Length High
  Serial.write(0x17);//AT Command Request
  Serial.write((byte)0x0);//Frame ID
  Serial.write((byte)0x0);//Serial Number of Destination
  Serial.write((byte)0x0);
  Serial.write((byte)0x0);
  Serial.write((byte)0x0);
  Serial.write((byte)0x0);
  Serial.write((byte)0x0);
  Serial.write(0xFF);
  Serial.write(0xFF);//End of Serial Number of Destinition

  // 16 bit of recipient or 0xFFFE if unknown
  Serial.write(0xFF);
  Serial.write(0xFE);
  Serial.write(0x02);//Apply changes immediately

  //Command name in ASCII characters
  Serial.write('D');
  Serial.write('1');

  //command data in as many bytes after length bytes
  Serial.write(value);

  //checksum is all bytes after length bytes
  long sum = 0x17 + 0xFF+ 0xFF + 0xFF + 0xFE + 0x02 + 'D' + '1' + value;
  Serial.write (0xFF - ( sum & 0xFF) );
  delay(10); // avoiding overwhelming
} 




