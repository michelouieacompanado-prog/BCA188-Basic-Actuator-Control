# Hardware Lab Report: DC Motor Driver Circuit & PWM Onset Analysis

## 1. Task Summary
The objective of this lab is to construct a motor driver circuit using an ESP32 microcontroller, a DRV8833 dual H-bridge driver board, and a 3V DC toy motor. The project demonstrates both digital button-controlled hold-to-run behavior and potentiometer-driven PWM speed control to experimentally determine the minimum duty cycle required for the onset of rotation.

---

## 2. Component Specifications & Datasheet References

### 3V DC Motor (130-Size Toy Motor)
* **Rated Voltage:** $3.0\text{ V}$ to $5.0\text{ V DC}$
* **Stall Current:** $\approx 0.8\text{ A}$ to $1.5\text{ A}$ (at $3\text{ V} - 5\text{ V}$ load)
* **Datasheet:** [Adafruit 130-Size 3V DC Motor Reference Datasheet](https://www.adafruit.com/product/711)

### Dual H-Bridge Motor Driver Module
* **Silicon IC Part Marking:** `DVR8833 54K CTKJ` (Texas Instruments DRV8833 Dual H-Bridge Motor Driver IC)
* **Breakout Layout:** Red Dual H-Bridge Module (TB6612-compatible PCB footprint and pin labels)
* **Operating Voltage Range ($\text{V}_{\text{M}}$):** $2.7\text{ V}$ to $10.8\text{ V}$
* **Logic Voltage ($\text{V}_{\text{CC}}$):** $2.7\text{ V}$ to $5.5\text{ V}$
* **Output Current:** $1.2\text{ A RMS}$ continuous per channel ($2.0\text{ A}$ peak per channel)
* **Datasheet:** [Texas Instruments DRV8833 Datasheet](https://www.ti.com/lit/ds/symlink/drv8833.pdf)

---

## 3. Circuit Wiring & Hardware Connections

### Wiring Summary Table
| DRV8833 Module Pin | Target Connection | Functional Description |
| :--- | :--- | :--- |
| **`VM`** | **ESP32 Expansion Board 5V Output** | Dedicated high-current motor power rail |
| **`GND`** | **ESP32 GND Pin** | Common zero-voltage reference for signals and power |
| **`STBY` / `nSLEEP`** | **ESP32 GPIO 27** | Active-HIGH driver wake-up / sleep control line |
| **`AIN1`** | **ESP32 GPIO 21** | Logic input 1 for Channel A H-Bridge (PWM line) |
| **`AIN2`** | **ESP32 GPIO 22** | Logic input 2 for Channel A H-Bridge (Grounded for forward drive) |
| **`AO1`** | **Motor Terminal 1** | Channel A output lead 1 |
| **`AO2`** | **Motor Terminal 2** | Channel A output lead 2 |
| **`Pushbutton`** | **ESP32 GPIO 23 to GND** | Digital control switch using ESP32 internal pull-up |
| **`Potentiometer Pin 1`**| **ESP32 3.3V** | Analog reference high |
| **`Potentiometer Pin 2`**| **ESP32 GND** | Analog reference ground |
| **`Potentiometer Wiper`** | **ESP32 GPIO 34** | Analog voltage input line for PWM scaling |

---

## 4. Explanation of Pin Functions & Logic States

* **`AIN1` & `AIN2`:** Directional logic control inputs for H-Bridge Output Channel A.
  * `AIN1 = HIGH, AIN2 = LOW`: Forward motion drive.
  * `AIN1 = LOW, AIN2 = HIGH`: Reverse motion drive.
  * `AIN1 = LOW, AIN2 = LOW`: **Coast Mode** (the output transistors enter a high-impedance state, letting the motor glide naturally to a stop without electrical braking).
* **`STBY` / `nSLEEP`:** Active-HIGH sleep enable line. Driving this pin **HIGH** powers internal charge pumps and logic circuitry; driving it **LOW** forces low-power sleep mode.
* **`VM`:** Motor power supply rail. Drives high current directly to the H-bridge power MOSFETs. Powering `VM` from the ESP32 shield's stable 5V rail prevents brownouts caused by starting current spikes.
* **`Common Ground (GND)`:** Shares a common 0V reference across the microcontroller, motor driver, and external power sources, preventing floating digital logic signals.

---

## 5. Source Code

### Part A: Example 7 — Hold Button to Run Motor
```cpp
#include <Arduino.h>

const uint8_t BUTTON_PIN = 23;
const uint8_t MOTOR_IN1 = 21;
const uint8_t MOTOR_IN2 = 22;
const uint8_t DRIVER_SLEEP_PIN = 27; // Connected to STBY / nSLEEP pin

void setup() {
  pinMode(DRIVER_SLEEP_PIN, OUTPUT);
  digitalWrite(DRIVER_SLEEP_PIN, LOW); // Hold driver in sleep mode initially

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(DRIVER_SLEEP_PIN, HIGH); // Wake up the driver module
  delay(2); // One-time driver wake-up interval
}

void loop() {
  const bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN1, buttonPressed ? HIGH : LOW); // HIGH runs motor, LOW coasts motor
}
```

### Part B: Optional Extension — Potentiometer PWM & Onset of Rotation
```cpp
#include <Arduino.h>

const uint8_t POT_PIN = 34;           // Analog input pin connected to potentiometer wiper
const uint8_t MOTOR_IN1 = 21;         // PWM control pin
const uint8_t MOTOR_IN2 = 22;         // Direction control pin
const uint8_t DRIVER_SLEEP_PIN = 27;  // Connected to STBY / nSLEEP pin

void setup() {
  Serial.begin(115200);

  pinMode(DRIVER_SLEEP_PIN, OUTPUT);
  digitalWrite(DRIVER_SLEEP_PIN, HIGH); // Driver active

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  digitalWrite(MOTOR_IN2, LOW); // Fixed forward direction
}

void loop() {
  int potValue = analogRead(POT_PIN); // ADC reading (0–4095 on ESP32)
  
  // Map ADC reading to 8-bit PWM duty cycle range (0–255)
  uint8_t pwmValue = map(potValue, 0, 4095, 0, 255);
  
  // Output PWM to AIN1 while keeping AIN2 LOW
  analogWrite(MOTOR_IN1, pwmValue);

  // Compute duty cycle percentage
  float dutyCyclePct = (pwmValue / 255.0) * 100.0;

  Serial.print("Raw ADC: ");
  Serial.print(potValue);
  Serial.print(" | PWM (0-255): ");
  Serial.print(pwmValue);
  Serial.print(" | Duty Cycle: ");
  Serial.print(dutyCyclePct, 1);
  Serial.println("%");

  delay(100);
}
```

---

## 6. Circuit Documentation & Verification Media

This section details the photographic and video evidence verifying circuit functionality, wiring integrity, and experimental PWM metrics.

### Part A: Example 7 Documentation (Button Hold-to-Run)

#### 1. Circuit Setup Photograph
* **File:** `picexample7.jpeg`
* **Description:** Photographic evidence of the complete physical circuit setup for Example 7. Shows the ESP32 expansion board, DRV8833 driver module, 3V DC motor connections, and pushbutton wired to GPIO 23.

#### 2. Functional Demonstration Video
* **File:** `videxample7.mp4`
* **Description:** Video demonstration verifying button-controlled run and coast operation. Pressing the button causes the motor to spin; releasing it allows the motor shaft to coast naturally to a stop.

---

### Part B: Optional Extension Documentation (Potentiometer PWM Control)

#### 1. PWM Circuit Setup Photograph
* **File:** `picpmw.jpeg`
* **Description:** Photograph showing the extension circuit layout with a 10kΩ potentiometer connected to 3.3V, GND, and GPIO 34 (analog wiper input).

#### 2. PWM Speed Control Video
* **File:** `vidpmw1.mp4`
* **Description:** Video recording demonstrating variable motor speed control as the potentiometer knob is swept from 0% to 100% duty cycle.

#### 3. Rotational Onset & Serial Monitor Verification Video
* **File:** `vidpmw2.mp4`
* **Description:** Dual-view video capturing the motor shaft beginning rotation while simultaneously recording the live Serial Monitor feed. Demonstrates the precise point of static friction breakdown at **27.5% duty cycle** (`PWM = 70`).

---

## 7. Experimental Results & Observed Behavior

1. **Button Run / Coast Verification:**
   * **Button Pressed:** GPIO 23 triggers `LOW`. `AIN1` transitions to `HIGH` while `AIN2` stays `LOW`, energizing the H-bridge and driving the motor forward.
   * **Button Released:** GPIO 23 turns `HIGH`. `AIN1` transitions to `LOW`. With both input lines `LOW`, the motor enters **Coast Mode** and naturally glides to a stop.
2. **Measured Onset of Rotation (Experimental Output):**
   * **Supply Voltage ($\text{V}_{\text{M}}$):** $5.0\text{ V DC}$ (from ESP32 Expansion Shield)
   * **PWM Command Value at Onset:** `70` (out of 255)
   * **Measured Onset Duty Cycle:** **`27.5%`**
   * **Observation:** At duty cycles below 27.5%, current through the H-bridge produces an audible humming noise, but the magnetic torque is insufficient to overcome the motor's internal static friction and brush cogging. At exactly 27.5% duty cycle, the motor overcomes static friction and begins to rotate very slowly. The onset threshold is a measured result dependent on physical friction and mechanical load, rather than a fixed value.
