#include "crow.h"
#include "deviceManager.h"
#include "tankManager.h"
#include "electricManager.h"
#include "serialReader.h"
#include <json.hpp>

// Variables globales (ya las tienes)
extern DeviceManager deviceManager;
extern WaterTank tankManager;
extern Device electricDevice;
extern SaveDataVars dataStruct;

int main()
{
    crow::SimpleApp app;

    // ===== ENDPOINTS DE DISPOSITIVOS =====
    // GET /api/devices - Listar todos los dispositivos
    CROW_ROUTE(app, "/api/devices").methods("GET"_method)([&](const crow::request &req)
                                                          {
        nlohmann::json response;
        try {
            nlohmann::json devices = nlohmann::json::array();
            
            // Dispositivos del sistema (basado en tu estructura actual)
            devices.push_back({
                {"id", 1},
                {"name", "Sensor Temperatura Interior"},
                {"type", "sensor"},
                {"active", true},
                {"value", dataStruct.tempData[0]},
                {"unit", "°C"},
                {"status", "normal"}
            });
            
            devices.push_back({
                {"id", 2},
                {"name", "Sensor Temperatura Exterior"},
                {"type", "sensor"},
                {"active", true},
                {"value", dataStruct.tempData[1]},
                {"unit", "°C"},
                {"status", "normal"}
            });
            
            devices.push_back({
                {"id", 3},
                {"name", "Bomba Agua"},
                {"type", "actuator"},
                {"active", !tankManager.isStopPumpNeeded()},
                {"status", tankManager.isStopPumpNeeded() ? "stopped" : "running"}
            });
            
            devices.push_back({
                {"id", 4},
                {"name", "Sistema Eléctrico"},
                {"type", "system"},
                {"active", electricDevice.isActivated(1)},
                {"voltage", dataStruct.corrData[0]},
                {"unit", "V"}
            });
            
            response = {
                {"success", true},
                {"data", devices},
                {"timestamp", dataStruct.timeString},
                {"count", devices.size()}
            };
            
        } catch (const std::exception& e) {
            response = {
                {"success", false},
                {"error", e.what()}
            };
            return crow::response(500, response.dump());
        }
        
        return crow::response(200, response.dump()); });

    // POST /api/devices/{id}/toggle - Activar/desactivar dispositivo
    CROW_ROUTE(app, "/api/devices/<int>/toggle").methods("POST"_method)([&](const crow::request &req, int device_id)
                                                                        {
        try {
            // Usar tu ElectricManager existente
            bool currentState = electricDevice.isActivated(device_id);
            
            // Aquí implementarías toggle (necesitas agregar esta función)
            // electricDevice.toggle(device_id);
            
            nlohmann::json response = {
                {"success", true},
                {"device_id", device_id},
                {"new_state", !currentState}
            };
            
            return crow::response(200, response.dump());
        } catch (const std::exception& e) {
            nlohmann::json error = {
                {"success", false},
                {"error", e.what()}
            };
            return crow::response(400, error.dump());
        } });

    // ===== ENDPOINTS DE TANQUES =====
    // GET /api/tanks - Estado de todos los tanques
    CROW_ROUTE(app, "/api/tanks").methods("GET"_method)([&](const crow::request &req)
                                                        {
        nlohmann::json response;
        try {
            // Calcular niveles de tanques (ajustar según tu lógica específica)
            double whiteLevel = std::max(0.0, 75.0 - dataStruct.capData[0]);
            double grayLevel = std::max(0.0, 54.0 - dataStruct.capData[1]);
            double blackLevel = std::max(0.0, 54.0 - dataStruct.capData[2]);
            
            response = {
                {"success", true},
                {"data", {
                    {"white", {
                        {"id", "tank_white"},
                        {"name", "Agua Potable"},
                        {"level", whiteLevel},
                        {"capacity", 75},
                        {"critical", tankManager.isWhiteCritical()},
                        {"stopPump", tankManager.isStopPumpNeeded()},
                        {"status", tankManager.isWhiteCritical() ? "critical" : "normal"},
                        {"unit", "litros"}
                    }},
                    {"gray", {
                        {"id", "tank_gray"},
                        {"name", "Agua Gris"},
                        {"level", grayLevel},
                        {"capacity", 54},
                        {"critical", tankManager.isGrayCritical()},
                        {"status", tankManager.isGrayCritical() ? "critical" : "normal"},
                        {"unit", "litros"}
                    }},
                    {"black", {
                        {"id", "tank_black"},
                        {"name", "Agua Negra"},
                        {"level", blackLevel},
                        {"capacity", 54},
                        {"critical", tankManager.isBlackCritical()},
                        {"status", tankManager.isBlackCritical() ? "critical" : "normal"},
                        {"unit", "litros"}
                    }}
                }},
                {"timestamp", dataStruct.timeString},
                {"alerts", {
                    {"white_critical", tankManager.isWhiteCritical()},
                    {"gray_critical", tankManager.isGrayCritical()},
                    {"black_critical", tankManager.isBlackCritical()},
                    {"pump_stop_needed", tankManager.isStopPumpNeeded()}
                }}
            };
            
        } catch (const std::exception& e) {
            response = {
                {"success", false},
                {"error", e.what()}
            };
            return crow::response(500, response.dump());
        }
        
        return crow::response(200, response.dump()); });

    // ===== ENDPOINTS DE SENSORES =====
    // GET /api/sensors - Datos de todos los sensores
    CROW_ROUTE(app, "/api/sensors").methods("GET"_method)([&](const crow::request &req)
                                                          {
        nlohmann::json response;
        try {
            response = {
                {"success", true},
                {"data", {
                    {"temperature", {
                        {"interior", {
                            {"value", dataStruct.tempData[0]},
                            {"unit", "°C"},
                            {"status", (dataStruct.tempData[0] > 15 && dataStruct.tempData[0] < 30) ? "normal" : "warning"},
                            {"timestamp", dataStruct.timeString}
                        }},
                        {"exterior", {
                            {"value", dataStruct.tempData[1]},
                            {"unit", "°C"},
                            {"status", "normal"},
                            {"timestamp", dataStruct.timeString}
                        }}
                    }},
                    {"electrical", {
                        {"battery", {
                            {"value", dataStruct.corrData[0]},
                            {"unit", "V"},
                            {"status", (dataStruct.corrData[0] > 11.5) ? "normal" : "low"},
                            {"timestamp", dataStruct.timeString}
                        }},
                        {"climate", {
                            {"value", dataStruct.corrData[1]},
                            {"unit", "A"},
                            {"status", "normal"},
                            {"timestamp", dataStruct.timeString}
                        }},
                        {"lights", {
                            {"value", dataStruct.corrData[2]},
                            {"unit", "A"},
                            {"status", "normal"},
                            {"timestamp", dataStruct.timeString}
                        }},
                        {"fridge", {
                            {"value", dataStruct.corrData[3]},
                            {"unit", "A"},
                            {"status", "normal"},
                            {"timestamp", dataStruct.timeString}
                        }}
                    }},
                    {"water_levels", {
                        {"white_tank", {
                            {"value", dataStruct.capData[0]},
                            {"unit", "raw"},
                            {"level_percent", std::max(0.0, ((75.0 - dataStruct.capData[0]) / 75.0) * 100)},
                            {"timestamp", dataStruct.timeString}
                        }},
                        {"gray_tank", {
                            {"value", dataStruct.capData[1]},
                            {"unit", "raw"},
                            {"level_percent", std::max(0.0, ((54.0 - dataStruct.capData[1]) / 54.0) * 100)},
                            {"timestamp", dataStruct.timeString}
                        }},
                        {"black_tank", {
                            {"value", dataStruct.capData[2]},
                            {"unit", "raw"},
                            {"level_percent", std::max(0.0, ((54.0 - dataStruct.capData[2]) / 54.0) * 100)},
                            {"timestamp", dataStruct.timeString}
                        }}
                    }}
                }},
                {"timestamp", dataStruct.timeString},
                {"system_status", "operational"}
            };
            
        } catch (const std::exception& e) {
            response = {
                {"success", false},
                {"error", e.what()}
            };
            return crow::response(500, response.dump());
        }
        
        return crow::response(200, response.dump()); });

    // ===== ENDPOINT DE ESTADO GENERAL =====

    // GET /api/status - Dashboard general del sistema
    CROW_ROUTE(app, "/api/status").methods("GET"_method)([&](const crow::request &req)
                                                         {
        nlohmann::json response;
        try {
            // Calcular estadísticas del sistema
            double whiteLevel = std::max(0.0, ((75.0 - dataStruct.capData[0]) / 75.0) * 100);
            double grayLevel = std::max(0.0, ((54.0 - dataStruct.capData[1]) / 54.0) * 100);
            double blackLevel = std::max(0.0, ((54.0 - dataStruct.capData[2]) / 54.0) * 100);
            
            int devicesActive = 0;
            int devicesTotal = 4; // Temperatura interior, exterior, bomba, sistema eléctrico
            
            // Contar dispositivos activos
            if (dataStruct.tempData[0] > -50) devicesActive++; // Sensor temp interior
            if (dataStruct.tempData[1] > -50) devicesActive++; // Sensor temp exterior
            if (!tankManager.isStopPumpNeeded()) devicesActive++; // Bomba
            if (electricDevice.isActivated(1)) devicesActive++; // Sistema eléctrico
            
            // Determinar estado general del sistema
            std::string systemStatus = "normal";
            if (tankManager.isWhiteCritical() || tankManager.isBlackCritical() || 
                tankManager.isGrayCritical() || dataStruct.corrData[0] < 11.5) {
                systemStatus = "warning";
            }
            if (dataStruct.corrData[0] < 11.0) {
                systemStatus = "critical";
            }
            
            response = {
                {"success", true},
                {"data", {
                    {"devices", {
                        {"active", devicesActive},
                        {"total", devicesTotal},
                        {"percentage", (devicesActive * 100.0) / devicesTotal}
                    }},
                    {"tanks", {
                        {"white", {
                            {"level", whiteLevel},
                            {"critical", tankManager.isWhiteCritical()}
                        }},
                        {"gray", {
                            {"level", grayLevel},
                            {"critical", tankManager.isGrayCritical()}
                        }},
                        {"black", {
                            {"level", blackLevel},
                            {"critical", tankManager.isBlackCritical()}
                        }},
                        {"pump_status", tankManager.isStopPumpNeeded() ? "stopped" : "running"}
                    }},
                    {"sensors", {
                        {"temperature", {
                            {"interior", dataStruct.tempData[0]},
                            {"exterior", dataStruct.tempData[1]}
                        }},
                        {"current", {
                            {"battery", dataStruct.corrData[0]},
                            {"climate", dataStruct.corrData[1]},
                            {"lights", dataStruct.corrData[2]},
                            {"fridge", dataStruct.corrData[3]}
                        }}
                    }},
                    {"electrical", {
                        {"battery_voltage", dataStruct.corrData[0]},
                        {"battery_status", (dataStruct.corrData[0] > 12.0) ? "good" : 
                                         (dataStruct.corrData[0] > 11.5) ? "low" : "critical"},
                        {"total_consumption", dataStruct.corrData[1] + dataStruct.corrData[2] + dataStruct.corrData[3]}
                    }},
                    {"alerts", {
                        {"active_count", 
                            (tankManager.isWhiteCritical() ? 1 : 0) +
                            (tankManager.isGrayCritical() ? 1 : 0) +
                            (tankManager.isBlackCritical() ? 1 : 0) +
                            (dataStruct.corrData[0] < 11.5 ? 1 : 0)
                        },
                        {"white_tank_low", tankManager.isWhiteCritical()},
                        {"gray_tank_full", tankManager.isGrayCritical()},
                        {"black_tank_full", tankManager.isBlackCritical()},
                        {"battery_low", dataStruct.corrData[0] < 11.5},
                        {"pump_stopped", tankManager.isStopPumpNeeded()}
                    }}
                }},
                {"timestamp", dataStruct.timeString},
                {"system_status", systemStatus},
                {"uptime", "Sistema operacional"} // Podrías calcular uptime real si quieres
            };
            
        } catch (const std::exception& e) {
            response = {
                {"success", false},
                {"error", e.what()},
                {"timestamp", dataStruct.timeString}
            };
            return crow::response(500, response.dump());
        }
        
        return crow::response(200, response.dump()); });

    // ===== SERVIR ARCHIVOS ESTÁTICOS =====

    // Servir el frontend desde carpeta 'web'
    CROW_ROUTE(app, "/")
    ([](const crow::request &req)
     { return crow::load_text("web/index.html"); });

    // CORS para desarrollo
    CROW_ROUTE(app, "/api/<path>").methods("OPTIONS"_method)([](const crow::request &req, const std::string &path)
                                                             {
        crow::response res(200);
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        return res; });

    // Configurar CORS para todas las respuestas
    app.get_middleware<crow::CORSHandler>().global().headers("Content-Type", "Authorization").methods("GET"_method, "POST"_method, "PUT"_method, "DELETE"_method).origin("*");

    std::cout << "🌐 Servidor web iniciado en http://localhost:8080" << std::endl;
    std::cout << "📊 Dashboard disponible en http://localhost:8080" << std::endl;
    std::cout << "🔌 API REST disponible en http://localhost:8080/api/*" << std::endl;

    app.port(8080).multithreaded().run();

    return 0;
}
