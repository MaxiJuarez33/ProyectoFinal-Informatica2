#ifndef ELECTRICMANAGER_H
#define ELECTRICMANAGER_H

#include <iostream>
#include <algorithm>
#include "serialReader.h"
#include "deviceManager.h"
#include "tankManager.h"

extern SerialReader serialReader;
extern SaveDataVars dataSensor;
extern DeviceManager deviceManager;
extern Dispositivo deviceStruct;

class DeviceData
{
public:
    DeviceData() {};
    ~DeviceData() {};
    int id;
    std::string name;
    std::string type;
    double maxValue;
    double minValue;
    int pin;
    int priority;

    std::string getName() const { return this->name; }
    int getID() const { return this->id; }
    std::string getType() const { return this->type; }
    double getMaxValue() const { return this->maxValue; }
    double getMinValue() const { return this->minValue; }
    int getPin() const { return this->pin; }
    int getPriority() const { return this->priority; }
};

class Device : public DeviceData
{
public:
    Device();
    ~Device();
    void saveDevice(const std::string name, const std::string type, double maxValue, double minValue, int pin, int priority);
    auto getDeviceByID(int id);
    auto getDeviceByKeyword(const std::string keyword);
};

class DeviceControl
{
private:
    int priority;
    double batteryLevel = dataSensor.corrData[2];

public:
    DeviceControl();
    ~DeviceControl();
    bool isActivated(int id);
    void activate(int id);
    void deactivate(int id);
    bool isPeopleHome();
    bool canActivate();
    bool isPriority();
    void setPriority(int priority);
};

#endif
