/*
    Para empezar, se hace bastante rustico, asumiento que se usan 4 sensores, y que se usan los pines 33, 25, 26 y 27 para los sensores de corriente.
    Se asume que estos sensores estan dedicados a un dispositivo especifico, cada uno.
    33 Heladera
    25 Cocina
    26 Aire acondicionado

    DE PRINCIPIO, SE ASUME QUE LA BATERIA ES DE 10000 mAh


    A INVESTIGAR
    Manejar los dispositivos por json
*/

#include <iostream>
#include <map>
#include <algorithm>
#include "serialReader.h"

SaveDataVars dataStruct;
SerialReader serialReader("COM5");

const float maxConsumption = dataStruct.corrData[3] / 2;

class Device
{
private:
    double consumption;
    int priority;
    std::string deviceName;
    static std::map<std::string, char> pinMap;
    static std::map<std::string, double *> currentMap;

    static void initializeMaps()
    {
        if (pinMap.empty() && currentMap.empty())
        {
            pinMap["heladera"] = 33;
            pinMap["cocina"] = 25;
            pinMap["aire acondicionado"] = 26;

            currentMap["heladera"] = &dataStruct.corrData[0];
            currentMap["cocina"] = &dataStruct.corrData[1];
            currentMap["aire acondicionado"] = &dataStruct.corrData[2];
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
    Device(double consumption, int priority, std::string deviceName)
        : consumption(consumption), priority(priority), deviceName(deviceName)
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
        serialReader.setDigitalHigh(&pin);
    }

    void deactivate()
    {
        char pin = pinMapped();
        serialReader.setDigitalLow(&pin);
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
        return false;
    }
};

std::map<std::string, char> Device::pinMap;
std::map<std::string, double *> Device::currentMap;

int main()
{
    Device heladera(150.0, 1, "heladera");
    Device cocina(150.0, 2, "cocina");

    std::cout << cocina.isPriority() << std::endl;

    // if (!heladera.isActivated())
    // {
    //     heladera.deactivate();
    // }

    return 0;
}
