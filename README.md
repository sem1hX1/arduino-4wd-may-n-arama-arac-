# Arduino Tabanlı Güneş Enerjili ve Metal Dedektörlü 4WD Araç

![Proje Görseli](https://via.placeholder.com/800x400.png?text=Projenizin+Fotoğrafını+Buraya+Ekleyin)
*Projenizin bir fotoğrafını veya GIF demosunu buraya ekleyerek daha çekici hale getirebilirsiniz.*

---

## 📖 Genel Bakış

Bu proje, **Arduino** platformu kullanılarak geliştirilmiş, 4 tekerlekten çekişli (4WD) bir akıllı araçtır. Çevresindeki metal nesneleri tespit edip kullanıcıyı uyarabilen bir **metal dedektörü**, kendi enerjisini üretebilen bir **güneş paneli** ve akıllı **otonom aydınlatma** sistemi gibi modern özelliklerle donatılmıştır.

Bu araç, hem hobi elektroniği meraklıları hem de otonom sistemler ve yenilenebilir enerji konularına ilgi duyan öğrenciler için harika bir başlangıç noktasıdır.

---

## ✨ Temel Özellikler

-   **🛰️ Metal Algılama:** Aracın ön kısmında bulunan hassas metal dedektörü sayesinde, çevredeki metal nesneleri algılar ve sesli/görsel bir uyarı sistemi ile kullanıcıyı bilgilendirir.

-   **⚡ Hibrit Şarj Sistemi:**
    -   **Güneş Paneli:** Üzerinde bulunan güneş paneli ile güneş enerjisini kullanarak kendi kendini şarj edebilir ve çalışma süresini uzatabilir.
    -   **Type-C Portu:** Modern ve hızlı şarj için Type-C bağlantı noktasına sahiptir.

-   **💡 Otomatik Aydınlatma:** LDR (Işık Bağımlı Direnç) sensörü sayesinde ortam ışığını sürekli olarak ölçer. Hava karardığında veya karanlık bir ortama girildiğinde farlarını **otonom** şekilde açar ve aydınlık ortamlarda kapatarak enerji tasarrufu sağlar.

-   **🚗 4WD Şasi:** Dört tekerlekten çekiş sistemi, araca zorlu arazi koşullarında bile yüksek manevra kabiliyeti ve stabilite kazandırır.

-   **🧩 Genişletilebilir Platform:** Arduino tabanlı olması sayesinde, yeni sensörler (örneğin mesafe sensörü, kamera vb.) ve modüller eklenerek kolayca geliştirilebilir.

---

## 🛠️ Donanım Bileşenleri

| Bileşen            | Açıklama                                      |
| ------------------ | --------------------------------------------- |
| **Mikrodenetleyici** | Arduino UNO (veya Nano/Mega)                  |
| **Şasi** | 4WD Robot Araç Kiti                           |
| **Motor Sürücü** | L298N veya benzeri bir motor sürücü kartı     |
| **Sensörler** | Metal Dedektör Modülü, LDR Işık Sensörü       |
| **Güç Sistemi** | Li-Po/Li-Ion Pil, Güneş Paneli, Type-C Şarj Modülü |
| **Aydınlatma** | LED'ler (Farlar için)                         |
| **Uyarı** | Buzzer (Metal uyarısı için - isteğe bağlı)    |
| **Diğer** | Jumper Kablolar, Anahtar vb.                  |

---

## ⚙️ Yazılım ve Kurulum

Projenin kodları Arduino IDE kullanılarak geliştirilmiştir.

1.  Bu repoyu bilgisayarınıza klonlayın veya ZIP olarak indirin:
    ```bash
    git clone [https://github.com/KULLANICI-ADINIZ/PROJE-ADINIZ.git](https://github.com/KULLANICI-ADINIZ/PROJE-ADINIZ.git)
    ```

2.  Gerekli kütüphaneleri Arduino IDE'nize yükleyin (eğer varsa, burada listeleyin).
    ```
    - ÖrnekKutuphane1.h
    - ÖrnekKutuphane2.h
    ```

3.  `.ino` uzantılı dosyayı Arduino IDE ile açın.

4.  Doğru kart ve port seçimini yaptıktan sonra kodu Arduino'nuza yükleyin.

---

## 🚀 Kullanım

1.  Aracın pilinin dolu olduğundan emin olun. Aracı güneş ışığına bırakarak veya Type-C portu üzerinden şarj edebilirsiniz.
2.  Aracın güç anahtarını açın.
3.  Araç, metal bir nesneye yaklaştığında uyarı verecektir.
4.  Ortam karardığında farlar otomatik olarak yanacaktır.

---

## 🤝 Katkıda Bulunma

Bu projeye katkıda bulunmak isterseniz, lütfen bir "pull request" oluşturun veya "issue" açarak fikirlerinizi ve geliştirmelerinizi paylaşın. Her türlü katkıya açığız!

1.  Projeyi Fork'layın.
2.  Yeni bir dal oluşturun (`git checkout -b ozellik/YeniOzellik`).
3.  Değişikliklerinizi commit'leyin (`git commit -m 'Yeni bir özellik eklendi'`).
4.  Dalınızı push'layın (`git push origin ozellik/YeniOzellik`).
5.  Bir Pull Request açın.

---

## 📜 Lisans

Bu proje MIT Lisansı altında lisanslanmıştır. Daha fazla bilgi için `LICENSE` dosyasına bakın.
