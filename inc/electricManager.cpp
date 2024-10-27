#include "electricManager.h"

SerialReader serialReader("COM5");
SaveDataVars dataSensor;
DeviceManager deviceManager;
Dispositivo deviceStruct;

const float lowLevelBattery = dataSensor.corrData[0] * (3 / 4); // 75% de la corriente maxima

Device::Device() {}
Device::~Device() {}

void Device::saveDevice(const std::string name, const std::string type, double maxValue, double minValue, int pin, int priority)
{
    deviceStruct = deviceManager.setValues(deviceStruct, name, type, maxValue, minValue, pin, priority);
    deviceManager.fileManage(deviceStruct);
}

auto Device::getDeviceByID(int id)
{
    Dispositivo dispositivo = deviceManager.getDevice("", id);

    if (dispositivo.id == 0)
    {
        std::cerr << "No se pudo encontrar un dispositivo con el ID: " << id << std::endl;
        return *this;
    }

    this->id = dispositivo.id;
    this->name = dispositivo.name;
    this->type = dispositivo.type;
    this->maxValue = dispositivo.maxValue;
    this->minValue = dispositivo.minValue;
    this->pin = dispositivo.pin;
    this->priority = dispositivo.priority;

    return *this;
}

auto Device::getDeviceByKeyword(const std::string keyword)
{
    Dispositivo dispositivo = deviceManager.getDevice(keyword, -1);

    if (dispositivo.name.empty())
    {
        std::cerr << "No se pudo encontrar un dispositivo con el keyword: " << keyword << std::endl;
        return *this;
    }

    this->id = dispositivo.id;
    this->name = dispositivo.name;
    this->type = dispositivo.type;
    this->maxValue = dispositivo.maxValue;
    this->minValue = dispositivo.minValue;
    this->pin = dispositivo.pin;
    this->priority = dispositivo.priority;

    return *this;
}

DeviceControl::DeviceControl() {}
DeviceControl::~DeviceControl() {}

bool DeviceControl::isActivated(int id)
{
    // No se como implementar esto
    return true;
}

void DeviceControl::activate(int id)
{
    Device device;
    int pin = device.getDeviceByID(id).getPin();
    char pinChar = pin + '0';
    serialReader.setDigitalHigh(&pinChar);
}

void DeviceControl::deactivate(int id)
{
    Device device;
    int pin = device.getDeviceByID(id).getPin();
    char pinChar = pin + '0';
    serialReader.setDigitalLow(&pinChar);
}

bool DeviceControl::isPeopleHome() { return dataSensor.corrData[2] = 0; }

bool DeviceControl::canActivate() { return batteryLevel > lowLevelBattery && isPeopleHome(); }