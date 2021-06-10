#include "Adafruit_NeoPixel.h"

#ifndef TAPE_H
#define TAPE_H

const int DEFAULT_BRIGHTNESS = 20;
const int DEFAULT_COLOR = 0x0000ff;
const int BLACK_COLOR = 0x000000;

class Tape {
  struct TapeSkipModel {
    int from;
    int to;
  };
  
  public:
    enum ShowingModeType { full, part };
  
    Tape(int ledsCount, int pin, int brightness = DEFAULT_BRIGHTNESS, int color = DEFAULT_COLOR);
    void setupBrightness(int value);
    void setupColor(int value);
    void show();
    void hide();
    void prepare();
    bool getIsShown();
    void settings(ShowingModeType showingMode, int* values, int size);

   private:
    Adafruit_NeoPixel _strip;
    int _ledsCount;
    int _pin;
    int _brightness;
    int _color;
    TapeSkipModel* _tapeSkipModels;
    int _tapeSkipModelsSize = 0;
    bool isShown = false;
    ShowingModeType _showingModelType = ShowingModeType::full;
};

#endif /* TAPE_H */
