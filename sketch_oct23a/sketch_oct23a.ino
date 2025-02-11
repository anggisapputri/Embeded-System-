#include <Servo.h>

Servo servo;

int trig1 = 9;
int echo1 = 8;
int trig2 = 11;
int echo2 = 10;
int buzzer = 12;

long durasi, jarak;
long durasi1, jarak1;

unsigned long stopTime = 0; // Time when the vehicle stops at the middle
bool vehicleStopped = false; // Status to track if vehicle is stopped

void setup() {
  servo.attach(6);
  servo.write(90);  // Start with the gate closed at 90 degrees
  Serial.begin(9600);

  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(buzzer, OUTPUT); // Set buzzer pin as output
}

void loop() {
  // Sensor 1 (Exit sensor)
  digitalWrite(trig1, LOW); 
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  durasi = pulseIn(echo1, HIGH);
  jarak = (durasi * 0.034) / 2;

  // Sensor 2 (Entry sensor)
  digitalWrite(trig2, LOW); 
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  durasi1 = pulseIn(echo2, HIGH);
  jarak1 = (durasi1 * 0.034) / 2;

  // Print distances for debugging
  Serial.print("Jarak Sensor 1: ");
  Serial.println(jarak);
  Serial.print("Jarak Sensor 2: ");
  Serial.println(jarak1);

  // Gate and Alarm control logic
  if (jarak < 20 && jarak1 > 20) { 
    // Vehicle leaving the complex
    servo.write(0);  // Open gate to 0 degrees
    Serial.println("Kendaraan Melintas Keluar");
    noTone(buzzer);
    stopTime = 0;  // Reset stop time as the vehicle left
    vehicleStopped = false;
  } 
  else if (jarak1 < 20 && jarak > 20) { 
    // Vehicle entering the complex
    servo.write(90);  // Close gate to 90 degrees
    Serial.println("Kendaraan Melintas Masuk");
    tone(buzzer, 1000);  // Short beep
    delay(500);
    noTone(buzzer);
    stopTime = 0;  // Reset stop time as the vehicle entered
    vehicleStopped = false;
  } 
  else if (jarak1 < 20 && jarak < 20) {
    // Vehicle in the middle of the gate
    Serial.println("Mobil berada pada tengah palang pintu");
    servo.write(90);  // Keep the gate closed
    if (!vehicleStopped) {
      stopTime = millis();  // Start the timer when vehicle stops
      vehicleStopped = true;
    }
  }

  // Check if vehicle has stopped in the middle for more than 5 seconds
  if (vehicleStopped && (millis() - stopTime > 5000)) {
    tone(buzzer, 1000);  // Continuous beep if vehicle stops for more than 5 seconds
  }

  delay(500);  // Small delay for smooth sensor readings
}
