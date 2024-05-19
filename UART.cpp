#include <iostream>  // Incluye la librería estándar para entrada y salida
#include <windows.h> // Incluye la librería de Windows para manejo del puerto serie
#include <thread>    // Incluye la librería para manejo de hilos
#include <chrono>    // Incluye la librería para manejo del tiempo

// Estructura para almacenar los datos de temperatura y humedad
struct TempData
{
    int tempInt = 0; // Temperatura interior
    int tempExt = 0; // Temperatura exterior
    int humGen = 0;  // Humedad general
};

// Función para leer datos del puerto serie
void read_from_serial(HANDLE hSerial)
{
    TempData data;   // Variable para almacenar los datos leídos
    DWORD bytesRead; // Variable para almacenar el número de bytes leídos
    while (true)
    {
        // Leer datos desde el puerto serie
        if (ReadFile(hSerial, &data, sizeof(data), &bytesRead, NULL) && bytesRead == sizeof(data))
        {
            // Mostrar los datos en la consola, sobrescribiendo la línea anterior
            std::cout << "\rTemp Interior: " << data.tempInt
                      << " | Temp Exterior: " << data.tempExt
                      << " | Humedad General: " << data.humGen
                      << std::flush; // Asegura que la salida se muestre inmediatamente
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Espera opcional para evitar saturar la consola
    }
}

// Función para escribir comandos al puerto serie
void write_to_serial(HANDLE hSerial)
{
    DWORD bytesRead;     // Variable para almacenar el número de bytes leídos
    TempData data;       // Variable para almacenar los datos leídos
    std::string command; // Cadena para almacenar el comando a enviar
    DWORD bytesWritten;  // Variable para almacenar el número de bytes escritos
    while (true)
    {
        // Leer datos desde el puerto serie
        if (ReadFile(hSerial, &data, sizeof(data), &bytesRead, NULL) && bytesRead == sizeof(data))
        {
            // Determinar el comando basado en los datos de temperatura
            if (data.tempInt >= 3500)
            {
                command = "ACTUATOR_ON\n"; // Comando para activar el actuador
            }
            else if (data.tempInt < 3500)
            {
                command = "ACTUATOR_OFF\n"; // Comando para desactivar el actuador
            }
        }
        // Enviar el comando al puerto serie
        WriteFile(hSerial, command.c_str(), command.size(), &bytesWritten, NULL);
    }
}

int main()
{
    const char *portname = "COM5"; // Nombre del puerto serie (cambiar según sea necesario)

    // Abrir el puerto serie
    HANDLE hSerial = CreateFile(portname,
                                GENERIC_READ | GENERIC_WRITE, // Acceso de lectura y escritura
                                0,                            // Modo de compartición
                                NULL,                         // Seguridad predeterminada
                                OPEN_EXISTING,                // Solo abrir si el puerto existe
                                FILE_ATTRIBUTE_NORMAL,        // Atributos normales del archivo
                                NULL);                        // Sin plantillas de archivo

    // Verificar si el puerto serie se abrió correctamente
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error al abrir el puerto serie: " << portname << std::endl;
        return 1; // Salir del programa con código de error
    }

    // Configurar los parámetros del puerto serie
    DCB dcbSerialParams = {0};                           // Estructura para los parámetros del puerto serie
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams); // Establecer el tamaño de la estructura

    // Obtener la configuración actual del puerto serie
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        std::cerr << "Error getting state" << std::endl;
        return 1; // Salir del programa con código de error
    }

    // Establecer los parámetros del puerto serie
    dcbSerialParams.BaudRate = CBR_9600;   // Velocidad en baudios
    dcbSerialParams.ByteSize = 8;          // Tamaño del byte
    dcbSerialParams.StopBits = ONESTOPBIT; // Bits de parada
    dcbSerialParams.Parity = NOPARITY;     // Sin paridad

    // Aplicar la configuración al puerto serie
    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        std::cerr << "Error setting state" << std::endl;
        return 1; // Salir del programa con código de error
    }

    // Configurar los tiempos de espera del puerto serie
    COMMTIMEOUTS timeouts = {0};               // Estructura para los tiempos de espera
    timeouts.ReadIntervalTimeout = 50;         // Tiempo entre caracteres leídos
    timeouts.ReadTotalTimeoutConstant = 50;    // Tiempo total de espera de lectura
    timeouts.ReadTotalTimeoutMultiplier = 10;  // Multiplicador de tiempo de espera de lectura
    timeouts.WriteTotalTimeoutConstant = 50;   // Tiempo total de espera de escritura
    timeouts.WriteTotalTimeoutMultiplier = 10; // Multiplicador de tiempo de espera de escritura

    // Aplicar los tiempos de espera al puerto serie
    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        std::cerr << "Error setting timeouts" << std::endl;
        return 1; // Salir del programa con código de error
    }

    // Crear hilos para leer y escribir en el puerto serie simultáneamente
    std::thread reader(read_from_serial, hSerial);
    std::thread writer(write_to_serial, hSerial);

    // Esperar a que los hilos terminen (en este caso, nunca lo harán)
    reader.join();
    writer.join();

    // Cerrar el puerto serie
    CloseHandle(hSerial);
    return 0; // Salir del programa
}
