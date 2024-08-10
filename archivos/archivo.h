#ifndef _ARCHIVOH_
#define _ARCHIVOH_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Datamanager {
private:
    std::vector<std::string> variables;           
    std::vector<std::string> decisions;
    std::vector<double> varValues;

public:
    Datamanager();
    ~Datamanager();

    void addVariable(const std::string& var, const double values);
    void addDecision(const std::string& decision);
    int managedata(const std::string& filename, const std::vector<std::string>& data, const std::vector<double>& values);
};

#endif
