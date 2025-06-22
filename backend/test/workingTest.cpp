#include <iostream>
#include "deviceManager.h"

int main()
{
    std::cout << "🧪 Test de DeviceManager Funcional\n";
    std::cout << "==================================\n";

    DeviceManager deviceManager;
    Dispositivo dispositivo;

    // Test 1: Crear dispositivo básico
    std::cout << "\n1. Creando dispositivo de prueba..." << std::endl;
    dispositivo = deviceManager.setValues(dispositivo, "Sensor de Temperatura", "Analogico", 100.0, -40.0, 36, 3, true);

    if (dispositivo.id > 0)
    {
        std::cout << "✅ Dispositivo creado exitosamente!" << std::endl;
        std::cout << "   ID: " << dispositivo.id << std::endl;
        std::cout << "   Nombre: " << dispositivo.name << std::endl;
        std::cout << "   Pin: " << dispositivo.pin << std::endl;
    }
    else
    {
        std::cout << "❌ Error: No se pudo crear el dispositivo" << std::endl;
        return 1;
    }

    // Test 2: Guardar en archivo
    std::cout << "\n2. Guardando dispositivo en archivo..." << std::endl;
    try
    {
        deviceManager.fileManage(dispositivo);
        std::cout << "✅ Dispositivo guardado correctamente!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "❌ Error al guardar: " << e.what() << std::endl;
        return 1;
    }

    // Test 3: Buscar dispositivo
    std::cout << "\n3. Buscando dispositivo por nombre..." << std::endl;
    Dispositivo encontrado = deviceManager.getDevice("Temperatura", -1);

    if (encontrado.id > 0)
    {
        std::cout << "✅ Dispositivo encontrado!" << std::endl;
        std::cout << "   Nombre encontrado: " << encontrado.name << std::endl;
    }
    else
    {
        std::cout << "❌ No se pudo encontrar el dispositivo" << std::endl;
        return 1;
    }

    // Test 4: Crear segundo dispositivo
    std::cout << "\n4. Creando segundo dispositivo..." << std::endl;
    Dispositivo dispositivo2;
    dispositivo2 = deviceManager.setValues(dispositivo2, "Bomba de Agua", "Relay", 12.0, 0.0, 25, 5, false);

    if (dispositivo2.id > dispositivo.id)
    {
        std::cout << "✅ Segundo dispositivo creado con ID único!" << std::endl;
        std::cout << "   ID anterior: " << dispositivo.id << ", ID nuevo: " << dispositivo2.id << std::endl;
    }
    else
    {
        std::cout << "❌ Error: IDs no son únicos" << std::endl;
        return 1;
    }

    std::cout << "\n🎉 ¡TODOS LOS TESTS PASARON EXITOSAMENTE!" << std::endl;
    std::cout << "DeviceManager está funcionando correctamente.\n"
              << std::endl;

    return 0;
}
