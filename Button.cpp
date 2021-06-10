#include "Button.h"

Button::Button(int pin) {
  _pin = pin;
  
  pinMode(_pin, INPUT);
}

void Button::readSignal() {
  int buttonSignal = digitalRead(_pin);

  if (buttonSignal == 0) {
    onButtonPressed();
    delay(2000);
  }
}
