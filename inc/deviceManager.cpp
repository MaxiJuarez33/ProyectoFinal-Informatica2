#include "deviceManager.h"

DeviceManager::DeviceManager()
{
    filename = "devices";
}

DeviceManager::~DeviceManager()
{
}

Dispositivo DeviceManager::setValues(Dispositivo &structname, const std::string name, const std::string type, double maxValue, double minValue, int pin)
{
    int id = getNextId();

    structname.id = id;
    structname.name = name;
    structname.type = type;
    structname.maxValue = maxValue;
    structname.minValue = minValue;
    structname.pin = pin;

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
            contenidoExistente = json::array(); // Si falla el parseo, inicializar un array vacío
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
        std::cerr << "El archivo JSON no contiene una lista de dispositivos valida." << std::endl;
        return {};
    }

    for (const auto &dispositivo : contenidoExistente)
    {
        if ((!keyword.empty() && dispositivo["name"].get<std::string>().find(keyword) != std::string::npos) ||
            (id != -1 && dispositivo["id"] == id))
        {
            Dispositivo tempDevice;
            tempDevice.id = dispositivo["id"];
            tempDevice.name = dispositivo["name"];
            tempDevice.type = dispositivo["type"];
            tempDevice.maxValue = dispositivo["maxValue"];
            tempDevice.minValue = dispositivo["minValue"];
            tempDevice.pin = dispositivo["pin"];
            return tempDevice; // Devolver el dispositivo encontrado
        }
    }

    std::cerr << "No se encontro ningun dispositivo con los criterios proporcionados." << std::endl;
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
            return 1; // Si no se puede leer el archivo, empezamos con ID 1
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