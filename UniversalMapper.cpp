#include "UniversalMapper.h"

UniversalMapper::UniversalMapper() {  
}

std::map<char*, char*> UniversalMapper::getEmptyMap() {
  std::map<char*, char*> map = {};
  return map;
}

int UniversalMapper::charWithStarToInt(char* ch) {
  return atoi(ch);
}

int UniversalMapper::hexCharToInt(char* ch) {
  return strtol(ch,NULL,0);
}

UniversalMapper::IntArray UniversalMapper::stringToIntArraySplittedByComa(String str) {
  int numbersSize = 1;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == ',') {
      numbersSize += 1;
    }
  }

  int index = 0;
  int* numbers = new int[numbersSize];
  char separator[] = ",";
  char *token;
  token = strtok(strdup(str.c_str()), separator);
  while(token != NULL) 
  {
    numbers[index] = atoi(token);
    token = strtok(NULL, separator);
  }

  UniversalMapper::IntArray intArray;
  intArray.numbers = numbers;
  intArray.size = numbersSize;
  return intArray;
}

UniversalMapper::IntArray UniversalMapper::jsonArrayToIntArray(JsonArray jsonArray) {
  int size = jsonArray.size();
  int* numbers = new int[size];
  for (int i = 0; i < size; ++i) {
    numbers[i] = jsonArray[i];
  }

  UniversalMapper::IntArray intArray;
  intArray.numbers = numbers;
  intArray.size = size;
  return intArray;
}

char* UniversalMapper::constCharToChar(const char* ch) {
  return strdup(ch);
}

char* UniversalMapper::jsonArrayToChar(JsonArray jsonArray) {
  int size = jsonArray.size();
  int* numbers = new int[size];
  String charNumbersToSave = "";
  for (int i = 0; i < size; ++i) {
    numbers[i] = jsonArray[i];
    charNumbersToSave += String(numbers[i]) + (i == size - 1 ? "" : ",");
  }

  return strdup(charNumbersToSave.c_str());
}

char* UniversalMapper::intToChar(int value) {
  return strdup(String(value).c_str());
}
