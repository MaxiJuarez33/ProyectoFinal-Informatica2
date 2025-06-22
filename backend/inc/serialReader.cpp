#include "SerialReader.h"
#include <iostream>
#include <stdexcept>

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
        throw std::runtime_error("Error al abrir el puerto serie: " + std::string(portName));
    }

    isPortOpen = true;
    return true;
}

void SerialReader::configurePort()
{
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        throw std::runtime_error("No se pudo obtener la configuracion actual del puerto serie");
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        throw std::runtime_error("No se pudo aplicar la configuracion para el puerto serie");
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        throw std::runtime_error("No se pudo aplicar los tiempos de espera para el puerto serie");
    }
}

void SerialReader::readFromPort() // FUNCIONAMIENTO COMPROBADO
{
    SaveDataVars data;
    DWORD bytesRead;

    while (true)
    {
        if (ReadFile(hSerial, &data, sizeof(data), &bytesRead, NULL))
        {
            if (bytesRead != sizeof(data))
            {
                std::cerr << "Datos incompletos leídos del puerto serie" << std::endl;
                continue;
            }

            if (isFirstRead || memcmp(&data, &previousData, sizeof(SaveDataVars)) != 0)
            {
                previousData = data;
                isFirstRead = false;
            }
        }
        else
        {
            std::cerr << "Error al leer del puerto serie" << std::endl;
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

    if (!WriteFile(hSerial, command.c_str(), command.size(), &bytesWritten, NULL))
    {
        throw std::runtime_error("Error al escribir comando para establecer bajo el pin " + std::string(1, *relayPin));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void SerialReader::setDigitalHigh(const char *relayPin)
{
    DWORD bytesWritten;
    std::string command = std::to_string(*relayPin) + ",1\n";

    if (!WriteFile(hSerial, command.c_str(), command.size(), &bytesWritten, NULL))
    {
        throw std::runtime_error("Error al escribir comando para establecer alto el pin " + std::string(1, *relayPin));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

SaveDataVars SerialReader::readSensorData()
{
    SaveDataVars data;
    DWORD bytesRead;

    if (ReadFile(hSerial, &data, sizeof(data), &bytesRead, NULL))
    {
        if (bytesRead == sizeof(data))
        {
            return data;
        }
    }

    // Return previous data if read fails
    return previousData;
}
