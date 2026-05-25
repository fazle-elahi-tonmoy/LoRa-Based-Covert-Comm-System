#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22

#define LORA_NSS 17
#define LORA_RST 16
#define LORA_DIO0 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void sendAck(String id, int pid) {
  StaticJsonDocument<128> doc;

  doc["type"] = "ack";
  doc["id"] = id;
  doc["pid"] = pid;

  String packet;
  serializeJson(doc, packet);

  delay(50);

  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();
}

void sendToDashboard(String json) {
  Serial.print("<");
  Serial.print(json);
  Serial.println(">");
}

void broadcastCommand(String cmd) {
  unsigned long start = millis();

  while (millis() - start < 2000) {
    LoRa.beginPacket();
    LoRa.print(cmd);
    LoRa.endPacket();

    delay(200);
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin(OLED_SDA, OLED_SCL);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(1);

  LoRa.setPins(LORA_NSS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    display.setCursor(0, 0);
    display.print("LoRa Error");
    display.display();
    while (1)
      ;
  }

  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.enableCrc();

  display.setCursor(0, 0);
  display.print("Ground Station");
  display.display();
}

void loop() {
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    String incoming = LoRa.readString();

    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    StaticJsonDocument<256> doc;

    DeserializationError err = deserializeJson(doc, incoming);

    if (err) return;

    if (doc["type"] == "telemetry") {
      String id = doc["id"];
      int pid = doc["pid"];

      doc["rssi"] = rssi;
      doc["snr"] = snr;

      String out;
      serializeJson(doc, out);

      sendToDashboard(out);

      sendAck(id, pid);
    }
  }

  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');

    broadcastCommand(cmd);
  }
}