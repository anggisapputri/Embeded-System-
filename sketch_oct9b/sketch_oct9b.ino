#include <Servo.h>
Servo servo1;


void setup() {
  // put your setup code here, to run once:
  servo1.attach(9);
  servo1.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  int tunda = 10;
  for (int i=0; i<180; i++){
    servo1.write(i);
    delay(tunda);
  }

  for (int j=180; j>=0; j--){
    servo1.write(j);
    delay(tunda);
  }
}