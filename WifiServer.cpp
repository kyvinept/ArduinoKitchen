#include "WifiServer.h"

ESP8266WebServer server(80);

WifiServer::WifiServer(char* wifiName, char* wifiPassword, int* ips) {
  _wifiName = wifiName;
  _wifiPassword = wifiPassword;
  _ips = ips;
}

void WifiServer::connect() {
  IPAddress ip(_ips[0], _ips[1], _ips[2], _ips[3]);  //статический IP
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);
  
  WiFi.begin(_wifiName, _wifiPassword);
  WiFi.config(ip, gateway, subnet);

  Serial.println("Waiting.");
  while ((WiFi.status() != WL_CONNECTED))
  {
    delay(100);
    Serial.print(".");
  } 

  Serial.println("connected");
}

void WifiServer::handleClient() {
  server.handleClient();
}

void WifiServer::startServer() {
  server.begin();
}

void WifiServer::setupEndpoint(EndpointModel* endpointModels, int length) {
  for (int i = 0; i < length; i++) {
    EndpointModel item = endpointModels[i];

    server.on(endpointModels[i].requestName, [item]() {
      if (server.hasArg("plain") != false) {
        StaticJsonDocument<200> json;
        DeserializationError error = deserializeJson(json, server.arg("plain"));
        if (error) {
          return server.send(422, "text/html", "{}");
        }

        item.callback(json);
      } else {
        StaticJsonDocument<200> json;
        char jsonStr[] = "{}";
        deserializeJson(json, jsonStr);
        item.callback(json);
      }
      
      server.send(200, "text/html", "{}"); 
    }); 
  }
}
