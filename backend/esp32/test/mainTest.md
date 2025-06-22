```
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
    double corrData[3]; // Cambiado de 4 a 3
} dataVars;

// Instancias de objetos analógicos para los potenciómetros
analog pot1(4);
analog pot2(2);
analog pot3(15);

std::map<int, digital> relays = {
    {10, digital(10)},
    {9, digital(9)},
    {8, digital(8)},
    {7, digital(7)}};

HardwareSerial mySerial(0);
QueueHandle_t uartQueue;

void varsManager(void *parameter) // Funcionamiento comprobado
{
    while (1)
    {
        // Leer valores de los potenciómetros y agregar prints de debug
        dataVars.corrData[0] = pot1.recep();
        // Serial.print("Potenciómetro 1: ");
        // Serial.println(dataVars.corrData[0]);

        dataVars.corrData[1] = pot2.recep();
        // Serial.print("Potenciómetro 2: ");
        // Serial.println(dataVars.corrData[1]);

        dataVars.corrData[2] = pot3.recep();
        // Serial.print("Potenciómetro 3: ");
        // Serial.println(dataVars.corrData[2]);

        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay para ejecutar otras tareas y no solaparlas
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

        char timeString[10];
        strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);

        mySerial.println(timeString);
        delay(100);

        // mySerial.write((uint8_t *)&dataVars, sizeof(dataVars));
        delay(100);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void uartReceiver(void *parameter) // Funcionamiento comprobado por simulacion total, falta simular datos enviando desde C++
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
        vTaskDelay(100 / portTICK_PERIOD_MS); // Cambiar tiempo en el test final si es necesario
    }
}

void relayManager(void *parameter) // Funciona, comprobado con circuito fisico
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
                    // Imprimir en lugar de emitir una salida
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

    uartQueue = xQueueCreate(10, sizeof(Data));

    xTaskCreatePinnedToCore(varsManager, "Vars Manager", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(uartTransmitter, "UART Transmitter", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(uartReceiver, "UART Receiver", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(relayManager, "Relay Manager", 4096, NULL, 1, NULL, 1);
}

void loop()
{
}
```
