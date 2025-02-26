/**
 * Switch
 *
 * test a switch between GROUND and D3/GPIO-0
 * turns onboard LED ON when button pressed
 */
#include "Arduino.h"

// Set LED_BUILTIN if it is not defined by Arduino framework
#define SWITCH_PIN D3 // GPIO 0

void setup() {
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  // LED
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial) {
    delay(100);
  }
  Serial.print("Set up!");
}

void loop() {
  int buttonState = digitalRead(SWITCH_PIN);
  Serial.println(buttonState);
  delay(100);
  digitalWrite(LED_BUILTIN, buttonState);
}
