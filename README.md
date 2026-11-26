# IoT-Smart-Street-Light-System
Automated solar-tracking and motion-dimming smart street light prototype built with Arduino Uno (Maker Uno), dual LDR sensors, PIR motion sensor, SG90 servo motor, and manual override.

# IoT Smart Street Light System

An automated, energy-saving street light prototype built with an **Arduino Uno (Maker Uno)** that tracks sunlight during the day and adjusts street light brightness based on motion at night.

## Overview
Standard street lights waste electricity by staying at full brightness all night, even on empty roads, while fixed solar panels often miss sunlight due to poor angles. This individual IoT project solves both issues using two automated modes:
* **Daytime Energy Maximization:** Uses two light sensors (LDRs) and a servo motor to tilt a solar panel toward the brightest light source.
* **Nighttime Power Conservation:** Keeps the street light dimmed at 50% when the road is empty, instantly boosts it to 100% when motion is detected, and includes a manual maintenance switch.

## Key Features
* **Dual-LDR Solar Tracking:** Continuously compares light levels from two LDR sensors and rotates an **SG90** servo motor toward the brighter side, using a built-in margin (`error = 20`) to stop the motor from shaking and angle limits (`0°`–`180°`) to protect the gears.
* **Headlight Protection (3-Second Filter):** Requires about 3 seconds of continuous bright light before switching from Night Mode to Day Mode, preventing passing car headlights from turning off the street light by mistake.
* **Motion-Activated Dimming:** Uses a **PIR** heat-motion sensor and smooth brightness control (PWM) to switch between **0%** (daytime), **50%** (night idle), and **100%** (motion detected).
* **Nighttime Home Reset:** Automatically returns the solar panel servo to its resting angle (`90°`) at night to prepare for the next morning.
* **Manual Maintenance Override:** Uses the Maker Uno's on-board push button with a 50 ms debounce filter to toggle the entire system ON or OFF—shutting off the LED and detaching the servo to save power when OFF.

## Hardware & Pin Mapping
| Component | Pin | Role |
| :--- | :--- | :--- |
| **Arduino Uno / Maker Uno** | — | Main microcontroller |
| **2 × LDR Sensors (with resistors)** | `A0`, `A1` | Day/night detection & sunlight direction tracking |
| **PIR Motion Sensor** | Digital `5` | Detects movement (people/vehicles) at night |
| **On-Board Push Button** | Digital `2` | Manual ON/OFF maintenance override (`INPUT_PULLUP`) |
| **SG90 Micro Servo Motor** | Digital `9` | Rotates the solar panel (`0°`–`180°`) |
| **LED Street Light (with resistor)** | Digital `3` (PWM) | Dimmable light output (0% / 50% / 100%) |

## Planned Future Upgrades
* **Edge AI Object Filtering:** Replace simple heat-only motion sensing with an **ESP32-CAM** running a lightweight **YOLO** model so the light only brightens to 100% for people and vehicles, ignoring stray animals.
* **Battery Storage:** Add a Lithium-Ion battery pack, charge controller (`TP4056`), and 5V boost converter so the street light runs fully off stored solar power.

## Demo Video & Prototype Photos
* **Video Demonstration:** [Watch Prototype Demo on Google Drive](https://drive.google.com/file/d/1q0o4LLZf01sJOBLILDTHwTlrll4aKvtK/view?usp=sharing)

## Author
* **Wong Yi Pin** — Artificial Intelligence Undergraduate, Xiamen University Malaysia (XMUM)
* *Individual Project for SWE403: Technology and Application of Internet of Things*
