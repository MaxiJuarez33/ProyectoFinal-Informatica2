#include <iostream>
#include "Archivo.h"

int main() {

    Jsonmanajer jsonManager;

    Dispositivo dispositivo;

    dispositivo = jsonManager.set_valores(dispositivo, 1, "Sensor de Temperatura", "Analogico", 100.0, 0.0, 13);

    jsonManager.FileManage("dispositivo", dispositivo);


    return 0;
}
