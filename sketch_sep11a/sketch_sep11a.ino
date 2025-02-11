int led1= 13;
int led2= 11;
int led3= 8;
int led4= 6;

void setup() 
{
  pinMode(led1, OUTPUT); //bersifat wajib ditulis
  digitalWrite(led1, LOW);//intruksi 
  delay(1000); //satuan dalam ms
  pinMode(led2, OUTPUT); //bersifat wajib ditulis
  digitalWrite(led2, LOW);//intruksi 
  delay(1000); //satuan dalam ms
  pinMode(led3, OUTPUT); //bersifat wajib ditulis
  digitalWrite(led3, LOW);//intruksi 
  delay(1000); //satuan dalam ms
  pinMode(led4, OUTPUT); //bersifat wajib ditulis
  digitalWrite(led4, LOW);//intruksi 
  delay(1000); //satuan dalam ms
}

void loop() 
{
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  delay(200);

  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  delay(200);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, LOW);
  delay(200);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, HIGH);
  delay(200);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, LOW);
  delay(200);

  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  delay(200);

}