#include <LiquidCrystal.h>
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void initLCD()
{
  lcd.begin(16, 2);  
}

void clearLCD()
{
  lcd.clear();
}

void jumpToStart()
{
  lcd.setCursor(0, 0);
}

void jumpToLineTwo()
{
  lcd.setCursor(0, 1);
}

void writeLCD(char myText[])
{
  lcd.print(myText);
} 
