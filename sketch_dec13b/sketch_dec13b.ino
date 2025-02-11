#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Wire.h>
#include <MPU6050.h>
#include <Math.h>

// Konfigurasi WiFi
const char* ssid = "Rel (2)";
const char* password = "12345678";

// Konfigurasi Telegram
#define BOTtoken "7714927813:AAFLITgHCj4rFVXG3HMu1bMu04PLqv1tBgw"
#define telegram_chat_id "7383054617"
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Pin untuk relay dan buzzer
const int relayPin = 19; // Ganti dengan pin yang terhubung ke relay
const int buzzerPin = 18; // Pin untuk buzzer

// MPU6050 Setup
MPU6050 mpu;
int16_t ax, ay, az;
float axg, ayg, azg;
float roll, previous_roll = 0, roll_change = 0;
unsigned long rollStartTime = 0; // Waktu mulai pemantauan roll
unsigned long lastEarthquakeTime = 0; // Waktu deteksi terakhir gempa
bool pesan = true;

// Waktu kalibrasi
long last_time = 0;
long calibration_time = 2000;  // waktu kalibrasi sensor (2 detik)
float roll_threshold_sedang = 8; // ambang batas untuk gempa sedang (dalam derajat)
float roll_threshold_kuat = 11;   // ambang batas untuk gempa kuat (dalam derajat)
float roll_threshold_pelan = 4;  // ambang batas untuk gempa pelan (dalam derajat)

// Waktu Bot Telegram
unsigned long lastTimeBotRan;
const int botRunningDelay = 1000; // Delay untuk pengecekan pesan (1 detik)

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Inisialisasi MPU6050
  mpu.initialize();
  
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
  Serial.println("MPU6050 connected successfully");

  // Kalibrasi sensor
  Serial.println("Kalibrasi Sensor...");
  long start_time = millis();
  while (millis() - start_time < calibration_time) {
    mpu.getAcceleration(&ax, &ay, &az);  // Membaca data akselerometer
    axg = ax / 16384.0;  // Mengkonversi ke nilai g
    ayg = ay / 16384.0;
    azg = az / 16384.0;
    delay(10);
  }
  Serial.println("Kalibrasi selesai!");

  // Setup pin relay dan buzzer
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // Relay off saat awal
  digitalWrite(buzzerPin, LOW); // Buzzer off saat awal

  // Koneksi WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup koneksi secure client untuk Telegram
  client.setInsecure(); // Nonaktifkan validasi sertifikat untuk kesederhanaan
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String message_chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    Serial.println("Pesan baru:");
    Serial.println(text);

    if (text == "/sayaaman") {
      digitalWrite(relayPin, HIGH); // Relay on
      bot.sendMessage(message_chat_id, "Pintu dikunci!", "Markdown");
    } else if (text == "/buka") {
      digitalWrite(relayPin, LOW); // Relay off
      bot.sendMessage(message_chat_id, "Relay telah dimatikan!", "Markdown");
    } else {
      bot.sendMessage(message_chat_id, "Perintah tidak dikenali. Gunakan /sayaaman untuk mengunci dan /buka untuk membuka kunci.", "Markdown");
    }
  }
}

void loop() {
  // Cek pesan dari Telegram
  if (millis() > lastTimeBotRan + botRunningDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  // Membaca data akselerometer
  mpu.getAcceleration(&ax, &ay, &az);
  axg = ax / 16384.0;  // Mengkonversi akselerometer ke nilai g
  ayg = ay / 16384.0;
  azg = az / 16384.0;

  // Menghitung sudut roll (dalam derajat)
  roll = atan2(ayg, azg) * 180.0 / PI;

  // Menghitung perubahan roll
  roll_change = abs(roll - previous_roll);

  

  // Cek jika ada perubahan roll yang signifikan
  if (roll_change > roll_threshold_kuat || roll_change > roll_threshold_sedang || roll_change > roll_threshold_pelan) {
    digitalWrite(relayPin, HIGH);  // Matikan relay saat gempa pelan terdeteksi
    digitalWrite(buzzerPin, HIGH); // Nyalakan buzzer saat gempa kuat
    pesan = false;
    Serial.println("Deteksi Perubahan Roll!");

    // Mulai pengambilan nilai perubahan roll selama 1 detik
    rollStartTime = millis();

    // Pemantauan roll selama 1 detik untuk mengukur roll_change
    while (millis() - rollStartTime < 1000) {  
      mpu.getAcceleration(&ax, &ay, &az); 
      axg = ax / 16384.0;
      ayg = ay / 16384.0;
      azg = az / 16384.0;
      roll = atan2(ayg, azg) * 180.0 / PI;

      // Hitung perubahan roll selama 1 detik
      roll_change = abs(roll - previous_roll);

      delay(10); // Membaca ulang setiap 10ms
    }

    // Hanya kirim pesan jika roll_change melebihi threshold gempa
    if (roll_change > roll_threshold_kuat) {
      bot.sendMessage(telegram_chat_id, "Gempa Kuat Terdeteksi!", "Markdown");
      lastEarthquakeTime = millis(); // Catat waktu gempa terakhir
    } else if (roll_change > roll_threshold_sedang) {
      bot.sendMessage(telegram_chat_id, "Gempa Sedang Terdeteksi!", "Markdown");
      lastEarthquakeTime = millis(); // Catat waktu gempa terakhir
    } else if (roll_change > roll_threshold_pelan) {
      bot.sendMessage(telegram_chat_id, "Gempa Pelan Terdeteksi! ", "Markdown");
      lastEarthquakeTime = millis(); // Catat waktu gempa terakhir
    }
  }else{
    // Menampilkan data
    Serial.print("tidak ada gempa");
    Serial.print("\tRoll: ");
    Serial.print(roll);
    Serial.print("\tChange: ");
    Serial.println(roll_change);
  }

  // Jika tidak ada gempa selama lebih dari 5 detik, nyalakan relay dan kirim pesan
  if (millis() - lastEarthquakeTime > 10000 && !pesan) {
    pesan = true;
    digitalWrite(relayPin, LOW);  // Nyalakan relay untuk membuka pintu
    bot.sendMessage(telegram_chat_id, "Tidak ada gempa selama 10 detik. Pintu dikunci otomatis.", "Markdown");
    digitalWrite(buzzerPin, LOW);  // Matikan buzzer setelah 5 detik
  }

  // Simpan nilai roll saat ini untuk perbandingan berikutnya
  previous_roll = roll;

  delay(100);  // Memberikan jeda untuk pembacaan berikutnya
}