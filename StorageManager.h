#include <EEPROM.h>
#include <map>
#include "Adafruit_NeoPixel.h"

#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

class StorageManager {
  private:
    std::map<char*, char*> _savedValues = {};
    void syncValues();
    void loadValues();
  
  public:
    StorageManager();
    void save(char* key, char* value);
    char* getValue(char* key);
};

#endif /* STORAGEMANAGER_H */
