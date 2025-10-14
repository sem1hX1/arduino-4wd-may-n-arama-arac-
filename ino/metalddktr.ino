/*
 * Metal detector with buzzer
 * Based on: https://www.instructables.com/id/Simple-Arduino-Metal-Detector/
 * * Bu kod, bir bobine kısa elektrik darbeleri (pulse) göndererek manyetik alan oluşturur.
 * Metal yaklaştığında, bobin endüktansı değişir ve bobine bağlı kapasitörün deşarj süresi
 * (analog okuma) değişir. Bu değişiklik algılanarak metal tespit edilir.
 */

// --- Pin ve Temel Parametre Tanımlamaları ---

int out_pin = 13;               // Arduino'nun dahili LED'i (veya ek bir çıkış pini).
const byte npulse = 12;         // Her ölçüm döngüsünde bobine gönderilecek darbe sayısı.

const byte pin_pulse = A0;      // Bobini şarj etmek için darbeleri gönderen pin.
const byte pin_cap   = A1;      // Bobin-kapasitör devresindeki gerilimi (deşarj tepkisini) ölçen pin.
const byte pin_LED   = A2;      // Metal tespit edildiğinde yanacak harici LED pini.
const byte pin_buzzer= A3;      // Metal tespit edildiğinde ötecek buzzer pini.

void setup() {
  // --- Başlangıç Ayarları: Pinleri Çıkış/Giriş olarak ayarla ve Sıfırla ---
  
  pinMode(pin_pulse, OUTPUT);
  digitalWrite(pin_pulse, LOW);
  
  pinMode(pin_cap, INPUT);      // Kapasitör gerilimini ölçmek için giriş pini.
  
  pinMode(pin_LED, OUTPUT);
  digitalWrite(pin_LED, LOW);
  
  pinMode(pin_buzzer, OUTPUT);
  digitalWrite(pin_buzzer, LOW);
  
  pinMode(out_pin, OUTPUT);       // Doğru kullanım bu
  digitalWrite(out_pin, LOW);     // Başlangıçta LOW
}


// --- Global Değişkenler: Kalibrasyon ve Filtreleme İçin ---

const int nmeas = 256;         // Gürültüyü azaltmak için her döngüde alınacak ölçüm sayısı.
long int sumsum = 0;           // Uzun vadeli toplamı (kayar ortalamayı / baseline'ı) tutan değişken.
long int skip = 0;             // Ortalamayı güncellemeden önceki atlama (bekleme) sayacı.
long int diff = 0;             // Anlık ölçüm ile uzun vadeli ortalama arasındaki fark (Metal Sinyali).
long int flash_period = 0;     // LED/buzzer'ın yanıp sönme periyodu (sinyal gücüne bağlı).
long unsigned int prev_flash = 0; // Son yanıp sönme zamanını tutar.


void loop() {
  // --- Ana Ölçüm Döngüsü Başlangıcı ---

  int minval = 2000;  // Ölçülen en düşük değeri tutar. (Gürültü filtreleme için)
  int maxval = 0;     // Ölçülen en yüksek değeri tutar. (Gürültü filtreleme için)

  long unsigned int sum = 0; // 258 ölçümün toplamını tutar.

  // Gürültüyü azaltmak için nmeas+2 (258) kere ölçüm yap.
  for (int imeas = 0; imeas < nmeas + 2; imeas++) {
    
    // 1. Kapasitörü Boşalt (Deşarj)
    // Ölçüm pinini OUTPUT ve LOW yaparak kapasitördeki yükü sıfırla.
    pinMode(pin_cap, OUTPUT);
    digitalWrite(pin_cap, LOW);
    delayMicroseconds(20);
    pinMode(pin_cap, INPUT); // Pini tekrar ölçüm için INPUT moduna al.

    // 2. Bobine Darbe Gönder (Manyetik Alan Oluştur)
    for (int ipulse = 0; ipulse < npulse; ipulse++) {
      digitalWrite(pin_pulse, HIGH);
      delayMicroseconds(3);
      digitalWrite(pin_pulse, LOW);
      delayMicroseconds(3);
    }

    // 3. Değeri Oku
    // Bobinin deşarj tepkisini analog olarak oku (Metal varsa değer değişecektir).
    int val = analogRead(pin_cap);
    
    // Min/Max değerleri kaydet ve toplamı al.
    minval = min(val, minval);
    maxval = max(val, maxval);
    sum += val;

    // --- LED ve Buzzer Zamanlaması Kontrolü ---
    
    long unsigned int timestamp = millis();
    byte ledstat = 0; // 0=Kapalı, 1=Pozitif Sapma, 2=Negatif Sapma (Genellikle Metal Sinyali)
    
    // Kısa yanıp sönme kontrolü (Sürekli yanma durumu).
    if (timestamp < prev_flash + 12) {
      if (diff > 0) ledstat = 1;
      if (diff < 0) ledstat = 2; 
    }
    // Periyodik yanıp sönme kontrolü (flash_period'a bağlı).
    if (timestamp > prev_flash + flash_period) {
      if (diff > 0) ledstat = 1;
      if (diff < 0) ledstat = 2;
      prev_flash = timestamp;
    }
    // Periyot çok uzunsa (sinyal çok zayıfsa) kapat.
    if (flash_period > 1000) ledstat = 0; 

    // LED ve Buzzer Çıkışlarını Ayarla
    if (ledstat == 0) {
      digitalWrite(pin_LED, LOW);
      digitalWrite(pin_buzzer, LOW);
      digitalWrite(out_pin, LOW);
    }
    if (ledstat == 1) { // Genellikle metal sinyali (diff < 0) olduğu için bu durum (1) LOW bırakılır.
      digitalWrite(pin_LED, LOW);
      digitalWrite(pin_buzzer, LOW);
      digitalWrite(out_pin, LOW);
    }
    if (ledstat == 2) { // Metal Algılandı (Negatif sapma = Güçlü Sinyal)
      digitalWrite(pin_LED, HIGH);
      digitalWrite(pin_buzzer, HIGH);
      digitalWrite(out_pin, HIGH);  
    } else {
      digitalWrite(pin_LED, LOW);
      digitalWrite(pin_buzzer, LOW);
      digitalWrite(out_pin, LOW);
    }
  }

  // --- Kalibrasyon ve Fark Hesaplama ---

  // Aykırı değerleri (min ve max) atarak toplamı temizle (Gürültü Filtreleme).
  sum -= minval;
  sum -= maxval;

  // sumsum ilk kez çalışıyorsa, başlangıç değerini ata. (Hızlı başlatma)
  if (sumsum == 0) sumsum = sum << 6;
  
  // Kayar Ortalama (AvgSum) hesapla. (>> 6 = /64, +32 = yuvarlama)
  long int avgsum = (sumsum + 32) >> 6;
  
  // Anlık ölçüm ile uzun vadeli referans arasındaki farkı bul.
  diff = sum - avgsum;

  // --- Otomatik Kalibrasyon Adaptasyonu ---
  
  // Eğer fark çok küçükse (normal dalgalanma), ortalamayı yavaşça güncelle.
  if (abs(diff) < (avgsum >> 10)) {
    sumsum = sumsum + sum - avgsum;
    skip = 0;
  } else {
    // Fark büyükse (metal sinyali), ortalamayı hemen güncelleme, atla.
    skip++;
  }
  
  // Eğer anomali (büyük fark) durumu 64 döngüden fazla sürerse, ortam değişmiştir, yeni referans al.
  if (skip > 64) {
    sumsum = sum << 6;
    skip = 0;
  }

  // Sinyal gücüne göre uyarı periyodunu hesapla. (Sinyal büyüdükçe diff büyür, flash_period küçülür.)
  if (diff == 0) flash_period = 1000000; // Sinyal yoksa periyodu çok uzat (sürekli kapalı).
  else flash_period = avgsum / (2 * abs(diff));
}