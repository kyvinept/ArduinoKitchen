#include "StorageManager.h"

#include <iostream>

StorageManager::StorageManager() {
  EEPROM.begin(512);

  loadValues();
}

void StorageManager::save(char* key, char* value) {
  std::map<char*, char*>::iterator it = _savedValues.find(key); 
  if (it != _savedValues.end()) {
    it->second = value;
  } else {
    Serial.println("insert");
    _savedValues.insert(std::pair<char*,char*>(key, value));
  }

  syncValues();
  loadValues();
}

void StorageManager::syncValues() {
  std::string str = "";

  for (const auto& keyValue : _savedValues) {
    str += std::string(keyValue.first) + "=" + std::string(keyValue.second) + ";";
  }

  Serial.println("sync");
  Serial.println(str.c_str());

  for (int i = 0; i < str.length(); i++) {
    EEPROM.write(i, str[i]);
  }
  
  EEPROM.commit();
}

char* StorageManager::getValue(char* key) {
  Serial.println("getValue");
  Serial.println(key);

  for (const auto& keyValue : _savedValues) {
    Serial.println(keyValue.first);
    Serial.println(keyValue.second);
    
    if (keyValue.first == key) {
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
