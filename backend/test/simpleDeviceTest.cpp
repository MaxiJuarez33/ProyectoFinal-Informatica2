#include "simpleTest.h"
#include "deviceManager.h"
#include <fstream>

// Test básico de creación de dispositivo
TEST_CASE(DeviceManager_SetValues_CreatesCorrectly)
{
    DeviceManager deviceManager;
    Dispositivo testDevice;

    // Limpiar archivo de test
    std::ofstream file("test_devices.json");
    file << "[]";
    file.close();

    // Test
    std::string name = "Test Sensor";
    std::string type = "Temperatura";
    double maxValue = 100.0;
    double minValue = -40.0;
    int pin = 36;
    int priority = 3;
    bool active = true;

    Dispositivo result = deviceManager.setValues(
        testDevice, name, type, maxValue, minValue, pin, priority, active);

    ASSERT_EQ(result.name, name);
    ASSERT_EQ(result.type, type);
    ASSERT_EQ(result.maxValue, maxValue);
    ASSERT_EQ(result.minValue, minValue);
    ASSERT_EQ(result.pin, pin);
    ASSERT_EQ(result.priority, priority);
    ASSERT_TRUE(result.active);
    ASSERT_GT(result.id, 0);

    // Limpiar
    remove("test_devices.json");
}

// Test de valores extremos
TEST_CASE(DeviceManager_HandlesExtremeValues)
{
    DeviceManager deviceManager;
    Dispositivo testDevice;

    std::ofstream file("test_devices.json");
    file << "[]";
    file.close();

    Dispositivo result = deviceManager.setValues(
        testDevice, "Extreme", "Test", 999999.99, -999999.99, 255, 5, false);

    ASSERT_EQ(result.maxValue, 999999.99);
    ASSERT_EQ(result.minValue, -999999.99);
    ASSERT_EQ(result.pin, 255);
    ASSERT_EQ(result.priority, 5);
    ASSERT_FALSE(result.active);

    remove("test_devices.json");
}

// Test de múltiples dispositivos
TEST_CASE(DeviceManager_MultipleDevices_UniqueIds)
{
    DeviceManager deviceManager;
    Dispositivo device1, device2;

    std::ofstream file("test_devices.json");
    file << "[]";
    file.close();

    device1 = deviceManager.setValues(device1, "Device1", "Type1", 100.0, 0.0, 1, 1, true);
    device2 = deviceManager.setValues(device2, "Device2", "Type2", 200.0, 0.0, 2, 2, false);

    ASSERT_NE(device1.id, device2.id);
    ASSERT_GT(device2.id, device1.id);

    remove("test_devices.json");
}

// Test de strings vacíos
TEST_CASE(DeviceManager_EmptyStrings_HandledCorrectly)
{
    DeviceManager deviceManager;
    Dispositivo testDevice;

    std::ofstream file("test_devices.json");
    file << "[]";
    file.close();

    Dispositivo result = deviceManager.setValues(testDevice, "", "", 0.0, 0.0, 0, 0, false);

    ASSERT_TRUE(result.name.empty());
    ASSERT_TRUE(result.type.empty());
    ASSERT_GT(result.id, 0);

    remove("test_devices.json");
}
