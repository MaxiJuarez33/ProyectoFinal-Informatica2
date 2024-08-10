#include "archivo.h"
#include <string>
#include <vector>

int main() {
    Datamanager data;
    
    std::string temp = "temp";
    data.addVariable("TempInt", 35);

    std::vector<std::string> variables = {"TempInt"};
    std::vector<double> varValues = {35};

    data.managedata(temp, variables, varValues);

    return 0;
}
