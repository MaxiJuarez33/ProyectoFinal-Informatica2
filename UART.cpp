#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>

struct TempData
{
    int tempInt = 0;
    int tempExt = 0;
    int humGen = 0;
};

void read_from_serial(HANDLE hSerial)
{
    TempData data;
    DWORD bytesRead;
    while (true)
    {
        if (ReadFile(hSerial, &data, sizeof(data), &bytesRead, NULL) && bytesRead == sizeof(data))
        {

            std::cout << "\rTemp Interior: " << data.tempInt
                      << " | Temp Exterior: " << data.tempExt
                      << " | Humedad General: " << data.humGen
                      << std::flush;
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Espera un poco para evitar saturar la consola
    }
}

void write_to_serial(HANDLE hSerial)
{
    DWORD bytesRead;

    TempData data;
    std::string command;
    DWORD bytesWritten;
    while (true)
    {
        if (ReadFile(hSerial, &data, sizeof(data), &bytesRead, NULL) && bytesRead == sizeof(data))
        {
            if (data.tempInt >= 3500)
            {
                command = "ACTUATOR_ON\n";
            }
            else if (data.tempInt < 3500)
            {
                command = "ACTUATOR_OFF\n";
            }
        }
        WriteFile(hSerial, command.c_str(), command.size(), &bytesWritten, NULL);
    }
}

int main()
{
    const char *portname = "COM5"; // Cambia esto al puerto correcto

    HANDLE hSerial = CreateFile(portname,
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error al abrir el puerto serie: " << portname << std::endl;
        return 1;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        std::cerr << "Error getting state" << std::endl;
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        std::cerr << "Error setting state" << std::endl;
        return 1;
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        std::cerr << "Error setting timeouts" << std::endl;
        return 1;
    }

    std::thread reader(read_from_serial, hSerial);
    std::thread writer(write_to_serial, hSerial);

    reader.join();
    writer.join();

    CloseHandle(hSerial);
    return 0;
}
