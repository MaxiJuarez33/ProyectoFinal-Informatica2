#include "archivo.h"

int main()
{
    Datamanager data;

    std::string fileName = "corriente";
    data.addVariable("heladera", 88);

    std::vector<std::string> variables = {"corrienteHeladera"};
    std::vector<double> varValues = {88};

    data.managedata(fileName, variables, varValues);

    system("pause");

    return 0;
}
