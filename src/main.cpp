#include "serialReader.h"
#include <iostream>

int pin = 12;

int main()
{
    const char *portName = "COM5";
    SerialReader serialReader(portName);

    if (!serialReader.openPort())
    {
        system("pause");
        return 1;
    }

    serialReader.configurePort();
    serialReader.startReading();

    serialReader.setDigitalHigh(&pin);
    Sleep(3000);
    serialReader.setDigitalLow(&pin);

    system("pause");

    return 0;
}