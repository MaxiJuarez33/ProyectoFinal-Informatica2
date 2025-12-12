#include <iostream>
#include "tankManager.h"
#include "serialReader.h"

// Declarar la variable externa definida en tankManager.cpp
extern SaveDataVars dataStruct;

int main()
{
    std::cout << "🧪 Test de TankManager Funcional\n";
    std::cout << "===============================\n";

    int testsPassed = 0;
    int totalTests = 0;

    // Test 1: Tanque blanco con nivel normal
    std::cout << "\n1. Test: Tanque blanco - Nivel normal" << std::endl;
    totalTests++;
    dataStruct.capData[0] = 20.0; // Nivel que debería ser normal
    dataStruct.capData[1] = 10.0; // Gris normal
    dataStruct.capData[2] = 10.0; // Negro normal

    WaterTank tank1;
    if (!tank1.isWhiteCritical() && !tank1.isGrayCritical() && !tank1.isBlackCritical())
    {
        std::cout << "✅ PASS: Niveles normales detectados correctamente" << std::endl;
        testsPassed++;
    }
    else
    {
        std::cout << "❌ FAIL: Niveles normales no detectados correctamente" << std::endl;
    }

    // Test 2: Tanque blanco crítico
    std::cout << "\n2. Test: Tanque blanco - Nivel crítico" << std::endl;
    totalTests++;
    dataStruct.capData[0] = 70.0; // Nivel crítico (debería ser bajo)

    WaterTank tank2;
    if (tank2.isWhiteCritical())
    {
        std::cout << "✅ PASS: Nivel crítico del tanque blanco detectado" << std::endl;
        testsPassed++;
    }
    else
    {
        std::cout << "❌ FAIL: Nivel crítico del tanque blanco no detectado" << std::endl;
    }

    // Test 3: Mostrar estado de tanques
    std::cout << "\n3. Test: Mostrar estado de tanques" << std::endl;
    totalTests++;
    try
    {
        std::cout << "Estado actual de los tanques:" << std::endl;
        tank2.tankStatus();
        std::cout << "✅ PASS: tankStatus() ejecutado sin errores" << std::endl;
        testsPassed++;
    }
    catch (const std::exception &e)
    {
        std::cout << "❌ FAIL: Error en tankStatus(): " << e.what() << std::endl;
    }

    // Resultados finales
    std::cout << "\n===============================" << std::endl;
    std::cout << "📊 RESULTADOS DE TANKMANAGER:" << std::endl;
    std::cout << "   Total: " << totalTests << " tests" << std::endl;
    std::cout << "   ✅ Pasaron: " << testsPassed << std::endl;
    std::cout << "   ❌ Fallaron: " << (totalTests - testsPassed) << std::endl;

    if (testsPassed == totalTests)
    {
        std::cout << "\n🎉 ¡TODOS LOS TESTS DE TANKMANAGER PASARON!" << std::endl;
        std::cout << "El sistema de gestión de tanques funciona correctamente.\n"
                  << std::endl;
    }
    else
    {
        std::cout << "\n⚠️  Algunos tests fallaron. Revisa la implementación de TankManager.\n"
                  << std::endl;
    }

    return 0;
}
