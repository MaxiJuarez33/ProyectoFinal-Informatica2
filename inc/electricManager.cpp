#include "electricManager.h"

DeviceManager deviceManager;
Dispositivo deviceStruct;

Device::Device() {}
Device::~Device() {}

void Device::saveDevice(const std::string name, const std::string type, double maxValue, double minValue, int pin, int priority, bool active)
{
    deviceStruct = deviceManager.setValues(deviceStruct, name, type, maxValue, minValue, pin, priority, active);
    deviceManager.fileManage(deviceStruct);
}

Dispositivo Device::getDeviceById(int id)
{
    Dispositivo dispositivo = deviceManager.getDevice("", id);

    if (dispositivo.id == 0)
    {
        std::cerr << "Dispositivo no encontrado con ID: " << id << std::endl;
        throw std::runtime_error("Dispositivo no encontrado");
    }

    return dispositivo;
}

bool Device::isActivated(int id)
{
    Dispositivo tempDev = getDeviceById(id);
    return tempDev.active;
}

// Device::canActivate(int id) // no se puede programar por ahora, falta sistema de manejo de consumo en realtime
// {
//     Dispositivo tempDev = getDeviceById(id);
//     return tempDev.priority ;
// }

bool Device::isPriority(int id)
{
    Dispositivo tempDev = getDeviceById(id);
    return tempDev.priority >= priorityThreshold; // Agregar que tambien se analice el consumo de la casa y bateria restante
}