#include "serialReader.h"
#include <iostream>
#include <stdexcept>
#include <cstring>

#ifndef _WIN32
#include <random>
#include <ctime>
#endif

SerialReader::SerialReader(const char *portName)
    : portName(portName), isPortOpen(false)
{
#ifdef _WIN32
    hSerial = INVALID_HANDLE_VALUE;
    memset(&dcbSerialParams, 0, sizeof(dcbSerialParams));
    memset(&timeouts, 0, sizeof(timeouts));
#endif
}

SerialReader::~SerialReader()
{
#ifdef _WIN32
    if (isPortOpen)
    {
        CloseHandle(hSerial);
    }
#endif
}

bool SerialReader::openPort()
{
#ifdef _WIN32
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
#else
    std::cout << "MOCK: Puerto " << portName << " abierto simulado." << std::endl;
#endif

    isPortOpen = true;
    return true;
}

void SerialReader::configurePort()
{
#ifdef _WIN32
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
#else
    std::cout << "MOCK: Puerto configurado." << std::endl;
#endif
}

void SerialReader::readFromPort() // FUNCIONAMIENTO COMPROBADO
{
#ifdef _WIN32
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
#else
    // Mock implementation
    while (true)
    {
        SaveDataVars data;
        // Generar datos aleatorios
        data.tempData[0] = 20.0 + (rand() % 100) / 10.0; // 20-30 C
        data.tempData[1] = 10.0 + (rand() % 100) / 10.0; // 10-20 C
        data.capData[0] = 50.0 + (rand() % 50);
        data.capData[1] = 30.0 + (rand() % 50);
        data.capData[2] = 10.0 + (rand() % 50);
        data.corrData[0] = 12.0 + (rand() % 20) / 10.0; // 12-14 V

        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
        strncpy(data.timeString, buf, 10);

        previousData = data;
        isFirstRead = false;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
#endif
}

void SerialReader::startReading()
{
    std::thread reader(&SerialReader::readFromPort, this);
    reader.join();
}

void SerialReader::setDigitalLow(const char *relayPin)
{
#ifdef _WIN32
    DWORD bytesWritten;
    std::string command = std::to_string(*relayPin) + ",0\n";

    if (!WriteFile(hSerial, command.c_str(), command.size(), &bytesWritten, NULL))
    {
        throw std::runtime_error("Error al escribir comando para establecer bajo el pin " + std::string(1, *relayPin));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#else
    std::cout << "MOCK: Pin " << (int)*relayPin << " set LOW" << std::endl;
#endif
}

void SerialReader::setDigitalHigh(const char *relayPin)
{
#ifdef _WIN32
    DWORD bytesWritten;
    std::string command = std::to_string(*relayPin) + ",1\n";

    if (!WriteFile(hSerial, command.c_str(), command.size(), &bytesWritten, NULL))
    {
        throw std::runtime_error("Error al escribir comando para establecer alto el pin " + std::string(1, *relayPin));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#else
    std::cout << "MOCK: Pin " << (int)*relayPin << " set HIGH" << std::endl;
#endif
}

SaveDataVars SerialReader::readSensorData()
{
#ifdef _WIN32
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
#else
    return previousData;
#endif
}
