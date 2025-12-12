#include "crow.h"
#include <json.hpp>
#include <cstdlib>
#include <ctime>

int main()
{
    crow::SimpleApp app;

    // Initialize random number generator for demo data
    std::srand(std::time(nullptr));

    // CORS middleware for development
    auto corsMiddleware = [](const crow::request &req, crow::response &res, const std::function<void()> &complete)
    {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");

        if (req.method == "OPTIONS"_method)
        {
            res.code = 200;
            res.end();
            return;
        }

        complete();
    };

    // Enable CORS for all routes
    CROW_ROUTE(app, "/api/<path>").methods("OPTIONS"_method)([](const crow::request &req, const std::string &path)
                                                             {
        crow::response res(200);
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        return res; });

    // GET /api/status - Sistema general
    CROW_ROUTE(app, "/api/status").methods("GET"_method)([&](const crow::request &req)
                                                         {
        nlohmann::json response;
        try {
            // Generate dynamic demo data
            double battery_voltage = 12.0 + (std::rand() % 10) / 10.0; // 12.0-12.9V
            double white_level = 70.0 + (std::rand() % 30); // 70-99%
            double gray_level = 20.0 + (std::rand() % 40); // 20-59%
            double black_level = 10.0 + (std::rand() % 30); // 10-39%
            
            response = {
                {"success", true},
                {"data", {
                    {"devices", {
                        {"active", 3},
                        {"total", 4},
                        {"percentage", 75.0}
                    }},
                    {"tanks", {
                        {"white", {
                            {"level", white_level},
                            {"critical", white_level < 20}
                        }},
                        {"gray", {
                            {"level", gray_level},
                            {"critical", gray_level > 80}
                        }},
                        {"black", {
                            {"level", black_level},
                            {"critical", black_level > 80}
                        }},
                        {"pump_status", white_level > 20 ? "running" : "stopped"}
                    }},
                    {"sensors", {
                        {"temperature", {
                            {"interior", 22.0 + (std::rand() % 50) / 10.0}, // 22-26.9°C
                            {"exterior", 15.0 + (std::rand() % 100) / 10.0} // 15-24.9°C
                        }},
                        {"current", {
                            {"battery", battery_voltage},
                            {"climate", 1.5 + (std::rand() % 20) / 10.0}, // 1.5-3.4A
                            {"lights", 0.5 + (std::rand() % 15) / 10.0}, // 0.5-1.9A
                            {"fridge", 1.0 + (std::rand() % 25) / 10.0} // 1.0-3.4A
                        }}
                    }},
                    {"electrical", {
                        {"battery_voltage", battery_voltage},
                        {"battery_status", battery_voltage > 12.0 ? "good" : (battery_voltage > 11.5 ? "low" : "critical")},
                        {"total_consumption", 3.0 + (std::rand() % 40) / 10.0} // 3.0-6.9A
                    }},
                    {"alerts", {
                        {"active_count", white_level < 20 ? 1 : 0},
                        {"white_tank_low", white_level < 20},
                        {"gray_tank_full", gray_level > 80},
                        {"black_tank_full", black_level > 80},
                        {"battery_low", battery_voltage < 11.5},
                        {"pump_stopped", white_level <= 20}
                    }}
                }},
                {"timestamp", std::to_string(std::time(nullptr))},
                {"system_status", white_level < 20 || battery_voltage < 11.5 ? "warning" : "normal"}
            };
            
        } catch (const std::exception& e) {
            response = {
                {"success", false},
                {"error", e.what()}
            };
            return crow::response(500, response.dump());
        }
        
        crow::response res(200, response.dump());
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Content-Type", "application/json");
        return res; });

    // GET /api/devices - Lista de dispositivos
    CROW_ROUTE(app, "/api/devices").methods("GET"_method)([&](const crow::request &req)
                                                          {
        nlohmann::json response;
        try {
            nlohmann::json devices = nlohmann::json::array();
            
            devices.push_back({
                {"id", 1},
                {"name", "Sensor Temperatura Interior"},
                {"type", "sensor"},
                {"active", true},
                {"value", 22.0 + (std::rand() % 50) / 10.0},
                {"unit", "°C"},
                {"status", "normal"}
            });
            
            devices.push_back({
                {"id", 2},
                {"name", "Sensor Temperatura Exterior"},
                {"type", "sensor"},
                {"active", true},
                {"value", 15.0 + (std::rand() % 100) / 10.0},
                {"unit", "°C"},
                {"status", "normal"}
            });
            
            devices.push_back({
                {"id", 3},
                {"name", "Bomba Agua"},
                {"type", "actuator"},
                {"active", true},
                {"status", "running"}
            });
            
            devices.push_back({
                {"id", 4},
                {"name", "Sistema Eléctrico"},
                {"type", "system"},
                {"active", true},
                {"voltage", 12.0 + (std::rand() % 10) / 10.0},
                {"unit", "V"}
            });
            
            response = {
                {"success", true},
                {"data", devices},
                {"timestamp", std::to_string(std::time(nullptr))},
                {"count", devices.size()}
            };
            
        } catch (const std::exception& e) {
            response = {
                {"success", false},
                {"error", e.what()}
            };
            return crow::response(500, response.dump());
        }
        
        crow::response res(200, response.dump());
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Content-Type", "application/json");
        return res; });

    // GET /api/tanks - Estado de tanques
    CROW_ROUTE(app, "/api/tanks").methods("GET"_method)([&](const crow::request &req)
                                                        {
        nlohmann::json response;
        try {
            double white_level = 70.0 + (std::rand() % 30);
            double gray_level = 20.0 + (std::rand() % 40);
            double black_level = 10.0 + (std::rand() % 30);
            
            response = {
                {"success", true},
                {"data", {
                    {"white", {
                        {"id", "tank_white"},
                        {"name", "Agua Potable"},
                        {"level", white_level},
                        {"capacity", 75},
                        {"critical", white_level < 20},
                        {"status", white_level < 20 ? "critical" : "normal"},
                        {"unit", "litros"}
                    }},
                    {"gray", {
                        {"id", "tank_gray"},
                        {"name", "Agua Gris"},
                        {"level", gray_level},
                        {"capacity", 54},
                        {"critical", gray_level > 80},
                        {"status", gray_level > 80 ? "critical" : "normal"},
                        {"unit", "litros"}
                    }},
                    {"black", {
                        {"id", "tank_black"},
                        {"name", "Agua Negra"},
                        {"level", black_level},
                        {"capacity", 54},
                        {"critical", black_level > 80},
                        {"status", black_level > 80 ? "critical" : "normal"},
                        {"unit", "litros"}
                    }}
                }},
                {"timestamp", std::to_string(std::time(nullptr))}
            };
            
        } catch (const std::exception& e) {
            response = {
                {"success", false},
                {"error", e.what()}
            };
            return crow::response(500, response.dump());
        }
        
        crow::response res(200, response.dump());
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Content-Type", "application/json");
        return res; });

    // GET /api/sensors - Datos de sensores
    CROW_ROUTE(app, "/api/sensors").methods("GET"_method)([&](const crow::request &req)
                                                          {
        nlohmann::json response;
        try {
            double interior_temp = 22.0 + (std::rand() % 50) / 10.0;
            double exterior_temp = 15.0 + (std::rand() % 100) / 10.0;
            double battery_voltage = 12.0 + (std::rand() % 10) / 10.0;
            
            response = {
                {"success", true},
                {"data", {
                    {"temperature", {
                        {"interior", {
                            {"value", interior_temp},
                            {"unit", "°C"},
                            {"status", (interior_temp > 15 && interior_temp < 30) ? "normal" : "warning"},
                            {"timestamp", std::to_string(std::time(nullptr))}
                        }},
                        {"exterior", {
                            {"value", exterior_temp},
                            {"unit", "°C"},
                            {"status", "normal"},
                            {"timestamp", std::to_string(std::time(nullptr))}
                        }}
                    }},
                    {"electrical", {
                        {"battery", {
                            {"value", battery_voltage},
                            {"unit", "V"},
                            {"status", battery_voltage > 11.5 ? "normal" : "low"},
                            {"timestamp", std::to_string(std::time(nullptr))}
                        }},
                        {"climate", {
                            {"value", 1.5 + (std::rand() % 20) / 10.0},
                            {"unit", "A"},
                            {"status", "normal"},
                            {"timestamp", std::to_string(std::time(nullptr))}
                        }},
                        {"lights", {
                            {"value", 0.5 + (std::rand() % 15) / 10.0},
                            {"unit", "A"},
                            {"status", "normal"},
                            {"timestamp", std::to_string(std::time(nullptr))}
                        }},
                        {"fridge", {
                            {"value", 1.0 + (std::rand() % 25) / 10.0},
                            {"unit", "A"},
                            {"status", "normal"},
                            {"timestamp", std::to_string(std::time(nullptr))}
                        }}
                    }}
                }},
                {"timestamp", std::to_string(std::time(nullptr))},
                {"system_status", "operational"}
            };
            
        } catch (const std::exception& e) {
            response = {
                {"success", false},
                {"error", e.what()}
            };
            return crow::response(500, response.dump());
        }
        
        crow::response res(200, response.dump());
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Content-Type", "application/json");
        return res; });

    // Home page
    CROW_ROUTE(app, "/")([](const crow::request &req)
                         {
        crow::response res(200);
        res.set_header("Content-Type", "text/html");
        res.write("<!DOCTYPE html><html><head><title>IoT Dashboard Backend</title></head><body><h1>🚐 IoT Dashboard Backend</h1><p>✅ API funcionando en puerto 8080</p><ul><li><a href='/api/status'>GET /api/status</a></li><li><a href='/api/devices'>GET /api/devices</a></li><li><a href='/api/tanks'>GET /api/tanks</a></li><li><a href='/api/sensors'>GET /api/sensors</a></li></ul></body></html>");
        return res; });

    std::cout << "🌐 Servidor IoT Dashboard iniciado en http://localhost:8080" << std::endl;
    std::cout << "📊 API REST disponible:" << std::endl;
    std::cout << "  - GET http://localhost:8080/api/status" << std::endl;
    std::cout << "  - GET http://localhost:8080/api/devices" << std::endl;
    std::cout << "  - GET http://localhost:8080/api/tanks" << std::endl;
    std::cout << "  - GET http://localhost:8080/api/sensors" << std::endl;

    app.port(8080).multithreaded().run();

    return 0;
}
