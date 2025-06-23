#include "analog_pwm_inverter.h"

AnalogPwmInverter::AnalogPwmInverter(int analogPin, int pwmPin)
    : _analogPin(analogPin), _pwmPin(pwmPin) {}

void AnalogPwmInverter::begin() {
  pinMode(_analogPin, INPUT);
  pinMode(_pwmPin, OUTPUT);
  
  configurePwmTimer();
  
  if (_debugEnabled) {
    Serial.begin(9600);
  }
}

void AnalogPwmInverter::update() {
  unsigned long now = millis();
  if (now - _lastUpdateTime >= _updateInterval) {
    _lastUpdateTime = now;
    
    // Take multiple readings and average them
    float voltageSum = 0;
    for (int i = 0; i < _numReadings; i++) {
      int analogValue = analogRead(_analogPin);
      float voltage = (analogValue * 5.0 / 1023.0) / _voltageDividerRatio;
      voltageSum += voltage;
      delay(_readingDelay);
    }
    float averageVoltage = voltageSum / _numReadings;
    
    // Calculate PWM value (inverse proportion)
    float ratio = averageVoltage / _maxVoltage;
    ratio = constrain(ratio, 0.0f, 1.0f);
    int pwmValue = (int)((1.0f - ratio) * (_maxPwm - _minPwm) + _minPwm + 0.5f);
    
    // Perform analogWrite only if the PWM value has changed
    if (pwmValue != _previousPwmValue) {
      analogWrite(_pwmPin, pwmValue);
      _previousPwmValue = pwmValue;
    }
    
    // Debug output
    if (_debugEnabled) {
      Serial.print("Average Voltage: ");
      Serial.print(averageVoltage);
      Serial.print("V, PWM: ");
      Serial.println(pwmValue);
    }
  }
}

// Configuration methods
void AnalogPwmInverter::setVoltageRange(float maxVoltage, float dividerRatio) {
  if (maxVoltage > 0) {
    _maxVoltage = maxVoltage;
  }
  if (dividerRatio > 0) {
    _voltageDividerRatio = dividerRatio;
  }
}

void AnalogPwmInverter::setPwmRange(int minPwm, int maxPwm) {
  _minPwm = constrain(minPwm, 0, 255);
  _maxPwm = constrain(maxPwm, 0, 255);
  if (_minPwm > _maxPwm) {
    int tmp = _minPwm;
    _minPwm = _maxPwm;
    _maxPwm = tmp;
  }
}

void AnalogPwmInverter::setReadings(int numReadings, int readingDelay) {
  if (numReadings > 0) {
    _numReadings = numReadings;
  }
  if (readingDelay >= 0) {
    _readingDelay = readingDelay;
  }
}

void AnalogPwmInverter::setUpdateInterval(unsigned long interval) {
  if (interval > 0) {
    _updateInterval = interval;
  }
}

void AnalogPwmInverter::setDebug(bool enabled) {
  _debugEnabled = enabled;
}

void AnalogPwmInverter::setPwmFrequency(unsigned long frequency) {
  if (frequency > 0 && frequency <= (F_CPU / 256)) {
    _pwmFrequency = frequency;
    configurePwmTimer();
  }
}

void AnalogPwmInverter::configurePwmTimer() {
  // Timer configuration for Pin 9 (Timer 1)
  // First, determine which timer this pin uses
  if (_pwmPin == 9 || _pwmPin == 10) {  // Timer 1
    TCCR1A = 0;  // Clear timer control registers
    TCCR1B = 0;
    
    // Set PWM mode to Fast PWM, 8-bit
    TCCR1A |= (1 << WGM10);
    TCCR1B |= (1 << WGM12);
    
    // Set non-inverting mode
    if (_pwmPin == 9) {
      TCCR1A |= (1 << COM1A1);
    } else {
      TCCR1A |= (1 << COM1B1);
    }
    
    // Calculate and set prescaler
    PwmPrescaler prescaler = calculatePrescaler(_pwmFrequency);
    switch (prescaler) {
      case PRESCALER_1:
        TCCR1B |= (1 << CS10);
        break;
      case PRESCALER_8:
        TCCR1B |= (1 << CS11);
        break;
      case PRESCALER_64:
        TCCR1B |= (1 << CS11) | (1 << CS10);
        break;
      case PRESCALER_256:
        TCCR1B |= (1 << CS12);
        break;
      case PRESCALER_1024:
        TCCR1B |= (1 << CS12) | (1 << CS10);
        break;
    }
    
    if (_debugEnabled) {
      Serial.print("PWM Frequency set to approximately: ");
      Serial.print(F_CPU / (prescaler * 256)); // 256 for 8-bit mode
      Serial.println(" Hz");
    }
  }
}

AnalogPwmInverter::PwmPrescaler AnalogPwmInverter::calculatePrescaler(unsigned long frequency) {
  // Calculate optimal prescaler based on desired frequency
  unsigned long cycles = F_CPU / (frequency * 256); // 256 for 8-bit mode
  
  if (cycles <= 1) return PRESCALER_1;
  if (cycles <= 8) return PRESCALER_8;
  if (cycles <= 64) return PRESCALER_64;
  if (cycles <= 256) return PRESCALER_256;
  return PRESCALER_1024;
}
