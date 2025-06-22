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

    std::thread reader(&SerialReader::startReading, &serialReader); // Hilo para leer y no bloquear el uart, permitiendo enviar el comando para cambiar el estado del relay
    reader.detach();

    while (1)
    {
        serialReader.setDigitalHigh(&pin);
        Sleep(3000);
        serialReader.setDigitalLow(&pin);
    }

    system("pause");

    return 0;
}
