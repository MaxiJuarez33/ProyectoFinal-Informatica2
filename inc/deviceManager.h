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
    std::string nombre;
    std::string tipo;
    double valorMax;
    double valorMin;
    int pin;
};

class DeviceManager
{

public:
    DeviceManager();
    ~DeviceManager();

    void fileManage(Dispositivo &structname);
    Dispositivo setValues(Dispositivo &structname, const std::string nombre, const std::string tipo, double valorMax, double valorMin, int pin);
    void getDevice(const std::string &nombre, int id);

private:
    std::string nombre;
    int id;
    std::string filename;
    std::vector<Dispositivo> dispositivos;
    int getNextId();
};

#endif
