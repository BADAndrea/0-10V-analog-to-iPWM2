#include <Arduino.h>
#include "analog_pwm_inverter.h"

AnalogPwmInverter inverter(A0, 9);

void setup() {
  // Optional: Configure the inverter
  inverter.setVoltageRange(10.0, 0.5);
  inverter.setPwmRange(0, 255);
  inverter.setReadings(5, 2);
  inverter.setUpdateInterval(100);
  inverter.setDebug(true);
  
  // Set PWM frequency (in Hz)
  inverter.setPwmFrequency(25000);  // Example: 25kHz
  
  // Initialize the inverter
  inverter.begin();
}

void loop() {
  inverter.update();
}