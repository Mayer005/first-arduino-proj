#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <stdint.h>

//! Button class
/*!
    @brief Represents a button with a specific pin.
    @details The button is initialized with a pin number and uses the internal
   pull-up resistor. The button state can be checked using the buttonIsHigh()
    @param pin The pin number to which the button is connected.
    @note The button pin should be connected to ground when pressed.
    @note The button pin should be set to INPUT_PULLUP mode.
*/
struct Button {
  const uint8_t pin;
  inline Button(uint8_t pin) : pin(pin) { pinMode(pin, INPUT_PULLUP); }
  inline bool buttonIsHigh() { return digitalRead(pin) == HIGH; }
  inline void setPinMode(uint8_t mode) { pinMode(pin, mode); }
};

#endif  // BUTTON_H