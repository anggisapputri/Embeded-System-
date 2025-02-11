#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C mylcd(0x27,16,2);

int trig1 = 9;
int echo1 = 8;
int trig2 = 11;
int echo2 = 10;

void setup() 
{
  mylcd.init();
  mylcd.backlight();

  pinMode(trigpin, OUTPUT); //karna dia memancarkan gelombang ultarionik
  pinMode(echopin,INPUT);
  Serial.begin(9600); //untuk setup
}

void loop() 
{
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);

  durasi=pulseIn(echopin,HIGH); //
  Serial.print("Durasi: ");
  Serial.println(durasi);
  jarak=durasi*0.017;

  Serial.print("Jarak: ");
  Serial.println(jarak);
  Serial.println(" cm");
  delay(750);
  mylcd.clear();
  mylcd.print("Durasi: ");
  mylcd.setCursor(8,0);
  mylcd.println(durasi);
  mylcd.setCursor(0,1);
  mylcd.print("Jarak: ");
  mylcd.setCursor(7,1);
  mylcd.println(jarak);
  mylcd.setCursor(12,1);
  mylcd.println("Cm");
  delay(150);

}