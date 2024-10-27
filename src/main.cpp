#include <iostream>
#include <algorithm>
#include "serialReader.h"
#include "deviceManager.h"
#include "tankManager.h"

SerialReader serialReader("COM5");
DeviceManager deviceManager;
Dispositivo deviceStruct;

// const float maxConsumption = dataStruct.corrData[3] / 2; // 50% de la corriente maxima

class deviceData
{
public:
  int id;
  std::string name;
  std::string type;
  double maxValue;
  double minValue;
  int pin;

  std::string getName() const { return this->name; }
  int getID() const { return this->id; }
  std::string getType() const { return this->type; }
  double getMaxValue() const { return this->maxValue; }
  double getMinValue() const { return this->minValue; }
  int getPin() const { return this->pin; }
};

class Device : public deviceData
{
public:
  Device() {};
  void saveDevice(const std::string name, const std::string type, double maxValue, double minValue, int pin);
  auto getDeviceByID(int id);
  auto getDeviceByKeyword(const std::string keyword);
};

void Device::saveDevice(const std::string name, const std::string type, double maxValue, double minValue, int pin)
{
  deviceStruct = deviceManager.setValues(deviceStruct, name, type, maxValue, minValue, pin);

  deviceManager.fileManage(deviceStruct);
}

auto Device::getDeviceByID(int id)
{
  Dispositivo dispositivo = deviceManager.getDevice("", id);

  if (dispositivo.id == 0)
  {
    std::cerr << "No se pudo encontrar un dispositivo con el ID: " << id << std::endl;
    return *this;
  }

  this->id = dispositivo.id;
  this->name = dispositivo.name;
  this->type = dispositivo.type;
  this->maxValue = dispositivo.maxValue;
  this->minValue = dispositivo.minValue;
  this->pin = dispositivo.pin;

  return *this;
}

auto Device::getDeviceByKeyword(const std::string keyword)
{
  Dispositivo dispositivo = deviceManager.getDevice(keyword, -1);

  if (dispositivo.name.empty())
  {
    std::cerr << "No se pudo encontrar un dispositivo con el keyword: " << keyword << std::endl;
    return *this;
  }

  this->id = dispositivo.id;
  this->name = dispositivo.name;
  this->type = dispositivo.type;
  this->maxValue = dispositivo.maxValue;
  this->minValue = dispositivo.minValue;
  this->pin = dispositivo.pin;

  return *this;
}

int main()
{
  return 0;
}