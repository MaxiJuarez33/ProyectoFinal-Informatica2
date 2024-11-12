#include "electricManager.h"

int main()
{
  Device device;
  Dispositivo tempDev;

  tempDev = device.getDeviceById(8);

  std::cout << tempDev.name << std::endl;

  return 0;
}
