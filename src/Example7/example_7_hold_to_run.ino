#include <Arduino.h>

const uint8_t BUTTON_PIN = 23;
const uint8_t MOTOR_IN1 = 21;
const uint8_t MOTOR_IN2 = 22;
const uint8_t DRIVER_SLEEP_PIN = 27; // Connected to STBY / nSLEEP pin

void setup() {
  pinMode(DRIVER_SLEEP_PIN, OUTPUT);
  digitalWrite(DRIVER_SLEEP_PIN, LOW); // Driver in sleep mode initially

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(DRIVER_SLEEP_PIN, HIGH); // Wake up the DRV8833 driver
  delay(2); // Driver wake-up interval
}

void loop() {
  const bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN1, buttonPressed ? HIGH : LOW); // HIGH = Forward Run, LOW = Coast
}
