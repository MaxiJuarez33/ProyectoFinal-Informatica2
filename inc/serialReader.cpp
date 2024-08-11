#include "SerialReader.h"
#include <iostream>
#include <stdexcept>
#include <windows.h>
#include <string>
#include <cstring>
#include <QDebug>

std::wstring SerialReader::convertToWString(const char *Array)
{
    size_t size = strlen(Array) + 1;
    std::wstring wString(size, L'\0');
    mbstowcs(&wString[0], Array, size);
    return wString;
}

SerialReader::SerialReader(const char *portName)
{
    this->portName = convertToWString(portName);
}

SerialReader::~SerialReader()
{
    if (hSerial != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hSerial);
    }
}

bool SerialReader::openPort()
{
    hSerial = CreateFile(this->portName.c_str(),
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        DWORD dwError = GetLastError();
        qDebug() << "Error al abrir el puerto: " << dwError;
        return false;
    }
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
                // Manejo de los datos recibidos
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
