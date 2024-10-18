#include <iostream>
#include "Archivo.h"

int main()
{

    Jsonmanajer jsonManager;

    Dispositivo dispositivo;

    dispositivo = jsonManager.set_valores(dispositivo, 1, "Sensor de Temperatura", "Analogico", 100.0, 0.0, 13);
    dispositivo = jsonManager.set_valores(dispositivo, 2, "Sensor de corriente", "Analogico", 1000.0, 0.0, 20);
    dispositivo = jsonManager.set_valores(dispositivo, 3, "Sensor de Temperatura", "Digital", 50.0, -20.0, 2);

    jsonManager.FileManage("dispositivo", dispositivo);

    return 0;
}
