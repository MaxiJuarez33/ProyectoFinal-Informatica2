#include <Arduino.h>
#include <HardwareSerial.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <time.h>
#include <map>
#include "pinControl.h"

struct Data
{
    int pin[4];
    int estado[4];
};

struct SaveDataVars
{
    double tempData[2]; // 1 Int & 2 Ext
    double capData[3];  // 1 Blancas & 2 Grises & 3 Negras
    double corrData[4];
} dataVars;

analog tempInt(17);
analog tempExt(16);
analog capBlancas(15);
analog capGrises(14);
analog capNegras(13);
analog sensCorr1(18);
analog sensCorr2(19);
analog sensCorr3(20);
analog sensCorr4(21);

std::map<int, digital> relays = {
    {10, digital(10)},
    {9, digital(9)},
    {8, digital(8)},
    {7, digital(7)}};

HardwareSerial mySerial(0);
QueueHandle_t uartQueue;

void varsManager(void *parameter)
{
    while (1)
    {
        dataVars.tempData[0] = tempInt.recep(); // Temperatura interior
        dataVars.tempData[1] = tempExt.recep(); // Temperatura exterior
        dataVars.capData[0] = capBlancas.recep();
        dataVars.capData[1] = capGrises.recep();
        dataVars.capData[2] = capNegras.recep();
        dataVars.corrData[0] = sensCorr1.recep();
        dataVars.corrData[1] = sensCorr2.recep();
        dataVars.corrData[2] = sensCorr3.recep();
        dataVars.corrData[3] = sensCorr4.recep();

        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay para ejecutar otras tareas y no solaparlas
    }
}

void uartTransmitter(void *parameter)
{
    while (1)
    {
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);

        char timeString[20];
        strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);

        mySerial.println(timeString);
        delay(100);

        mySerial.write((uint8_t *)&dataVars, sizeof(dataVars));
        delay(100);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void uartReceiver(void *parameter)
{
    while (1)
    {
        if (mySerial.available())
        {
            char buffer[50];
            size_t length = mySerial.readBytesUntil('\n', buffer, sizeof(buffer) - 1);

            if (length > 0)
            {
                buffer[length] = '\0';

                Data data;

                if (sscanf(buffer, "%d,%d;%d,%d", &data.pin[0], &data.estado[0], &data.pin[1], &data.estado[1]) == 4)
                {
                    if (xQueueSend(uartQueue, &data, portMAX_DELAY) != pdPASS)
                    {
                        Serial.println("Error al enviar a la cola");
                    }
                }
                else
                {
                    Serial.println("Error en el formato de los datos recibidos");
                }
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void relayManager(void *parameter)
{
    Data data;

    while (1)
    {
        if (xQueueReceive(uartQueue, &data, portMAX_DELAY) == pdPASS)
        {
            for (int i = 0; i < 2; i++)
            {
                if (relays.find(data.pin[i]) != relays.end())
                {
                    relays[data.pin[i]].emitir(data.estado[i]);

                    Serial.print("Pin: ");
                    Serial.print(data.pin[i]);
                    Serial.print(" Estado: ");
                    Serial.println(data.estado[i]);
                }
                else
                {
                    Serial.print("Pin no encontrado: ");
                    Serial.println(data.pin[i]);
                }
            }
        }
    }
}

void setup()
{
    Serial.begin(115200);
    mySerial.begin(9600);

    uartQueue = xQueueCreate(10, sizeof(Data)); // Cola de comunicacion entre tareas

    xTaskCreatePinnedToCore(varsManager, "Vars Manager", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(uartTransmitter, "UART Transmitter", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(uartReceiver, "UART Receiver", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(relayManager, "Relay Manager", 4096, NULL, 1, NULL, 1);
}

void loop()
{
}
