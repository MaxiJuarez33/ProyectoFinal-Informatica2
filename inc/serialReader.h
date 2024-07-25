#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>

struct SaveDataVars
{
    double tempData[2]; // 1 Int & 2 Ext
    double capData[3];  // 1 Blancas & 2 Grises & 3 Negras
    double corrData[4];
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

private:
    const char *portName;
    HANDLE hSerial;
    DCB dcbSerialParams;
    COMMTIMEOUTS timeouts;
    bool isPortOpen;
    SaveDataVars previousData;
    bool isFirstRead = true;
};

#endif
