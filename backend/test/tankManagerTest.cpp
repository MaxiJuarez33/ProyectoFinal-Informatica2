#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "tankManager.h"

// Mock para los datos del sensor
extern SaveDataVars dataStruct;

class TankManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Inicializar datos mock
        dataStruct.capData[0] = 50.0; // Tanque blanco
        dataStruct.capData[1] = 30.0; // Tanque gris
        dataStruct.capData[2] = 25.0; // Tanque negro

        tankManager = std::make_unique<WaterTank>();
    }

    std::unique_ptr<WaterTank> tankManager;
};

// Test básico de construcción
TEST_F(TankManagerTest, ConstructorInitializesCorrectly)
{
    // Assert - No debe lanzar excepciones
    EXPECT_NO_THROW(WaterTank tank);
}

// Test de nivel crítico del tanque blanco
TEST_F(TankManagerTest, WhiteTankCriticalLevelDetection)
{
    // Arrange - Simular nivel bajo
    dataStruct.capData[0] = 70.0; // Esto debería resultar en nivel bajo

    // Act
    WaterTank tank;
    bool isCritical = tank.isWhiteCritical();

    // Assert
    EXPECT_TRUE(isCritical);
}

// Test de nivel normal del tanque blanco
TEST_F(TankManagerTest, WhiteTankNormalLevel)
{
    // Arrange - Simular nivel normal
    dataStruct.capData[0] = 10.0; // Esto debería resultar en nivel alto

    // Act
    WaterTank tank;
    bool isCritical = tank.isWhiteCritical();

    // Assert
    EXPECT_FALSE(isCritical);
}

// Test de parada de bomba necesaria
TEST_F(TankManagerTest, StopPumpWhenVeryLow)
{
    // Arrange - Simular nivel muy bajo
    dataStruct.capData[0] = 73.0; // Nivel muy bajo

    // Act
    WaterTank tank;
    bool stopPump = tank.isStopPumpNeeded();

    // Assert
    EXPECT_TRUE(stopPump);
}

// Test de tanque gris crítico
TEST_F(TankManagerTest, GrayTankCriticalLevel)
{
    // Arrange - Simular tanque gris lleno
    dataStruct.capData[1] = 52.0; // Nivel alto = crítico

    // Act
    WaterTank tank;
    bool isCritical = tank.isGrayCritical();

    // Assert
    EXPECT_TRUE(isCritical);
}

// Test de tanque negro crítico
TEST_F(TankManagerTest, BlackTankCriticalLevel)
{
    // Arrange - Simular tanque negro lleno
    dataStruct.capData[2] = 52.0; // Nivel alto = crítico

    // Act
    WaterTank tank;
    bool isCritical = tank.isBlackCritical();

    // Assert
    EXPECT_TRUE(isCritical);
}

// Test de todos los tanques en estado normal
TEST_F(TankManagerTest, AllTanksNormalState)
{
    // Arrange - Niveles normales
    dataStruct.capData[0] = 20.0; // Blanco: nivel alto
    dataStruct.capData[1] = 10.0; // Gris: nivel bajo
    dataStruct.capData[2] = 10.0; // Negro: nivel bajo

    // Act
    WaterTank tank;

    // Assert
    EXPECT_FALSE(tank.isWhiteCritical());
    EXPECT_FALSE(tank.isGrayCritical());
    EXPECT_FALSE(tank.isBlackCritical());
    EXPECT_FALSE(tank.isStopPumpNeeded());
}

// Test de estado mixto
TEST_F(TankManagerTest, MixedTankStates)
{
    // Arrange - Estados mixtos
    dataStruct.capData[0] = 15.0; // Blanco: OK
    dataStruct.capData[1] = 52.0; // Gris: Crítico
    dataStruct.capData[2] = 10.0; // Negro: OK

    // Act
    WaterTank tank;

    // Assert
    EXPECT_FALSE(tank.isWhiteCritical());
    EXPECT_TRUE(tank.isGrayCritical());
    EXPECT_FALSE(tank.isBlackCritical());
}

// Test de valores extremos
TEST_F(TankManagerTest, ExtremeValues)
{
    // Arrange - Valores extremos
    dataStruct.capData[0] = 0.0;   // Mínimo
    dataStruct.capData[1] = 100.0; // Máximo teórico
    dataStruct.capData[2] = -5.0;  // Valor negativo

    // Act & Assert - No debe crashear
    EXPECT_NO_THROW({
        WaterTank tank;
        tank.isWhiteCritical();
        tank.isGrayCritical();
        tank.isBlackCritical();
        tank.isStopPumpNeeded();
    });
}