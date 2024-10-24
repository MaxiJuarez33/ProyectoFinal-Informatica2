/*
    Para empezar, se hace bastante rustico, asumiento que se usan 4 sensores, y que se usan los pines 33, 25, 26 y 27 para los sensores de corriente.
    Se asume que estos sensores estan dedicados a un dispositivo especifico, cada uno.
    33 Heladera
    25 Bomba de ague
    26 Ventilador a exterior
    27 Aire Acondicionado

    FALTAN PANEL SOLAR, INVERSOR y BATERIA

    COMO SE AGREGARON 3 SENSORES DE CORRIENTE, SE ASUME QUE ESTOS SON PARA DISPOSITIVOS COMO TV E ILUMINACION

    A INVESTIGAR
    Manejar los dispositivos por json
    Prediccion con regresion lineal usando los datos historicos (un programa que se ejecute periodicamente)
*/

#include <iostream>
#include <map>
#include <algorithm>
#include <ctime>
#include "serialReader.h"

SaveDataVars dataStruct;
SerialReader serialReader("COM5");

const float maxConsumption = dataStruct.corrData[3] / 2; // 50% de la corriente maxima

class Device
{
private:
    int priority;
    double batteryLevel = dataStruct.corrData[3];
    std::string deviceName;
    static std::map<std::string, char> pinMap;
    static std::map<std::string, double *> currentMap;

    static void initializeMaps()
    {
        if (pinMap.empty() && currentMap.empty())
        {
            pinMap["heladera"] = 33;
            pinMap["bombaAgua"] = 25;
            pinMap["aireAcondicionado"] = 27;
            pinMap["ventiladorExterior"] = 26;

            currentMap["heladera"] = &dataStruct.corrData[0];
            currentMap["bombaAgua"] = &dataStruct.corrData[1];
            currentMap["aireAcondionado"] = &dataStruct.corrData[2];
            currentMap["ventiladorExterior"] = &dataStruct.corrData[3];
        }
    }

    double currentMapped()
    {
        std::string lowerDeviceName = deviceName;
        std::transform(lowerDeviceName.begin(), lowerDeviceName.end(), lowerDeviceName.begin(), ::tolower);
        if (currentMap.find(lowerDeviceName) != currentMap.end())
        {
            return *currentMap[lowerDeviceName];
        }
        return 0;
    }

    char pinMapped()
    {
        std::string lowerDeviceName = deviceName;
        std::transform(lowerDeviceName.begin(), lowerDeviceName.end(), lowerDeviceName.begin(), ::tolower);
        if (pinMap.find(lowerDeviceName) != pinMap.end())
        {
            return pinMap[lowerDeviceName];
        }
        return -1;
    }

public:
    Device(int priority, std::string deviceName)
        : priority(priority), deviceName(deviceName)
    {
        initializeMaps();
    }

    bool isActivated()
    {
        return currentMapped() != 0;
    }

    void activate()
    {
        char pin = pinMapped();
        // serialReader.setDigitalHigh(&pin);
        std::cout << "Activando " << deviceName << std::endl;
    }

    void deactivate()
    {
        char pin = pinMapped();
        // serialReader.setDigitalLow(&pin);
        std::cout << "Desactivando " << deviceName << std::endl;
    }

    bool isPeopleHome()
    {
        return dataStruct.corrData[4] == 0 && dataStruct.corrData[5] == 0 && dataStruct.corrData[6] == 0;
    }

    bool canActivate()
    {
        return batteryLevel > 30 && isPeopleHome() /*|| se esta cargando la bateria*/;
    }

    bool isPriority() // SE DEBERIA AGREGAR MAS ANALISIS
    {
        if (isActivated() && currentMapped() < maxConsumption * 0.5) // Si esta activado o consume menos de la mitad de la corriente maxima
        {
            return true;
        }
        else if (!isActivated() && priority == 1) // Si no esta activado y es prioridad
        {
            return true;
        }
    }
};

class TempAnalyzer
{
private:
    double tempInt;
    double tempExt;
    // double batteryLevel = dataStruct.corrData[3];
    double batteryLevel = 100;
    double voltage = 3.3 / 4095.0; // ADC 12 bits del esp32
    double diffTemp;

    Device aireAcondicionado;
    Device ventiladorExterior;

    void tempConverter()
    {
        // tempInt = ((dataStruct.tempData[0] * voltage) * 100) - 273.15; // CONVIERTE LOS mv A GRADOS CELSIUS
        // tempExt = ((dataStruct.tempData[1] * voltage) * 100) - 273.15;
        tempInt = 28;
        tempExt = 26;
    }

    bool isBeetwen(double temp, double min, double max)
    {
        return temp > min && temp < max;
    }

    bool isColdestOutside()
    {
        diffTemp = tempInt - tempExt;
        return tempInt >= 28 && tempInt > tempExt && isBeetwen(diffTemp, 1, 5);
    }

public:
    tempAnalyzer() : aireAcondicionado(3, "aireAcondicionado"), ventiladorExterior(4, "ventiladorExterior")
    {
        analyze();
    }

    void analyze()
    {
        tempConverter();
        if (isBeetwen(tempInt, 26, 30) && batteryLevel > 50 && !isColdestOutside())
        {
            aireAcondicionado.activate(); // SE ASUME QUE EL AIRE O CLIMATIZADOR DETECTA LA TEMPERATURA Y SE AJUSTA SOLO (conveniente para la trama)
        }
        else if (isBeetwen(tempInt, 30, 35) && aireAcondicionado.canActivate() && !isColdestOutside())
        {
            aireAcondicionado.activate();
        }
        else if (tempInt > 35 && batteryLevel > 25 && aireAcondicionado.isPeopleHome())
        {
            aireAcondicionado.activate();
        }
        else if (isColdestOutside())
        {
            aireAcondicionado.deactivate();
            ventiladorExterior.activate();
        }
        else
        {
            aireAcondicionado.deactivate();
            ventiladorExterior.deactivate();
        }
    }
};

class EnergyManager
{
private:
    double batteryLevel;
    double maxConsumption;
    std::map<std::string, Device *> devices;

    std::map<std::string, int> daytimePriorities = {
        {"heladera", 1},
        {"bombaAgua", 2},
        {"aireAcondicionado", 3},
        {"ventiladorExterior", 4}};

    std::map<std::string, int> nighttimePriorities = {
        {"heladera", 1},
        {"ventiladorExterior", 2},
        {"bombaAgua", 3},
        {"aireAcondicionado", 4}};

    bool isDaytime()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int hour = ltm->tm_hour;

        return hour >= 6 && hour <= 20;
    }

    int getDevicePriority(std::string deviceName)
    {
        if (isDaytime())
        {
            return daytimePriorities[deviceName];
        }
        else
        {
            return nighttimePriorities[deviceName];
        }
    }

public:
    EnergyManager(double batteryLevel, double maxConsumption)
        : batteryLevel(batteryLevel), maxConsumption(maxConsumption) {}

    void addDevice(std::string deviceName, Device *device)
    {
        devices[deviceName] = device;
    }

    void manageDevices()
    {
        for (auto &devicePair : devices)
        {
            std::string deviceName = devicePair.first;
            Device *device = devicePair.second;

            int priority = getDevicePriority(deviceName);

            if (batteryLevel > 50 || (batteryLevel <= 50 && priority <= 2))
            {
                if (device->canActivate())
                {
                    device->activate();
                }
            }
            else
            {
                device->deactivate();
            }
        }
    }
};

std::map<std::string, char> Device::pinMap;
std::map<std::string, double *> Device::currentMap;

int main()
{
    // Device heladera(1, "heladera");
    // Device bombaAgua(3, "bombaAgua");

    TempAnalyzer tempAnalyzer;

    // std::cout << bombaAgua.isPriority() << std::endl;

    return 0;
}
