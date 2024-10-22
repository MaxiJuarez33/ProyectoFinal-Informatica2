#include "Archivo.h"

DeviceManager::DeviceManager()
{
    filename = "dispositivo";
}

DeviceManager::~DeviceManager()
{
}

Dispositivo DeviceManager::setValues(Dispositivo &structname, const std::string nombre, const std::string tipo, double valorMax, double valorMin, int pin)
{
    int id = getNextId();

    structname.id = id;
    structname.nombre = nombre;
    structname.tipo = tipo;
    structname.valorMax = valorMax;
    structname.valorMin = valorMin;
    structname.pin = pin;

    return structname;
}

void DeviceManager::fileManage(const std::string filename, Dispositivo &structname)
{
    json data;
    data["id"] = structname.id;
    data["nombre"] = structname.nombre;
    data["tipo"] = structname.tipo;
    data["valorMax"] = structname.valorMax;
    data["valorMin"] = structname.valorMin;
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

void DeviceManager::getDevice(const std::string &nombre, int id)
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

    bool encontrado = false;
    for (const auto &dispositivo : contenidoExistente)
    {
        if ((!nombre.empty() && dispositivo["nombre"] == nombre) || (id != -1 && dispositivo["id"] == id))
        {
            std::cout << "Dispositivo encontrado:\n";
            std::cout << dispositivo.dump(4) << std::endl;
            encontrado = true;
            break;
        }
    }

    if (!encontrado)
    {
        std::cout << "No se encontró ningún dispositivo con los criterios proporcionados." << std::endl;
    }
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