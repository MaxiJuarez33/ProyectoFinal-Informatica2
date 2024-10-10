#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <json.hpp>

using json = nlohmann::json;

struct Dispositivo {
    int id;
    std::string nombre;
    std::string tipo;
    double valorMax;
    double valorMin;
    int pin;


};

class Jsonmanajer
{

public:

    Jsonmanajer();
    ~Jsonmanajer();

    void FileManage(const std::string filename, Dispositivo &structname);
    Dispositivo set_valores(Dispositivo &structname, int id, const std::string nombre, const std::string tipo, double valorMax, double valorMin, int pin);

private:
    std::string filename; 
    std::vector<Dispositivo> dispositivos;
};

#endif
