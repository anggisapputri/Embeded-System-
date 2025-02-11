#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcdku(0x27, 16, 2);

int IRMASUK = 3;    //Masuk
int IRKELUAR = 4;    //Keluar

int totalCountIn = 0;    
int totalCountOut = 0;   
bool countIn = false;   
bool countOut = false;   

void setup() 
{
  Serial.begin(9600);
  lcdku.init();
  lcdku.backlight();
  pinMode(IRMASUK, INPUT);
  pinMode(IRKELUAR, INPUT);
}

void loop() {
  if (digitalRead(IRMASUK) == LOW && !countIn) {
    countIn = true;
    totalCountIn++;   
    delay(500);      
  }
  if (digitalRead(IRMASUK) == HIGH) {
    countIn = false; 
  }

  if (digitalRead(IRKELUAR) == LOW && !countOut) {
    countOut = true;
    if (totalCountIn > 0) {   
      totalCountIn--;         
    }
    delay(1000);              
  }
  if (digitalRead(IRKELUAR) == HIGH) {
    countOut = false;  
  }

  lcdku.setCursor(0, 0);
  lcdku.print("Orang Masuk: ");
  lcdku.print(totalCountIn);
  lcdku.setCursor(0, 1);
}