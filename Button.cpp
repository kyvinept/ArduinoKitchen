#include "Button.h"

Button::Button(int pin) {
  _pin = pin;
  
  pinMode(_pin, INPUT);
}

void Button::readSignal() {
  int buttonSignal = digitalRead(_pin);

  if (buttonSignal == 0) {
    if (_currentSignal == buttonSignal && !isLongPressedCallbackCalled) {
      onLongButtonPressed();
      isLongPressedCallbackCalled = true;
    } else {
      onButtonPressed();
    }

    _currentSignal = buttonSignal;
    delay(2000);
  } else {
    _currentSignal = -1;
    isLongPressedCallbackCalled = false;
  }
}
