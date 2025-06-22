#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "deviceManager.h"
#include <fstream>

class DeviceManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Limpiar archivo de test antes de cada prueba
        std::ofstream file("test_devices.json");
        file << "[]";
        file.close();
    }

    void TearDown() override
    {
        // Limpiar después de cada test
        remove("test_devices.json");
    }

    DeviceManager deviceManager;
    Dispositivo testDevice;
};

// Test básico de creación de dispositivo
TEST_F(DeviceManagerTest, SetValuesCreatesDeviceCorrectly)
{
    // Arrange
    std::string name = "Test Sensor";
    std::string type = "Temperatura";
    double maxValue = 100.0;
    double minValue = -40.0;
    int pin = 36;
    int priority = 3;
    bool active = true;

    // Act
    Dispositivo result = deviceManager.setValues(
        testDevice, name, type, maxValue, minValue, pin, priority, active);

    // Assert
    EXPECT_EQ(result.name, name);
    EXPECT_EQ(result.type, type);
    EXPECT_EQ(result.maxValue, maxValue);
    EXPECT_EQ(result.minValue, minValue);
    EXPECT_EQ(result.pin, pin);
    EXPECT_EQ(result.priority, priority);
    EXPECT_EQ(result.active, active);
    EXPECT_GT(result.id, 0); // ID debe ser mayor a 0
}

// Test de valores límite
TEST_F(DeviceManagerTest, HandlesExtremeValues)
{
    // Test con valores extremos
    Dispositivo result = deviceManager.setValues(
        testDevice, "Extreme", "Test", 999999.99, -999999.99, 255, 5, false);

    EXPECT_EQ(result.maxValue, 999999.99);
    EXPECT_EQ(result.minValue, -999999.99);
    EXPECT_EQ(result.pin, 255);
    EXPECT_EQ(result.priority, 5);
    EXPECT_FALSE(result.active);
}

// Test de búsqueda por nombre
TEST_F(DeviceManagerTest, GetDeviceByNameWorks)
{
    // Arrange
    deviceManager.setValues(testDevice, "Temperatura Interior", "Sensor", 100.0, -40.0, 36, 3, true);

    // Act
    Dispositivo found = deviceManager.getDevice("Temperatura Interior", -1);

    // Assert
    EXPECT_EQ(found.name, "Temperatura Interior");
    EXPECT_EQ(found.type, "Sensor");
}

// Test de búsqueda por palabra clave
TEST_F(DeviceManagerTest, GetDeviceByKeywordWorks)
{
    // Arrange
    deviceManager.setValues(testDevice, "Sensor de Temperatura", "Analogico", 100.0, -40.0, 36, 3, true);

    // Act
    Dispositivo found = deviceManager.getDevice("Temperatura", -1);

    // Assert
    EXPECT_EQ(found.name, "Sensor de Temperatura");
}

// Test de dispositivo no encontrado
TEST_F(DeviceManagerTest, GetDeviceReturnsEmptyForNotFound)
{
    // Act
    Dispositivo notFound = deviceManager.getDevice("NoExiste", -1);

    // Assert
    EXPECT_EQ(notFound.id, 0); // ID 0 indica no encontrado
}

// Test con strings vacíos
TEST_F(DeviceManagerTest, HandlesEmptyStrings)
{
    // Act
    Dispositivo result = deviceManager.setValues(testDevice, "", "", 0.0, 0.0, 0, 0, false);

    // Assert
    EXPECT_TRUE(result.name.empty());
    EXPECT_TRUE(result.type.empty());
    EXPECT_GT(result.id, 0); // Debe asignar ID incluso con valores vacíos
}

// Test de múltiples dispositivos
TEST_F(DeviceManagerTest, HandlesMultipleDevices)
{
    // Arrange & Act
    Dispositivo device1 = deviceManager.setValues(testDevice, "Device1", "Type1", 100.0, 0.0, 1, 1, true);
    Dispositivo device2;
    device2 = deviceManager.setValues(device2, "Device2", "Type2", 200.0, 0.0, 2, 2, false);

    // Assert
    EXPECT_NE(device1.id, device2.id); // IDs deben ser diferentes
    EXPECT_GT(device2.id, device1.id); // ID debe incrementar
}
