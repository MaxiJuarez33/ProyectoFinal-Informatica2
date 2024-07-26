#include "pinControl.h"

analog::analog(uint8_t pin) : analogpin(pin) {}

double analog::recep()
{
    return analogRead(analogpin);
}

double analog::readCurrentACS712()
{
    const float sensitivity = 0.1; // Sensibilidad en V/A para ACS712-20A
    const float Vref = 2.5;        // Voltaje de referencia en el punto medio

    int sensorValue = analogRead(analogpin);
    float voltage = sensorValue * (3.3 / 1023.0); // Ajustado para entrada de ESP32
    float current = (voltage - Vref) / sensitivity;

    return current;
}

digital::digital(uint8_t pin) : digitalpin(pin)
{
    pinMode(digitalpin, OUTPUT);
}

void digital::emitir(uint8_t digitalval)
{
    digitalWrite(digitalpin, digitalval);
}
