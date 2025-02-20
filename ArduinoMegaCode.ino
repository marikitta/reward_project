//#define RX 11
//#define TX 10
#define RST_PIN  12 // RES pin
#define SS_PIN  53 // SDA (SS) pin
//#include <SoftwareSerial.h>
#include <SPI.h>
#include "MFRC522.h"
#include <Keypad.h>
String rfid_uid = "";
int score = 0;
const byte ROWS = 4; // 4 строки
const byte COLS = 4; // 4 столбца
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
}; 
byte rowPins[ROWS] = {9,8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//SoftwareSerial mySerial(RX, TX);
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
}
void loop()  {
    // Ожидание сканирования RFID-карты
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        rfid_uid = "";
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            rfid_uid += String(mfrc522.uid.uidByte[i], HEX);
        }
        Serial.print("RFID UID: ");
        Serial.println(rfid_uid);
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        
        Serial.println("Введите баллы (используйте '*' для отрицательного числа, '#' — пропуск ввода): ");
        score = getScoreFromKeypad();

        Serial.print("Сохранено: UID=");
        Serial.print(rfid_uid);
        Serial.print(", Баллы=");
        Serial.println(score);
        if (score == 0){
          String dataToSend=rfid_uid;
          Serial1.println(dataToSend);
          Serial.println(dataToSend);
        }
        else {
          String dataToSend=rfid_uid + " " + String(score);
          Serial1.println(dataToSend);
          Serial.println(dataToSend);
        }
    }
}

// Функция для получения числа с клавиатуры
int getScoreFromKeypad() {
    String input = "";
    bool isNegative = false;

    while (true) {
        char key = keypad.getKey();
        if (key) {
            if (key == '*') {
                isNegative = true;
                Serial.print('-');
            } else if (key == '#') {
                Serial.println();
                return (input.length() == 0) ? 0 : (isNegative ? -input.toInt() : input.toInt());  // Если ввода не было — 0
            } else if (isdigit(key)) {
                input += key;
                Serial.print(key);
            }
        }
    }
}