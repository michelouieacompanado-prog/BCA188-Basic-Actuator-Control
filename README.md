# ESP32 DRV8833 DC Motor Controller Project

This repository contains the Arduino code, circuit schematics, datasheet references, and submission documentation for driving a 3V DC motor using an **ESP32 microcontroller** and a **DRV8833 Dual H-Bridge Motor Driver Module**.

## Project Structure

```
.
├── lab_report.md             # Complete lab report and submission documentation
├── README.md                 # Project repository overview and setup guide
├── src/
│   ├── example_7_hold_to_run.ino  # Example 7: Button hold-to-run control
│   └── potentiometer_pwm.ino      # Optional Extension: Potentiometer PWM control
└── media/
    ├── picexample7.jpeg      # Photo of Example 7 circuit setup
    ├── videxample7.jpeg      # Demonstration video of button run/coast behavior
    ├── picpmw.jpeg           # Photo of Potentiometer PWM circuit setup
    ├── vidpmw1.jpeg          # Demonstration video of PWM motor speed control
    └── vidpmw2.jpeg          # Demonstration video showing motor spinning alongside Serial Monitor
```

## Required Hardware

1. **ESP32 Microcontroller Board** (with expansion shield)
2. **DRV8833 Dual H-Bridge Driver Board** (Red PCB labeled `DVR8833 54K CTKJ`)
3. **130-Size 3V DC Toy Motor**
4. **10kΩ Potentiometer** (for PWM extension)
5. **Pushbutton**
6. **Jumper Wires**

## Pin Connection Guide

| Component Pin | ESP32 Connection | Description | 
| :--- | :--- | :--- | 
| **`VM`** | **5V Rail** | High-current motor power supply rail | 
| **`GND`** | **GND** | Shared common ground reference | 
| **`STBY` / `nSLEEP`** | **GPIO 27** | Active-HIGH sleep wake-up line | 
| **`AIN1`** | **GPIO 21** | PWM / Direction signal input | 
| **`AIN2`** | **GPIO 22** | Direction control (tied LOW for forward) | 
| **`AO1` / `AO2`** | **Motor Terminals** | H-Bridge Output A1 and A2 | 
| **`Pushbutton`** | **GPIO 23** | Digital input switch (Internal pull-up) | 
| **`Potentiometer Wiper`** | **GPIO 34** | Analog ADC input | 

## Setup & Execution

1. Connect the ESP32 to your computer via USB.
2. Open the desired sketch in **Arduino IDE**.
3. Select board **ESP32 Dev Module** and set baud rate to **`115200`**.
4. Upload the sketch and monitor output in the Serial Monitor.
