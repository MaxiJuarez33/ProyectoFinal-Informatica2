#include "simpleTest.h"

// Inicializar variables estáticas
std::vector<std::function<void()>> SimpleTest::tests;
std::vector<std::string> SimpleTest::testNames;
int SimpleTest::passedTests = 0;
int SimpleTest::failedTests = 0;
