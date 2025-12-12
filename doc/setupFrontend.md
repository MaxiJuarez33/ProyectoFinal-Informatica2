# 🌐 Setup Completo del Frontend Web con Astro

## 🎯 ¿Por qué Astro es PERFECTO para tu proyecto IoT?

### ✅ Ventajas Clave:

- **⚡ Ultra rápido**: Ideal para conexión limitada en RV
- **🔋 Bajo consumo**: Menos JavaScript = menos batería
- **📱 Responsive**: Dashboard perfecto para móviles y tablets
- **🛠️ Flexible**: Puedes usar React, Vue o vanilla JS cuando lo necesites
- **🎨 Fácil**: Menos configuración que React puro

### ❌ Por qué NO usar Electron:

- 🐌 Muy pesado (Chrome completo incluido)
- 🔋 Alto consumo de memoria y batería
- 🌐 Tu proyecto ya tiene web backend, no necesita app de escritorio
- 📱 Menos responsive que web moderna

## 🚀 Instalación Paso a Paso

### 1. Preparar Node.js

```powershell
# Si no tienes Node.js:
winget install OpenJS.NodeJS
# O descargar desde: https://nodejs.org/
```

### 2. Crear el proyecto Astro

```powershell
# Ir a tu carpeta de proyecto
cd c:\Facultad\ProyectoFinal-Informatica2

# Crear frontend
npm create astro@latest web-frontend

# ⚠️ Cuando pregunte, seleccionar:
# - Template: "Empty"
# - TypeScript: "Yes" (recomendado para proyectos grandes)
# - Dependencies: "Yes"
# - Git: "No" (ya tienes git en proyecto padre)
```

### 3. Instalar dependencias del dashboard

```powershell
cd web-frontend

# UI y estilos
npm install @astrojs/tailwind tailwindcss
npm install lucide-astro  # Iconos modernos

# Gráficos y charts
npm install chart.js recharts

# HTTP client para API
npm install axios

# Componentes interactivos (opcional)
npm install @astrojs/react react react-dom
```

## 📁 Estructura Profesional del Proyecto

```
web-frontend/
├── src/
│   ├── components/
│   │   ├── ui/
│   │   │   ├── Card.astro
│   │   │   ├── Button.astro
│   │   │   └── Badge.astro
│   │   ├── dashboard/
│   │   │   ├── SystemOverview.astro
│   │   │   ├── DeviceGrid.astro
│   │   │   └── AlertsPanel.astro
│   │   ├── devices/
│   │   │   ├── DeviceCard.astro
│   │   │   ├── DeviceControls.astro
│   │   │   └── DeviceStatus.astro
│   │   ├── tanks/
│   │   │   ├── TankGauge.astro
│   │   │   ├── TankChart.astro
│   │   │   └── WaterLevels.astro
│   │   └── sensors/
│   │       ├── SensorGrid.astro
│   │       ├── TemperatureChart.astro
│   │       └── CurrentChart.astro
│   ├── layouts/
│   │   ├── Layout.astro          # Layout principal
│   │   └── DashboardLayout.astro # Layout específico dashboard
│   ├── pages/
│   │   ├── index.astro           # 🏠 Dashboard principal
│   │   ├── devices/
│   │   │   ├── index.astro       # 🔌 Lista de dispositivos
│   │   │   └── [id].astro        # 🔍 Detalle de dispositivo
│   │   ├── tanks/
│   │   │   └── index.astro       # 🛢️ Monitoreo de tanques
│   │   ├── sensors/
│   │   │   └── index.astro       # 📊 Dashboard de sensores
│   │   └── api/                  # 🔄 Proxy endpoints (opcional)
│   │       └── [...path].ts
│   ├── scripts/
│   │   ├── api.ts               # Cliente API para C++ backend
│   │   ├── realtime.ts          # WebSocket client (futuro)
│   │   └── utils.ts             # Utilidades
│   └── styles/
│       ├── global.css           # Estilos globales
│       └── components.css       # Estilos componentes
├── public/
│   ├── favicon.svg
│   └── icons/                   # Iconos del sistema
└── package.json
```

## ⚙️ Configuración Profesional

### `astro.config.mjs`

```javascript
import { defineConfig } from "astro/config";
import tailwind from "@astrojs/tailwind";
import react from "@astrojs/react";

export default defineConfig({
  integrations: [tailwind(), react()],

  // Servidor de desarrollo
  server: {
    port: 3000,
    host: true, // Acceso desde otros dispositivos en red local
  },

  // Proxy para conectar con tu backend C++
  vite: {
    server: {
      proxy: {
        "/api": {
          target: "http://localhost:8080",
          changeOrigin: true,
          secure: false,
        },
      },
    },
  },

  // Optimizaciones para producción
  build: {
    inlineStylesheets: "auto",
  },
});
```

### `tailwind.config.cjs`

```javascript
/** @type {import('tailwindcss').Config} */
module.exports = {
  content: ["./src/**/*.{astro,html,js,jsx,md,mdx,svelte,ts,tsx,vue}"],
  theme: {
    extend: {
      colors: {
        // Colores del sistema IoT
        primary: "#3B82F6", // Azul
        secondary: "#1F2937", // Gris oscuro
        success: "#10B981", // Verde
        warning: "#F59E0B", // Amarillo
        danger: "#EF4444", // Rojo
        info: "#06B6D4", // Cyan
        // Estados de dispositivos
        "device-on": "#10B981",
        "device-off": "#6B7280",
        "tank-critical": "#EF4444",
        "tank-normal": "#10B981",
      },
      fontFamily: {
        sans: ["Inter", "system-ui", "sans-serif"],
      },
      animation: {
        "pulse-slow": "pulse 3s ease-in-out infinite",
        "bounce-subtle": "bounce 2s ease-in-out infinite",
      },
    },
  },
  plugins: [],
};
```

## 🛠️ Workflow de Desarrollo

### Comandos Esenciales

```powershell
# Desarrollo frontend (puerto 3000)
cd web-frontend
npm run dev

# Build para producción
npm run build

# Preview del build
npm run preview

# Linting y formato
npm run lint
npm run format
```

### Desarrollo Full-Stack

```powershell
# Terminal 1: Backend C++
cd c:\Facultad\ProyectoFinal-Informatica2
g++ -I inc -I lib -o webServer.exe src/webServer.cpp src/*.cpp inc/*.cpp lib/*.cpp
./webServer.exe

# Terminal 2: Frontend Astro
cd web-frontend
npm run dev

# 🎉 Resultado:
# Backend API:  http://localhost:8080/api/*
# Frontend:     http://localhost:3000
# Dashboard:    http://localhost:3000 (conecta automáticamente)
```

## 🔗 Integración Backend ↔ Frontend

### Cliente API TypeScript (`src/scripts/api.ts`)

```typescript
interface SystemStatus {
  devices: { active: number; total: number };
  tanks: { critical: boolean; levels: number[] };
  sensors: { temperature: number[]; current: number[] };
}

class IoTApi {
  private baseUrl = "/api"; // Proxy a localhost:8080

  async getSystemStatus(): Promise<SystemStatus> {
    const response = await fetch(`${this.baseUrl}/status`);
    const data = await response.json();
    return data.data;
  }

  async toggleDevice(deviceId: number): Promise<boolean> {
    const response = await fetch(`${this.baseUrl}/devices/${deviceId}/toggle`, {
      method: "POST",
    });
    const data = await response.json();
    return data.success;
  }

  async getTanksStatus() {
    const response = await fetch(`${this.baseUrl}/tanks`);
    return response.json();
  }
}

export const api = new IoTApi();
```

## 📱 Acceso y URLs

- **🎨 Frontend Dashboard**: http://localhost:3000
- **🔌 Backend API**: http://localhost:8080/api/\*
- **📊 Sistemas integrados**: Frontend conecta automáticamente a backend
- **📱 Móvil**: Accesible desde cualquier dispositivo en la red local

## 🎯 Próximos Pasos

1. **Crear proyecto**: `npm create astro@latest web-frontend`
2. **Instalar dependencias**: Seguir comandos de arriba
3. **Configurar**: Copiar archivos de configuración
4. **Desarrollar**: Crear componentes del dashboard
5. **Integrar**: Conectar con tu API C++ existente
6. **Desplegar**: Build final para producción

## 🏆 Resultado Final

Un dashboard web profesional que:

- ⚡ Carga instantáneamente
- 📱 Funciona en cualquier dispositivo
- 🔄 Se conecta a tu backend C++ en tiempo real
- 🎨 Se ve moderno y profesional
- 🔋 Consume mínima batería
- 📡 Funciona offline después de la primera carga
