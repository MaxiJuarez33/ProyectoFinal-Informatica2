#include "electricManager.h"
#include <iostream>

int main()
{
  std::string maxVerstappen;
  double checoPerez;
  Device device;
  // device.saveDevice("sensor de fantasmas", "testo", 2.2, 1.1, 33);
  // device.saveDevice("sensor de otaku", "agua", 222.22, 112.1, 2);
  // device.saveDevice("testo", "tes", 1, 0, 22);
  maxVerstappen = device.getDeviceByID(3).getName();
  checoPerez = device.getDeviceByKeyword("fantasmas").getMaxValue();
  std::cout << maxVerstappen << std::endl;
  std::cout << checoPerez << std::endl;
  Device device;

  std::string nambre;

  nambre = device.getDeviceByID(1);

  std::cout << nambre << std::endl;

  return 0;
}
