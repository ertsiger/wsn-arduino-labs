/**
 * Chatting with Xbee - Version 1
 * In this version an Xbee connected to XCTU software sends a
 * message finished by an INTRO (like in a real chat). Because
 * of display length restrictions, just those characters that
 * fit in the screen are displayed.
 */

#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Wire.h>  

// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

SoftwareSerial xbee(0, 1); // Pin: RX, TX

/**
 * Constants
 */
// Number of flashes before starting to show the characters received
const int N_FLASHES = 3;

// Number of rows in the display
const int N_ROWS = 2;

// Number of characters in each row
const int MAX_CHARS_PER_ROW = 16;

/**
 * Global variables
 */
// String that will be formed from the chars sent by the other xbee
String input = "";

void setup() 
{
  // Initialize the lcd for 16 chars 2 lines, turn on backlight
  lcd.begin(MAX_CHARS_PER_ROW, N_ROWS);

  // Quick 3 flashes of backlight 
  for (int i = 0; i < N_FLASHES; ++i)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on  

   // set the data rate for the SoftwareSerial port
   xbee.begin(9600);
}

void loop()
{
  // If the xbee is receiving something...
  if (xbee.available())
  { 
    // Read the character received from the other xbee
    char c = xbee.read();
    
    // If INTRO key is pressed, print the string in the display
    //  Otherwise, keep reading it
    if (c == 13 || c == 10)
    {
      printStringInLCD();
      input = "";
    }
    else
    {
      input += c;
    }
  }
}

// Updates the LCD parameters (row, position in row) depending on
//  the current position
void printStringInLCD()
{
  // Size restriction of the display
  int length = min(input.length(), MAX_CHARS_PER_ROW * N_ROWS);
  
  int row = 0;
  int pos = 0;
  
  // Important to clear the display before writting new text
  lcd.clear();
  
  // Print char by char in order to take advantage from all the display
  //  The string can also be divided in two parts and print them separately
  for (int i = 0; i < length; ++i)
  {
    pos = i % MAX_CHARS_PER_ROW;
    
    if (i == MAX_CHARS_PER_ROW)
    {
      ++row; 
    }
    
    lcd.setCursor(pos, row);
    lcd.print(input.charAt(i));   
  }
}
