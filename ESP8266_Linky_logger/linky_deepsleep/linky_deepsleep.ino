// Linky TIC -> MQTT, simple deep-sleep version (single file)
//
// ESP-12E with GPIO16 wired to RST.
//   * Every 10 minutes: read ONE complete TIC frame, connect Wi-Fi (static IP),
//     publish the four values, go back to deep sleep.
//   * Press the RESET button: no TIC / MQTT, Wi-Fi + ArduinoOTA stay up for 60 s
//     so you can upload a new sketch, then the normal cycle resumes.
//
// Reset reasons: deep-sleep wake-up -> normal cycle, reset button -> OTA window.
#include "secret.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

extern "C" {
#include <user_interface.h>
}



// static IP (adjust gateway/DNS if your box is not 192.168.1.1)
const IPAddress STATIC_IP(192, 168, 1, 7);
const IPAddress GATEWAY(192, 168, 1, 1);
const IPAddress SUBNET(255, 255, 255, 0);
const IPAddress DNS_IP(192, 168, 1, 1);

#define TIC_RX          14
#define TIC_BAUD        1200              // historic mode
#define INVERT          false
#define SLEEP_MS        60000UL          // wake-up interval (10 min)
#define BOOT_MS         250UL             // boot time not seen by millis()
#define TIC_TIMEOUT_MS  10000UL           // max wait for one complete frame
#define WIFI_TIMEOUT_MS 15000UL
#define OTA_WINDOW_MS   60000UL
#define DEBUG_PRINT     0                 // set to 0 for the lowest power

#if DEBUG_PRINT
#define LOG(...) Serial.printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

struct Field { const char* label; bool numeric; String value; bool got; };
Field FIELDS[] = {
  {"HCHC", true,  "", false},   // off-peak index, Wh
  {"HCHP", true,  "", false},   // peak index, Wh
  {"PTEC", false, "", false},   // current period (HP.. / HC..)
  {"PAPP", true,  "", false}    // apparent power, VA
};
const int NF = sizeof(FIELDS) / sizeof(FIELDS[0]);

SoftwareSerial tic;
WiFiClient wifi;
PubSubClient mqtt(wifi);
bool frameBad = false;

// Keep the radio off until we need it (runs before the Wi-Fi stack starts).
void preinit() { ESP8266WiFiClass::preinitWiFiOff(); }

// ---------------------------------------------------------------- TIC

// One TIC line without LF/CR: "LABEL VALUE C" (C = checksum character)
void handleLine(const String& l) {
  int n = l.length();
  if (n < 5) return;

  // historic checksum: label + space + value, masked to 6 bits, plus 0x20
  unsigned sum = 0;
  for (int i = 0; i <= n - 3; i++) sum += (uint8_t)l[i];
  if ((char)((sum & 0x3F) + 0x20) != l[n - 1]) {
    frameBad = true;
    LOG("bad checksum: %s\n", l.c_str());
    return;
  }

  int sepFirst = l.indexOf(' ');
  int sepLast  = n - 2;
  int sepPrev  = l.lastIndexOf(' ', sepLast - 1);
  if (sepFirst < 0 || sepPrev < 0) return;
  String label = l.substring(0, sepFirst);
  String value = l.substring(sepPrev + 1, sepLast);

  for (int i = 0; i < NF; i++) {
    if (label == FIELDS[i].label) {
      FIELDS[i].value = value;
      FIELDS[i].got = true;
    }
  }
}

bool allFieldsGot() {
  for (int i = 0; i < NF; i++) if (!FIELDS[i].got) return false;
  return true;
}

// Waits for one complete frame (STX ... ETX) with valid checksums and all four fields.
bool readTic() {
  pinMode(TIC_RX, INPUT);
  tic.begin(TIC_BAUD, SWSERIAL_7E1, TIC_RX, -1, INVERT);

  String line;
  bool inFrame = false, ok = false;
  unsigned long t0 = millis();

  while (!ok && millis() - t0 < TIC_TIMEOUT_MS) {
    while (tic.available() && !ok) {
      char c = tic.read();
      if (c == 0x02) {                                   // STX: new frame
        inFrame = true;
        frameBad = false;
        line = "";
        for (int i = 0; i < NF; i++) FIELDS[i].got = false;
      } else if (inFrame) {
        if (c == 0x03) {                                 // ETX: end of frame
          if (!frameBad && allFieldsGot()) ok = true;
          else inFrame = false;                          // wait for the next frame
        }
        else if (c == 0x0A) line = "";                   // LF: start of a line
        else if (c == 0x0D) { handleLine(line); line = ""; }   // CR: end of a line
        else if (c >= 0x20 && line.length() < 120) line += c;
      }
    }
    delay(1);                                            // feeds the watchdog
  }
  tic.end();
  LOG("TIC %s after %lu ms\n", ok ? "ok" : "timeout", millis() - t0);
  return ok;
}

// ---------------------------------------------------------------- Wi-Fi / MQTT

bool wifiConnect() {
  WiFi.forceSleepWake();                                 // radio was off since boot
  delay(1);
  WiFi.persistent(false);                                // no flash writes
  WiFi.mode(WIFI_STA);
  WiFi.config(STATIC_IP, GATEWAY, SUBNET, DNS_IP);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < WIFI_TIMEOUT_MS) delay(10);
  bool ok = (WiFi.status() == WL_CONNECTED);
  LOG("WiFi %s after %lu ms\n", ok ? "ok" : "timeout", millis() - t0);
  return ok;
}

void publishAll() {
  mqtt.setServer(MQTT_HOST, 1883);
  mqtt.setSocketTimeout(5);
  if (!mqtt.connect(MQTT_ID, MQTT_USER, MQTT_PASS)) {
    LOG("MQTT fail rc=%d\n", mqtt.state());
    return;                                              // data is dropped
  }
  for (int i = 0; i < NF; i++) {
    String topic   = String("linky/") + FIELDS[i].label;
    String payload = FIELDS[i].numeric ? String(FIELDS[i].value.toInt())  // drops leading zeros
                                       : FIELDS[i].value;
    mqtt.publish(topic.c_str(), payload.c_str());
    LOG("%s = %s\n", topic.c_str(), payload.c_str());
  }
  mqtt.disconnect();                                     // flushes, then closes
}

// ---------------------------------------------------------------- sleep

void goToSleep() {
  WiFi.disconnect(true);
  delay(1);
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);

  unsigned long used = millis() + BOOT_MS;               // subtract active time
  unsigned long ms = (used + 1000UL >= SLEEP_MS) ? 1000UL : SLEEP_MS - used;
  LOG("sleep %lu ms\n", ms);
#if DEBUG_PRINT
  Serial.flush();
#endif
  ESP.deepSleep((uint64_t)ms * 1000ULL);
  for (;;) delay(10);                                    // deepSleep() returns before sleeping
}

// ---------------------------------------------------------------- main

void setup() {
  Serial.begin(115200);
  LOG("\nLinky -> MQTT (deep sleep)\n");

  bool otaMode = (ESP.getResetInfoPtr()->reason == REASON_EXT_SYS_RST);   // reset button
  LOG("mode: %s\n", otaMode ? "OTA window" : "normal");

  if (!otaMode && !readTic()) goToSleep();               // no valid frame: nothing to send
  if (!wifiConnect()) goToSleep();

  if (otaMode) {
    ArduinoOTA.setHostname(OTA_HOST);
    ArduinoOTA.setPassword(OTA_PASS);
    ArduinoOTA.onStart([]()  { LOG("OTA start\n"); });
    ArduinoOTA.onEnd([]()    { LOG("OTA end\n"); });
    ArduinoOTA.onError([](ota_error_t e) { LOG("OTA error %u\n", e); });
    ArduinoOTA.begin();
    LOG("OTA open for %lu s at %s\n", OTA_WINDOW_MS / 1000UL, WiFi.localIP().toString().c_str());

    unsigned long t0 = millis();
    while (millis() - t0 < OTA_WINDOW_MS) {              // an update in progress runs to the end and reboots
      ArduinoOTA.handle();
      delay(1);
    }
  } else {
    publishAll();
  }
  goToSleep();
}

void loop() {}                                           // never reached: setup() ends in deep sleep
