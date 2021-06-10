#include "Tape.h"
#include "Button.h"
#include "WifiServer.h"
#include "StorageManager.h"

#include <stdlib.h>

#define TAPE_PIN D2
#define BUTTON_PIN D4
#define NUM_LEDS 204

Tape *tape;
Button *button;
WifiServer *wifiServer;
StorageManager *storageManager;

void setup() {
  tape = new Tape(NUM_LEDS, TAPE_PIN);
  tape->prepare();

  Serial.begin(115000);

  connectToWifi();

  setupEndpoints();
  wifiServer->startServer();

  setupButton();

  setupStorageManager();
}

void loop() {
  wifiServer->handleClient();

  button->readSignal();
  delay(100);
}

void connectToWifi() {
  int* ips = new int[4];
  ips[0] = 192;
  ips[1] = 168;
  ips[2] = 0;
  ips[3] = 17;
  wifiServer = new WifiServer("Popka", "666345jk", ips);
  wifiServer->connect();
}

void setupButton() {
  button = new Button(BUTTON_PIN);
  button->onButtonPressed = []() {
    Serial.println("onButtonPressed");
    if (tape->getIsShown()) {
      tape->hide();
    } else {
      tape->show();
    }
  };
}

void setupStorageManager() {
  storageManager = new StorageManager();
  int brightness = atoi(storageManager->getValue("brightness"));
  if (brightness != 0) {
    tape->setupBrightness(brightness);
  }
}

void setupEndpoints() {
  const int endpointModelsLength = 6;
  EndpointModel endpointModels[endpointModelsLength];
  endpointModels[0].requestName = "/on";
  endpointModels[0].requestType = HTTP_GET;
  endpointModels[0].callback = [](StaticJsonDocument<200> json) {
    Serial.println("CALLBACK ON");
    tape->show();
  };
  endpointModels[1].requestName = "/off";
  endpointModels[1].requestType = HTTP_GET;
  endpointModels[1].callback = [](StaticJsonDocument<200> json) {    
    Serial.println("CALLBACK OFF");
    tape->hide();
  };
  endpointModels[2].requestName = "/settings";
  endpointModels[2].requestType = HTTP_POST;
  endpointModels[2].callback = [](StaticJsonDocument<200> json) {    
    Serial.print("CALLBACK SETTINGS");
    const char* mode = json["mode"];
    JsonArray values = json["values"];
    int size = values.size();
    int* numbers = new int[size];
    for (int i = 0; i < size; ++i) {
        numbers[i] = values[i];
    }
    tape->settings(std::string(mode) == "full" ? Tape::ShowingModeType::full : Tape::ShowingModeType::part, numbers, size);
  };
  endpointModels[3].requestName = "/color";
  endpointModels[3].requestType = HTTP_POST;
  endpointModels[3].callback = [](StaticJsonDocument<200> json) {    
    Serial.print("CALLBACK COLOR");
    int color = json["value"];
    tape->setupColor(color);
  };
  endpointModels[4].requestName = "/brightness";
  endpointModels[4].requestType = HTTP_POST;
  endpointModels[4].callback = [](StaticJsonDocument<200> json) {    
    Serial.print("CALLBACK BRIGHTNESS ");
    int brightness = json["value"];
    tape->setupBrightness(brightness);
    char* brightnessCharValue = strdup(String(brightness).c_str());
    storageManager->save("brightness", brightnessCharValue);
  };
  endpointModels[5].requestName = "/";
  endpointModels[5].requestType = HTTP_GET;
  endpointModels[5].callback = [](StaticJsonDocument<200> json) {    
    Serial.println("CALLBACK HOME LINK");
  };
  
  wifiServer->setupEndpoint(endpointModels, endpointModelsLength);
}
