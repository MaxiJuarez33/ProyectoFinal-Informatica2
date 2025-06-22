#include "crow.h"
#include <json.hpp>

int main()
{
    crow::SimpleApp app;

    // GET /api/status - Status endpoint
    CROW_ROUTE(app, "/api/status").methods("GET"_method)([](const crow::request &req)
    {
        crow::response res(200);
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Content-Type", "application/json");
        
        nlohmann::json status = {
            {"status", "ok"},
            {"message", "IoT Dashboard Backend is running"},
            {"timestamp", std::time(nullptr)},
            {"version", "1.0.0"}
        };
        
        res.write(status.dump());
        return res;
    });

    // GET /api/devices - Mock devices endpoint
    CROW_ROUTE(app, "/api/devices").methods("GET"_method)([](const crow::request &req)
    {
        crow::response res(200);
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Content-Type", "application/json");
        
        nlohmann::json devices = nlohmann::json::array();
        
        devices.push_back({
            {"id", 1},
            {"name", "Sensor Temperatura Interior"},
            {"type", "sensor"},
            {"active", true},
            {"value", 23.5},
            {"unit", "°C"},
            {"status", "normal"}
        });
        
        devices.push_back({
            {"id", 2},
            {"name", "Sensor Temperatura Exterior"},
            {"type", "sensor"},
            {"active", true},
            {"value", 18.2},
            {"unit", "°C"},
            {"status", "normal"}
        });
        
        devices.push_back({
            {"id", 3},
            {"name", "Bomba Agua"},
            {"type", "actuator"},
            {"active", true},
            {"value", 0},
            {"unit", "state"},
            {"status", "off"}
        });
        
        res.write(devices.dump());
        return res;
    });

    // GET /api/tanks - Mock tanks endpoint  
    CROW_ROUTE(app, "/api/tanks").methods("GET"_method)([](const crow::request &req)
    {
        crow::response res(200);
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Content-Type", "application/json");
        
        nlohmann::json tanks = nlohmann::json::array();
        
        tanks.push_back({
            {"id", 1},
            {"name", "Tanque Agua Blanca"},
            {"type", "white_water"},
            {"level", 75.5},
            {"capacity", 1000},
            {"status", "normal"},
            {"critical_level", 20},
            {"unit", "L"}
        });
        
        tanks.push_back({
            {"id", 2},
            {"name", "Tanque Agua Gris"},
            {"type", "gray_water"},
            {"level", 45.2},
            {"capacity", 500},
            {"status", "normal"},
            {"critical_level", 10},
            {"unit", "L"}
        });
        
        tanks.push_back({
            {"id", 3},
            {"name", "Tanque Agua Negra"},
            {"type", "black_water"},
            {"level", 12.8},
            {"capacity", 300},
            {"status", "warning"},
            {"critical_level", 90},
            {"unit", "L"}
        });
        
        res.write(tanks.dump());
        return res;
    });

    // GET /api/sensors - Mock sensors endpoint
    CROW_ROUTE(app, "/api/sensors").methods("GET"_method)([](const crow::request &req)
    {
        crow::response res(200);
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Content-Type", "application/json");
        
        nlohmann::json sensors = nlohmann::json::array();
        
        sensors.push_back({
            {"id", 1},
            {"name", "Temperatura Interior"},
            {"type", "temperature"},
            {"value", 23.5},
            {"unit", "°C"},
            {"timestamp", std::time(nullptr)},
            {"status", "active"}
        });
        
        sensors.push_back({
            {"id", 2},
            {"name", "Temperatura Exterior"},
            {"type", "temperature"},
            {"value", 18.2},
            {"unit", "°C"},
            {"timestamp", std::time(nullptr)},
            {"status", "active"}
        });
        
        sensors.push_back({
            {"id", 3},
            {"name", "Nivel Tanque Blanco"},
            {"type", "level"},
            {"value", 75.5},
            {"unit", "%"},
            {"timestamp", std::time(nullptr)},
            {"status", "active"}
        });
        
        res.write(sensors.dump());
        return res;
    });

    // Home endpoint
    CROW_ROUTE(app, "/")
    ([](const crow::request &req)
     { 
        crow::response res(200);
        res.set_header("Content-Type", "text/html");
        res.write("<!DOCTYPE html><html><head><title>IoT Dashboard API</title></head><body>"
                  "<h1>🏠 IoT Dashboard Backend API</h1>"
                  "<p>🌐 Backend server is running on port 8080</p>"
                  "<h2>📚 Available Endpoints:</h2>"
                  "<ul>"
                  "<li><a href='/api/status'>GET /api/status</a> - Server status</li>"
                  "<li><a href='/api/devices'>GET /api/devices</a> - List all devices</li>"
                  "<li><a href='/api/tanks'>GET /api/tanks</a> - Tank information</li>"
                  "<li><a href='/api/sensors'>GET /api/sensors</a> - Sensor data</li>"
                  "</ul>"
                  "</body></html>");
        return res;
     });

    // CORS preflight
    CROW_ROUTE(app, "/api/<path>").methods("OPTIONS"_method)([](const crow::request &req, const std::string &path)
    {
        crow::response res(200);
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        return res;
    });
        
    std::cout << "🌐 Servidor web iniciado en http://localhost:8080" << std::endl;
    std::cout << "📊 Dashboard API disponible en http://localhost:8080" << std::endl;
    std::cout << "🔌 API endpoints disponibles en http://localhost:8080/api/*" << std::endl;

    app.port(8080).multithreaded().run();

    return 0;
}
