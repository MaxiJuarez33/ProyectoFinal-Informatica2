#include "archivo.h"
#include <string>

void Datamanager::addVariable(const std::string& var, const double values){
    variables.push_back(var);
    varValues.push_back(values);
}

void Datamanager::addDecision(const std::string& decision) {
    decisions.push_back(decision);
}

void Datamanager::managedata(std::string& filename, const std::vector<std::string>& data, const double values){

    std::string formato = ".csv";
    std::string fullname = filename.append(formato);
    std::ofstream archivo(fullname);

    if (!archivo) {
        //std::cerr << "Error al abrir el archivo para escritura." << std::endl;
        std::cout << "Error al abrir el archivo para escritora." << std::endl;
    }
}