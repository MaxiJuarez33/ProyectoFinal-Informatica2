#ifndef _PINCONTROLH_
#define _PINCONTROLH_

#include <Arduino.h>

class analog
{
private:
    uint8_t analogpin;

public:
    analog(uint8_t analogpin);
    double recep();
    double readCurrentACS712();
};

class digital
{
private:
    uint8_t digitalpin;

public:
    digital() : digitalpin(0) {}
    digital(uint8_t digitalpin);
    void emitir(uint8_t digitalval);
};

#endif
