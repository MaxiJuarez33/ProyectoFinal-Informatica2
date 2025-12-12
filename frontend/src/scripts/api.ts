// Cliente API para conectar con el backend C++
class IoTApi {
  private baseUrl = '/api'; // Usa el proxy configurado en Astro

  // Método genérico para hacer requests
  private async request(endpoint: string, options: RequestInit = {}): Promise<any> {
    try {
      const response = await fetch(`${this.baseUrl}${endpoint}`, {
        headers: {
          'Content-Type': 'application/json',
          ...options.headers,
        },
        ...options,
      });

      const data = await response.json();
      
      if (!response.ok) {
        throw new Error(data.error || `HTTP ${response.status}: ${response.statusText}`);
      }

      return data;
    } catch (error) {
      console.error(`API Error en ${endpoint}:`, error);
      throw error;
    }
  }

  // === ENDPOINTS DEL SISTEMA ===

  // Estado general del sistema (para dashboard principal)
  async getSystemStatus() {
    return this.request('/status');
  }

  // === DISPOSITIVOS ===

  // Obtener todos los dispositivos
  async getDevices() {
    return this.request('/devices');
  }

  // Obtener dispositivo específico
  async getDevice(deviceId: number) {
    return this.request(`/devices/${deviceId}`);
  }

  // Controlar dispositivo (toggle on/off)
  async toggleDevice(deviceId: number, action: string = 'toggle') {
    return this.request(`/devices/${deviceId}/toggle`, {
      method: 'POST',
      body: JSON.stringify({ action }),
    });
  }

  // === TANQUES ===

  // Obtener estado de todos los tanques
  async getTanks() {
    return this.request('/tanks');
  }

  // === SENSORES ===

  // Obtener datos de todos los sensores
  async getSensors() {
    return this.request('/sensors');
  }

  // === UTILIDADES ===

  // Verificar si el backend está disponible
  async checkConnection(): Promise<boolean> {
    try {
      await this.getSystemStatus();
      return true;
    } catch {
      return false;
    }
  }

  // Obtener timestamp actual del sistema
  async getTimestamp(): Promise<string> {
    const status = await this.getSystemStatus();
    return status.timestamp || new Date().toISOString();
  }
}

// Instancia singleton del API client
export const api = new IoTApi();

// === TIPOS TYPESCRIPT ===

export interface SystemStatus {
  success: boolean;
  data: {
    devices: {
      active: number;
      total: number;
      percentage: number;
    };
    tanks: {
      white: TankStatus;
      gray: TankStatus;
      black: TankStatus;
      pump_status: string;
    };
    sensors: {
      temperature: {
        interior: number;
        exterior: number;
      };
      current: {
        battery: number;
        climate: number;
        lights: number;
        fridge: number;
      };
    };
    electrical: {
      battery_voltage: number;
      battery_status: string;
      total_consumption: number;
    };
    alerts: {
      active_count: number;
      white_tank_low: boolean;
      gray_tank_full: boolean;
      black_tank_full: boolean;
      battery_low: boolean;
      pump_stopped: boolean;
    };
  };
  timestamp: string;
  system_status: string;
  uptime: string;
}

export interface TankStatus {
  level: number;
  critical: boolean;
}

export interface Device {
  id: number;
  name: string;
  type: string;
  active: boolean;
  value?: number;
  unit?: string;
  status: string;
  voltage?: number;
}

export interface Tank {
  id: string;
  name: string;
  level: number;
  capacity: number;
  critical: boolean;
  status: string;
  unit: string;
}

export interface SensorData {
  success: boolean;
  data: {
    temperature: {
      interior: SensorReading;
      exterior: SensorReading;
    };
    electrical: {
      battery: SensorReading;
      climate: SensorReading;
      lights: SensorReading;
      fridge: SensorReading;
    };
    water_levels: {
      white_tank: SensorReading;
      gray_tank: SensorReading;
      black_tank: SensorReading;
    };
  };
  timestamp: string;
  system_status: string;
}

export interface SensorReading {
  value: number;
  unit: string;
  status: string;
  timestamp: string;
  level_percent?: number;
}

// === FUNCIONES DE UTILIDAD ===

// Formatear valores numéricos
export function formatValue(value: number, decimals: number = 1): string {
  return value.toFixed(decimals);
}

// Obtener color basado en estado
export function getStatusColor(status: string): string {
  const colors = {
    normal: 'green',
    good: 'green',
    warning: 'yellow',
    low: 'orange',
    critical: 'red',
    offline: 'gray',
    running: 'blue',
    stopped: 'red'
  };
  return colors[status as keyof typeof colors] || 'gray';
}

// Formatear timestamp
export function formatTimestamp(timestamp: string): string {
  try {
    const date = new Date(timestamp);
    return date.toLocaleString('es-ES', {
      year: 'numeric',
      month: '2-digit',
      day: '2-digit',
      hour: '2-digit',
      minute: '2-digit',
      second: '2-digit'
    });
  } catch {
    return timestamp;
  }
}

// Calcular tiempo relativo (ej: "hace 5 minutos")
export function getRelativeTime(timestamp: string): string {
  try {
    const now = new Date();
    const past = new Date(timestamp);
    const diffMs = now.getTime() - past.getTime();
    const diffMins = Math.floor(diffMs / 60000);
    
    if (diffMins < 1) return 'Ahora mismo';
    if (diffMins < 60) return `Hace ${diffMins} min`;
    if (diffMins < 1440) return `Hace ${Math.floor(diffMins / 60)} h`;
    return `Hace ${Math.floor(diffMins / 1440)} días`;
  } catch {
    return 'Tiempo desconocido';
  }
}

// === CONFIGURACIÓN DE REFRESH AUTOMÁTICO ===

export class AutoRefresh {
  private intervals: { [key: string]: NodeJS.Timeout } = {};
  
  start(name: string, callback: () => void, intervalMs: number = 30000) {
    this.stop(name); // Detener intervalo existente
    this.intervals[name] = setInterval(callback, intervalMs);
  }
  
  stop(name: string) {
    if (this.intervals[name]) {
      clearInterval(this.intervals[name]);
      delete this.intervals[name];
    }
  }
  
  stopAll() {
    Object.keys(this.intervals).forEach(name => this.stop(name));
  }
}

export const autoRefresh = new AutoRefresh();

// === MANEJO DE ERRORES GLOBAL ===

export function handleApiError(error: any): string {
  if (error.message) {
    return error.message;
  }
  if (typeof error === 'string') {
    return error;
  }
  return 'Error desconocido del sistema';
}

// === VALIDACIONES ===

export function isValidDevice(device: any): device is Device {
  return device && 
         typeof device.id === 'number' &&
         typeof device.name === 'string' &&
         typeof device.type === 'string' &&
         typeof device.active === 'boolean' &&
         typeof device.status === 'string';
}

export function isValidSystemStatus(data: any): data is SystemStatus {
  return data &&
         data.success === true &&
         data.data &&
         data.data.devices &&
         data.data.tanks &&
         data.data.sensors &&
         typeof data.timestamp === 'string';
}

// Exportar por defecto la instancia de API
export default api;
