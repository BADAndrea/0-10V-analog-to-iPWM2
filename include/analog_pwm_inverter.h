#ifndef ANALOG_PWM_INVERTER_H
#define ANALOG_PWM_INVERTER_H

#include <Arduino.h>

class AnalogPwmInverter {
  public:
    AnalogPwmInverter(int analogPin, int pwmPin);
    void begin();
    void update();
    
    // Configuration methods
    void setVoltageRange(float maxVoltage, float dividerRatio);
    void setPwmRange(int minPwm, int maxPwm);
    void setReadings(int numReadings, int readingDelay);
    void setUpdateInterval(unsigned long interval);
    void setDebug(bool enabled);
    
    // Add new method for frequency control
    void setPwmFrequency(unsigned long frequency);
    
    // Enum for available prescaler values
    enum PwmPrescaler {
      PRESCALER_1 = 1,
      PRESCALER_8 = 8,
      PRESCALER_64 = 64,
      PRESCALER_256 = 256,
      PRESCALER_1024 = 1024
    };
    
  private:
    const int _analogPin;
    const int _pwmPin;
    
    // Configuration
    float _maxVoltage = 10.0;
    float _voltageDividerRatio = 0.5;
    int _minPwm = 0;
    int _maxPwm = 255;
    int _numReadings = 5;
    int _readingDelay = 2;
    unsigned long _updateInterval = 100;
    bool _debugEnabled = true;
    
    // State
    unsigned long _lastUpdateTime = 0;
    int _previousPwmValue = -1;
    
    void configurePwmTimer();
    PwmPrescaler calculatePrescaler(unsigned long frequency);
    unsigned long _pwmFrequency = 490; // Default Arduino PWM frequency
};

#endif 
