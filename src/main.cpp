
#include <Arduino_FreeRTOS.h>

#include "counter.h"


void __attribute__((weak)) loop() {}

TaskHandle_t xCounterHandle = NULL;

void vCounterTask(void *pvParameters) {
  Counter counter;

  for (;;) {
    for (uint8_t i = 0; i < 10; i++) {
      counter.currentValue = i * 1111;
      
      for(uint32_t start = millis(); millis() - start < 1000; ) {
        counter.draw();
      }
    }
  }
}

void setup() {
   for (uint8_t seg = 0; seg < 8; seg++) {
    pinMode(segmentPins[seg], OUTPUT);
  }
  
  // Digit pinek beállítása
  for (uint8_t dig = 0; dig < 4; dig++) {
    pinMode(digitPins[dig], OUTPUT);
  }

  xTaskCreate(
      vCounterTask,
      "CounterTask",                // Task name
      1000,                     // Stack size
      NULL,                     // Parameters
      1,                        // Priority
      &xCounterHandle           // Task handle
  );
}

