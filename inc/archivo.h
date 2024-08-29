#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <iostream>
#include <fstream>
#include <string>

typedef struct data
{
    float id;
    float valormax;
    float valormin;
    const int pin;
} data;

class Jsonmanajer {
public:
    Jsonmanajer();
    ~Jsonmanajer();
    int managejson(const std::string &filename, const std::string &dataname, data &data);
};

#endif 

