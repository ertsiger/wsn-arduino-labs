/**
 * Chatting with Xbee - Version 2
 * In this version an Xbee connected to XCTU software sends an infinite
 * message to the Xbee connected to the Arduino. The message is displayed
 * character by character while the message is written in XCTU. If the
 * message is filled with characters, then the screen is cleared.
 */

#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Wire.h>  

// Set the LCD address to 0x27 for a 20 chars 4 line display
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
int currentRow = 0;
int nextRow = 0;
int currentPos = 0;
int nextPos = 0;

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
    updateLCDParams();
 
    // Set the cursor position in the display   
    lcd.setCursor(currentPos, currentRow);
    
    // Read the character received from the other xbee
    char c = xbee.read();
    
    // If INTRO key is pressed...
    if (c == 13 || c == 10)
    {
      nextRow = (currentRow+1) % N_ROWS;
      nextPos = 0;
    }
    else
    {
      // Print char throught the display
      lcd.print(c);
 
      // If we reach the end of the current row...
      if (currentPos == MAX_CHARS_PER_ROW-1)
      {
        nextRow = (currentRow+1) % N_ROWS;
        nextPos = 0;
      }
      else
      {
        nextRow = currentRow;
        nextPos = (currentPos+1) % MAX_CHARS_PER_ROW; 
      }
    }
  }
}

// Updates the LCD parameters (row, position in row) depending on
//  the current position
void updateLCDParams()
{
  currentRow = nextRow;
  currentPos = nextPos;
  
  if (currentRow == 0 && currentPos == 0)
  {
    lcd.clear();
  }
}
