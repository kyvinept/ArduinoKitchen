#include "StorageManager.h"

#include <iostream>

StorageManager::StorageManager() {
  EEPROM.begin(512);

  loadValues();
}

std::map<char*, char*> StorageManager::getMap() {
  return _savedValues;
}

void StorageManager::save(char* key, char* value) {
  bool isExist = false;
  for (const auto& keyValue : _savedValues) {
    Serial.println(keyValue.first);
    Serial.println(key);
    Serial.println(strcmp(keyValue.first, key));
    if (strcmp(keyValue.first, key) == 0) {
      _savedValues[keyValue.first] = value;
      isExist = true;
      break;
    }
  }
  
  if (!isExist) {
    _savedValues.insert(std::pair<char*,char*>(key, value));
  }

  syncValues();
}

void StorageManager::syncValues() {
  std::string str = "";

  for (const auto& keyValue : _savedValues) {
    str += std::string(keyValue.first) + "=" + std::string(keyValue.second) + ";";
  }

  Serial.println("sync");
  Serial.println(str.c_str());

  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, i > str.length() ? ' ' : str[i]);
  }
  
  EEPROM.commit();
}

char* StorageManager::getValue(char* key) {
  for (const auto& keyValue : _savedValues) {
    if (strcmp(keyValue.first, key) == 0) {
      return keyValue.second;
    }
  }
  
  return "";
}

void StorageManager::loadValues() {
  std::string values = "";
  for (int i = 0; i < 512; i++) {
    values += EEPROM.read(i);
  }

  Serial.println(values.c_str());
  std::string foundKey = "";
  std::string tempValue = "";
  for (int i = 0; i < values.length(); i++) {
    if (values[i] == '=') {
      foundKey = tempValue;
      tempValue = "";
      continue;
    }

    if (values[i] == ';') {
      _savedValues.insert(std::pair<char*, char*>(strdup(foundKey.c_str()), strdup(tempValue.c_str())));
      tempValue = "";
      continue;
    }

    tempValue += values[i];
  }
}

//std::map<const char*, const char*> my_map = {
//        { "A", "1" },
//        { "B", "2" },
//        { "C", "3" }
//    };
//    



//const char* str = "login=kyvinept;password=12412Pas;";
//    const char* keys[] = {
//        "login",
//        "password"
//    };
//    
//    
//    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
//        int matchIndex = 0;
//        int strLength = strlen(str);
//        for (int j = 0; j < strLength; j++) {
//            if (str[j] == keys[i][matchIndex]) {
//                matchIndex++;
//            } else {
//                matchIndex = 0;
//            }
//            
//            if (matchIndex == strlen(keys[i]) && matchIndex != 0) {
//                char* loginValue = new char[50];
//                for (int k = j + 2, valueIndex = 0; k < strLength; k++, valueIndex++) {
//                    if (str[k] == ';') {
//                        cout << loginValue << endl;
//                        break;
//                    }
//                    
//                    loginValue[valueIndex] = str[k];
//                }
//            }
//        }
//    }
