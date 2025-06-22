#include <iostream>
#include <fstream>
#include <cassert>
#include "deviceManager.h"

// Contador de tests
int totalTests = 0;
int passedTests = 0;
int failedTests = 0;

// Función para ejecutar un test
void runTest(const std::string &testName, bool (*testFunc)())
{
    std::cout << "[ RUN      ] " << testName;
    totalTests++;

    try
    {
        if (testFunc())
        {
            std::cout << " [✅ PASS]" << std::endl;
            passedTests++;
        }
        else
        {
            std::cout << " [❌ FAIL]" << std::endl;
            failedTests++;
        }
    }
    catch (const std::exception &e)
    {
        std::cout << " [❌ ERROR: " << e.what() << "]" << std::endl;
        failedTests++;
    }
}

// Test 1: Creación básica de dispositivo
bool test_DeviceCreation()
{
    DeviceManager deviceManager;
    Dispositivo testDevice;

    // Limpiar archivo
    std::ofstream file("test_devices.json");
    file << "[]";
    file.close();

    // Crear dispositivo
    Dispositivo result = deviceManager.setValues(
        testDevice, "Test Sensor", "Temperatura", 100.0, -40.0, 36, 3, true);

    // Verificar
    bool success = (result.name == "Test Sensor" &&
                    result.type == "Temperatura" &&
                    result.maxValue == 100.0 &&
                    result.minValue == -40.0 &&
                    result.pin == 36 &&
                    result.priority == 3 &&
                    result.active == true &&
                    result.id > 0);

    // Limpiar
    remove("test_devices.json");
    return success;
}

// Test 2: Múltiples dispositivos
bool test_MultipleDevices()
{
    DeviceManager deviceManager;
    Dispositivo device1, device2;

    std::ofstream file("test_devices.json");
    file << "[]";
    file.close();

    device1 = deviceManager.setValues(device1, "Device1", "Type1", 100.0, 0.0, 1, 1, true);
    device2 = deviceManager.setValues(device2, "Device2", "Type2", 200.0, 0.0, 2, 2, false);

    bool success = (device1.id != device2.id && device2.id > device1.id);

    remove("test_devices.json");
    return success;
}

// Test 3: Valores extremos
bool test_ExtremeValues()
{
    DeviceManager deviceManager;
    Dispositivo testDevice;

    std::ofstream file("test_devices.json");
    file << "[]";
    file.close();

    Dispositivo result = deviceManager.setValues(
        testDevice, "Extreme", "Test", 999999.99, -999999.99, 255, 5, false);

    bool success = (result.maxValue == 999999.99 &&
                    result.minValue == -999999.99 &&
                    result.pin == 255 &&
                    result.priority == 5 &&
                    result.active == false);

    remove("test_devices.json");
    return success;
}

// Test 4: Strings vacíos
bool test_EmptyStrings()
{
    DeviceManager deviceManager;
    Dispositivo testDevice;

    std::ofstream file("test_devices.json");
    file << "[]";
    file.close();

    Dispositivo result = deviceManager.setValues(testDevice, "", "", 0.0, 0.0, 0, 0, false);

    bool success = (result.name.empty() && result.type.empty() && result.id > 0);

    remove("test_devices.json");
    return success;
}

// Test 5: Guardar y recuperar dispositivo
bool test_SaveAndRetrieve()
{
    DeviceManager deviceManager;
    Dispositivo testDevice;

    // Limpiar archivo
    std::ofstream file("test_devices.json");
    file << "[]";
    file.close();

    // Crear y guardar dispositivo
    testDevice = deviceManager.setValues(testDevice, "Temperature Sensor", "Analog", 125.0, -50.0, 39, 4, true);
    deviceManager.fileManage(testDevice);

    // Intentar recuperar por nombre
    Dispositivo retrieved = deviceManager.getDevice("Temperature Sensor", -1);

    bool success = (retrieved.name == "Temperature Sensor" &&
                    retrieved.type == "Analog" &&
                    retrieved.id > 0);

    remove("test_devices.json");
    return success;
}

int main()
{
    std::cout << "🧪 Sistema de Testing Funcional - DeviceManager\n";
    std::cout << "===============================================\n\n";

    // Ejecutar todos los tests
    runTest("DeviceCreation", test_DeviceCreation);
    runTest("MultipleDevices", test_MultipleDevices);
    runTest("ExtremeValues", test_ExtremeValues);
    runTest("EmptyStrings", test_EmptyStrings);
    runTest("SaveAndRetrieve", test_SaveAndRetrieve);

    // Resultados finales
    std::cout << "\n===============================================\n";
    std::cout << "📊 RESULTADOS FINALES:\n";
    std::cout << "   Total: " << totalTests << " tests\n";
    std::cout << "   ✅ Pasaron: " << passedTests << "\n";
    std::cout << "   ❌ Fallaron: " << failedTests << "\n";

    if (failedTests == 0)
    {
        std::cout << "\n🎉 ¡TODOS LOS TESTS PASARON EXITOSAMENTE!\n";
        std::cout << "Tu DeviceManager está funcionando correctamente.\n";
    }
    else
    {
        std::cout << "\n⚠️  Algunos tests fallaron. Revisa la implementación.\n";
    }

    return failedTests;
}
