// --- Motor Kontrol Pin Tanımlamaları ---

// Motor A (Sol Motor Grubu) Pinleri
const int speedPinA = 3;    // Motor A hız kontrol pini (PWM - ~ işareti olan pin olmalı)
const int motorA_in1 = 4;   // Motor A yön pini 1
const int motorA_in2 = 5;   // Motor A yön pini 2

// Motor B (Sağ Motor Grubu) Pinleri
const int motorB_in1 = 6;   // Motor B yön pini 1
const int motorB_in2 = 7;   // Motor B yön pini 2
const int speedPinB = 9;    // Motor B hız kontrol pini (PWM - ~ işareti olan pin olmalı)

// --- Değişkenler ---

int state = 'S';            // Mevcut durumu (komutu) tutar. Başlangıçta 'S' (DUR) ayarlı.
int vSpeed = 255;           // Motor hızı (PWM değeri). 0 (min) ile 255 (maks) arası.

void setup() {
  // --- Pin Modlarının Ayarlanması ---
  
  // Motor A çıkış pinleri
  pinMode(speedPinA, OUTPUT);
  pinMode(motorA_in1, OUTPUT);
  pinMode(motorA_in2, OUTPUT);
  
  // Motor B çıkış pinleri
  pinMode(speedPinB, OUTPUT);
  pinMode(motorB_in1, OUTPUT);
  pinMode(motorB_in2, OUTPUT);

  // Seri haberleşmeyi başlat (Bluetooth veya USB komutları için)
  Serial.begin(9600);
}

void loop() {
  // --- Seri Komut Kontrolü ---
  
  // Seri portta veri varsa oku ve durumu güncelle
  if (Serial.available() > 0) {
    state = Serial.read();
    Serial.print("Gelen komut: ");
    Serial.println((char)state); // Gelen ASCII karakteri ekrana yazdır.
  }

  // --- Hız Ayarları (0-4 arası komutlar) ---
  if (state == '0') vSpeed = 0;   // DUR
  else if (state == '1') vSpeed = 100; // Yavaş
  else if (state == '2') vSpeed = 180; // Orta
  else if (state == '3') vSpeed = 200; // Hızlıya yakın
  else if (state == '4') vSpeed = 255; // Tam Hız

  // --- Motor Kontrol Komutları (Hareket) ---
  if (state == 'F') { // İleri
    ileri();
  }
  else if (state == 'B') {  // Geri
    geri();
  }
  else if (state == 'R') {  // Sağa Pivot (Araç etrafında döner)
    sagaPivot(); // Fonksiyon adı 'sagaPivot' olmasına rağmen 'R' (Sağ) tuşu ile sağa dönüş (Pivot) yapılır.
  }
  else if (state == 'L') {  // Sola Pivot (Araç etrafında döner)
    solaPivot(); // Fonksiyon adı 'solaPivot' olmasına rağmen 'L' (Sol) tuşu ile sola dönüş (Pivot) yapılır.
  }
  else if (state == 'S') {  // Dur
    dur();
  }
  // Çapraz Hareket Komutları
  else if (state == 'G') {  // İleri Sol
    ileriSol();
  }
  else if (state == 'J') {  // Geri Sol
    geriSol();
  }
  else if (state == 'I') {  // İleri Sağ
    ileriSag();
  }
  else if (state == 'H') {  // Geri Sağ
    geriSag();
  }
}


// ###########################################################
// ##                                                       ##
// ##                  HAREKET FONKSİYONLARI                ##
// ##                                                       ##
// ###########################################################

// --- Düz Hareket Fonksiyonları ---

void ileri() {
  // Motor A (Sol): İleri
  digitalWrite(motorA_in1, HIGH);
  digitalWrite(motorA_in2, LOW);
  // Motor B (Sağ): İleri (Yön pinleri Motor A'nın tersi olabilir, motor bağlantısına göre kontrol edilmeli)
  digitalWrite(motorB_in1, LOW); 
  digitalWrite(motorB_in2, HIGH);
  
  // Ayarlanan hızda çalıştır
  analogWrite(speedPinA, vSpeed);
  analogWrite(speedPinB, vSpeed);
}

void geri() {
  // Motor A (Sol): Geri
  digitalWrite(motorA_in1, LOW);
  digitalWrite(motorA_in2, HIGH);
  // Motor B (Sağ): Geri
  digitalWrite(motorB_in1, HIGH);
  digitalWrite(motorB_in2, LOW);
  
  // Ayarlanan hızda çalıştır
  analogWrite(speedPinA, vSpeed);
  analogWrite(speedPinB, vSpeed);
}

void dur() {
  // Tüm motorları durdur (IN1 ve IN2'yi LOW yaparak motor sürücüde frenleme yap)
  digitalWrite(motorA_in1, LOW);
  digitalWrite(motorA_in2, LOW);
  digitalWrite(motorB_in1, LOW);
  digitalWrite(motorB_in2, LOW);
  
  // Hız pinlerini de sıfırla
  analogWrite(speedPinA, 0);
  analogWrite(speedPinB, 0);
}

// --- Pivot Dönüş Fonksiyonları (Tek Tekerlek Çalışır) ---

void solaPivot() {
  // Araç Sola Döner (Sol Motorlar İleri, Sağ Motorlar Durur)
  // Motor A (Sol): İleri
  digitalWrite(motorA_in1, HIGH);
  digitalWrite(motorA_in2, LOW);
  analogWrite(speedPinA, vSpeed);
  
  // Motor B (Sağ): Dur
  digitalWrite(motorB_in1, LOW);
  digitalWrite(motorB_in2, LOW);
  analogWrite(speedPinB, 0);
}

void sagaPivot() {
  // Araç Sağa Döner (Sağ Motorlar İleri, Sol Motorlar Durur)
  // Motor A (Sol): Dur
  digitalWrite(motorA_in1, LOW);
  digitalWrite(motorA_in2, LOW);
  analogWrite(speedPinA, 0);
  
  // Motor B (Sağ): İleri
  digitalWrite(motorB_in1, LOW); // Motor B'nin ileri yönü.
  digitalWrite(motorB_in2, HIGH);
  analogWrite(speedPinB, vSpeed);
}

// --- Yumuşak Dönüş Fonksiyonları (Çapraz Dönüşler) ---

void ileriSol() {
  // İleri ve Sola Yönelme (Sol Motor yavaş, Sağ Motor hızlı)
  digitalWrite(motorA_in1, HIGH);
  digitalWrite(motorA_in2, LOW);
  digitalWrite(motorB_in1, LOW);
  digitalWrite(motorB_in2, HIGH);
  
  analogWrite(speedPinA, 50);     // Sol Motor yavaşlatılır
  analogWrite(speedPinB, vSpeed); // Sağ Motor tam hızda bırakılır
}

void geriSol() {
  // Geri ve Sola Yönelme (Sol Motor yavaş, Sağ Motor hızlı - Geri yönde)
  digitalWrite(motorA_in1, LOW);
  digitalWrite(motorA_in2, HIGH);
  digitalWrite(motorB_in1, HIGH);
  digitalWrite(motorB_in2, LOW);
  
  analogWrite(speedPinA, vSpeed); // Sol Motor hızlı
  analogWrite(speedPinB, 50);     // Sağ Motor yavaşlatılır
}

void ileriSag() {
  // İleri ve Sağa Yönelme (Sol Motor hızlı, Sağ Motor yavaş)
  digitalWrite(motorA_in1, HIGH);
  digitalWrite(motorA_in2, LOW);
  digitalWrite(motorB_in1, LOW);
  digitalWrite(motorB_in2, HIGH);
  
  analogWrite(speedPinA, vSpeed); // Sol Motor tam hızda bırakılır
  analogWrite(speedPinB, 50);     // Sağ Motor yavaşlatılır
}

void geriSag() {
  // Geri ve Sağa Yönelme (Sol Motor yavaş, Sağ Motor hızlı - Geri yönde)
  digitalWrite(motorA_in1, LOW);
  digitalWrite(motorA_in2, HIGH);
  digitalWrite(motorB_in1, HIGH);
  digitalWrite(motorB_in2, LOW);
  
  analogWrite(speedPinA, 50);     // Sol Motor yavaşlatılır
  analogWrite(speedPinB, vSpeed); // Sağ Motor tam hızda bırakılır
}