/**
 * Traffic lights
 * Represents the traffic lights system we find in cities. It is a variation in
 * which the permission for a green light is asked through a button. 
 */

// Button and traffic lights ports
const int BUTTON = 2;
const int ledR = 13;
const int ledY = 12;
const int ledG = 8;

// Time waited before starting to change from red to turn on alternatively red and yellow
const int waitForPass = 5000;

// Time we keep the green light on
const int timeGreen = 10000;

// Time we keep a light on when alternates with another one
const int alternTurnTime = 200;

// Number of times that two lights are alternatively turned on/off
const int Nblinks = 14;

// Indicates whether the button is pushed (1) or not (0)
int val = 0;


// the setup routine runs once when you press reset:
void setup()
{                
  // initialize the digital pin as an output.
  pinMode(ledR, OUTPUT);
  pinMode(ledY, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);  
}


// the loop routine runs over and over again forever:
void loop()
{
  // Read the value of the button (to see if it is pushed or not)
  val = digitalRead(BUTTON);
  delay(10);
  
  // Print the value for debugging reasons
  Serial.println(val);
  
  // Turn the red light on
  digitalWrite(ledR, HIGH);
  
  // If the red button is pushed, make the transition to green.
  //  Then return to red again.
  if (val == HIGH)
  {
    // Keep the red light on for 'waitForPass' time
    //  Then turn this light off
    delay(waitForPass);
    digitalWrite(ledR, LOW);
    
    // Turn on alternatively both yellow and red lights
    turnLightsAlternatively(ledY, ledR);
    
    // Turn on the green light for a specified amount of time
    digitalWrite(ledG, HIGH);
    delay(timeGreen);
    digitalWrite(ledG, LOW);
    
    // Turn on alternatively both yellow and green lights
    //  When finished, note that red light will be turned on
    //  automatically when loop function restarts.
    turnLightsAlternatively(ledY, ledG);
  }
}


// Function to turn two lights alternatively
void turnLightsAlternatively(const int light1, const int light2)
{
  for (int i = 0; i < Nblinks; ++i)
  {
    digitalWrite(light1, HIGH);
    delay(alternTurnTime);
    digitalWrite(light1, LOW);
    
    digitalWrite(light2, HIGH);
    delay(alternTurnTime);
    digitalWrite(light2, LOW);
  }
}
