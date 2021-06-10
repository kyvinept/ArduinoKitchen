#import <ESP8266WebServer.h>
#include <ArduinoJson.h>

struct EndpointModel {
  char* requestName;
  HTTPMethod requestType;
  void (*callback)(StaticJsonDocument<200> json);
};
