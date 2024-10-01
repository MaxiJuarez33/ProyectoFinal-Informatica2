#include "tankController.h"
#include <iostream>
#include <cmath>
#include <numbers>

// Constructor que inicializa los niveles y valores máximos
WaterTank::WaterTank() 
    : whiteMax(75), grayBlackMax(54), whiteCriticLevel(20), whiteStopPumpLevel(10), grayBlackCriticLevel(50)
{
    levelMap();  // Llamada para inicializar los niveles
}

// Función privada que calcula los niveles de agua a partir de los sensores
void WaterTank::levelMap() 
{
    whiteLevel = whiteMax - (dataStruct.capData[0] * std::pow(0.15, 2) * std::numbers::pi);
    grayLevel = grayBlackMax - (dataStruct.capData[1] * std::pow(0.125, 2) * std::numbers::pi);
    blackLevel = grayBlackMax - (dataStruct.capData[2] * std::pow(0.125, 2) * std::numbers::pi);
}

// Funciones que verifican niveles críticos
bool WaterTank::isWhiteCritical() 
{
    return whiteLevel < whiteCriticLevel;
}

bool WaterTank::isGrayCritical() 
{
    return grayLevel < grayBlackCriticLevel;
}

bool WaterTank::isBlackCritical() 
{
    return blackLevel < grayBlackCriticLevel;
}

bool WaterTank::isStopPumpNeeded() 
{
    return whiteLevel < whiteStopPumpLevel;
}

// Función que muestra el estado de los tanques
void WaterTank::tankStatus() 
{
    std::cout << "White Level: " << whiteLevel << std::endl;
    std::cout << "Gray Level: " << grayLevel << std::endl;
    std::cout << "Black Level: " << blackLevel << std::endl;
}
