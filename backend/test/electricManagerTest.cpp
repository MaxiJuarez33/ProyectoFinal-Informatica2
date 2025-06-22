#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "electricManager.h"

class ElectricManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Limpiar archivos de test
        std::ofstream file("devices.json");
        file << "[]";
        file.close();

        device = std::make_unique<Device>();
    }

    void TearDown() override
    {
        remove("devices.json");
    }

    std::unique_ptr<Device> device;
};

// Test de guardado de dispositivo
TEST_F(ElectricManagerTest, SaveDeviceWorks)
{
    // Act
    EXPECT_NO_THROW({
        device->saveDevice("Test Device", "Relay", 12.0, 0.0, 25, 3, true);
    });
}

// Test de dispositivo con prioridad alta
TEST_F(ElectricManagerTest, HighPriorityDeviceDetection)
{
    // Arrange
    device->saveDevice("High Priority", "Critical", 100.0, 0.0, 33, 5, true);

    // Buscar el dispositivo guardado (asumiendo ID 1)
    // Act
    bool isPriority = device->isPriority(1);

    // Assert
    EXPECT_TRUE(isPriority);
}

// Test de dispositivo con prioridad baja
TEST_F(ElectricManagerTest, LowPriorityDeviceDetection)
{
    // Arrange
    device->saveDevice("Low Priority", "Optional", 50.0, 0.0, 26, 1, false);

    // Act
    bool isPriority = device->isPriority(1);

    // Assert
    EXPECT_FALSE(isPriority);
}

// Test de dispositivo en el umbral de prioridad
TEST_F(ElectricManagerTest, ThresholdPriorityDevice)
{
    // Arrange - Prioridad exactamente en el umbral (3)
    device->saveDevice("Threshold Device", "Border", 75.0, 0.0, 27, 3, true);

    // Act
    bool isPriority = device->isPriority(1);

    // Assert
    EXPECT_TRUE(isPriority); // >= 3 es prioridad
}

// Test de estado de activación
TEST_F(ElectricManagerTest, DeviceActivationStatus)
{
    // Arrange
    device->saveDevice("Active Device", "Sensor", 100.0, 0.0, 36, 4, true);

    // Act
    bool isActive = device->isActivated(1);

    // Assert
    EXPECT_TRUE(isActive);
}

// Test de dispositivo inactivo
TEST_F(ElectricManagerTest, InactiveDeviceStatus)
{
    // Arrange
    device->saveDevice("Inactive Device", "Sensor", 100.0, 0.0, 39, 2, false);

    // Act
    bool isActive = device->isActivated(1);

    // Assert
    EXPECT_FALSE(isActive);
}

// Test de obtener dispositivo por ID válido
TEST_F(ElectricManagerTest, GetDeviceByValidId)
{
    // Arrange
    device->saveDevice("Valid Device", "Test", 50.0, 0.0, 32, 3, true);

    // Act
    Dispositivo retrieved = device->getDeviceById(1);

    // Assert
    EXPECT_EQ(retrieved.name, "Valid Device");
    EXPECT_EQ(retrieved.type, "Test");
    EXPECT_EQ(retrieved.pin, 32);
    EXPECT_EQ(retrieved.priority, 3);
    EXPECT_TRUE(retrieved.active);
}

// Test de error con ID inválido
TEST_F(ElectricManagerTest, GetDeviceByInvalidIdThrows)
{
    // Act & Assert
    EXPECT_THROW({
        device->getDeviceById(999); // ID que no existe
    },
                 std::runtime_error);
}

// Test de múltiples dispositivos con diferentes prioridades
TEST_F(ElectricManagerTest, MultipleDevicesWithDifferentPriorities)
{
    // Arrange
    device->saveDevice("Low Priority", "Optional", 10.0, 0.0, 25, 1, true);
    device->saveDevice("High Priority", "Critical", 100.0, 0.0, 26, 5, true);
    device->saveDevice("Medium Priority", "Normal", 50.0, 0.0, 27, 3, true);

    // Act & Assert
    EXPECT_FALSE(device->isPriority(1)); // Prioridad 1 < 3
    EXPECT_TRUE(device->isPriority(2));  // Prioridad 5 >= 3
    EXPECT_TRUE(device->isPriority(3));  // Prioridad 3 >= 3
}

// Test de valores extremos en dispositivos
TEST_F(ElectricManagerTest, ExtremeDeviceValues)
{
    // Act & Assert - No debe fallar con valores extremos
    EXPECT_NO_THROW({
        device->saveDevice("Extreme", "Test", 999999.0, -999999.0, 255, 5, true);
        device->getDeviceById(1);
        device->isPriority(1);
        device->isActivated(1);
    });
}
