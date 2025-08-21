
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include "button.h"
#include "counter.h"
#include "led.h"

void __attribute__((weak)) loop() {}

TaskHandle_t xCounterHandle = NULL;
TaskHandle_t xLedHandle = NULL;
QueueHandle_t xGreenEventQueue = NULL;
SemaphoreHandle_t xButtonSemaphore = NULL;

long stopTime;

LED greenLed(26);
LED redLed(40);

Button button(19);

volatile bool isGreen = true;

/*!
    @brief Interrupt Service Routine for button press.
    @details This function is called when the button is pressed. It gives a
   semaphore to signal the button press event. It's faster then using a queue
   for button presses, as it avoids the overhead of queue management.
*/
void buttonISR() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(xButtonSemaphore, &xHigherPriorityTaskWoken);

  if (xHigherPriorityTaskWoken == pdTRUE) {
    portYIELD_FROM_ISR();
  }
}

/*!
    @brief Task to handle the counter logic.
    @details This task increments the counter every millisecond when the green
   LED is on. It listens for events from the green LED task to reset the counter
   when the green LED is turned on.
*/
void vCounterTask(void* pvParameters) {
  Counter counter;
  uint32_t lastUpdateTime = millis();
  bool lastGreenState = true;

  for (;;) {
    bool currentGreenState;
    uint32_t currentTime = millis();
    if (xQueueReceive(xGreenEventQueue, &currentGreenState, 0) == pdTRUE) {
      if (!lastGreenState && currentGreenState) {
        counter.reset();
      }
      lastGreenState = currentGreenState;
    }

    if (!isGreen) {
      counter.draw();
      continue;
    }

    if (currentTime - lastUpdateTime >= 1) {
      lastUpdateTime = currentTime;
      counter.increment();
      counter.draw();
    }

    counter.draw();
  }
}

/*!
    @brief Task to handle the LED logic.
    @details This task toggles the green and red LEDs, waits for a button press,
   and sends the state of the green LED to the queue. It also handles the timing
   for how long the green LED stays on before switching to red.
*/
void vLedTask(void* pvParameters) {
  for (;;) {
    greenLed.on();
    redLed.off();
    isGreen = true;
    bool greenStateToSend = isGreen;
    if (xGreenEventQueue != NULL)
      xQueueSend(xGreenEventQueue, &greenStateToSend, portMAX_DELAY);

    attachInterrupt(digitalPinToInterrupt(button.pin), buttonISR, FALLING);

    xSemaphoreTake(xButtonSemaphore, 0);

    xSemaphoreTake(xButtonSemaphore, portMAX_DELAY);

    detachInterrupt(digitalPinToInterrupt(button.pin));

    greenLed.off();
    redLed.on();
    isGreen = false;
    greenStateToSend = isGreen;
    if (xGreenEventQueue != NULL)
      xQueueSend(xGreenEventQueue, &greenStateToSend, portMAX_DELAY);

    stopTime = random(1500, 8000);
    vTaskDelay(pdMS_TO_TICKS(stopTime));
  }
}

/*!
    @brief Setup function for the Arduino.
    @details This function initializes the pins, creates the queue and
   semaphore, and starts the tasks. It sets up the segment and digit pins for
   the counter display.
*/
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
    Serial.println("Error: Could not create the queue!");
  }

  xButtonSemaphore = xSemaphoreCreateBinary();
  if (xButtonSemaphore != NULL) {
    xSemaphoreTake(xButtonSemaphore, 0);
  }

  pinMode(greenLed.getPin(), OUTPUT);
  pinMode(redLed.getPin(), OUTPUT);

  xTaskCreate(vCounterTask, "CounterTask", 1000, NULL, 1, &xCounterHandle);

  xTaskCreate(vLedTask, "LedTask", 1000, NULL, 1, &xLedHandle);
}
