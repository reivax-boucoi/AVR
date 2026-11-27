#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

// ---- settings ----
const char* WIFI_SSID = "SFR_D430";
const char* WIFI_PASS = "kms59jjsp4jsx77rrpa2";
const char* MQTT_HOST = "192.168.1.9";
const char* MQTT_USER = "mqttuser";
const char* MQTT_PASS = "mqttuser";
const char* OTA_HOST  = "linky-esp";
const char* OTA_PASS  = "password";      // password asked when uploading over Wi-Fi

#define TIC_RX     14
#define TIC_BAUD   1200                   // historic mode
#define INVERT     false
#define PUB_PERIOD 10000UL                // ms between publishes per field
#define DEBUG_PRINT 1

struct Field { const char* label; bool numeric; unsigned long last; };
Field FIELDS[] = {
  {"HCHC",  true,  0},   // off-peak index, Wh
  {"HCHP",  true,  0},   // peak index, Wh
  {"PTEC",  false, 0},   // current period (HP.. / HC..)
  {"PAPP",  true,  0}    // apparent power, VA
};
const int NF = sizeof(FIELDS) / sizeof(FIELDS[0]);

SoftwareSerial tic;
WiFiClient wifi;
PubSubClient mqtt(wifi);
String line;
unsigned long lastMqttTry = 0;

void ticStart() {
  pinMode(TIC_RX, INPUT);
  tic.begin(TIC_BAUD, SWSERIAL_7E1, TIC_RX, -1, INVERT);
}

void mqttReconnect() {
  if (millis() - lastMqttTry < 5000) return;      // retry every 5 s, never block
  lastMqttTry = millis();
  Serial.print("MQTT...");
  if (mqtt.connect("linky-esp", MQTT_USER, MQTT_PASS,
                   "linky/status", 0, true, "offline")) {
    mqtt.publish("linky/status", "online", true);
    Serial.println("ok");
  } else {
    Serial.print("fail rc=");
    Serial.println(mqtt.state());
  }
}

void handleLine(const String& l) {
  int n = l.length();
  if (n < 5) return;

  // historic checksum: label + space + value, masked to 6 bits, plus 0x20
  unsigned sum = 0;
  for (int i = 0; i <= n - 3; i++) sum += (uint8_t)l[i];
  if ((char)((sum & 0x3F) + 0x20) != l[n - 1]) {
    Serial.println("bad checksum: " + l);
    return;
  }

  int sepFirst = l.indexOf(' ');
  int sepLast  = n - 2;
  int sepPrev  = l.lastIndexOf(' ', sepLast - 1);
  if (sepFirst < 0 || sepPrev < 0) return;
  String label = l.substring(0, sepFirst);
  String value = l.substring(sepPrev + 1, sepLast);

#if DEBUG_PRINT
  Serial.println(label + " = " + value);
#endif

  if (!mqtt.connected()) return;
  for (int i = 0; i < NF; i++) {
    if (label == FIELDS[i].label && millis() - FIELDS[i].last >= PUB_PERIOD) {
      FIELDS[i].last = millis();
      String topic   = "linky/" + label;
      String payload = FIELDS[i].numeric ? String(value.toInt()) : value;  // drops leading zeros
      mqtt.publish(topic.c_str(), payload.c_str());
    }
  }
}

void setup() {
  Serial.begin(115200);
  ticStart();
  Serial.println("\nLinky -> MQTT + OTA");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 20000) {
    delay(250);
    Serial.print('.');
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : "no WiFi yet");

  ArduinoOTA.setHostname(OTA_HOST);
  ArduinoOTA.setPassword(OTA_PASS);
  ArduinoOTA.onStart([]() { tic.end(); Serial.println("OTA start"); });
  ArduinoOTA.onEnd([]()   { Serial.println("\nOTA end"); });
  ArduinoOTA.onError([](ota_error_t e) { Serial.printf("OTA error %u\n", e); ticStart(); });
  ArduinoOTA.begin();

  mqtt.setServer(MQTT_HOST, 1883);
}

void loop() {
  ArduinoOTA.handle();                            // keep this call frequent

  if (WiFi.status() == WL_CONNECTED) {
    if (mqtt.connected()) mqtt.loop();
    else mqttReconnect();
  }

  while (tic.available()) {
    char c = tic.read();
    if (c == 0x0A)      line = "";                         // LF: start of a line
    else if (c == 0x0D) { handleLine(line); line = ""; }   // CR: end of a line
    else if (c >= 0x20 && line.length() < 120) line += c;
  }
}
