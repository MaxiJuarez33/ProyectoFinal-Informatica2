#include "../inc/deviceManager.h"
#include <iostream>
#include <fstream>
#include <cassert>

// Framework de testing ultra simple
class TestFramework
{
private:
    static int totalTests;
    static int passedTests;
    static int failedTests;

public:
    static void runTest(const std::string &testName, void (*testFunc)())
    {
        std::cout << "[ RUN      ] " << testName << std::endl;
        totalTests++;

        try
        {
            testFunc();
            std::cout << "[       OK ] " << testName << std::endl;
            passedTests++;
        }
        catch (const std::exception &e)
        {
            std::cout << "[  FAILED  ] " << testName << " - " << e.what() << std::endl;
            failedTests++;
        }
        catch (...)
        {
            std::cout << "[  FAILED  ] " << testName << " - Error desconocido" << std::endl;
            failedTests++;
        }
    }

    static void printResults()
    {
        std::cout << "\n==========================================\n";
        std::cout << "Tests ejecutados: " << totalTests << std::endl;
        std::cout << "✅ Pasaron: " << passedTests << std::endl;
        std::cout << "❌ Fallaron: " << failedTests << std::endl;

        if (failedTests == 0)
        {
            std::cout << "\n🎉 ¡Todos los tests pasaron!\n";
        }
        else
        {
            std::cout << "\n⚠️  Algunos tests fallaron.\n";
        }
    }

    static int getFailedCount() { return failedTests; }
};

// Inicializar variables estáticas
int TestFramework::totalTests = 0;
int TestFramework::passedTests = 0;
int TestFramework::failedTests = 0;

// Macros simples
#define ASSERT_EQ(expected, actual)                                       \
    if ((expected) != (actual))                                           \
    {                                                                     \
        throw std::runtime_error("ASSERT_EQ failed: values don't match"); \
    }

#define ASSERT_TRUE(condition)                          \
    if (!(condition))                                   \
    {                                                   \
        throw std::runtime_error("ASSERT_TRUE failed"); \
    }

#define ASSERT_FALSE(condition)                          \
    if (condition)                                       \
    {                                                    \
        throw std::runtime_error("ASSERT_FALSE failed"); \
    }

#define ASSERT_GT(a, b)                               \
    if (!((a) > (b)))                                 \
    {                                                 \
        throw std::runtime_error("ASSERT_GT failed"); \
    }

#define ASSERT_NE(a, b)                               \
    if ((a) == (b))                                   \
    {                                                 \
        throw std::runtime_error("ASSERT_NE failed"); \
    }

// Tests para DeviceManager
void test_DeviceManager_SetValues_CreatesCorrectly()
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

void test_DeviceManager_HandlesExtremeValues()
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

void test_DeviceManager_MultipleDevices_UniqueIds()
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

void test_DeviceManager_EmptyStrings_HandledCorrectly()
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

int main()
{
    std::cout << "🧪 Ejecutando Tests Simples - Proyecto Final\n";
    std::cout << "==========================================\n\n";

    // Ejecutar todos los tests
    TestFramework::runTest("DeviceManager_SetValues_CreatesCorrectly", test_DeviceManager_SetValues_CreatesCorrectly);
    TestFramework::runTest("DeviceManager_HandlesExtremeValues", test_DeviceManager_HandlesExtremeValues);
    TestFramework::runTest("DeviceManager_MultipleDevices_UniqueIds", test_DeviceManager_MultipleDevices_UniqueIds);
    TestFramework::runTest("DeviceManager_EmptyStrings_HandledCorrectly", test_DeviceManager_EmptyStrings_HandledCorrectly);

    TestFramework::printResults();
    return TestFramework::getFailedCount();
}
