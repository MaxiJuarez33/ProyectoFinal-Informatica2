#include "electricManager.h"

DeviceManager deviceManager;
Dispositivo deviceStruct;

Device::Device() {}
Device::~Device() {}

void Device::saveDevice(const std::string name, const std::string type, double maxValue, double minValue, int pin, int priority)
{
    deviceStruct = deviceManager.setValues(deviceStruct, name, type, maxValue, minValue, pin, priority);
    deviceManager.fileManage(deviceStruct);
}

Dispositivo Device::getDeviceById(int id)
{
    Dispositivo dispositivo = deviceManager.getDevice("", id);

    if (dispositivo.id == 0) // Asumiendo que un ID de 0 significa no válido
    {
        std::cerr << "Dispositivo no encontrado con ID: " << id << std::endl;
        throw std::runtime_error("Dispositivo no encontrado"); // Lanzar excepción si el dispositivo no es válido
    }

    return dispositivo;
}
