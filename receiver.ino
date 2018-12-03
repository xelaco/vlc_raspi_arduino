/* xelaco 2018
 * Visible light communication between a Raspberry Pi and an Arduino
 * receiver.ino
 * 
 * ___Requirements for the receiver___
 * > Arduino
 * > Same LED as sender
 * > Wires
 * > 1 Mohm resistor
 * > LCD circuit (optional) --> LCD 16x2, 10 Kohm potentiometer, 220 ohm resistor, wires, breadboard
 * > This program must be uploaded to an Arduino.
 * 
 * ___Circuit___
 * LED: (+) to pin A0 /// (-) to ground
 * resistor : in parallel with the LED
 * LCD circuit (optional): see Arduino official documentation
 */

#include <LiquidCrystal.h>
#define RECEIVER A0

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int bits[7] = {0};
char carac = '\0';
int count = 0;

void setup()
{
  lcd.begin(16, 2);
  pinMode(RECEIVER, INPUT);
}

void loop()
{
  for(int i = 0; i < 6; i++)
  {
    bits[i] = bits[i+1];
  }
  bits[6] = (analogRead(RECEIVER) > 1) ? 1 : 0;

  if(bits[3] == 1 && bits[4] == 1 && bits[5] == 1 && bits[6] == 1)
  {
    bits[7] = {0};
    carac = '\0';
    delay(500);
    do
    {
      for (int i = 0; i <= 6; i++)
      {
        bits[i] = (analogRead(RECEIVER) > 1) ? 1 : 0;
        delay(500);
      }
      lcd.setCursor(count, 0);
      carac = bits[6] * 1 + bits[5] * 2 + bits[4] * 4 + bits[3] * 8 + bits[2] * 16 + bits[1] * 32 + bits[0] * 64;
      if(carac != '\0')
      {
        lcd.print(carac);
      }
      bits[7] = {0};
      count++;
    }while(carac != '\0');
    count = 0;
    delay(5000);
    lcd.clear();
  }
  delay(500);
}
