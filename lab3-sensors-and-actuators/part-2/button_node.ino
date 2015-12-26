/**
 * Button Part
 *  While a song is not being played, the user can
 *  introduce a song number using a button and sending
 *  it to the buzzer part. Once it receives the song
 *  name from the buzzer part, it prints it in the lcd
 *  screen along with its songId until it has been
 *  completely playedd.
 */

#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

// Set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
// Set the LCD I2C address
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

/**
 * Constants
 */
// Button that increases the songIndex
const int BUTTON = 2;

// Button that sends the song to the other XBee
const int ENTER = 3;

// Char used to indicate the end of the song
const char END_CHAR = 'E';

// String used to indicate the song id is not in the allowed range
const String SONG_NOT_FOUND = "Song not found";

// Char used to indicate that the song title has been completely sent
const char END_SONG_TITLE_CHAR = '\n';

/**
 * Global variables
 */
int songIndex = 0;
String songName = "";
boolean isPlayingSong = false;

/**
 * Helper functions
 */
// Prints information of a song in LCD screen
//  First line: songIndex - Second line: songName
void printCurrentInformation()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(songIndex);
  lcd.setCursor(0, 1);
  lcd.print(songName); 
}

/**
 * Main functions
 */
void setup()
{
  pinMode(BUTTON, INPUT);
  pinMode(ENTER, INPUT);

  lcd.begin(16,2);
  lcd.backlight(); 
  printCurrentInformation();

  Serial.begin(9600);
}

void loop()
{
  // If a song is being played, not allow to
  // modify or send songIndex
  if (!isPlayingSong)
  {
    // Increments the songIndex very time is pressed
    if (digitalRead(BUTTON) == HIGH)
    {
      ++songIndex;
      printCurrentInformation();
      delay(300);
    }
  
    // Sends the current selected song
    if (digitalRead(ENTER) == HIGH)
    {
      Serial.print(songIndex);
      delay(300);
    }
  }

  // If we are receiving sth through the serial connection...
  if (Serial.available() > 0)
  {
    // It would be easier if we did 'readString()' but
    // some delay problems appeared. It is much faster
    // it we receive the song name char by char
    char c = Serial.read();
    
    if (isPlayingSong)
    {
      // Reset the lcd screen showing that the song has ended
      if (c == END_CHAR)
      {
        songIndex = 0; 
        songName = "End of the song";
        isPlayingSong = false;
        printCurrentInformation();
        songName = ""; // restore song name
      }
    }
    else
    {
      // Is the symbol that tells that the songName has been
      // completely introduced
      if (c == END_SONG_TITLE_CHAR)
      {
        // If the songIndex did not exist, 
        if (songName == SONG_NOT_FOUND)
        {
          songIndex = 0;
          printCurrentInformation();
          songName = "";
        }
        else
        {
          isPlayingSong = true;
          printCurrentInformation();
        }
      }
      else
      {
        // Keep forming the name of the song
        songName += c;  
      }
    }
  }
}


