#include <Servo.h>
// --- Hardware Pin Definitions ---
Servo sg90;
int LDR1 = A0;
int LDR2 = A1;
int servopin = 9;
int LED = 3;
int PIR = 5;
int BUTTON = 2;
// --- System Thresholds & Parameters ---
// Adjust this threshold based on your LDR readings in darkness
int darkThreshold = 400;
// --- System State Vairables ---
bool systemOn = false; // System starts OFF
int initial_position = 90; // Servo starts facing the middle (90 degrees)
int error = 20; // Deadband margin for solar tracking to prevent jitter
// Button state variable to detect when the button is pressed (edge detection)
int lastButtonState = HIGH;
void setup() {
// Attach the servo and set it to the default starting angle
sg90.attach(servopin);
sg90.write(initial_position);
// Configure Input/Output pins
pinMode(LDR1, INPUT);
pinMode(LDR2, INPUT);
pinMode(PIR, INPUT);
pinMode(LED, OUTPUT);
// Use internal pull-up resistor for the button (reads HIGH when unpressed)
pinMode(BUTTON, INPUT_PULLUP);
// Initialize serial communication for debugging purposes
Serial.begin(9600);
Serial.println("System ready. Press button to toggle ON/OFF.");
// Brief pause to let sensors (like the PIR) stabilize on startup
delay(2000);
}
void loop() {
// Read the current physical state of the push button
int buttonState = digitalRead(BUTTON);
// ---------------------------------------------------------
// 1. Manual Override: Hardware Debouncing & Edge Detection
// ---------------------------------------------------------
// Check if button changed from HIGH (unpressed) to LOW (pressed)
if (buttonState == LOW && lastButtonState == HIGH) {
delay(50); //50ms delay to ignore physical button "bounces"
systemOn = !systemOn; // Toggle system state
if (!systemOn) {
// If system turned OFF: Turn off LED and detach servo to save power
analogWrite(LED, 0);
sg90.detach();
} else {
// If system turned ON: Reattach servo and restore to its last known angl
sg90.attach(servopin);
sg90.write(initial_position);
}
}
// Update lastButtonState for the next loop iteration
lastButtonState = buttonState;
// ---------------------------------------------------------
// 2. Main System Logic (Only runs if System is ON)
// ---------------------------------------------------------
if (systemOn) {
// Read current environmental data from sensors
int R1 = analogRead(LDR1);
int R2 = analogRead(LDR2);
int pirState = digitalRead(PIR);
// Print live sensor data to the Serial Monitor for debugging
Serial.print("System: ON");
Serial.print(" Button: "); Serial.print(buttonState);
Serial.print(" LDR1: "); Serial.print(R1);
Serial.print(" LDR2: "); Serial.print(R2);
Serial.print(" PIR: "); Serial.println(pirState);
// Determine if the environment is dark
bool isDark = (R1 > darkThreshold && R2 > darkThreshold);
// ---------------------------------------------------------
// 3. LDR Debouncing (Headlight Protection)
// ---------------------------------------------------------
// Prevents passing car headlights from tricking the system into Daytime Mode
static int lightCounter = 0;
if (!isDark) {
lightCounter++; // Increment counter if light is detected
} else {
lightCounter = 0; // Reset to zero immediately if darkness returns
}
// Require 30 loops (about 3 seconds) of bright light to switch to Daytime Mode
bool sustainedDaylight = (lightCounter > 30);
// ---------------------------------------------------------
// 4. Daytime Mode (Solar Tracking)
// ---------------------------------------------------------
if (sustainedDaylight) {
analogWrite(LED, 0); // Ensure Street Light is OFF during the day
// Calculate difference between the two LDRs
int diff = abs(R1 - R2);
// If difference is larger than the error margin, move the servo
if (diff > error) {
if (R1 < R2) {
initial_position++; // Move towards LDR2
} else {
initial_position--; // Move towards LDR1
}
// Constrain servo movement between 0 and 180 degrees to prevent mechanical damage
initial_position = constrain(initial_position, 0, 180);
sg90.write(initial_position);
}
// ---------------------------------------------------------
// 5. NIGHTTIME MODE (Motion-Activated Street Light)
// ---------------------------------------------------------
} else {
// Resets the solar panel to face East (90 degrees) to prepare for sunrise
initial_position = 90;
sg90.write(initial_position);
// Power-Optimized LED Control based on PIR motion
if (pirState == HIGH) {
analogWrite(LED, 255); // Motion detected -> 100% brightness
} else {
analogWrite(LED, 127); // No Motion detected -> 50% brightness
}
}
}
// Small delay to stabilize the main loop
delay(100);
}