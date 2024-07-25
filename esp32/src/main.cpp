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

analog tempInt(36);
analog tempExt(39);
analog capBlancas(34);
analog capGrises(35);
analog capNegras(32);
analog sensCorr1(33);
analog sensCorr2(25);
analog sensCorr3(26);
analog sensCorr4(27);

std::map<int, digital> relays = {
    {2, digital(2)},
    {19, digital(19)},
    {18, digital(18)},
    {5, digital(5)}};

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

        // mySerial.println(timeString); // No se lee desde el otro lado e irrumpe el formato de los datos
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

                // Suponemos que la cadena es del tipo "pin,estado\n"
                if (sscanf(buffer, "%d,%d", &data.pin[0], &data.estado[0]) == 2)
                {
                    // Enviamos los datos a la cola
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

void relayManager(void *parameter) // Funcionamiento comprobado con hardcode
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

    uartQueue = xQueueCreate(10, sizeof(Data));

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
Comentar puntos de debug
Definir cuantos sensores se usan
Evaluar si conviene usar vectores estructuras a estructuras con arrays
Variable para delay de mySerial.write
*/