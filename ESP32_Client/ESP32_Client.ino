#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPSPlus.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1);
TinyGPSPlus gps;
HardwareSerial LoRaSerial(1);
HardwareSerial gpsSerial(2);

#define LORA_RX 18
#define LORA_TX 19

#define GPS_RX 25
#define GPS_TX 26

#define beeper 13
#define button 4

#define DEVICE_ID 3
#define fixed_delay 500 * DEVICE_ID

float lat = 0;
float lon = 0;
int sat = 0;
float bat = 0;
bool button_query = 0;

unsigned long lastPing = 0, t = 0, cooldown = 0;
String loraBuffer = "";

float readBattery() {
  int raw = analogRead(33);
  float v = (float)raw / 780;
  return v;
}

void sendTelemetry() {
  delay(fixed_delay);
  String message = String(DEVICE_ID);
  message += "|";
  message += String(sat);
  message += "|";
  message += String(lat, 6);
  message += "|";
  message += String(lon, 6);
  String cmd = "AT+SEND=1," + String(message.length()) + "," + message + "\r\n";
  LoRaSerial.print(cmd);
  lastPing = millis();
}


void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("ID:");
  display.print(DEVICE_ID);
  display.print("       BAT:");
  display.println(bat);
  display.print("SAT COUNT: ");
  display.println(sat);
  display.print("LATTITUDE: ");
  display.println(lat, 6);
  display.print("LONGITUDE: ");
  display.println(lon, 6);
  display.print("Last Ping: ");
  display.print((millis() - lastPing) / 1000);
  display.println("s");
  display.println(loraBuffer);
  display.display();
}

void text(String t, int x, int y, int size) {
  display.clearDisplay();
  display.setTextSize(size);
  display.setCursor(x, y);
  display.print(t);
  display.display();
}

bool LoRa_check() {
  LoRaSerial.print("AT\r\n");
  delay(100);
  String feedback = LoRaSerial.readStringUntil('\n');
  if (feedback.indexOf("+READY") != -1 || feedback.indexOf("+OK") != -1) {
    LoRaSerial.print("AT+ADDRESS=0\r\n");
    return 1;
  }
  return 0;
}

void beep(int interval, int repeat) {
  for (int i = 0; i < repeat; i++) {
    digitalWrite(beeper, 1);
    delay(interval);
    digitalWrite(beeper, 0);
    if (i < repeat - 1) delay(interval);
  }
}

void setup() {
  pinMode(beeper, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(SSD1306_WHITE);
  text("BOOTING...", 4, 24, 2);

  Serial.begin(115200);
  LoRaSerial.begin(115200, SERIAL_8N1, LORA_RX, LORA_TX);
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  delay(1000);
  if (LoRa_check() == 0) {
    while (1) {
      text("LoRa FAIL", 11, 24, 2);
      delay(1000);
      if (LoRa_check() == 1) break;
    }
  }
}

void loop() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
    // yield();
  }

  if (gps.location.isValid()) {
    lat = gps.location.lat();
    lon = gps.location.lng();
  }

  sat = gps.satellites.value();
  bat = readBattery();

  if (LoRaSerial.available()) {
    loraBuffer = LoRaSerial.readStringUntil('\n');
    if (loraBuffer.indexOf("UPDATE") != -1) sendTelemetry();
    if (loraBuffer.indexOf("FIRE") != -1) {
      int index1 = loraBuffer.indexOf("|") + 1;
      int device = loraBuffer[index1] - '0';
      if (device == DEVICE_ID || device == 0) {
        text("  FIRE!  ", 11, 24, 2);
        beep(1000, 1);
        delay(3000);
      }
    }

    if (loraBuffer.indexOf("MSG") != -1) {
      int index1 = loraBuffer.indexOf("|");
      int device = loraBuffer[index1 + 1] - '0';
      if (device == DEVICE_ID) {
        int index2 = loraBuffer.indexOf("|", index1 + 1);
        int index3 = loraBuffer.indexOf(",", index2 + 1);
        String message = loraBuffer.substring(index2 + 1, index3);
        text(message, 0, 24, 2);
        beep(100, 2);
        delay(3000);
      }
    }

    if (loraBuffer.indexOf("READY|") != -1) {
      int index1 = loraBuffer.indexOf("|") + 1;
      text("READY: " + String(loraBuffer[index1]), 0, 24, 2);
      beep(100, 2);
      delay(3000);
    }
  }

  if (millis() - t > 1000) {
    updateDisplay();
    t = millis();
  }

  if (digitalRead(button) == 0 && millis() - cooldown > 5000) {
    button_query = 1;
    cooldown = millis();
  }

  if (button_query && millis() - lastPing > 2000) {
    button_query = 0;
    String cmd = "AT+SEND=0,7,READY|" + String(DEVICE_ID) + "\r\n";
    LoRaSerial.print(cmd);
  }

  delay(10);
}