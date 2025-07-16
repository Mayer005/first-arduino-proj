#ifndef COUNTER_H
#define COUNTER_H

#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>

constexpr uint8_t digitPins[4] = {9, 10, 11, 12};
constexpr uint8_t segmentPins[8] = {2, 3, 4, 5, 6, 7, 8};
constexpr uint32_t refreshInterval = 2000;

struct Counter {
    uint16_t currentValue;


    const uint8_t digitSegments[10] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111  // 9
    };

    Counter() : currentValue(0) {}

    void increment();
    void decrement();
    void draw(uint16_t currentValue);
    void draw();
    void drawDigit(int position, uint8_t digit);
    void clear();
    inline void reset() {
        currentValue = 0;
    }
};

#endif // COUNTER_H