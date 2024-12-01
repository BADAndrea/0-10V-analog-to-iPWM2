#include <Arduino.h>

// Debug flag
const bool DEBUG_ENABLED = true;  // Set to false to disable serial debugging

// Constants for pin configuration
const int ANALOG_INPUT_PIN = A0;  // Analog input pin
const int PWM_OUTPUT_PIN = 9;     // PWM output pin (use a PWM-capable pin)

// Configuration for PWM output range (0-255)
const int MIN_PWM = 0;    // Minimum PWM output level
const int MAX_PWM = 255;  // Maximum PWM output level

// Configuration for input voltage range
const float MAX_VOLTAGE = 10.0;  // Maximum input voltage (before voltage divider)
const float VOLTAGE_DIVIDER_RATIO = 0.5;  // Voltage divider ratio (e.g., 5V/10V = 0.5)

// Variables for non-blocking operation
unsigned long lastUpdateTime = 0;
const unsigned long UPDATE_INTERVAL = 1000;  // Update every second

void setup() {
  pinMode(ANALOG_INPUT_PIN, INPUT);
  pinMode(PWM_OUTPUT_PIN, OUTPUT);
  
  // Initialize serial only if debugging is enabled
  if (DEBUG_ENABLED) {
    Serial.begin(9600);
  }
}

void loop() {
  // Non-blocking update check
  if (millis() - lastUpdateTime >= UPDATE_INTERVAL) {
    lastUpdateTime = millis();
    
    // Read and process analog input
    int analogValue = analogRead(ANALOG_INPUT_PIN);
    float voltage = (analogValue * 5.0 / 1023.0) / VOLTAGE_DIVIDER_RATIO;
    
    // Calculate PWM value (inverse proportion)
    int pwmValue = map(voltage * 100, 
                      0, 
                      MAX_VOLTAGE * 100, 
                      MAX_PWM, 
                      MIN_PWM);
    
    pwmValue = constrain(pwmValue, MIN_PWM, MAX_PWM);
    analogWrite(PWM_OUTPUT_PIN, pwmValue);
    
    // Calculate PWM percentage
    float pwmPercentage = ((float)pwmValue / MAX_PWM) * 100;
    
    // Debug output only if enabled
    if (DEBUG_ENABLED) {
      Serial.print("Voltage: ");
      Serial.print(voltage);
      Serial.print("V, PWM: ");
      Serial.print(pwmValue);
      Serial.print(" (");
      Serial.print(pwmPercentage);
      Serial.println("%)");
    }
  }
  
  // Main loop is now free for other tasks
  // Add other non-blocking code here if needed
}