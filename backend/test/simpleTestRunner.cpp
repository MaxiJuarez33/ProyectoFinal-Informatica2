#include "simpleTest.h"

int main()
{
    SimpleTest::runAllTests();
    return SimpleTest::getFailedCount();
}
