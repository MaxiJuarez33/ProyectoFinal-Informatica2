// API configuration
const API_BASE_URL = 'http://localhost:8080/api';

/**
 * Fetch data from the backend API
 * @param {string} endpoint - API endpoint (e.g., 'devices', 'tanks', 'sensors')
 * @returns {Promise<any>} - API response data
 */
export async function fetchFromAPI(endpoint) {
  try {
    const response = await fetch(`${API_BASE_URL}/${endpoint}`, {
      method: 'GET',
      headers: {
        'Content-Type': 'application/json',
      },
    });

    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }

    const data = await response.json();
    return data;
  } catch (error) {
    console.error(`Error fetching ${endpoint}:`, error);
    throw error;
  }
}

/**
 * Check if the backend API is available
 * @returns {Promise<boolean>} - True if API is available
 */
export async function checkAPIAvailability() {
  try {
    await fetchFromAPI('status');
    return true;
  } catch (error) {
    return false;
  }
}

/**
 * Transform backend device data to frontend format
 * @param {Array} devices - Raw device data from backend
 * @returns {Object} - Transformed data for frontend
 */
export function transformDeviceData(devices) {
  const activeDevices = devices.filter(device => device.active);
  
  return {
    active: activeDevices.length,
    total: devices.length,
    percentage: devices.length > 0 ? Math.round((activeDevices.length / devices.length) * 100) : 0
  };
}

/**
 * Transform backend tank data to frontend format
 * @param {Array} tanks - Raw tank data from backend
 * @returns {Object} - Transformed data for frontend
 */
export function transformTankData(tanks) {
  const whiteTank = tanks.find(tank => tank.type === 'white_water') || {};
  const grayTank = tanks.find(tank => tank.type === 'gray_water') || {};
  const blackTank = tanks.find(tank => tank.type === 'black_water') || {};
  
  return {
    white: {
      level: whiteTank.level || 0,
      critical: whiteTank.level <= whiteTank.critical_level
    },
    gray: {
      level: grayTank.level || 0,
      critical: grayTank.level <= grayTank.critical_level
    },
    black: {
      level: blackTank.level || 0,
      critical: blackTank.level >= blackTank.critical_level
    },
    pump_status: "running" // This would come from device status
  };
}

/**
 * Transform backend sensor data to frontend format
 * @param {Array} sensors - Raw sensor data from backend
 * @returns {Object} - Transformed data for frontend
 */
export function transformSensorData(sensors) {
  const tempSensors = sensors.filter(sensor => sensor.type === 'temperature');
  const interiorTemp = tempSensors.find(sensor => sensor.name.includes('Interior'));
  const exteriorTemp = tempSensors.find(sensor => sensor.name.includes('Exterior'));
  
  return {
    temperature: {
      interior: interiorTemp?.value || 0,
      exterior: exteriorTemp?.value || 0
    },
    current: {
      battery: 12.4, // These would come from electrical device data
      climate: 2.1,
      lights: 1.5,
      fridge: 3.2
    }
  };
}

/**
 * Get demo data (fallback when API is not available)
 * @returns {Object} - Demo data structure
 */
export function getDemoData() {
  return {
    devices: { active: 4, total: 4, percentage: 100 },
    tanks: {
      white: { level: 75, critical: false },
      gray: { level: 45, critical: false },
      black: { level: 30, critical: false },
      pump_status: "running"
    },
    sensors: {
      temperature: { interior: 22, exterior: 18 },
      current: { battery: 12.4, climate: 2.1, lights: 1.5, fridge: 3.2 }
    },
    electrical: {
      battery_voltage: 12.4,
      battery_status: "good",
      total_consumption: 6.8
    }
  };
}
