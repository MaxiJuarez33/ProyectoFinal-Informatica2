#ifndef TANKMANAGER_H
#define TANKMANAGER_H

class WaterTank
{
private:
    double whiteLevel;
    double grayLevel;
    double blackLevel;
    double whiteMax;
    double grayBlackMax;
    double whiteCriticLevel;
    double whiteStopPumpLevel;
    double grayBlackCriticLevel;

    // Función privada para determinar el nivel de líquido
    void levelMap();

public:
    // Constructor
    WaterTank();

    // Funciones públicas para verificar los niveles críticos
    bool isWhiteCritical();
    bool isGrayCritical();
    bool isBlackCritical();
    bool isStopPumpNeeded();

    // Función para mostrar el estado del tanque
    void tankStatus();
};

#endif
