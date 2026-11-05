#include <Arduino.h>

const uint8_t POT_PIN = 34;           // Analog input pin for potentiometer wiper
const uint8_t MOTOR_IN1 = 21;         // PWM output pin
const uint8_t MOTOR_IN2 = 22;         // Direction control pin
const uint8_t DRIVER_SLEEP_PIN = 27;  // Connected to STBY / nSLEEP pin

void setup() {
  Serial.begin(115200);

  pinMode(DRIVER_SLEEP_PIN, OUTPUT);
  digitalWrite(DRIVER_SLEEP_PIN, HIGH); // Wake up the driver

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  digitalWrite(MOTOR_IN2, LOW); // Fixed forward rotation
}

void loop() {
  int potValue = analogRead(POT_PIN); // Reads 0 - 4095 on ESP32
  
  // Map 12-bit ADC reading down to 8-bit PWM (0 - 255)
  uint8_t pwmValue = map(potValue, 0, 4095, 0, 255);
  
  // Output PWM signal to MOTOR_IN1
  analogWrite(MOTOR_IN1, pwmValue);

  // Compute duty cycle percentage
  float dutyCyclePct = (pwmValue / 255.0) * 100.0;

  Serial.print("Raw ADC: ");
  Serial.print(potValue);
  Serial.print(" | PWM Value (0-255): ");
  Serial.print(pwmValue);
  Serial.print(" | Duty Cycle: ");
  Serial.print(dutyCyclePct, 1);
  Serial.println("%");

  delay(100);
}
