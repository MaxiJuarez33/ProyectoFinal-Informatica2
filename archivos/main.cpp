#include "archivo.h"

int main(){

    Datamanager data;
    std::string variables = "variables";

    std::string temp = "temp";

    data.addVariable("TempInt", 35);
    data.managedata(temp, "TempInt", 35);
    
}