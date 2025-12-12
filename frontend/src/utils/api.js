// API utility functions for IoT Dashboard
const API_BASE_URL = 'http://localhost:8080/api';

/**
 * Generic API fetch function with error handling
 */
async function apiRequest(endpoint, options = {}) {
  try {
    const response = await fetch(`${API_BASE_URL}${endpoint}`, {
      ...options,
      headers: {
        'Content-Type': 'application/json',
        ...options.headers,
      },
    });

    if (!response.ok) {
      throw new Error(`API Error: ${response.status} ${response.statusText}`);
    }

    return await response.json();
  } catch (error) {
    console.error(`API request failed for ${endpoint}:`, error);
    throw error;
  }
}

/**
 * Fetch system status
 */
export async function getSystemStatus() {
  return apiRequest('/status');
}

/**
 * Fetch all devices
 */
export async function getDevices() {
  return apiRequest('/devices');
}

/**
 * Toggle device state
 */
export async function toggleDevice(deviceId) {
  return apiRequest(`/devices/${deviceId}/toggle`, {
    method: 'POST',
  });
}

/**
 * Fetch all tanks data
 */
export async function getTanks() {
  return apiRequest('/tanks');
}

/**
 * Fetch all sensors data
 */
export async function getSensors() {
  return apiRequest('/sensors');
}

/**
 * Check if API is available
 */
export async function checkApiConnection() {
  try {
    const response = await fetch(`${API_BASE_URL}/status`);
    return response.ok;
  } catch (error) {
    return false;
  }
}

/**
 * Fallback demo data (used when API is not available)
 */
export const demoData = {
  status: {
    success: true,
    data: {
      devices: { active: 3, total: 4, percentage: 75 },
      tanks: {
        white: { level: 85, critical: false },
        gray: { level: 45, critical: false },
        black: { level: 20, critical: false },
        pump_status: "running"
      },
      sensors: {
        temperature: { interior: 22.5, exterior: 18.3 },
        current: { battery: 12.6, climate: 2.1, lights: 0.8, fridge: 1.5 }
      },
      electrical: {
        battery_voltage: 12.6,
        battery_status: "good",
        total_consumption: 4.4
      },
      alerts: {
        active_count: 0,
        white_tank_low: false,
        gray_tank_full: false,
        black_tank_full: false,
        battery_low: false,
        pump_stopped: false
      }
    },
    timestamp: new Date().toISOString(),
    system_status: "normal"
  },
  devices: {
    success: true,
    data: [
      { id: 1, name: "Sensor Temperatura Interior", type: "sensor", active: true, value: 22.5, unit: "°C", status: "normal" },
      { id: 2, name: "Sensor Temperatura Exterior", type: "sensor", active: true, value: 18.3, unit: "°C", status: "normal" },
      { id: 3, name: "Bomba Agua", type: "actuator", active: true, status: "running" },
      { id: 4, name: "Sistema Eléctrico", type: "system", active: true, voltage: 12.6, unit: "V" }
    ]
  },  tanks: {
    success: true,
    data: {
      white: { id: "tank_white", name: "Agua Potable", level: 63.75, capacity: 75, critical: false, status: "normal", unit: "litros", current_volume: 47.8 },
      gray: { id: "tank_gray", name: "Agua Gris", level: 24.3, capacity: 54, critical: false, status: "normal", unit: "litros", current_volume: 13.1 },
      black: { id: "tank_black", name: "Agua Negra", level: 10.8, capacity: 54, critical: false, status: "normal", unit: "litros", current_volume: 5.8 }
    }
  },
  sensors: {
    success: true,
    data: {
      temperature: {
        interior: { value: 22.5, unit: "°C", status: "normal" },
        exterior: { value: 18.3, unit: "°C", status: "normal" }
      },
      electrical: {
        battery: { value: 12.6, unit: "V", status: "normal" },
        climate: { value: 2.1, unit: "A", status: "normal" },
        lights: { value: 0.8, unit: "A", status: "normal" },
        fridge: { value: 1.5, unit: "A", status: "normal" }
      }
    }
  }
};
