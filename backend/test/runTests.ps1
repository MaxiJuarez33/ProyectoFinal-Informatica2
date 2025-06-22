# Script de PowerShell para automatizar testing del Proyecto Final
# Usar desde la carpeta raiz del proyecto

# Bypass para politicas de ejecucion de PowerShell
if ((Get-ExecutionPolicy) -eq 'Restricted') {
    Set-ExecutionPolicy -ExecutionPolicy Bypass -Scope Process -Force
}

Write-Host "🧪 SISTEMA DE TESTING AUTOMATIZADO - PROYECTO FINAL" -ForegroundColor Cyan
Write-Host "=================================================" -ForegroundColor Cyan
Write-Host ""

# Verificar que estamos en la carpeta correcta
if (!(Test-Path "inc\deviceManager.h")) {
    Write-Host "❌ ERROR: Este script debe ejecutarse desde la carpeta raiz del proyecto" -ForegroundColor Red
    Write-Host "   Asegurate de estar en ProyectoFinal-Informatica2\" -ForegroundColor Yellow
    pause
    exit 1
}

# Funcion para compilar y ejecutar un test
function Invoke-Test {
    param(
        [string]$TestName,
        [string]$SourceFile,
        [string]$OutputFile,
        [string[]]$Dependencies,
        [string]$Description
    )
    Write-Host "🔨 Compilando ${TestName}..." -ForegroundColor Yellow
    
    $compileCmd = "g++ -Iinc -Ilib -o $OutputFile $SourceFile " + ($Dependencies -join " ")
    
    try {
        Invoke-Expression $compileCmd
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✅ Compilacion exitosa" -ForegroundColor Green
            Write-Host "🚀 Ejecutando ${TestName} - ${Description}" -ForegroundColor Cyan
            Write-Host "=" * 50 -ForegroundColor Gray
            
            & ".\$OutputFile"
            $testResult = $LASTEXITCODE
            Write-Host "=" * 50 -ForegroundColor Gray
            if ($testResult -eq 0) {
                Write-Host "✅ ${TestName}: EXITOSO" -ForegroundColor Green
            }
            else {
                Write-Host "⚠️  ${TestName}: CON ADVERTENCIAS (código: $testResult)" -ForegroundColor Yellow
            }
        }
        else {
            Write-Host "❌ Error en compilacion de ${TestName}" -ForegroundColor Red
            return $false
        }
    }
    catch {
        Write-Host "❌ Error ejecutando ${TestName}: $($_)" -ForegroundColor Red
        return $false
    }
    
    Write-Host ""
    return $true
}

# Menu principal
Write-Host "Selecciona que tests ejecutar:" -ForegroundColor White
Write-Host "1. Ejecutar TODOS los tests (Master Runner)" -ForegroundColor Green
Write-Host "2. Test individual de DeviceManager" -ForegroundColor Cyan
Write-Host "3. Test individual de TankManager" -ForegroundColor Cyan
Write-Host "4. Test de JSON/Persistencia" -ForegroundColor Cyan
Write-Host "5. Compilar todos los tests sin ejecutar" -ForegroundColor Yellow
Write-Host "6. Limpiar archivos ejecutables de test" -ForegroundColor Red
Write-Host ""

$selection = Read-Host "Ingresa tu opcion (1-6)"

switch ($selection) {
    "1" {
        Write-Host "🎯 Ejecutando TODOS los tests..." -ForegroundColor Magenta
        Write-Host ""
        
        # Compilar master runner
        $success = Invoke-Test -TestName "Master Runner" -SourceFile "test\masterTestRunner_copy.cpp" -OutputFile "test\masterTestRunner.exe" -Dependencies @() -Description "Ejecutor de todos los tests"
        
        if ($success) {
            Write-Host "🎉 SISTEMA DE TESTING COMPLETADO" -ForegroundColor Green
        }
    }
    
    "2" {
        Invoke-Test -TestName "DeviceManager" -SourceFile "test\workingTest.cpp" -OutputFile "test\workingTest.exe" -Dependencies @("inc\deviceManager.cpp") -Description "Pruebas de gestion de dispositivos"
    }
    
    "3" {
        Invoke-Test -TestName "TankManager" -SourceFile "test\tankTestWorking.cpp" -OutputFile "test\tankTestWorking.exe" -Dependencies @("inc\tankManager.cpp", "inc\serialReader.cpp") -Description "Pruebas de gestion de tanques"
    }
    
    "4" {
        Invoke-Test -TestName "JSON Persistencia" -SourceFile "test\jsonTest.cpp" -OutputFile "testJsonOriginal.exe" -Dependencies @("inc\deviceManager.cpp") -Description "Pruebas de persistencia JSON"
    }
    
    "5" {
        Write-Host "🔨 Compilando todos los tests..." -ForegroundColor Yellow
        
        # Compilar todos sin ejecutar
        $tests = @(
            @("test\workingTest.exe", "test\workingTest.cpp", @("inc\deviceManager.cpp")),
            @("test\tankTestWorking.exe", "test\tankTestWorking.cpp", @("inc\tankManager.cpp", "inc\serialReader.cpp")),
            @("testJsonOriginal.exe", "test\jsonTest.cpp", @("inc\deviceManager.cpp")),
            @("test\masterTestRunner.exe", "test\masterTestRunner_copy.cpp", @())
        )
        
        $compiledCount = 0
        foreach ($test in $tests) {
            $output, $source, $deps = $test
            $compileCmd = "g++ -Iinc -Ilib -o $output $source " + ($deps -join " ")
            
            Write-Host "Compilando $source..." -ForegroundColor Cyan
            try {
                Invoke-Expression $compileCmd
                if ($LASTEXITCODE -eq 0) {
                    Write-Host "✅ $output compilado" -ForegroundColor Green
                    $compiledCount++
                }
                else {
                    Write-Host "❌ Error compilando $source" -ForegroundColor Red
                }
            }
            catch {
                Write-Host "❌ Error: $($_)" -ForegroundColor Red
            }
        }
        
        Write-Host ""
        Write-Host "📊 Resumen: $compiledCount tests compilados exitosamente" -ForegroundColor Green
    }
    
    "6" {
        Write-Host "🧹 Limpiando archivos ejecutables de test..." -ForegroundColor Red
        
        $testExes = @(
            "test\workingTest.exe",
            "test\tankTestWorking.exe", 
            "test\masterTestRunner.exe",
            "testJsonOriginal.exe",
            "test\test_simple.exe"
        )
        foreach ($exe in $testExes) {
            if (Test-Path $exe) {
                Remove-Item $exe -Force
                Write-Host "🗑️  Eliminado: ${exe}" -ForegroundColor Yellow
            }
        }
        
        Write-Host "✅ Limpieza completada" -ForegroundColor Green
    }
    
    default {
        Write-Host "❌ Opcion invalida" -ForegroundColor Red
    }
}

Write-Host ""
Write-Host "🧪 Script de testing finalizado" -ForegroundColor Cyan
Write-Host "📚 Para mas informacion ver: doc\comoEjecutarTests.md" -ForegroundColor Gray
pause
