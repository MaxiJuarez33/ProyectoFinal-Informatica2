#include <Arduino.h>
#include <HardwareSerial.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <time.h>
#include <map>
#include "pinControl.h"

#define delayValue 100
#define taskDelayValue 1000

struct SaveDataVars
{
    double tempData[2]; // 1 Int & 2 Ext
    double capData[3];  // 1 Blancas & 2 Grises & 3 Negras
    double corrData[4];
    char timeString[10];
} dataVars;

analog tempInt(36);
analog tempExt(39);
analog capBlancas(34);
analog capGrises(35);
analog capNegras(32);
analog sensCorr1(33);
analog sensCorr2(25);
analog sensCorr3(26);
analog sensCorr4(27);

HardwareSerial mySerial(0);
QueueHandle_t uartQueue;

void varsManager(void *parameter) // Funcionamiento comprobado
{
    while (1)
    {
        dataVars.tempData[0] = tempInt.recep(); // Temperatura interior
        dataVars.tempData[1] = tempExt.recep(); // Temperatura exterior
        dataVars.capData[0] = capBlancas.recep();
        dataVars.capData[1] = capGrises.recep();
        dataVars.capData[2] = capNegras.recep();
        dataVars.corrData[0] = sensCorr1.readCurrentACS712();
        dataVars.corrData[1] = sensCorr2.readCurrentACS712();
        dataVars.corrData[2] = sensCorr3.readCurrentACS712();
        dataVars.corrData[3] = sensCorr4.readCurrentACS712();

        vTaskDelay(taskDelayValue / portTICK_PERIOD_MS); // Delay para ejecutar otras tareas y no solaparlas
    }
}

void uartTransmitter(void *parameter) // Funcionamiento comprobado
{
    while (1)
    {
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);

        strftime(dataVars.timeString, sizeof(dataVars.timeString), "%H:%M:%S", &timeinfo);

        delay(delayValue);

        mySerial.write((uint8_t *)&dataVars, sizeof(dataVars));
        delay(delayValue);

        vTaskDelay(taskDelayValue / portTICK_PERIOD_MS);
    }
}

void uartReceiver(void *parameter) // Funcionamiento comprobado
{
    int data[2];
    while (1)
    {
        if (Serial.available())
        {
            String command = Serial.readStringUntil('\n');

            int separatorIndex = command.indexOf(',');

            if (separatorIndex != -1)
            {
                String pinStr = command.substring(0, separatorIndex);
                String valueStr = command.substring(separatorIndex + 1);

                int pin = pinStr.toInt();
                int value = valueStr.toInt();

                if (pin > 0 && (value == 0 || value == 1))
                {
                    data[0] = pin;
                    data[1] = value;

                    if (xQueueSend(uartQueue, &data, portMAX_DELAY) != pdPASS)
                    {
                        // Serial.println("Error al enviar a la cola");
                    }
                    else
                    {
                        // Serial.println("Datos enviados a la cola: Pin " + String(pin) + " = " + String(value));
                    }
                }
                else
                {
                    // Serial.println("Pin o valor no válidos.");
                }
            }
            else
            {
                // Serial.println("Formato de comando incorrecto.");
            }
        }
        vTaskDelay(taskDelayValue / portTICK_PERIOD_MS);
    }
}

void relayManager(void *parameter) // Funcionamiento comprobado
{
    int data[2];
    while (1)
    {
        if (xQueueReceive(uartQueue, &data, portMAX_DELAY) == pdPASS)
        {
            int pin = data[0];
            int value = data[1];

            digital relayPin(pin);

            relayPin.emitir(value);
        }
        vTaskDelay(taskDelayValue / portTICK_PERIOD_MS);
    }
}

void setup()
{
    Serial.begin(115200);
    mySerial.begin(9600);

    uartQueue = xQueueCreate(10, sizeof(int[2]));

    xTaskCreatePinnedToCore(varsManager, "Vars Manager", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(uartTransmitter, "UART Transmitter", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(uartReceiver, "UART Receiver", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(relayManager, "Relay Manager", 4096, NULL, 1, NULL, 1);
}

void loop()
{
}

/*
RETOQUES FINALES

Variable para delay de vTask
Definir cuantos sensores se usan
Variable para delay de mySerial.write
*/