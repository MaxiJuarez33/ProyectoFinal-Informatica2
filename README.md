# 🏠 ProyectoFinal-Informatica2 - Sistema IoT para RV

Sistema integral de monitoreo y control para motorhomes/vehículos recreativos, desarrollado en C++ con interfaz web moderna.

## 🚀 Características del Sistema

- **🔌 Control de Dispositivos**: Gestión inteligente de dispositivos eléctricos
- **💧 Monitoreo de Tanques**: Control de niveles de agua potable, gris y negra
- **🌡️ Sensores Ambientales**: Temperatura interior/exterior y consumo eléctrico
- **⚡ Gestión Eléctrica**: Monitoreo de batería y consumo energético
- **📡 Comunicación ESP32**: Interfaz con microcontrolador para sensores
- **🌐 API REST**: Backend web con endpoints RESTful
- **📱 Dashboard Web**: Interfaz web responsive y moderna
- **🧪 Testing Automatizado**: Sistema completo de tests unitarios

## 🛠️ Tecnologías Utilizadas

- **Backend**: C++17, Crow Framework, nlohmann/json
- **Frontend**: Astro/React, Tailwind CSS, Chart.js
- **Hardware**: ESP32, PlatformIO
- **Testing**: Framework personalizado tipo Jest para C++

## ⚡ Inicio Rápido

### 1. Sistema Principal (Backend C++)

```bash
# Compilar proyecto principal
g++ -I inc -I lib -std=c++17 -o main.exe src/main.cpp inc/*.cpp

# Ejecutar sistema
./main.exe
```

### 2. Servidor Web con API REST

```bash
# Compilar servidor web
g++ -I inc -I lib -std=c++17 -o webServer.exe src/webServer.cpp inc/*.cpp

# Ejecutar servidor en http://localhost:8080
./webServer.exe
```

### 3. 🌐 Interfaz Web (Recomendado: Astro)

```bash
# Crear frontend moderno
npm create astro@latest web-frontend
cd web-frontend && npm install @astrojs/tailwind axios chart.js

# Desarrollo en http://localhost:3000
npm run dev
```

**📖 Guía completa de frontend**: [`doc/setupFrontend.md`](doc/setupFrontend.md)

## 🧪 Sistema de Testing

**Nuevo sistema de testing automatizado implementado:**

- ✅ Tests unitarios para todos los managers
- 🔄 Scripts automatizados (PowerShell y Batch)
- 📊 Reportes detallados con emojis
- 🎯 Testing de persistencia JSON

### Ejecutar Tests

```bash
# Script PowerShell (recomendado)
./test/runTests.ps1

# Script Batch alternativo
./test/runTests.bat

# Manual
cd test && make && ./masterTestRunner.exe
```

**📖 Documentación completa**: [`doc/comoEjecutarTests.md`](doc/comoEjecutarTests.md)

## 🌐 API REST Endpoints

El servidor web expone una API completa:

- `GET /api/status` - Estado general del sistema
- `GET /api/devices` - Lista de dispositivos
- `GET /api/tanks` - Estado de tanques
- `GET /api/sensors` - Datos de sensores

**Ejemplo de respuesta:**

```json
{
  "success": true,
  "data": {
    "devices": { "active": 3, "total": 4 },
    "tanks": {
      "white": { "level": 75, "critical": false },
      "black": { "level": 30, "critical": true }
    },
    "sensors": {
      "temperature": { "interior": 22.5 }
    }
  }
}
```

## 📁 Estructura del Proyecto

### Directorios Principales

- `src/` - Código fuente principal
- `inc/` - Headers y implementaciones de clases
- `test/` - Sistema de testing automatizado (similar a Jest para C++)
- `esp32/` - Código para microcontrolador ESP32
- `doc/` - Documentación del proyecto
- `lib/` - Librerías externas (Crow, JSON)
- `bin/` - Archivos .exe o ejecutables
- `obj/` - Archivos de pre-creación de ejecutables (no tocar)

### Componentes Principales

1. **DeviceManager** - Gestión de dispositivos del sistema
2. **TankManager** - Control de tanques de agua
3. **ElectricManager** - Gestión del sistema eléctrico
4. **SerialReader** - Comunicación con ESP32

## 📖 Documentación

- [`doc/comoEjecutar.md`](doc/comoEjecutar.md) - Instrucciones de compilación y ejecución
- [`doc/comoEjecutarTests.md`](doc/comoEjecutarTests.md) - Guía del sistema de testing
- [`doc/setupFrontend.md`](doc/setupFrontend.md) - Crear interfaz web moderna
- [`doc/deviceManager.md`](doc/deviceManager.md) - Documentación del gestor de dispositivos
- [`doc/serialReader.md`](doc/serialReader.md) - Comunicación serie

---

## 🔧 Comandos Git para el Proyecto

### Comandos Frecuentes

- `git pull` - Actualiza la versión según último commit
- `git branch nombreDeRama` - Crear rama de trabajo
- `git branch -l` - Lista de ramas existentes
- `git checkout <Nombre de la rama>` - Cambiar de rama
- `git status` - Ver estado de archivos del repo
- `git fetch` - Ver últimos cambios de la rama
- `git add <Archivo/carpeta o .>` - Añadir cambios a cola de subida
- `git commit -m "TEXTO"` - Descripción de cambios
- `git push origin <Rama>` - Subir archivos a rama

### Workflow de Trabajo

#### Revisar Updates

```bash
git fetch
git pull origin rama  # si hay cambios
```

#### Subir Updates

```bash
git add .
git commit -m "mensaje breve sobre la update"
git push origin <nombre-de-la-rama>
```

## 📝 Uso de Issues en GitHub

- **Issues** permite hacer seguimiento de tareas, discutir ideas y reportar bugs
- Crear Issues dinámicos y concisos para dudas (0 WhatsApp)
- Al comentar, verificar si el comentario cierra el Issue
- Todo en Issues será usado para informes

### Projects

- Usar Projects linkeados a Issues para mejor seguimiento
- Cambiar estado de tareas al terminarlas
- Solicitar nuevas tareas via Issues y @owner

## 🔄 Pull Requests

### Pull Request a Main

**Requisitos:**

- Haber testeado todo usando métodos planteados
- Optimizar al máximo el código
- Carpetas con nombres correctos según lo acordado

### Pull Request a Pre-Main

**Para revisión de código:**

- Carpetas con nombres correctos
- Variables, funciones, clases con formato acordado

---

## 🚀 Próximos Pasos Recomendados

1. **Implementar Frontend**: Usar Astro para crear dashboard web
2. **WebSocket**: Agregar actualizaciones en tiempo real
3. **Autenticación**: Acceso seguro al sistema
4. **PWA**: App instalable en móviles
5. **Notificaciones**: Alertas push para eventos críticos

## 📄 Archivos Importantes

- `Makefile` - Automatiza compilación y testing
- `.gitignore` - Selecciona archivos a no subir
- `README.md` - Este archivo con información principal

**🏠 Sistema IoT RV** - Monitoreo inteligente para tu hogar sobre ruedas
