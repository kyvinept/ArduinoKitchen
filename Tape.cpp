#include "Tape.h"

Tape::Tape(int ledsCount, int pin, int brightness, int color) {
  _brightness = brightness;
  _color = color;
  _ledsCount = ledsCount;
  _pin = pin;
  _strip = Adafruit_NeoPixel(ledsCount, pin, NEO_GRB + NEO_KHZ800);
}

bool Tape::getIsShown() {
  return isShown;
}

void Tape::prepare() {
  _strip.begin();
  _strip.setBrightness(_brightness);
  _strip.clear();
  _strip.show();
}

void Tape::setupBrightness(int value) {
  _strip.setBrightness(value);
  _strip.show();
}

void Tape::setupColor(int value) {
  _color = value;
  _strip.show();
}

void Tape::show() {
  isShown = true;

  if (_showingModelType == Tape::ShowingModeType::part) {
    for (int j = 0; j < _ledsCount; j++) {
      bool isCovered = false;
      for (int i = 0; i < _tapeSkipModelsSize; i++) {
        if (j >= _tapeSkipModels[i].from && j <= _tapeSkipModels[i].to) {
          isCovered = true;
        }
      }

      if (isCovered) {
        _strip.setPixelColor(j, _color);
      } else {
        _strip.setPixelColor(j, BLACK_COLOR);
      }
    }
  } else {
    for (int i = 0; i < _ledsCount; i++) {
      _strip.setPixelColor(i, _color);
    } 
  }

  _strip.show();
}

void Tape::hide() {
  isShown = false;
  _strip.clear();
  _strip.show();
//  for (int i = 0; i < _ledsCount; i++) {
//    _strip.setPixelColor(i, BLACK_COLOR);
//  } 
//  _strip.show();
}

void Tape::settings(Tape::ShowingModeType showingMode, int* values, int size) {
  _showingModelType = showingMode;
  if (showingMode == Tape::ShowingModeType::full) {
    return;
  }
  
  TapeSkipModel* tapeSkipModels = new TapeSkipModel[size / 2];
  for (int i = 0, j = 0; i < size; i+=2, j++) {
    tapeSkipModels[j].from = values[i];
    tapeSkipModels[j].to = values[i+1];
  }
  
  _tapeSkipModels = tapeSkipModels;
  _tapeSkipModelsSize = size / 2;
}
