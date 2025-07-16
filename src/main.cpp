
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include "counter.h"


void __attribute__((weak)) loop() {}

TaskHandle_t xCounterHandle = NULL;

void vCounterTask(void *pvParameters) {
  Counter counter;
  uint32_t lastUpdateTime = millis();


  for (;;) {
    uint32_t currentTime = millis();

    if(currentTime - lastUpdateTime >= 1) {
      lastUpdateTime = currentTime;
      counter.increment();
      counter.draw(counter.currentValue);
    }

    counter.draw();
  }
}

void setup() {
   for (uint8_t seg = 0; seg < 8; seg++) {
    pinMode(segmentPins[seg], OUTPUT);
  }
  

  for (uint8_t dig = 0; dig < 4; dig++) {
    pinMode(digitPins[dig], OUTPUT);
  }

  xTaskCreate(
      vCounterTask,
      "CounterTask",                
      1000,                     
      NULL,                     
      1,                        
      &xCounterHandle           
  );
}

