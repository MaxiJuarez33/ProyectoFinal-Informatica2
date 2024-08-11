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
    double corrData[4];
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

private:
    std::wstring portName; // Change type to std::wstring
    HANDLE hSerial;
    DCB dcbSerialParams;
    COMMTIMEOUTS timeouts;
    bool isPortOpen;
    SaveDataVars previousData;
    bool isFirstRead = true;
    std::wstring convertToWString(const char *charArray); // Update declaration
};

#endif // SERIALREADER_H