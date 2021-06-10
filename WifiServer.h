#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "EndpointModel.h"

#ifndef WIFISERVER_H
#define WIFISERVER_H

class WifiServer {
  private:
    char* _wifiName;
    char* _wifiPassword;
    int* _ips;
  
  public:
    WifiServer(char* wifiName, char* wifiPassword, int* ips);
    void connect();
    void handleClient();
    void setupEndpoint(EndpointModel* endpointModels, int length);
    void startServer();
};

#endif /* WIFISERVER_H */
