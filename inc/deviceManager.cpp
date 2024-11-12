#include "deviceManager.h"

DeviceManager::DeviceManager()
{
    filename = "devices";
}

DeviceManager::~DeviceManager()
{
}

Dispositivo DeviceManager::setValues(Dispositivo &structname, const std::string name, const std::string type, double maxValue, double minValue, int pin, int priority, bool active)
{
    int id = getNextId();

    structname.id = id;
    structname.name = name;
    structname.type = type;
    structname.maxValue = maxValue;
    structname.minValue = minValue;
    structname.pin = pin;
    structname.priority = priority;
    structname.active = active;

    return structname;
}

void DeviceManager::fileManage(Dispositivo &structname)
{
    json data;
    data["id"] = structname.id;
    data["name"] = structname.name;
    data["type"] = structname.type;
    data["maxValue"] = structname.maxValue;
    data["minValue"] = structname.minValue;
    data["pin"] = structname.pin;
    data["priority"] = structname.priority;
    data["active"] = structname.active;

    std::string archivoNombre = filename + ".json";

    std::ifstream archivoLectura(archivoNombre);
    json contenidoExistente;

    if (archivoLectura.is_open())
    {
        try
        {
            archivoLectura >> contenidoExistente;
        }
        catch (json::parse_error &e)
        {
            std::cerr << "Error al parsear el archivo JSON: " << e.what() << std::endl;
            contenidoExistente = json::array();
        }
        archivoLectura.close();
    }

    if (!contenidoExistente.is_array())
    {
        contenidoExistente = json::array();
    }

    contenidoExistente.push_back(data);

    std::ofstream archivoEscritura(archivoNombre);
    if (!archivoEscritura.is_open())
    {
        std::cerr << "Error al abrir el archivo: " << archivoNombre << std::endl;
        return;
    }

    archivoEscritura << contenidoExistente.dump(7);

    archivoEscritura.close();

    std::cout << "Dispositivo agregado al archivo '" << archivoNombre << "' correctamente." << std::endl;
}

Dispositivo DeviceManager::getDevice(const std::string &keyword, int id)
{
    std::string archivoNombre = filename + ".json";
    std::ifstream archivoLectura(archivoNombre);
    json contenidoExistente;

    if (!archivoLectura.is_open())
    {
        std::cerr << "Error al abrir el archivo: " << archivoNombre << std::endl;
        return {};
    }

    try
    {
        archivoLectura >> contenidoExistente;
    }
    catch (json::parse_error &e)
    {
        std::cerr << "Error al parsear el archivo JSON: " << e.what() << std::endl;
        return {};
    }

    archivoLectura.close();

    if (!contenidoExistente.is_array())
    {
        std::cerr << "El archivo JSON no contiene una lista de dispositivos válida." << std::endl;
        return {};
    }

    for (const auto &dispositivo : contenidoExistente)
    {
        if ((!keyword.empty() && dispositivo.contains("name") && dispositivo["name"].get<std::string>().find(keyword) != std::string::npos) ||
            (id != -1 && dispositivo.contains("id") && dispositivo["id"] == id))
        {
            Dispositivo tempDevice;

            if (dispositivo.contains("id"))
                tempDevice.id = dispositivo["id"];
            if (dispositivo.contains("name"))
                tempDevice.name = dispositivo["name"];
            if (dispositivo.contains("type"))
                tempDevice.type = dispositivo["type"];
            if (dispositivo.contains("maxValue"))
                tempDevice.maxValue = dispositivo["maxValue"];
            if (dispositivo.contains("minValue"))
                tempDevice.minValue = dispositivo["minValue"];
            if (dispositivo.contains("pin"))
                tempDevice.pin = dispositivo["pin"];
            if (dispositivo.contains("priority"))
                tempDevice.priority = dispositivo["priority"];
            if (dispositivo.contains("active"))
                tempDevice.active = dispositivo["active"];

            return tempDevice;
        }
    }

    std::cerr << "No se encontró ningún dispositivo con los criterios proporcionados." << std::endl;
    return {};
}

int DeviceManager::getNextId()
{
    std::string archivoNombre = filename + ".json";
    std::ifstream archivoLectura(archivoNombre);
    json contenidoExistente;

    int maxId = 0;

    if (archivoLectura.is_open())
    {
        try
        {
            archivoLectura >> contenidoExistente;
        }
        catch (json::parse_error &e)
        {
            std::cerr << "Error al parsear el archivo JSON: " << e.what() << std::endl;
            return 1;
        }

        archivoLectura.close();

        if (contenidoExistente.is_array())
        {
            for (const auto &dispositivo : contenidoExistente)
            {
                int currentId = dispositivo["id"];
                if (currentId > maxId)
                {
                    maxId = currentId;
                }
            }
        }
    }

    return maxId + 1;
}

void DeviceManager::updateDevice(const Dispositivo &structname)
{
    std::string archivoNombre = filename + ".json";
    std::ifstream archivoLectura(archivoNombre);
    json contenidoExistente;

    if (!archivoLectura.is_open())
    {
        std::cerr << "Error al abrir el archivo: " << archivoNombre << std::endl;
        return;
    }

    try
    {
        archivoLectura >> contenidoExistente;
    }
    catch (json::parse_error &e)
    {
        std::cerr << "Error al parsear el archivo JSON: " << e.what() << std::endl;
        return;
    }

    archivoLectura.close();

    if (!contenidoExistente.is_array())
    {
        std::cerr << "El archivo JSON no contiene una lista de dispositivos válida." << std::endl;
        return;
    }

    // Buscar el dispositivo y actualizarlo
    for (auto &dispositivo : contenidoExistente)
    {
        if (dispositivo["id"] == structname.id)
        {
            dispositivo["name"] = structname.name;
            dispositivo["type"] = structname.type;
            dispositivo["maxValue"] = structname.maxValue;
            dispositivo["minValue"] = structname.minValue;
            dispositivo["pin"] = structname.pin;
            dispositivo["priority"] = structname.priority;
            dispositivo["active"] = structname.active;
        }
    }

    // Guardar los cambios en el archivo
    std::ofstream archivoEscritura(archivoNombre);
    if (!archivoEscritura.is_open())
    {
        std::cerr << "Error al abrir el archivo: " << archivoNombre << std::endl;
        return;
    }

    archivoEscritura << contenidoExistente.dump(7);
    archivoEscritura.close();

    std::cout << "Dispositivo con ID " << structname.id << " actualizado correctamente." << std::endl;
}
