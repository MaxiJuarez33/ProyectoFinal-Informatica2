Para ejecutar los tests se tiene que crear los .exe con comandos desde la carpeta raiz del proyecto.
Para poner el comando fijarse de estar en la posicion de la carpeta raiz, si hicieron todo bien deberia ser ProyectoFinal-Informatica2
Si estan en una carpeta interna no va a funcionar el comando

## Tests implementados al dia 11/6

### Test individual de DeviceManager

g++ -Iinc -Ilib -o test/workingTest test/workingTest.cpp inc/deviceManager.cpp

### Test individual de TankManager

g++ -Iinc -Ilib -o test/tankTestWorking test/tankTestWorking.cpp inc/tankManager.cpp inc/serialReader.cpp

### Test de JSON/persistencia (original corregido)

g++ -Iinc -Ilib -o testJsonOriginal test/jsonTest.cpp inc/deviceManager.cpp

### Test master (ejecuta todos los tests)

g++ -Iinc -Ilib -o test/masterTestRunner test/masterTestRunner_copy.cpp

## Como ejecutar

### Metodo 1: Script automatizado (recomendado)

#### PowerShell (recomendado)

Desde la carpeta raiz ejecutar:
.\test\runTests.ps1

#### Batch/CMD (alternativa)

Desde la carpeta raiz ejecutar:
.\test\runTests.bat

Los scripts permiten:

- Ejecutar todos los tests automaticamente
- Ejecutar tests individuales
- Compilar todos sin ejecutar
- Limpiar archivos ejecutables (solo PowerShell)
- Menu interactivo para seleccionar opciones

### Metodo 2: Manual

Despues de compilar, ejecutar desde la carpeta raiz:

### Ejecutar test individual de DeviceManager

.\test\workingTest.exe

### Ejecutar test individual de TankManager

.\test\tankTestWorking.exe

### Ejecutar test de JSON

.\testJsonOriginal.exe

### Ejecutar TODOS los tests (recomendado)

.\test\masterTestRunner.exe

## Que hace cada test

### workingTest.cpp - DeviceManager

- Crear dispositivo basico con todos los parametros
- Guardar dispositivo en archivo JSON
- Buscar dispositivo por nombre/palabra clave
- Crear segundo dispositivo con ID unico
- Verificar que los IDs se incrementen automaticamente

### tankTestWorking.cpp - TankManager

- Simular datos de sensores para testing
- Verificar deteccion de niveles normales en tanques
- Verificar deteccion de niveles criticos
- Ejecutar tankStatus() sin errores
- Mostrar calculos de volumenes de agua

### jsonTest.cpp - Persistencia

- Crear dispositivo con parametros extremos para testing
- Guardar en archivo devices.json
- Buscar dispositivo guardado
- Verificar que el archivo JSON se crea correctamente

### masterTestRunner.cpp - Ejecutor principal

- Ejecuta todos los tests automaticamente
- Muestra resumen consolidado de resultados
- Reporta estado general del sistema de testing
- Similar a "npm test" de JavaScript

## Funcionamiento del framework de testing

### Formato de salida

Los tests usan emojis y colores para mostrar resultados:

- ✅ PASS: Test exitoso
- ❌ FAIL: Test fallido
- 🧪 Indicador de inicio de tests
- 📊 Resumen de resultados

### Estructura de un test

Cada test sigue el patron:

1. Limpiar/preparar datos
2. Ejecutar funcionalidad
3. Verificar resultados
4. Reportar estado
5. Limpiar archivos temporales

### Para agregar nuevos tests

1. Copiar archivo que funciona: Copy-Item workingTest.cpp nuevoTest.cpp
2. Modificar contenido manteniendo estructura
3. Compilar con patron: g++ -Iinc -Ilib -o test/nuevoTest test/nuevoTest.cpp inc/modulo.cpp
4. Agregar al masterTestRunner si se desea

## TODO

- Test para ElectricManager con sistema de prioridades
- Test para SerialReader con mocks (complejo por dependencias Windows)
- Tests de integracion entre modulos
- Script automatizado (.bat o .ps1) para compilar y ejecutar
- Integracion con GitHub Actions para CI/CD

## Notas importantes

- Los tests crean archivos temporales (test_devices.json) que se limpian automaticamente
- El sistema detecta automaticamente si los modulos funcionan correctamente
- TankManager puede mostrar algunas fallas esperadas por configuracion de umbrales
- El framework es equivalente a Jest para JavaScript pero adaptado a C++

## Comparacion con Jest (JavaScript/TypeScript)

| Jest                  | Sistema C++                    |
| --------------------- | ------------------------------ |
| `npm test`            | `.\test\runTests.ps1`          |
| `npm test -- --watch` | Ejecutar script periodicamente |
| `describe()`          | Secciones con comentarios      |
| `it()`                | Funciones de test individuales |
| `expect().toBe()`     | `if (condition)` + reportes    |
| Test runner integrado | masterTestRunner.exe           |
| Reportes coloridos    | Emojis + colores de consola    |
| Mocks automaticos     | Variables extern + simulacion  |
| Coverage reports      | Cobertura manual documentada   |

## Filosofia del sistema

Al igual que Jest facilita el testing en JavaScript, este sistema:

- Hace que testear en C++ sea simple y visual
- Proporciona feedback inmediato sobre el estado del codigo
- Permite desarrollo dirigido por tests (TDD)
- Facilita la integracion continua y deteccion temprana de bugs
- Mantiene la calidad del codigo a medida que el proyecto crece
