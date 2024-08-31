#include "archivo.h"

std::string jsonTest = "jsonTest";
std::string device = "Heladera";

int main()
{
    data data(18);

    data.id = 33;
    data.valormax = 10;
    data.valormin = 0;

    Jsonmanajer jsonManager;
    jsonManager.managejson(jsonTest, device, data);

    return 0;
}
