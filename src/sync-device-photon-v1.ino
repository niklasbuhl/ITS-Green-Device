// #include "Particle.h"
#include "neopixel.h"
// #include "JsonParserGeneratorRK.h"
#include "HttpClient/HttpClient.h"
#define ARDUINOJSON_ENABLE_PROGMEM 0
#include <ArduinoJson.h>
// #include "SparkJson.h"

#define PIXEL_COUNT 8
#define PIXEL_PIN D2
#define PIXEL_TYPE WS2812

#define JSON_COLOR_TYPE "INT"
// #define JSON_COLOR_TYPE "STR"

const size_t capacityRGB = JSON_OBJECT_SIZE(3) + 10;

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

HttpClient http;

http_request_t request;
http_response_t response;
http_header_t headers[] = {
    // { "Content-Type", "application/json" },
    { "Content-Type", "text/html; charset=utf-8" },
    // { "Content-Type", "text/html"},
    // { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

void setup() {

  Serial.begin(9600);
  while (!Serial) continue;
  Serial.println("Hello World! Version 3");

  // WiFi.setCredentials("sebastians iPhone", "tnu2e36bmd2y2");
  WiFi.setCredentials("nbxyzmobile", "orangepanda");

  strip.begin();
  strip.show();
  strip.setBrightness(255);

  fill(0,255,0);
  strip.show();
  delay(1000);
  fill(0,0,0);
  strip.show();

}

void loop() {

  // Start Loop
  int startLoop = millis();
  int now = startLoop;

  // Request RGB

  request.hostname = "172.20.10.4";
  // request.hostname = "ekymjshb.p51.rt3.io";
  // http://gfeoqwmr.p50.rt3.io
  request.port = 80;
  // request.path = "/";
  request.path = "/api/session/getdevicecolor/";

  Serial.print("Connecting to: ");
  Serial.print(request.hostname);
  Serial.print(":");
  Serial.print(request.port);
  Serial.println(request.path);

  http.get(request, response, headers);

  Serial.print("Application\tResponse status: ");
  Serial.println(response.status);

  Serial.print("Application\tHTTP Response Body: ");
  Serial.println(response.body);

  Serial.printf("It took %i millis to request.\n", millis() - now);
  now = millis();

  int len = response.body.length();
  char buf[len];
  response.body.toCharArray(buf, len);

  DynamicJsonDocument doc(capacityRGB);

  DeserializationError error = deserializeJson(doc, buf);

  if (error) {
   Serial.print(F("deserializeJson() failed: "));
   Serial.println(error.c_str());
   return;
  }

  int rInt;
  int gInt;
  int bInt;

  // If the response is string numbers
  if (JSON_COLOR_TYPE == "STR") {
    const char *rCharArr = doc["r"];
    const char *gCharArr = doc["g"];
    const char *bCharArr = doc["b"];

    String rStr = rCharArr;
    String gStr = gCharArr;
    String bStr = bCharArr;

    rInt = rStr.toInt();
    gInt = gStr.toInt();
    bInt = bStr.toInt();

  // if the response is int numbers
  } else if (JSON_COLOR_TYPE == "INT") {

    rInt = doc["r"];
    gInt = doc["g"];
    bInt = doc["b"];

  }

  Serial.printf("It took %i millis to get JSON values.\n", millis() - now);
  now = millis();

  Serial.printf("RGB: %i, %i, %i\n", rInt, gInt, bInt);

  fill(rInt, gInt, bInt);
  strip.show();

  Serial.printf("It took %i millis to control the RGB.\n", millis() - now);

  Serial.printf("It took %i millis to run the loop.\n", millis() - startLoop);

  delay(500);

}

void fill(int r, int g, int b) {

  /* code */
  for(int i= 0 ; i < PIXEL_COUNT ; i++) {
    strip.setPixelColor(i,r,g,b);
  }

}
