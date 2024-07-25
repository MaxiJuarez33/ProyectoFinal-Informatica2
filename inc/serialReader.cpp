#include "SerialReader.h"

SerialReader::SerialReader(const char *portName)
    : portName(portName), hSerial(INVALID_HANDLE_VALUE), isPortOpen(false)
{
    memset(&dcbSerialParams, 0, sizeof(dcbSerialParams));
    memset(&timeouts, 0, sizeof(timeouts));
}

SerialReader::~SerialReader()
{
    if (isPortOpen)
    {
        CloseHandle(hSerial);
    }
}

bool SerialReader::openPort()
{
    hSerial = CreateFile(portName,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cout << "Error al abrir el puerto serie: " << portName << std::endl;
        return false;
    }

    std::cout << "Puerto serie abierto" << std::endl;
    isPortOpen = true;
    return true;
}

void SerialReader::configurePort()
{
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        std::cout << "No se pudo obtener la configuracion actual del puerto serie" << std::endl;
        return;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        std::cout << "No se pudo aplicar la configuracion para el puerto serie" << std::endl;
        return;
    }

    std::cout << "Nueva configuracion aplicada" << std::endl;

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        std::cout << "No se pudo aplicar los tiempos de espera para el puerto serie" << std::endl;
        return;
    }

    std::cout << "Tiempos de espera aplicados\n"
              << std::endl;
}

void SerialReader::readFromPort() // FUNCIONAMIENTO COMPROBADO
{
    SaveDataVars data;
    DWORD bytesRead;

    while (true)
    {
        if (ReadFile(hSerial, &data, sizeof(data), &bytesRead, NULL) && bytesRead == sizeof(data))
        {
            if (isFirstRead || memcmp(&data, &previousData, sizeof(SaveDataVars)) != 0)
            {
                std::cout << "\rTemp Int: " << data.tempData[0]
                          << " | Temp Ext: " << data.tempData[1]
                          << " | Cap Blancas: " << data.capData[0]
                          << " | Cap Grises: " << data.capData[1]
                          << " | Cap Negras: " << data.capData[2]
                          << " | Corriente 1: " << data.corrData[0]
                          << " | Corriente 2: " << data.corrData[1]
                          << " | Corriente 3: " << data.corrData[2]
                          << " | Corriente 4: " << data.corrData[3]
                          << std::flush;

                previousData = data;
                isFirstRead = false;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void SerialReader::startReading()
{
    std::thread reader(&SerialReader::readFromPort, this);
    reader.join();
}

void SerialReader::setDigitalLow(const char *relayPin)
{
    DWORD bytesWritten;
    std::string command = std::to_string(*relayPin) + ",0\n";

    WriteFile(hSerial, command.c_str(), command.size(), &bytesWritten, NULL);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void SerialReader::setDigitalHigh(const char *relayPin)
{
    DWORD bytesWritten;
    std::string command = std::to_string(*relayPin) + ",1\n";

    WriteFile(hSerial, command.c_str(), command.size(), &bytesWritten, NULL);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
