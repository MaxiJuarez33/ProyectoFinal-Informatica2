#ifndef ELECTRICMANAGER_H_
#define ELECTRICMANAGER_H_

#include <iostream>
#include "deviceManager.h"

extern DeviceManager deviceManager;
extern Dispositivo deviceStruct;

class Device
{
public:
    Device();
    ~Device();

    void saveDevice(const std::string name, const std::string type, double maxValue, double minValue, int pin, int priority);

    Dispositivo getDeviceById(int id);
};

#endif
