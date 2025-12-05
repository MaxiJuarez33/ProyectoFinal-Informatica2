#ifndef SERIALREADER_H
#define SERIALREADER_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>

struct SaveDataVars
{
    double tempData[2]; // 1 Int & 2 Ext
    double capData[3];  // 1 Blancas & 2 Grises & 3 Negras
    double corrData[7]; // SE AGREGAN 3 MAS DE FORMA TEMPORAL, ORIGINALMENTE SON 4 LOS DECLARADOS EN ESP32
    char timeString[10];
};

class SerialReader
{
public:
    SerialReader(const char *portName);
    ~SerialReader();
    bool openPort();
    void configurePort();
    void readFromPort();
    void startReading();
    void setDigitalLow(const char *relayPin);
    void setDigitalHigh(const char *relayPin);
    SaveDataVars readSensorData();

    const char *portName;
    bool isPortOpen;
    SaveDataVars previousData;
    bool isFirstRead = true;

#ifdef _WIN32
    HANDLE hSerial;
    DCB dcbSerialParams;
    COMMTIMEOUTS timeouts;
#endif
};

#endif
