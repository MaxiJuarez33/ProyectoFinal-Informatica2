#include "electricManager.h"

int main()
{
  Device device;
  Dispositivo tempDev;
  DeviceManager deviceManager;

  tempDev = device.getDeviceById(8);

  tempDev.active = true;

  deviceManager.updateDevice(tempDev);

  std::cout << tempDev.active << std::endl;

  return 0;
}
