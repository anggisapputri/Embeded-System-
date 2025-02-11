#include <Wire.h>               
#include <LiquidCrystal_I2C.h>  


LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int sensorPin = 2; 
int count = 0;            
bool detected = false;    

void setup() {
  pinMode(sensorPin, INPUT);   
  lcd.init();                   
  lcd.backlight();              

  
  lcd.setCursor(0, 0);
  lcd.print("People Count: ");
  lcd.setCursor(0, 1);
  lcd.print(count);
}

void loop() {
  int sensorValue = digitalRead(sensorPin); 

  if (sensorValue == HIGH && !detected) { 
    count++; 
    detected = true;  
    lcd.clear();
    lcd.setCursor(0, 0);  
    lcd.print("People Count: ");
    lcd.setCursor(0, 1);  
    lcd.print(count);
    delay(1000);  
  } 
  else if (sensorValue == LOW) {
    detected = false; 
  }
  
  delay(100);  
}