#include "archivo.h"

Datamanager::Datamanager()
{
}

Datamanager::~Datamanager()
{
}

void Datamanager::addVariable(const std::string &var, const double values)
{
    variables.push_back(var);
    varValues.push_back(values);
}

void Datamanager::addDecision(const std::string &decision)
{
    decisions.push_back(decision);
}

int Datamanager::managedata(const std::string &filename, const std::vector<std::string> &data, const std::vector<double> &values)
{
    std::string formato = ".csv";
    std::string fullname = filename + formato;
    std::ofstream archivo(fullname);

    if (!archivo)
    {
        std::cout << "Error al abrir el archivo para escritura." << std::endl;
        return 0;
    }

    for (size_t i = 0; i < data.size(); ++i)
    {
        archivo << data[i] << "," << values[i] << std::endl;
    }

    archivo.close();
}