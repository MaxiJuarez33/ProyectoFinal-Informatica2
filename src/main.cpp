#include <iostream>
#include <cmath>
#include <numbers>

class WaterTank
{
private:
    double whiteLevel;
    double grayLevel;
    double blackLevel;
    double whiteMax = 75; // Litros
    double grayBlackMax = 54;
    double whiteCriticLevel = 20;
    double whiteStopPumpLevel = 10;
    double grayBlackCriticLevel = 50;

    // Función para determinar el nivel de líquido mediante el dato del sensor
    void levelMap()
    {
        // PARA BLANCAS SE SUPONE DIÁMETRO DE 0.3m y 1.06m de altura
        whiteLevel = whiteMax - (dataStruct.capData[0] * std::pow(0.15, 2) * std::numbers::pi);
        // PARA GRIS Y NEGRAS SE SUPONE DIÁMETRO DE 0.25m y 1.10m de altura
        grayLevel = grayBlackMax - (dataStruct.capData[1] * std::pow(0.125, 2) * std::numbers::pi);
        blackLevel = grayBlackMax - (dataStruct.capData[2] * std::pow(0.125, 2) * std::numbers::pi);
    }

public:

    bool isWhiteCritical()
    {
        return whiteLevel < whiteCriticLevel;
    }

    bool isGrayCritical()
    {
        return grayLevel < grayBlackCriticLevel;
    }

    bool isBlackCritical()
    {
        return blackLevel < grayBlackCriticLevel;
    }

    bool isStopPumpNeeded()
    {
        return whiteLevel < whiteStopPumpLevel;
    }

    WaterTank()
    {
        levelMap();
    }

    void tankStatus()
    {
        std::cout << "White Level: " << whiteLevel << std::endl;
        std::cout << "Gray Level: " << grayLevel << std::endl;
        std::cout << "Black Level: " << blackLevel << std::endl;
    }
};
