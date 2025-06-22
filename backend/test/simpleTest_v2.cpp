#include <iostream>
#include "deviceManager.h"

int main()
{
    std::cout << "🧪 Test Simple de DeviceManager\n";
    std::cout << "==============================\n";

    try
    {
        DeviceManager deviceManager;
        Dispositivo testDevice;

        // Test básico
        std::cout << "Creando dispositivo de prueba..." << std::endl;
        testDevice = deviceManager.setValues(testDevice, "Test Device", "Sensor", 100.0, 0.0, 36, 3, true);

        // Verificar que se creó correctamente
        if (testDevice.name == "Test Device" && testDevice.id > 0)
        {
            std::cout << "✅ Test PASÓ: Dispositivo creado correctamente" << std::endl;
            std::cout << "   - ID: " << testDevice.id << std::endl;
            std::cout << "   - Nombre: " << testDevice.name << std::endl;
            std::cout << "   - Tipo: " << testDevice.type << std::endl;
            std::cout << "   - Pin: " << testDevice.pin << std::endl;
            std::cout << "   - Prioridad: " << testDevice.priority << std::endl;
            std::cout << "   - Activo: " << (testDevice.active ? "Sí" : "No") << std::endl;
        }
        else
        {
            std::cout << "❌ Test FALLÓ: Dispositivo no se creó correctamente" << std::endl;
            return 1;
        }

        std::cout << "\n🎉 ¡Test completado exitosamente!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "❌ Error durante el test: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
