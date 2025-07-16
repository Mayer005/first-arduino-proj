#include <math.h>

#include "counter.h"



void Counter::increment() {
    currentValue++;
    if (currentValue > 9999) {
        currentValue = 0; // Reset to 0 if it exceeds 9999
    }
}

void Counter::decrement() {
    if (currentValue > 0) {
        currentValue--;
    } else {
        currentValue = 9999; // Wrap around to 9999 if it goes below 0
    }
}

void Counter::draw(uint16_t currentValue) {
    uint16_t temp = currentValue;

    
    for (int i = 3; i >= 0; i--) {
        uint8_t digit = temp % 10;
        drawDigit(i, digit);
        temp = temp / 10;
    }
}

void Counter::draw() {
    draw(currentValue);
}

void Counter::drawDigit(int position, uint8_t digit) {
    uint8_t segments = digitSegments[digit];
    clear();

    for (uint8_t seg = 0; seg < 7; seg++) {
        digitalWrite(segmentPins[seg], segments & (1 << seg));
    }

    digitalWrite(digitPins[position], LOW);

}

void Counter::clear() {
    for (const auto& pin : digitPins) {
        digitalWrite(pin, HIGH);
    }

    for (const auto& pin : segmentPins) {
        digitalWrite(pin, LOW);
    }
}