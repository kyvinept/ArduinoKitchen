#import <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <map>

struct EndpointModel {
  char* requestName;
  HTTPMethod requestType;
  std::map<char*, char*> (*callback)(StaticJsonDocument<200> json);
};
