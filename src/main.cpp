#include <Arduino.h>

#define LED_PIN 13

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH); // turn the LED on
  delay(1000);                 // wait for a second
  digitalWrite(LED_PIN, LOW);  // turn the LED off
  delay(1000);                 // wait for a second
}

// put function definitions here:
int myFunction(int x, int y) {
}