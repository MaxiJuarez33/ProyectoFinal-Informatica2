#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

QueueHandle_t uartQueue;

void uartReceiver(void *parameter)
{
    int data[2];
    while (1)
    {
        if (Serial.available())
        {
            // Leer los datos del puerto serie
            String command = Serial.readStringUntil('\n');
            // Analizar el comando recibido
            int separatorIndex = command.indexOf(',');
            if (separatorIndex != -1)
            {
                String pinStr = command.substring(0, separatorIndex);
                String valueStr = command.substring(separatorIndex + 1);

                // Convertir los valores de los strings a enteros
                int pin = pinStr.toInt();
                int value = valueStr.toInt();

                // Validar el pin y valor
                if (pin > 0 && (value == 0 || value == 1))
                {
                    data[0] = pin;
                    data[1] = value;

                    // Enviar los datos a la cola
                    if (xQueueSend(uartQueue, &data, portMAX_DELAY) != pdPASS)
                    {
                        Serial.println("Error al enviar a la cola");
                    }
                    else
                    {
                        Serial.println("Datos enviados a la cola: Pin " + String(pin) + " = " + String(value));
                    }
                }
                else
                {
                    Serial.println("Pin o valor no válidos.");
                }
            }
            else
            {
                Serial.println("Formato de comando incorrecto.");
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void pinProcessor(void *parameter)
{
    int data[2];
    while (1)
    {
        if (xQueueReceive(uartQueue, &data, portMAX_DELAY) == pdPASS)
        {
            int pin = data[0];
            int value = data[1];

            // Configurar el pin como salida y establecer el valor
            pinMode(pin, OUTPUT);
            digitalWrite(pin, value);

            Serial.println("Pin procesado: " + String(pin) + " establecido a " + String(value));
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    Serial.begin(9600);
    uartQueue = xQueueCreate(10, sizeof(int[2]));

    xTaskCreate(uartReceiver, "UART Receiver", 2048, NULL, 1, NULL);
    xTaskCreate(pinProcessor, "Pin Processor", 2048, NULL, 1, NULL);
}

void loop()
{
}
