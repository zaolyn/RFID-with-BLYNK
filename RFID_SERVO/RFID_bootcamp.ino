#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <MFRC522.h> // import file MFRC522 yang sudah kita install 
#include <Servo.h>

char auth[] = "***************************";
char ssid[] = "namahostpotmu";
char pass[] = "passwordhostpotmu";

#define SDA_PIN D2 // mendefinisikan SDA pin ke D2
#define RST_PIN D1 // mendefinisikan RST pin ke D1
#define motor D4   // mendefinisikan servo ke pin D4

Servo servo;
MFRC522 mfrc522(SDA_PIN, RST_PIN); // setting mfrc dengan SDA dan RST

WidgetLCD lcd (V1); // setting pin virtual LCD ke V1 di aplikasi blynk

void setup() {
  Serial.begin(9600);
  SPI.begin(); // inisiasi SPI bus
  Blynk.begin (auth, ssid, pass);
  mfrc522.PCD_Init();
  pinMode(motor, OUTPUT);
  Serial.println("Tap kartu anda disini !...");
  Serial.println();

}

void loop() {
  Blynk.run();
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  Serial.print("nomor UID anda: ");
  String content = "";
  byte letter;

  for (byte i = 0; i < mfrc522.uid.size; i++)    {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println();
  Serial.print("Message: ");
  content.toUpperCase();

  if (content.substring(1) == "") // masukkan id card
  {
    lcd.clear();
    lcd.print(0, 0, "UID for card is"); // menampilkan UID di Lcd blynk
    lcd.print(0, 1, content);
    delay(1000);
    buka(); //fungsi untuk menggerakan servo

  }
  else
  {
    lcd.clear();
    lcd.print(0, 0, "invalid card");
    lcd.print(0, 1, "acces denied");

  }

}

void buka() {
  servo.attach(motor);
  servo.write(90);
  delay(2000);
  servo.write(0);
}
