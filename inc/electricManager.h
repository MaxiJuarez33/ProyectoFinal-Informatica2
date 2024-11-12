#ifndef ELECTRICMANAGER_H_
#define ELECTRICMANAGER_H_

#include <iostream>
#include "deviceManager.h"

extern DeviceManager deviceManager;
extern Dispositivo deviceStruct;

class Device
{
private:
    const int priorityThreshold = 3; // Max es 5
public:
    Device();
    ~Device();

    void saveDevice(const std::string name, const std::string type, double maxValue, double minValue, int pin, int priority, bool active);

    Dispositivo getDeviceById(int id);

    bool isActivated(int id);
    // void activate(int id);
    // void deactivate(int id);
    // bool canActivate(int id);
    bool isPriority(int id);
};

#endif
