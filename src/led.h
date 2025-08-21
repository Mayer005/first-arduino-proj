#ifndef LED_H
#define LED_H

#include <Arduino.h>

//! LED class
/*!
    @brief Represents an LED with a specific pin.
    @details The LED can be turned on, off, or toggled.
    The LED state can be checked using the isOn() method.
    @param pin The pin number to which the LED is connected.
*/
class LED {
 public:
  LED(uint8_t pin);
  void on();
  void off();
  void toggle();
  inline bool isOn() const { return digitalRead(_pin) == HIGH; }
  inline uint8_t getPin() const { return _pin; }

 private:
  uint8_t _pin;
};

#endif  // LED_H
