#ifndef _TEMPH_
#define _TEMPH_

#include <arduino.h>

class temp
{
private:
    uint16_t pintemp, pintemp1;
public:
    temp(uint16_t pintemp);
    ~temp();

    void recep(uint16_t pintemp);
};

#endif