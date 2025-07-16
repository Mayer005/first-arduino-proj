#include <Arduino.h>
#include <Arduino_FreeRTOS.h>


TaskHandle_t xLedTaskHandle = NULL;

void vLedTask(void *pvParameters) {
    const TickType_t xDelay5000ms = pdMS_TO_TICKS(5000);
    
    for(;;) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle LED
        vTaskDelay(xDelay5000ms); // Várakozás 5000ms
    }
}

void setup() {
    // Serial inicializálása (opcionális)
    Serial.begin(115200);
    
    // LED pin kimenetre állítása
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    
    // LED task létrehozása
    xTaskCreate(
        vLedTask,               // Task függvény
        "LED Blink",            // Task név
        configMINIMAL_STACK_SIZE + 50, // Stack méret
        NULL,                   // Paraméterek
        1,                      // Prioritás
        &xLedTaskHandle         // Task handle
    );
    
    // A scheduler automatikusan indul a setup() lefutása után.
    // A vTaskStartScheduler() hívása itt szükségtelen.
}

void loop() {
  // Ez a függvény üresen marad, mivel az ütemező futtatja a taszkokat.
  // Alacsony prioritású idle task-ként funkcionálhat.
}