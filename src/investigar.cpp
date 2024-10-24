#include <iostream>
#include <map>
#include <algorithm>
#include <functional>
#include <memory>
#include "serialReader.h"

// Estructura para almacenar datos del sistema
SaveDataVars dataStruct;
SerialReader serialReader("COM5");

// Constantes de configuración
const float MAX_CONSUMPTION = dataStruct.corrData[3] / 2; // 50% de la corriente máxima

// Enumeración para los dispositivos
enum class DeviceType
{
    Heladera,
    BombaAgua,
    AireAcondicionado,
    VentiladorExterior
};

// Clase base abstracta para dispositivos
class Device
{
protected:
    int priority;
    double batteryLevel;
    DeviceType deviceType;

    virtual double getCurrent() const = 0;
    virtual char getPin() const = 0;

public:
    Device(int priority, DeviceType type)
        : priority(priority), deviceType(type), batteryLevel(dataStruct.corrData[3]) {}

    virtual ~Device() = default;

    virtual bool isActivated() const = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;

    bool canActivate() const
    {
        return batteryLevel > 30 && isPeopleHome();
    }

    bool isPriority() const
    {
        return (isActivated() && getCurrent() < MAX_CONSUMPTION * 0.5) || (!isActivated() && priority == 1);
    }

    bool isPeopleHome() const
    {
        return dataStruct.corrData[4] == 0 && dataStruct.corrData[5] == 0 && dataStruct.corrData[6] == 0;
    }
};

// Implementación concreta de un dispositivo específico
class SpecificDevice : public Device
{
private:
    static std::map<DeviceType, char> pinMap;
    static std::map<DeviceType, double *> currentMap;

    double getCurrent() const override
    {
        auto it = currentMap.find(deviceType);
        return (it != currentMap.end()) ? *(it->second) : 0.0;
    }

    char getPin() const override
    {
        auto it = pinMap.find(deviceType);
        return (it != pinMap.end()) ? it->second : -1;
    }

public:
    SpecificDevice(int priority, DeviceType type) : Device(priority, type)
    {
        initializeMaps();
    }

    bool isActivated() const override
    {
        return getCurrent() != 0;
    }

    void activate() override
    {
        char pin = getPin();
        // serialReader.setDigitalHigh(&pin);
        std::cout << "Activando " << static_cast<int>(deviceType) << std::endl; // Mostrar el tipo de dispositivo
    }

    void deactivate() override
    {
        char pin = getPin();
        // serialReader.setDigitalLow(&pin);
        std::cout << "Desactivando " << static_cast<int>(deviceType) << std::endl;
    }

    static void initializeMaps()
    {
        if (pinMap.empty() && currentMap.empty())
        {
            pinMap[DeviceType::Heladera] = 33;
            pinMap[DeviceType::BombaAgua] = 25;
            pinMap[DeviceType::AireAcondicionado] = 27;
            pinMap[DeviceType::VentiladorExterior] = 26;

            currentMap[DeviceType::Heladera] = &dataStruct.corrData[0];
            currentMap[DeviceType::BombaAgua] = &dataStruct.corrData[1];
            currentMap[DeviceType::AireAcondicionado] = &dataStruct.corrData[2];
            currentMap[DeviceType::VentiladorExterior] = &dataStruct.corrData[3];
        }
    }
};

std::map<DeviceType, char> SpecificDevice::pinMap;
std::map<DeviceType, double *> SpecificDevice::currentMap;

// Clase para análisis de temperatura
class TempAnalyzer
{
private:
    double tempInt;
    double tempExt;
    double batteryLevel;
    double voltage;
    double diffTemp;

    std::unique_ptr<Device> aireAcondicionado;
    std::unique_ptr<Device> ventiladorExterior;

    void tempConverter()
    {
        // tempInt = ((dataStruct.tempData[0] * voltage) * 100) - 273.15; // CONVIERTE LOS mv A GRADOS CELSIUS
        // tempExt = ((dataStruct.tempData[1] * voltage) * 100) - 273.15;
        tempInt = 28;
        tempExt = 26;
    }

    bool isBetween(double temp, double min, double max) const
    {
        return temp > min && temp < max;
    }

    bool isColdestOutside()
    {
        diffTemp = tempInt - tempExt;
        return tempInt >= 28 && tempInt > tempExt && isBetween(diffTemp, 1, 5);
    }

    void controlDevices()
    {
        if (isBetween(tempInt, 26, 30) && batteryLevel > 50 && !isColdestOutside())
        {
            aireAcondicionado->activate();
        }
        else if (isBetween(tempInt, 30, 35) && aireAcondicionado->canActivate() && !isColdestOutside())
        {
            aireAcondicionado->activate();
        }
        else if (tempInt > 35 && batteryLevel > 25 && aireAcondicionado->isPeopleHome())
        {
            aireAcondicionado->activate();
        }
        else if (isColdestOutside())
        {
            aireAcondicionado->deactivate();
            ventiladorExterior->activate();
        }
        else
        {
            aireAcondicionado->deactivate();
            ventiladorExterior->deactivate();
        }
    }

public:
    TempAnalyzer() : batteryLevel(100), voltage(3.3 / 4095.0),
                     aireAcondicionado(std::make_unique<SpecificDevice>(3, DeviceType::AireAcondicionado)),
                     ventiladorExterior(std::make_unique<SpecificDevice>(4, DeviceType::VentiladorExterior))
    {
        tempConverter();
        controlDevices();
    }

    void analyze()
    {
        controlDevices();
    }
};

int main()
{
    TempAnalyzer tempAnalyzer;

    // tempAnalyzer.analyze();

    return 0;
}
