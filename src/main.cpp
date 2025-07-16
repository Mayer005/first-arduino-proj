
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include "counter.h"
#include "led.h"


void __attribute__((weak)) loop() {}

TaskHandle_t xCounterHandle = NULL;
TaskHandle_t xLedHandle = NULL;
QueueHandle_t xGreenEventQueue = NULL;

long stopTime;

LED greenLed(26);
LED redLed(40);

volatile bool isGreen = false;

void vCounterTask(void *pvParameters) {
  Counter counter;
  uint32_t lastUpdateTime = millis();
  bool lastGreenState = true;


  for (;;) {
    bool currentGreenState;
    uint32_t currentTime = millis();
    if (xQueueReceive(xGreenEventQueue, &currentGreenState, 0) == pdTRUE) {
      if (lastGreenState && !currentGreenState) {
        counter.reset(); 
      }
      lastGreenState = currentGreenState;
    }

    if(isGreen) {
      counter.draw();
      continue;
    }

    if(currentTime - lastUpdateTime >= 1) {
      lastUpdateTime = currentTime;
      counter.increment();
      counter.draw();
    }

    counter.draw();
  }
}

void vLedTask(void *pvParameters) {
  for (;;) {
    stopTime = random(1500, 8000);

    vTaskDelay(pdMS_TO_TICKS(stopTime));

    redLed.toggle();
    greenLed.toggle();

    bool currentGreenState = greenLed.isOn();
        
        
    if (xGreenEventQueue != NULL) xQueueSend(xGreenEventQueue, &currentGreenState, portMAX_DELAY);
    isGreen = currentGreenState; 
  }
}

void setup() {
  randomSeed(analogRead(0)); 

   for (uint8_t seg = 0; seg < 8; seg++) {
    pinMode(segmentPins[seg], OUTPUT);
  }
  

  for (uint8_t dig = 0; dig < 4; dig++) {
    pinMode(digitPins[dig], OUTPUT);
  }

  xGreenEventQueue = xQueueCreate(1, sizeof(bool));
  if (xGreenEventQueue == NULL) {
      Serial.println("Hiba: Nem sikerült létrehozni a várólistát!");
  }



  pinMode(greenLed.getPin(), OUTPUT);
  pinMode(redLed.getPin(), OUTPUT);

  greenLed.off();
  redLed.on();

  xTaskCreate(
      vCounterTask,
      "CounterTask",                
      1000,                     
      NULL,                     
      1,                        
      &xCounterHandle           
  );

  xTaskCreate(
    vLedTask,
      "LedTask",                
      1000,                     
      NULL,                     
      1,                        
      &xLedHandle               
  );
}

