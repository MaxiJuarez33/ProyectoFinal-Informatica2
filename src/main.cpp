#include "electricManager.h"
#include <iostream>

int main()
{
  Device device;

  std::string nambre;

  nambre = device.getDeviceByID(1);

  std::cout << nambre << std::endl;

  return 0;
}
