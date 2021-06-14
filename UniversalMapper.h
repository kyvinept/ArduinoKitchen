#include "Adafruit_NeoPixel.h"
#include <ArduinoJson.h>
#include <map>

#ifndef UNIVERSALMAPPER_H
#define UNIVERSALMAPPER_H

class UniversalMapper {
  private:
  
  public:
    struct IntArray {
      int* numbers;
      int size;
    };
    
    UniversalMapper();
    static std::map<char*, char*> getEmptyMap();
    
    int charWithStarToInt(char* ch);
    int hexCharToInt(char* ch);
    IntArray stringToIntArraySplittedByComa(String str);
    IntArray jsonArrayToIntArray(JsonArray jsonArray);
    char* constCharToChar(const char* ch);
    char* jsonArrayToChar(JsonArray jsonArray);
    char* intToChar(int value);
};

#endif /* UNIVERSALMAPPER_H */
