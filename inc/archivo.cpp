#include "archivo.h"

Jsonmanajer::Jsonmanajer() {
}

Jsonmanajer::~Jsonmanajer() {
}

int Jsonmanajer::managejson(const std::string &filename, const std::string &dataname, data &data) { // Aquí también referencia
    std::string formato = ".json";
    std::string fullname = filename + formato;
    std::ofstream archivo(fullname, std::ios::app);

    if (!archivo) {
        std::cout << "Error al abrir el archivo para escritura." << std::endl;
        return 0;       
    } else {
        archivo << "{\n";
        archivo << "  \"Nombre\": \"" << dataname << "\",\n";
        archivo << "    \"id\": " << data.id << ",\n";
        archivo << "    \"valormax\": " << data.valormax << ",\n";
        archivo << "    \"valormin\": " << data.valormin << ",\n";
        archivo << "    \"pin\": " << data.pin << "\n";
        archivo << "},\n"; 
    }

    archivo.close();
    return 1; 
}