/***
s
***/

#ifndef HV_CC_LED_DRIVER_WEBUI_H
#define HV_CC_LED_DRIVER_WEBUI_H

#include "Arduino.h"
#include "stdlib_noniso.h"
#include "ESP8266WiFi.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include "FS.h"

#ifndef DEBUG_UI_PORT
#define DEBUG_UI_PORT Serial
#endif

#ifdef DEBUG_WEB
#define LOG_WEB(...) DEBUG_UI_PORT.printf( __VA_ARGS__ )
#else
#define LOG_WEB(...)
#endif

class WEBUIClass {

public:
  void init(AsyncWebServer& server);

private:
  static void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
  void settingsJson(char *result, size_t len);
  void networksJson(char *result, size_t len);
  void statusJson(char *result, size_t len);
  void scheduleJson(char *result, size_t len);
  void lightJson(char *result, size_t len);

  String ipAddressToString(uint8_t *ip);
  void stringToIP(const char *ip_string, uint8_t *octets);
};

extern WEBUIClass WEBUI;

#endif //HV_CC_LED_DRIVER_WEBUI_H
