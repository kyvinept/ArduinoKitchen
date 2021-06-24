#include "Adafruit_NeoPixel.h"

#ifndef BUTTON_H
#define BUTTON_H

class Button {
  private:
    int _pin;
    int _currentSignal = -1;
    bool isLongPressedCallbackCalled = false;
  
  public:
    void (*onButtonPressed)(void);
    void (*onLongButtonPressed)(void);
  
    Button(int pin);
    void readSignal();
};

#endif /* BUTTON_H */
