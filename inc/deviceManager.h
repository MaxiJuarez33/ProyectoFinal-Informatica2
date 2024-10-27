#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <json.hpp>

using json = nlohmann::json;

struct Dispositivo
{
    int id;
    std::string name;
    std::string type;
    double maxValue;
    double minValue;
    int pin;
};

class DeviceManager
{

public:
    DeviceManager();
    ~DeviceManager();

    void fileManage(Dispositivo &structname);
    Dispositivo setValues(Dispositivo &structname, const std::string name, const std::string type, double maxValue, double minValue, int pin);
    Dispositivo getDevice(const std::string &name, int id);

private:
    std::string name;
    int id;
    std::string filename;
    std::vector<Dispositivo> dispositivos;
    int getNextId();
};

#endif
