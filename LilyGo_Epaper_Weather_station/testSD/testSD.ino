#include "SD.h"
#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson
#include "calendar_events.h"

#define SPI_CS   42
#define SPI_MOSI 15
#define SPI_MISO 16
#define SPI_SCLK 11
void setup() {
    Serial.begin(115200);
    while (!Serial);

    Serial.println("Starting...");

}

void loop() {
    SPI.begin(SPI_SCLK, SPI_MISO, SPI_MOSI, SPI_CS);
    Serial.println("SPI ok...");
    if (SD.begin(SPI_CS)) {
        File f = SD.open("/config.json", FILE_READ);
        if (!f) Serial.println("open fail");

        StaticJsonDocument<4096> doc;
        if (deserializeJson(doc, f)) {
            f.close();
            Serial.println("deserialize fail");
        }
        f.close();

        // WIFI
        if (doc["wifi"]["ssid"]) Serial.println(doc["wifi"]["ssid"].as<String>());
        if (doc["wifi"]["password"]) Serial.println(doc["wifi"]["password"].as<String>());
        // OWM
        if (doc["owm"]["apikey"])   Serial.println(doc["owm"]["apikey"].as<String>());
        if (doc["owm"]["city"]) Serial.println(doc["owm"]["city"].as<String>());
        if (doc["owm"]["lat"])    Serial.println(doc["owm"]["lat"].as<String>());
        if (doc["owm"]["lon"])    Serial.println(doc["owm"]["lon"].as<String>());

        // Birthdays
        if (doc["birthdays"].is<JsonArray>()) {
            //birthdays.clear();
            for (JsonObject b : doc["birthdays"].as<JsonArray>()) {
                /*  birthdays.push_back({
                    b["day"],
                    b["month"],*/
                Serial.println(b["name"].as<String>());
                //});
            }
        }

        Serial.println("SD config loaded");

    } else {
        Serial.println("No SD card detected");
    }
    delay(5000);
}
