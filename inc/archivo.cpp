#include "Archivo.h"

Jsonmanajer::Jsonmanajer() {
}

Jsonmanajer::~Jsonmanajer() {
}

Dispositivo Jsonmanajer::set_valores(Dispositivo &structname, int id, const std::string nombre, const std::string tipo, double valorMax, double valorMin, int pin) {
    
    structname.id = id;
    structname.nombre = nombre;
    structname.tipo = tipo;
    structname.valorMax = valorMax;
    structname.valorMin = valorMin;
    structname.pin = pin;

    return structname;
}

void Jsonmanajer::FileManage(const std::string filename, Dispositivo &structname) {
    
    json data;
    data["id"] = structname.id;
    data["nombre"] = structname.nombre;
    data["tipo"] = structname.tipo;
    data["valorMax"] = structname.valorMax;
    data["valorMin"] = structname.valorMin;
    data["pin"] = structname.pin;   

    std::string archivoNombre = filename + ".json";
    
    std::ofstream archivo(archivoNombre);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << archivoNombre << std::endl;
        return;
    }

    archivo << data.dump(7);

    archivo.close();

    std::cout << "Archivo '" << archivoNombre << "' generado correctamente." << std::endl;
}
