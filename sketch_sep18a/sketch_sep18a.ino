int led=11;
int led2=8;
int led3=6;
int irsensor = 10;
int delai = 150;
int const buzzer = 12;

void setup() 
{
  Serial.begin(9600);
  pinMode(led, OUTPUT); //wajib ditulis
  pinMode(led2, OUTPUT); //wajib ditulis
  pinMode(led3, OUTPUT); //wajib ditulis
  pinMode(irsensor, INPUT);
  digitalWrite(led, LOW); //wajib
  digitalWrite(led2, LOW); //wajib
  digitalWrite(led3, LOW); //wajib
  delay(1000); //satuan milisecond
  noTone(buzzer);
}

void playIceCreamMelody() 
{
  // Nada mirip melodi es krim
  tone(buzzer, 880);  // A5
  delay(300);
  tone(buzzer, 784);  // G5
  delay(300);
  tone(buzzer, 659);  // E5
  delay(300);
  tone(buzzer, 784);  // G5
  delay(300);
  
  tone(buzzer, 880);  // A5
  delay(300);
  tone(buzzer, 988);  // B5
  delay(300);
  tone(buzzer, 880);  // A5
  delay(600);
  noTone(buzzer);      // Matikan buzzer setelah melodi selesai
}

void loop() 
{
  int irbaca=digitalRead(irsensor);
  if(irbaca==0)
  {    
    Serial.println("Ada objek terdeteksi");
    delay(50); // Menambahkan sedikit delay agar tidak terlalu cepat

    digitalWrite(led, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    delay(20);

    digitalWrite(led, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    delay(20);

    digitalWrite(led, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
    delay(20);

    // Mainkan melodi seperti es krim
    playIceCreamMelody();

    digitalWrite(led, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    delay(20);
  }  
  else
  {
    noTone(buzzer);
    Serial.println("Tidak ada objek yang terdeteksi");
    delay(50); // Tambahkan delay di sini juga
    digitalWrite(led, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    delay(20);
  }
}
