#include "Adafruit_NeoPixel.h"

#ifndef BUTTON_H
#define BUTTON_H

class Button {
  private:
    int _pin;
  
  public:
    void (*onButtonPressed)(void);
  
    Button(int pin);
    void readSignal();
};

#endif /* BUTTON_H */
