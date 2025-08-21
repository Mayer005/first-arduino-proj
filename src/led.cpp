#include "led.h"

LED::LED(uint8_t pin) : _pin(pin) {
  pinMode(_pin, OUTPUT);
  off();
}

void LED::on() {
  digitalWrite(_pin, HIGH);
}

void LED::off() {
  digitalWrite(_pin, LOW);
}

void LED::toggle() {
  digitalWrite(_pin, !digitalRead(_pin));
}
