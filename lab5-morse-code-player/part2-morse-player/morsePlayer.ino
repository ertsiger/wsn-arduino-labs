#include <XBee.h>

// Pins
const int ledPin = 13;
const int buzzerPin = 5;

/**
 * Morse Codes
 *  "s" for short (.), "l" for long (-) 
 */
const char longNote = 'l';
const char shortNote = 's';
 
// Morse code for Alphabet
const String letter[26] = {
  "sl","lsss","lsls","lss","s","ssls","lls","ssss","ss","slll","lsl",
  "slss","ll","ls","lll","slls","llsl","sls","sss","l","ssl","sssl","sll","lssl","lsll","llss"}; 

// Morse code for Numbers
const String number[10]= {
  "lllll","sllll","sslll","sssll","ssssl","sssss","lssss","llsss","lllss","lllls"};

/**
 * XBee objects
 */
XBee xbee;
XBeeResponse response;
// create reusable response objects for responses we expect to handle
ZBRxResponse rx;

void setup()
{
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  xbee = XBee();
  response = XBeeResponse();
  rx = ZBRxResponse();

  flashLed(ledPin, 10, 50);
}

void loop()
{
  // 1. This will read any data that is available:
  xbee.readPacket();
  if (xbee.getResponse().isAvailable())
  {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
    {
      xbee.getResponse().getZBRxResponse(rx);
      Serial.println(rx.getData(0));
      playCode(rx.getData(0));
    }
  }
}

// Flash the LED for certain number of times and delay between flashes
void flashLed(int pin, int times, int wait)
{
  for (int i = 0; i < times; ++i)
  {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);

    if (i + 1 < times)
    {
      delay(wait);
    }
  }
}

// Play the required tone for the char or int transmitted
void playCode(int ASCII_value)
{
  int intValue = ASCII_value - '0';

  // Integer read
  if (intValue >= 0 && intValue <= 9)
  {
    String toPlay = number[intValue];
    Serial.println(toPlay);
    for (int i = 0; i < toPlay.length(); ++i)
    {
      Serial.println(toPlay[i]);
      if(toPlay.charAt(i) == longNote)    
        largeTone();
      else if(toPlay.charAt(i) == shortNote)
        shortTone();
    } 
  }
  else // Char (A-Z) read
  {
    String toPlay = letter[ASCII_value-65];
    Serial.println(toPlay);
    for (int i = 0; i < toPlay.length(); ++i)
    {
      Serial.println(toPlay[i]);
      if (toPlay.charAt(i) == longNote)    
        largeTone();
      else if (toPlay.charAt(i) == shortNote)
        shortTone();
    }    
  }
}

void largeTone()
{
  delay(50);
  digitalWrite(ledPin, HIGH);
  analogWrite(buzzerPin, 2000);
  delay(500);
  digitalWrite(ledPin, LOW);
  analogWrite(buzzerPin, 0);
}

void shortTone()
{
  delay(50);
  digitalWrite(ledPin, HIGH);
  analogWrite(buzzerPin, 2000);
  delay(100);
  digitalWrite(ledPin, LOW);
  analogWrite(buzzerPin, 0);
}
