#include <iostream>
#include "archivo.h"

int main() {
    
    Jsonmanajer jsonManager;

    Dispositivo dispositivo;

    dispositivo = jsonManager.set_valores(dispositivo, 1, "Sensor de Temperatura", "Analogico", 100.0, 0.0, 13);

    jsonManager.FileManage("dispositivos", dispositivo);

    std::cout << "Datos del dispositivo guardados en 'dispositivo.json'." << std::endl;
}
