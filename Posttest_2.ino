#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define BLYNK_TEMPLATE_ID "TMPL6EA3TGWdq"
#define BLYNK_TEMPLATE_NAME "Posttest2"
#define BLYNK_AUTH_TOKEN "tvAIfrOJxWK1kN48NnCk4u82elDdjGKo"

#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "anj";  // type your wifi name
char pass[] = "12345678";  // type your wifi password

#define LEDPIN D8 //Kaki anoda dari LED terhubung ke pin D8 NodeMCU
#define DHTPIN D4 //Pin data dari DHT terhubung ke pin D4 NodeMCU
#define DHTTYPE DHT11 //Mengatur TYPE DHT (Karena ada 2 jenis [DHT11 & DHT22])

BlynkTimer timer;

DHT dht(DHTPIN, DHTTYPE);



void setup(){
  Serial.begin(115200);
  // Mengatur pin LED
  pinMode(LEDPIN, OUTPUT);
  // Setup Blynk
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  // Setup Sensor DHT 11
  dht.begin();
  // Fungsi mengirim data setiap 2,5 detik
  timer.setInterval(2500L, sendSensor);
}

void loop(){
  Blynk.run();
  timer.run();
}

void sendSensor(){
  float h = dht.readHumidity();
  // Mengecek kondisi apakah DHT bisa dibaca atau tidak
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Mengatur nilai kelembaban ke pin V1(Kelembaban) yang sudah diatur pada Blynk
  Blynk.virtualWrite(V1, h);
  
  // Menampilkan ke serial monitor

  Serial.print("Kelembapan : ");
  Serial.println(h);
  // Event log
  if(h <= 60){
    Blynk.logEvent("notifikasi","Kelembapan dibawah 60%");
    digitalWrite(LEDPIN, HIGH);
  }
  else if(h >= 60){
    Blynk.logEvent("notifikasi","Kelembapan diatas 60%");
    digitalWrite(LEDPIN, LOW);
  }
}