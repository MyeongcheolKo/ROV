/* Quick I2C Display test  */



/*  This sketch shows that 1602 LCD uses I2C communication.

   This routine will display the characters 0123456789ABCDEF on row 0 then FEDCBA9876543210

   on the next row.  Each character will be displayed one at a time with a programmable delay

   between each character.  At the end of each 32 character group, the delay will be reduced by

   25ms.  The delay will be printed to the screen before the start of the characters.

   The characters will also be sent out the Serial Monitor window

*/


#include <Wire.h>

#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD I2C address. Use 20 Space 4-line LCD.

//LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address. Use 16 Space 2-line LCD.


void setup() {

  lcd.init();

  lcd.backlight();                // Turn on the backlight. (lcd.noBacklight() turns off the backlight.)

  lcd.write(12);

  Serial.begin(9600);             // Starts serial communication at 9600 speed

  lcd.setCursor(0,0);

  
    for (int i = 0; i < 16; i++)
    {
      for (int r = 0; r < 4; r++)
      {
        for (int c = 0; c < 20; c++)
        {
          lcd.setCursor(c, r);
          lcd.print(i,HEX);
          delay(10);
        }
        
      }
    }

  lcd.clear();
  lcd.print("DONE");
  delay(5000);
  lcd.clear();
  


}


void loop() {



}

