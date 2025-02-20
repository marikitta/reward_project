//#include <SoftwareSerial.h>
//#define RX 7
//#define TX 8
//SoftwareSerial mySerial(RX, TX);
#include <ESP8266WiFi.h>
#define BOT_TOKEN "input"
#define WIFI_SSID "input"
#define WIFI_PASS "input"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void setup() {
  Serial.begin(115200);
  //mySerial.begin(4800);
  bot.setChatID("897310208");
  connectWiFi();
  //bot.attach(newMsg);
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
  Serial.print(msg.username);   // логин
  Serial.print(", ");
  Serial.println(msg.text);     // текст
}

void loop() {
  bot.tick();
  if (Serial.available()){
    String recivedData = Serial.readStringUntil('\n');
    recivedData.trim();
    Serial.println(recivedData);
    bot.sendMessage(recivedData);
  }
}

void connectWiFi() {
  delay(2000);
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
}