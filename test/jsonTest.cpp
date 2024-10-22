#include <iostream>
#include "deviceMAnager.h"

int main()
{

    DeviceManager deviceManager;

    Dispositivo dispositivo;

    dispositivo = deviceManager.setValues(dispositivo, "Sensor de fantasmas", "Espiritual", 999.33, 0.0, 999);

    deviceManager.fileManage(dispositivo);

    deviceManager.getDevice("Sensor de fantasmas", -1);

    return 0;
}
