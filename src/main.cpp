#include "serialReader.h"
#include <iostream>

char pin = 2;

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

    // Usar un thread separado para leer, así la comunicación no se bloquea
    std::thread reader(&SerialReader::startReading, &serialReader);
    reader.detach();

    serialReader.setDigitalHigh(&pin);
    Sleep(3000);
    serialReader.setDigitalLow(&pin);

    system("pause");

    return 0;
}
