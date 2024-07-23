#include "serialReader.h"
#include <iostream>

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

    return 0;
}
