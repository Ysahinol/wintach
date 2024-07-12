#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "INVENTIONLAND_5GHz";         // WiFi SSID'nizi girin
const char* password = "TRInvent070634$"; // WiFi şifrenizi girin

const int motorPin1 = 21;     // Motor sürücü IN1 pin
const int motorPin2 = 23;    // Motor sürücü IN2 pin
const int reedPin = 19;      // Reed switch pin

WebServer server(80);        // Web sunucusu portu

// index_html.h dosyasını dahil et
#include "index_html.h"

void setup() {
  Serial.begin(115200);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(reedPin, INPUT_PULLUP); // Pull-up direnci kullan

  // Motoru başlangıçta durdur
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);

  // WiFi'ye bağlan
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi'ye bağlanıyor...");
  }
  Serial.println("WiFi'ye bağlandı!");
  Serial.print("IP adresi: ");
  Serial.println(WiFi.localIP());

  // Web sunucusu rotalarını tanımla
  server.on("/", HTTP_GET, [](){
    server.send(200, "text/html", index_html);
  });

  server.on("/open", HTTP_GET, handleOpen);
  server.on("/close", HTTP_GET, handleClose);
  server.begin();
  Serial.println("Web sunucusu başlatıldı.");
}

void loop() {
  server.handleClient();

  // Reed switch'in durumunu sürekli kontrol et
  if (digitalRead(reedPin) == LOW) {
    // Reed switch 0 ise motoru durdur
    stopMotor();
  }
}

void handleOpen() {
  open();
  server.send(200, "text/plain", "Motor is open.");
}

void handleClose() {
  close();
  server.send(200, "text/plain", "Motor is close.");
}

void open() {
  Serial.println("Motor açılıyor...");
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  delay(10000);       // 10 saniye çalıştır
  stopMotor();
}

void close() {
  Serial.println("Motor kapanıyor...");
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  while (digitalRead(reedPin) == HIGH) {
    delay(100);
  }
  stopMotor();
}

void stopMotor() {
  Serial.println("Motor durduruluyor...");
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}
