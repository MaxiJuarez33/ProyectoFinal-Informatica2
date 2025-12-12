# Makefile para sistema de testing
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
INCLUDES = -Iinc -Ilib
TEST_DIR = test
BUILD_DIR = build
SRC_DIR = inc

# Google Test flags (asumiendo instalación local)
GTEST_FLAGS = -lgtest -lgtest_main -lgmock -lgmock_main -pthread

# Archivos fuente (sin main.cpp)
SOURCES = $(SRC_DIR)/deviceManager.cpp \
          $(SRC_DIR)/tankManager.cpp \
          $(SRC_DIR)/electricManager.cpp

# Archivos de test
TEST_SOURCES = $(TEST_DIR)/deviceManagerTest.cpp \
               $(TEST_DIR)/tankManagerTest.cpp \
               $(TEST_DIR)/electricManagerTest.cpp \
               $(TEST_DIR)/testRunner.cpp

# Ejecutable de tests
TEST_EXECUTABLE = $(BUILD_DIR)/run_tests

# Regla por defecto
all: $(TEST_EXECUTABLE)

# Crear directorio de build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compilar tests
$(TEST_EXECUTABLE): $(BUILD_DIR) $(SOURCES) $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_SOURCES) $(SOURCES) $(GTEST_FLAGS) -o $@

# Ejecutar todos los tests
test: $(TEST_EXECUTABLE)
	@echo "🧪 Ejecutando tests..."
	./$(TEST_EXECUTABLE)

# Ejecutar tests con verbose
test-verbose: $(TEST_EXECUTABLE)
	@echo "🧪 Ejecutando tests (verbose)..."
	./$(TEST_EXECUTABLE) --gtest_verbose

# Ejecutar solo tests de DeviceManager
test-device: $(TEST_EXECUTABLE)
	@echo "🔌 Ejecutando tests de DeviceManager..."
	./$(TEST_EXECUTABLE) --gtest_filter="DeviceManagerTest.*"

# Ejecutar solo tests de TankManager
test-tank: $(TEST_EXECUTABLE)
	@echo "🚰 Ejecutando tests de TankManager..."
	./$(TEST_EXECUTABLE) --gtest_filter="TankManagerTest.*"

# Ejecutar solo tests de ElectricManager
test-electric: $(TEST_EXECUTABLE)
	@echo "⚡ Ejecutando tests de ElectricManager..."
	./$(TEST_EXECUTABLE) --gtest_filter="ElectricManagerTest.*"

# Compilar proyecto principal
main: src/main.cpp $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) src/main.cpp $(SOURCES) -o $(BUILD_DIR)/ProyectoFinal

# Compilar tests individuales
test-json: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_DIR)/jsonTest.cpp $(SRC_DIR)/deviceManager.cpp -o $(BUILD_DIR)/jsonTest

test-serial: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_DIR)/serialReaderTest.cpp $(SRC_DIR)/serialReader.cpp -o $(BUILD_DIR)/serialTest

# Limpiar archivos compilados
clean:
	rm -rf $(BUILD_DIR)

# Limpiar y recompilar
rebuild: clean all

# Instalar Google Test (Ubuntu/Debian)
install-gtest:
	@echo "📦 Instalando Google Test..."
	sudo apt-get update
	sudo apt-get install libgtest-dev libgmock-dev
	sudo apt-get install cmake
	cd /usr/src/gtest && sudo cmake . && sudo make
	sudo cp *.a /usr/lib

# Ayuda
help:
	@echo "📚 Comandos disponibles:"
	@echo "  make all          - Compilar tests"
	@echo "  make test         - Ejecutar todos los tests"
	@echo "  make test-verbose - Ejecutar tests con salida detallada"
	@echo "  make test-device  - Ejecutar solo tests de DeviceManager"
	@echo "  make test-tank    - Ejecutar solo tests de TankManager"
	@echo "  make test-electric- Ejecutar solo tests de ElectricManager"
	@echo "  make main         - Compilar proyecto principal"
	@echo "  make clean        - Limpiar archivos compilados"
	@echo "  make rebuild      - Limpiar y recompilar"
	@echo "  make help         - Mostrar esta ayuda"

# Declarar targets que no son archivos
.PHONY: all test test-verbose test-device test-tank test-electric main clean rebuild install-gtest help
