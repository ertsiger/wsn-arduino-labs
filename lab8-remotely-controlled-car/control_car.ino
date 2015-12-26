/**
 * This code is used to control the remote car by sending
 * messages through an XBee.
 */

// Pins of the Arduino to which the switches to move the car are connected
const int LEFT_PIN = 2;
const int FORW_PIN = 3;
const int RIGHT_PIN = 4;

// Pins to which we will transmit the information we want to send to the XBee
const int D1_PIN = 8; // corresponds to right wheel
const int D2_PIN = 9; // corresponds to left wheel

// Keeps track of the desired remote on/off state (left, right, forward)
int lRI = false; 
int rRI = false;
int fRI = false;

// Keeps track of the previous remote on/off state
int last_lRI= false;
int last_rRI = false;
int last_fRI = false;

// records last time the remote was re-set to keep it in sync
unsigned long lastSent = 0;

void setup()
{  
  // declare the switch (directions) pins as inputs   
  pinMode(LEFT_PIN, INPUT);
  pinMode(FORW_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT);
  Serial.begin(9600);
}

void loop()
{
  setRemoteIndicators();
  sendInformation();
}

// Read the value of the switches
//   digitalRead() checks to see if there is voltage on the pin or not  
void setRemoteIndicators()
{
  if (digitalRead(LEFT_PIN) == HIGH) // move left
  {
    lRI = true;
    rRI = false;
    fRI = false;
  } 
  else if (digitalRead(FORW_PIN) == HIGH) // move forward
  {
    lRI = false;
    rRI = false;
    fRI = true;
  }
  else if (digitalRead(RIGHT_PIN) == HIGH) // move right
  {
    lRI = false;
    rRI = true;
    fRI = false;
  }
  else // do not move
  {
    lRI = false;
    rRI = false;
    fRI = false;
  }
}

void sendInformation()
{
  // set the indicators immediately when there's a state change
  if (lRI != last_lRI)
  {
    if (!lRI) setRemoteState(0x4,'1');
    if (lRI) setRemoteState(0x5,'1');
    last_lRI = lRI;
  }
  delay(10);
  
  if (rRI != last_rRI)
  {
    if (!rRI) setRemoteState(0x4,'2');
    if (rRI) setRemoteState(0x5,'2');
    last_rRI = rRI;
  }
  delay(10);
  
  if (fRI != last_fRI)
  {
    if (!fRI)
    {
      setRemoteState(0x4,'1');
      setRemoteState(0x4,'2');
    }
    if (fRI)
    {
      setRemoteState(0x5,'1');
      setRemoteState(0x5,'2');
    }
    last_fRI = fRI;
  }

  // re-set the indicator occasionally in case it's out of sync
  if (millis() - lastSent > 10000 )
  {
    if (!lRI) setRemoteState(0x4,'1');
    if (lRI) setRemoteState(0x5,'1');
    delay(10);
    
    if (!rRI) setRemoteState(0x4,'2');
    if (rRI) setRemoteState(0x5,'2');
    lastSent = millis();
  }
}

// Send msg through the xbee (note that the frame has to be formed)
void setRemoteState(int value, char pinNum)
{
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
  Serial.write(pinNum);

  //command data in as many bytes after length bytes
  Serial.write(value);

  //checksum is all bytes after length bytes
  long sum = 0x17 + 0xFF+ 0xFF + 0xFF + 0xFE + 0x02 + 'D' + pinNum + value;
  Serial.write (0xFF - ( sum & 0xFF) );
  delay(10); // avoiding overwhelming
}

