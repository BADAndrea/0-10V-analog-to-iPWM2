# 0-10V analog to iPWM2

Questo progetto permette di convertire un segnale analogico 0‑10 V in un segnale PWM inverso tramite Arduino Nano. Viene fornita una semplice libreria `AnalogPwmInverter` per configurare il range di tensione in ingresso, il range PWM di uscita e la frequenza del timer.

## Configurazione PlatformIO

Il file `platformio.ini` definisce un ambiente per la scheda `nanoatmega328new` con framework Arduino. Per compilare il firmware è sufficiente eseguire:

```bash
pio run
```

## Utilizzo di base

Di seguito l'esempio presente in `src/main.cpp` che mostra la configurazione minima della libreria:

```cpp
#include <Arduino.h>
#include "analog_pwm_inverter.h"

AnalogPwmInverter inverter(A0, 9);

void setup() {
  // Configurazione opzionale dell'inverter
  inverter.setVoltageRange(10.0, 0.5);
  inverter.setPwmRange(0, 255);
  inverter.setReadings(5, 2);
  inverter.setUpdateInterval(100);
  inverter.setDebug(true);

  // Frequenza PWM in Hz
  inverter.setPwmFrequency(25000);  // 25 kHz

  // Inizializza l'inverter
  inverter.begin();
}

void loop() {
  inverter.update();
}
```

Il metodo `update()` va richiamato nel loop principale affinché la libreria legga il valore analogico e aggiorni di conseguenza il duty cycle PWM.
