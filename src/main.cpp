#include <iostream>
#include "archivo.h"

int main() {
    Jsonmanajer jsonManager;

    data aparato1 = {1, 10.5, 2.3, 1};
    data aparato2 = {2, 20.0, 5.5, 2};

    if (jsonManager.managejson("datos", "horno", aparato1)) { 
        std::cout << "Información del aparato 1 agregada con éxito.\n";
    } else {
        std::cout << "Error al agregar información del aparato 1.\n";
    }

    if (jsonManager.managejson("datos", "microondas", aparato2)) { 
        std::cout << "Información del aparato 2 agregada con éxito.\n";
    } else {
        std::cout << "Error al agregar información del aparato 2.\n";
    }

    return 0;
}
