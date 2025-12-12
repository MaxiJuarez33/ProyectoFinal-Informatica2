@echo off
REM Script batch para automatizar testing del Proyecto Final
REM Usar desde la carpeta raiz del proyecto

echo 🧪 SISTEMA DE TESTING AUTOMATIZADO - PROYECTO FINAL
echo =================================================
echo.

REM Verificar que estamos en la carpeta correcta
if not exist "inc\deviceManager.h" (
    echo ❌ ERROR: Este script debe ejecutarse desde la carpeta raiz del proyecto
    echo    Asegurate de estar en ProyectoFinal-Informatica2\
    pause
    exit /b 1
)

echo Selecciona que tests ejecutar:
echo 1. Ejecutar TODOS los tests ^(Master Runner^)
echo 2. Test individual de DeviceManager
echo 3. Test individual de TankManager
echo 4. Test de JSON/Persistencia
echo 5. Compilar todos los tests
echo.

set /p selection="Ingresa tu opcion (1-5): "

if "%selection%"=="1" goto master
if "%selection%"=="2" goto device
if "%selection%"=="3" goto tank
if "%selection%"=="4" goto json
if "%selection%"=="5" goto compile
goto invalid

:master
echo 🎯 Ejecutando TODOS los tests...
echo.
echo 🔨 Compilando master runner...
g++ -Iinc -Ilib -o test\masterTestRunner.exe test\masterTestRunner_copy.cpp
if %errorlevel% neq 0 (
    echo ❌ Error en compilacion
    goto end
)
echo ✅ Compilacion exitosa
echo 🚀 Ejecutando todos los tests...
echo ==================================================
test\masterTestRunner.exe
echo ==================================================
echo 🎉 Tests completados
goto end

:device
echo 🔌 Test individual de DeviceManager...
echo.
echo 🔨 Compilando...
g++ -Iinc -Ilib -o test\workingTest.exe test\workingTest.cpp inc\deviceManager.cpp
if %errorlevel% neq 0 (
    echo ❌ Error en compilacion
    goto end
)
echo ✅ Compilacion exitosa
echo 🚀 Ejecutando DeviceManager test...
echo ==================================================
test\workingTest.exe
echo ==================================================
goto end

:tank
echo 🚰 Test individual de TankManager...
echo.
echo 🔨 Compilando...
g++ -Iinc -Ilib -o test\tankTestWorking.exe test\tankTestWorking.cpp inc\tankManager.cpp inc\serialReader.cpp
if %errorlevel% neq 0 (
    echo ❌ Error en compilacion
    goto end
)
echo ✅ Compilacion exitosa
echo 🚀 Ejecutando TankManager test...
echo ==================================================
test\tankTestWorking.exe
echo ==================================================
goto end

:json
echo 💾 Test de JSON/Persistencia...
echo.
echo 🔨 Compilando...
g++ -Iinc -Ilib -o testJsonOriginal.exe test\jsonTest.cpp inc\deviceManager.cpp
if %errorlevel% neq 0 (
    echo ❌ Error en compilacion
    goto end
)
echo ✅ Compilacion exitosa
echo 🚀 Ejecutando JSON test...
echo ==================================================
testJsonOriginal.exe
echo ==================================================
goto end

:compile
echo 🔨 Compilando todos los tests...
echo.
echo Compilando DeviceManager test...
g++ -Iinc -Ilib -o test\workingTest.exe test\workingTest.cpp inc\deviceManager.cpp
if %errorlevel% equ 0 echo ✅ DeviceManager test compilado

echo Compilando TankManager test...
g++ -Iinc -Ilib -o test\tankTestWorking.exe test\tankTestWorking.cpp inc\tankManager.cpp inc\serialReader.cpp
if %errorlevel% equ 0 echo ✅ TankManager test compilado

echo Compilando JSON test...
g++ -Iinc -Ilib -o testJsonOriginal.exe test\jsonTest.cpp inc\deviceManager.cpp
if %errorlevel% equ 0 echo ✅ JSON test compilado

echo Compilando Master Runner...
g++ -Iinc -Ilib -o test\masterTestRunner.exe test\masterTestRunner_copy.cpp
if %errorlevel% equ 0 echo ✅ Master Runner compilado

echo.
echo 📊 Todos los tests han sido compilados
echo Para ejecutar usar: .\test\masterTestRunner.exe
goto end

:invalid
echo ❌ Opcion invalida

:end
echo.
echo 🧪 Script de testing finalizado
echo 📚 Para mas informacion ver: doc\comoEjecutarTests.md
pause
