#include "../lib/crow.h"
#include "../lib/json.hpp"
#include "../inc/serialReader.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

using json = nlohmann::json;
using namespace std;

// Global variables for sensor data
SaveDataVars currentSensorData;
mutex dataMutex;
atomic<bool> esp32Connected(false);
atomic<bool> dataUpdated(false);

// Serial reader instance
SerialReader *serialReader = nullptr;

// Thread function to read from ESP32
void esp32ReaderThread()
{
    try
    {
        // Try different common COM ports
        const char *possiblePorts[] = {"COM3", "COM4", "COM5", "COM6", "COM7", "COM8"};
        bool connected = false;

        for (const char *port : possiblePorts)
        {
            try
            {
                cout << "Intentando conectar a " << port << "..." << endl;
                serialReader = new SerialReader(port);
                serialReader->openPort();
                serialReader->configurePort();
                cout << "✅ ESP32 conectado en " << port << endl;
                esp32Connected = true;
                connected = true;
                break;
            }
            catch (const exception &e)
            {
                cout << "❌ " << port << " no disponible: " << e.what() << endl;
                if (serialReader)
                {
                    delete serialReader;
                    serialReader = nullptr;
                }
            }
        }

        if (!connected)
        {
            cout << "⚠️  ESP32 no encontrado. Usando datos simulados." << endl;
            esp32Connected = false;
            return;
        }

        // Main reading loop
        while (esp32Connected)
        {
            try
            {
                SaveDataVars tempData;
                DWORD bytesRead;

                if (ReadFile(serialReader->hSerial, &tempData, sizeof(tempData), &bytesRead, NULL))
                {
                    if (bytesRead == sizeof(tempData))
                    {
                        lock_guard<mutex> lock(dataMutex);
                        currentSensorData = tempData;
                        dataUpdated = true;
                        cout << "📡 Datos ESP32 actualizados - Temp Int: " << tempData.tempData[0]
                             << "°C, Temp Ext: " << tempData.tempData[1] << "°C" << endl;
                    }
                }

                this_thread::sleep_for(chrono::milliseconds(1000));
            }
            catch (const exception &e)
            {
                cout << "❌ Error leyendo ESP32: " << e.what() << endl;
                esp32Connected = false;
                break;
            }
        }
    }
    catch (const exception &e)
    {
        cout << "❌ Error en thread ESP32: " << e.what() << endl;
        esp32Connected = false;
    }
}

// Generate mock data when ESP32 is not connected
SaveDataVars generateMockData()
{
    SaveDataVars mockData;

    // Temperature data (simulate realistic variations)
    static double baseTemp1 = 22.5, baseTemp2 = 18.3;
    mockData.tempData[0] = baseTemp1 + (rand() % 20 - 10) / 10.0; // ±1°C variation
    mockData.tempData[1] = baseTemp2 + (rand() % 20 - 10) / 10.0;

    // Tank levels (simulate slow changes)
    static double tankLevels[3] = {85.0, 45.0, 20.0};
    for (int i = 0; i < 3; i++)
    {
        tankLevels[i] += (rand() % 6 - 3) / 10.0; // ±0.3% variation
        if (tankLevels[i] < 0)
            tankLevels[i] = 0;
        if (tankLevels[i] > 100)
            tankLevels[i] = 100;
        mockData.capData[i] = tankLevels[i];
    }

    // Current readings
    mockData.corrData[0] = 12.6 + (rand() % 10 - 5) / 10.0; // Battery
    mockData.corrData[1] = 2.1 + (rand() % 6 - 3) / 10.0;   // Climate
    mockData.corrData[2] = 0.8 + (rand() % 4 - 2) / 10.0;   // Lights
    mockData.corrData[3] = 1.5 + (rand() % 4 - 2) / 10.0;   // Fridge

    // Time string
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    strftime(mockData.timeString, sizeof(mockData.timeString), "%H:%M:%S", localtime(&time_t));

    return mockData;
}

// Get current sensor data (real or mock)
SaveDataVars getCurrentData()
{
    if (esp32Connected && dataUpdated)
    {
        lock_guard<mutex> lock(dataMutex);
        return currentSensorData;
    }
    else
    {
        return generateMockData();
    }
}

int main()
{
    // Start ESP32 reader thread
    thread esp32Thread(esp32ReaderThread);
    esp32Thread.detach();

    // Wait a moment for connection attempt
    this_thread::sleep_for(chrono::seconds(2));

    crow::SimpleApp app;

    // Enable CORS
    app.get_middleware<crow::CORSHandler>().global().headers("X-Custom-Header", "Upgrade-Insecure-Requests").methods("POST"_method, "GET"_method);

    // Status endpoint
    CROW_ROUTE(app, "/api/status")([]
                                   {
        auto data = getCurrentData();
        
        json response = {
            {"success", true},
            {"data", {
                {"devices", {
                    {"active", 4},
                    {"total", 4},
                    {"percentage", 100}
                }},
                {"tanks", {
                    {"white", {
                        {"level", data.capData[0]},
                        {"critical", data.capData[0] < 20}
                    }},
                    {"gray", {
                        {"level", data.capData[1]},
                        {"critical", data.capData[1] > 80}
                    }},
                    {"black", {
                        {"level", data.capData[2]},
                        {"critical", data.capData[2] > 80}
                    }},
                    {"pump_status", "running"}
                }},
                {"sensors", {
                    {"temperature", {
                        {"interior", data.tempData[0]},
                        {"exterior", data.tempData[1]}
                    }},
                    {"current", {
                        {"battery", data.corrData[0]},
                        {"climate", data.corrData[1]},
                        {"lights", data.corrData[2]},
                        {"fridge", data.corrData[3]}
                    }}
                }},
                {"electrical", {
                    {"battery_voltage", data.corrData[0]},
                    {"battery_status", data.corrData[0] > 12.0 ? "good" : "low"},
                    {"total_consumption", data.corrData[1] + data.corrData[2] + data.corrData[3]}
                }},
                {"alerts", {
                    {"active_count", 0},
                    {"white_tank_low", data.capData[0] < 20},
                    {"gray_tank_full", data.capData[1] > 80},
                    {"black_tank_full", data.capData[2] > 80},
                    {"battery_low", data.corrData[0] < 12.0},
                    {"pump_stopped", false}
                }}
            }},
            {"timestamp", data.timeString},
            {"system_status", "normal"},
            {"esp32_connected", esp32Connected.load()}
        };
        
        return crow::response(200, response.dump()); });

    // Devices endpoint
    CROW_ROUTE(app, "/api/devices")([]
                                    {
        auto data = getCurrentData();
        
        json devices = {
            {"success", true},
            {"data", {
                {
                    {"id", 1},
                    {"name", "Sensor Temperatura Interior"},
                    {"type", "sensor"},
                    {"active", true},
                    {"value", data.tempData[0]},
                    {"unit", "°C"},
                    {"status", "normal"}
                },
                {
                    {"id", 2},
                    {"name", "Sensor Temperatura Exterior"},
                    {"type", "sensor"},
                    {"active", true},
                    {"value", data.tempData[1]},
                    {"unit", "°C"},
                    {"status", "normal"}
                },
                {
                    {"id", 3},
                    {"name", "Bomba Agua"},
                    {"type", "actuator"},
                    {"active", true},
                    {"status", "running"}
                },
                {
                    {"id", 4},
                    {"name", "Sistema Eléctrico"},
                    {"type", "system"},
                    {"active", true},
                    {"voltage", data.corrData[0]},
                    {"unit", "V"},
                    {"status", "normal"}
                }
            }},
            {"esp32_connected", esp32Connected.load()}
        };
        
        return crow::response(200, devices.dump()); });

    // Tanks endpoint
    CROW_ROUTE(app, "/api/tanks")([]
                                  {
        auto data = getCurrentData();
        
        json tanks = {
            {"success", true},
            {"data", {
                {"white", {
                    {"id", "tank_white"},
                    {"name", "Agua Potable"},
                    {"level", data.capData[0]},
                    {"capacity", 75},
                    {"critical", data.capData[0] < 20},
                    {"status", data.capData[0] < 20 ? "critical" : "normal"},
                    {"unit", "litros"}
                }},
                {"gray", {
                    {"id", "tank_gray"},
                    {"name", "Agua Gris"},
                    {"level", data.capData[1]},
                    {"capacity", 54},
                    {"critical", data.capData[1] > 80},
                    {"status", data.capData[1] > 80 ? "critical" : "normal"},
                    {"unit", "litros"}
                }},
                {"black", {
                    {"id", "tank_black"},
                    {"name", "Agua Negra"},
                    {"level", data.capData[2]},
                    {"capacity", 54},
                    {"critical", data.capData[2] > 80},
                    {"status", data.capData[2] > 80 ? "critical" : "normal"},
                    {"unit", "litros"}
                }},
                {"pump", {
                    {"status", "running"},
                    {"active", true}
                }}
            }},
            {"esp32_connected", esp32Connected.load()}
        };
        
        return crow::response(200, tanks.dump()); });

    // Sensors endpoint
    CROW_ROUTE(app, "/api/sensors")([]
                                    {
        auto data = getCurrentData();
        
        json sensors = {
            {"success", true},
            {"data", {
                {"temperature", {
                    {"interior", {
                        {"value", data.tempData[0]},
                        {"unit", "°C"},
                        {"status", "normal"}
                    }},
                    {"exterior", {
                        {"value", data.tempData[1]},
                        {"unit", "°C"},
                        {"status", "normal"}
                    }}
                }},
                {"electrical", {
                    {"battery", {
                        {"value", data.corrData[0]},
                        {"unit", "V"},
                        {"status", data.corrData[0] > 12.0 ? "normal" : "low"}
                    }},
                    {"climate", {
                        {"value", data.corrData[1]},
                        {"unit", "A"},
                        {"status", "normal"}
                    }},
                    {"lights", {
                        {"value", data.corrData[2]},
                        {"unit", "A"},
                        {"status", "normal"}
                    }},
                    {"fridge", {
                        {"value", data.corrData[3]},
                        {"unit", "A"},
                        {"status", "normal"}
                    }}
                }}
            }},
            {"esp32_connected", esp32Connected.load()}
        };
        
        return crow::response(200, sensors.dump()); });

    // Device control endpoint
    CROW_ROUTE(app, "/api/devices/<int>/toggle").methods("POST"_method)([](const crow::request &req, int deviceId)
                                                                        {
        if (esp32Connected && serialReader) {
            try {
                // Simple relay control - adjust pin numbers based on your hardware
                int relayPin = 2; // Default relay pin, adjust as needed
                
                // For this example, just toggle the pin
                // You might want to maintain device states and toggle accordingly
                serialReader->setDigitalHigh(std::to_string(relayPin).c_str());
                
                json response = {
                    {"success", true},
                    {"message", "Device toggled successfully"},
                    {"device_id", deviceId}
                };
                
                return crow::response(200, response.dump());
            } catch (const exception& e) {
                json error = {
                    {"success", false},
                    {"error", e.what()}
                };
                return crow::response(500, error.dump());
            }
        } else {
            json response = {
                {"success", true},
                {"message", "Device toggle simulated (ESP32 not connected)"},
                {"device_id", deviceId}
            };
            return crow::response(200, response.dump());
        } });

    cout << "🚀 Servidor híbrido iniciado en puerto 8080" << endl;
    cout << "🔌 ESP32 " << (esp32Connected ? "CONECTADO" : "DESCONECTADO") << endl;
    cout << "📡 Datos " << (esp32Connected ? "REALES" : "SIMULADOS") << endl;

    app.port(8080).multithreaded().run();

    return 0;
}
