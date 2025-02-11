#include <DHT.h>
#include <DHT_U.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C mylcd(0x27, 16, 2);

#define DHTS 8 //nama yang mewakili pin 8
#define DHTYPE DHT11

DHT dhtku(DHTS,DHTYPE);

void setup() 
{
  dhtku.begin();//mengaktifkan
  Serial.begin(9600);
  mylcd.init();
  mylcd.backlight();
}

void loop() 
{ 
  float suhu=dhtku.readTemperature()+6.5;
  float lembab=dhtku.readHumidity();

  //membuat intruksi melalukan mengecekan membaca atau tidak
  if(isnan(suhu) || isnan(lembab)) //isnan bukan number
  {
    mylcd.clear();
    Serial.println("No Data Captured");
    mylcd.print("No Data Captured");
    delay(250);
  } else
    {
      Serial.println("SUHU: ");
      Serial.print(suhu);
      Serial.println(" *C");

      Serial.println("KELEMBAPAN: ");
      Serial.print(lembab);
      Serial.println(" %RH");
      delay(150);
    }
}