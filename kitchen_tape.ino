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

  setupStorageManager();
  
  setupEndpoints();
  wifiServer->startServer();

  setupButton();
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
  
  char* stringColor = storageManager->getValue("color");
  if (stringColor != "") {
    int color = strtol(stringColor,NULL,0);
    tape->setupColor(color);
  }
  
  String mode = storageManager->getValue("mode");
  String partShowingTape = storageManager->getValue("partShowingTape");
  if (mode != "") {
    Tape::ShowingModeType showingModeType = mode == "full" ? Tape::ShowingModeType::full : Tape::ShowingModeType::part;

    switch(showingModeType) {
      case Tape::ShowingModeType::full:
        tape->settings(showingModeType);
        break;
      case Tape::ShowingModeType::part:
        if (partShowingTape.length()) {
          int numbersSize = 1;
          for (int i = 0; i < partShowingTape.length(); i++) {
            if (partShowingTape[i] == ',') {
              numbersSize += 1;
            }
          }
    
          int index = 0;
          int* numbers = new int[numbersSize];
          char separator[] = ",";
          char *token;
          token = strtok(strdup(partShowingTape.c_str()), separator);
          while(token != NULL) 
          {
            numbers[index] = atoi(token);
            token = strtok(NULL, separator);
          }
          tape->settings(showingModeType, numbers, numbersSize);
        }
        break;
    }
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

    std::map<char*, char*> map = {};
    return map;
  };
  endpointModels[1].requestName = "/off";
  endpointModels[1].requestType = HTTP_GET;
  endpointModels[1].callback = [](StaticJsonDocument<200> json) {    
    Serial.println("CALLBACK OFF");
    tape->hide();

    std::map<char*, char*> map = {};
    return map;
  };
  endpointModels[2].requestName = "/settings";
  endpointModels[2].requestType = HTTP_POST;
  endpointModels[2].callback = [](StaticJsonDocument<200> json) {    
    Serial.print("CALLBACK SETTINGS");
    const char* mode = json["mode"];
    JsonArray values = json["values"];
    int size = values.size();
    int* numbers = new int[size];
    String charNumbersToSave = "";
    for (int i = 0; i < size; ++i) {
        numbers[i] = values[i];
        charNumbersToSave += String(numbers[i]) + (i == size - 1 ? "" : ",");
    }
    tape->settings(std::string(mode) == "full" ? Tape::ShowingModeType::full : Tape::ShowingModeType::part, numbers, size);
    storageManager->save("mode", strdup(mode));
    Serial.println(charNumbersToSave.c_str());
    storageManager->save("partShowingTape", strdup(charNumbersToSave.c_str()));

    std::map<char*, char*> map = {};
    return map;
  };
  endpointModels[3].requestName = "/color";
  endpointModels[3].requestType = HTTP_POST;
  endpointModels[3].callback = [](StaticJsonDocument<200> json) {    
    Serial.print("CALLBACK COLOR");
    String stringValue = json["value"];
    int color = strtol(stringValue.c_str(),NULL,0);
    tape->setupColor(color);
    char* colorCharValue = strdup(stringValue.c_str());
    storageManager->save("color", colorCharValue);

    std::map<char*, char*> map = {};
    return map;
  };
  endpointModels[4].requestName = "/brightness";
  endpointModels[4].requestType = HTTP_POST;
  endpointModels[4].callback = [](StaticJsonDocument<200> json) {    
    Serial.print("CALLBACK BRIGHTNESS ");
    int brightness = json["value"];
    tape->setupBrightness(brightness);
    char* brightnessCharValue = strdup(String(brightness).c_str());
    storageManager->save("brightness", brightnessCharValue);

    std::map<char*, char*> map = {};
    return map;
  };
  endpointModels[5].requestName = "/";
  endpointModels[5].requestType = HTTP_GET;
  endpointModels[5].callback = [](StaticJsonDocument<200> json) {    
    Serial.println("CALLBACK HOME LINK");
    return storageManager->getMap();
  };
  
  wifiServer->setupEndpoint(endpointModels, endpointModelsLength);
}
