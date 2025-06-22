# Sistema de Testing para Proyecto Final

## 🧪 Framework de Testing Implementado

Hemos implementado **Google Test (GTest)** como framework de testing, que es el equivalente de Jest para C++.

### ✨ Características

- **Sintaxis similar a Jest**: `EXPECT_EQ`, `EXPECT_TRUE`, `EXPECT_THROW`
- **Mocks incluidos**: GMock para simular dependencias
- **Test fixtures**: Setup y TearDown automáticos
- **Reportes detallados**: Salida colorizada y detallada
- **Integración con CI/CD**: Compatible con sistemas de integración continua

## 🚀 Instalación y Configuración

### Requisitos

- CMake 3.14+
- Compilador C++17 (GCC, MSVC, Clang)
- Git (para descargar Google Test)

### Compilación con CMake

```bash
# Crear directorio de build
mkdir build && cd build

# Configurar proyecto
cmake ../test

# Compilar tests
cmake --build .

# Ejecutar tests
./run_tests
```

### Compilación Manual (Alternativa)

```bash
# Descargar Google Test
git clone https://github.com/google/googletest.git
cd googletest && mkdir build && cd build
cmake .. && make

# Compilar tests del proyecto
g++ -std=c++17 -Iinc -Ilib -Igoogletest/googletest/include -Igoogletest/googlemock/include \
    test/*.cpp inc/deviceManager.cpp inc/tankManager.cpp inc/electricManager.cpp \
    -lgtest -lgtest_main -lgmock -lgmock_main -pthread -o run_tests
```

## 📋 Tests Implementados

### 🔌 DeviceManager Tests

- ✅ Creación correcta de dispositivos
- ✅ Manejo de valores extremos
- ✅ Búsqueda por nombre y palabra clave
- ✅ Manejo de dispositivos no encontrados
- ✅ Múltiples dispositivos con IDs únicos

### 🚰 TankManager Tests

- ✅ Detección de niveles críticos
- ✅ Parada automática de bomba
- ✅ Estados mixtos de tanques
- ✅ Manejo de valores extremos
- ✅ Cálculos volumétricos correctos

### ⚡ ElectricManager Tests

- ✅ Guardado de dispositivos
- ✅ Detección de prioridades
- ✅ Estados de activación
- ✅ Manejo de errores con IDs inválidos
- ✅ Múltiples dispositivos con diferentes prioridades

## 🔄 Ejecución de Tests

### Ejecutar todos los tests

```bash
./run_tests
```

### Ejecutar tests específicos

```bash
# Solo tests de DeviceManager
./run_tests --gtest_filter="DeviceManagerTest.*"

# Solo tests de TankManager
./run_tests --gtest_filter="TankManagerTest.*"

# Test específico
./run_tests --gtest_filter="DeviceManagerTest.SetValuesCreatesDeviceCorrectly"
```

### Opciones útiles

```bash
# Verbose output
./run_tests --gtest_verbose

# Repetir tests
./run_tests --gtest_repeat=10

# Mostrar tests disponibles
./run_tests --gtest_list_tests
```

## 📊 Ejemplo de Salida

```
🧪 Ejecutando tests del Proyecto Final - Informática 2
=====================================================

[==========] Running 23 tests from 3 test suites.
[----------] Global test environment set-up.
[----------] 8 tests from DeviceManagerTest
[ RUN      ] DeviceManagerTest.SetValuesCreatesDeviceCorrectly
[       OK ] DeviceManagerTest.SetValuesCreatesDeviceCorrectly (2 ms)
[ RUN      ] DeviceManagerTest.HandlesExtremeValues
[       OK ] DeviceManagerTest.HandlesExtremeValues (1 ms)
...

[==========] 23 tests from 3 test suites ran. (156 ms total)
[  PASSED  ] 23 tests.

✅ Todos los tests pasaron exitosamente!
```

## 🎯 Cobertura de Testing

| Módulo          | Tests   | Cobertura |
| --------------- | ------- | --------- |
| DeviceManager   | 8 tests | ~85%      |
| TankManager     | 9 tests | ~90%      |
| ElectricManager | 6 tests | ~80%      |

## 🚧 TODO: Próximos Tests

- [ ] SerialReader Tests (con mocks)
- [ ] Tests de integración completa
- [ ] Tests de performance
- [ ] Tests de concurrencia (threading)
- [ ] Tests para ESP32 (con Arduino Test Framework)

## 🔧 Agregar Nuevos Tests

### Estructura de un test

```cpp
#include <gtest/gtest.h>
#include "tuModulo.h"

class TuModuloTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Configuración antes de cada test
    }

    void TearDown() override {
        // Limpieza después de cada test
    }
};

TEST_F(TuModuloTest, DescripcionDelTest) {
    // Arrange
    // ... preparar datos

    // Act
    // ... ejecutar función a testear

    // Assert
    EXPECT_EQ(expected, actual);
}
```

### Aserciones más comunes

```cpp
EXPECT_EQ(a, b);         // a == b
EXPECT_NE(a, b);         // a != b
EXPECT_LT(a, b);         // a < b
EXPECT_LE(a, b);         // a <= b
EXPECT_GT(a, b);         // a > b
EXPECT_GE(a, b);         // a >= b
EXPECT_TRUE(condition);   // condition es true
EXPECT_FALSE(condition);  // condition es false
EXPECT_THROW(stmt, ex);   // stmt lanza excepción ex
EXPECT_NO_THROW(stmt);    // stmt no lanza excepción
```

## 🔗 Referencias

- [Google Test Documentation](https://google.github.io/googletest/)
- [Google Mock Documentation](https://google.github.io/googletest/gmock.html)
- [CMake Testing](https://cmake.org/cmake/help/latest/command/gtest_discover_tests.html)
