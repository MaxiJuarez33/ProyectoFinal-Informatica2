#include <gtest/gtest.h>

// Archivo principal para ejecutar todos los tests
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    // Configurar comportamiento global de tests
    ::testing::FLAGS_gtest_print_time = true;
    ::testing::FLAGS_gtest_color = "yes";

    std::cout << "🧪 Ejecutando tests del Proyecto Final - Informática 2\n";
    std::cout << "=====================================================\n\n";

    int result = RUN_ALL_TESTS();

    if (result == 0)
    {
        std::cout << "\n✅ Todos los tests pasaron exitosamente!\n";
    }
    else
    {
        std::cout << "\n❌ Algunos tests fallaron. Revisa los resultados arriba.\n";
    }

    return result;
}
