#ifndef LED_H
#define LED_H

#include <Arduino.h>



class LED {
public:
  LED(uint8_t pin);
  void on();
  void off();
  void toggle();
  inline bool isOn() const {
    return digitalRead(_pin) == HIGH;
  }
  inline uint8_t getPin() const {
    return _pin;
  }

private:
  uint8_t _pin;
};

#endif // LED_H
