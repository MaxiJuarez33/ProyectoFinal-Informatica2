```
#include <HardwareSerial.h> // Incluye la librería para comunicación serie por hardware

#define sensInt 15 // Pin para el sensor de temperatura interior
#define sensExt 2  // Pin para el sensor de temperatura exterior
#define sensHum 4  // Pin para el sensor de humedad general

// Definimos el objeto serial para UART0
HardwareSerial mySerial(0); // UART0

// Estructura para almacenar los datos de temperatura y humedad
struct TempData
{
  int tempInt; // Temperatura interior
  int tempExt; // Temperatura exterior
  int humGen;  // Humedad general
};

void setup()
{
  // Configuramos la velocidad de la comunicación serie
  Serial.begin(115200); // Baudios para comunicacion entre pc y esp32
  mySerial.begin(9600); // Baudios para que se pueda leer desde C++

  // Inicializamos pines de sensores y actuadores (esto se hace con la libreria de decilli)
  pinMode(sensInt, INPUT); // Pin del sensor de temperatura interior
  pinMode(sensExt, INPUT); // Pin del sensor de temperatura exterior
  pinMode(sensHum, INPUT); // Pin del sensor de humedad general
  pinMode(5, OUTPUT);      // Pin del actuador
}

void loop()
{
  // Leer datos de sensores
  TempData tempData;
  tempData.tempInt = analogRead(sensInt); // Lectura del sensor de temperatura interior
  tempData.tempExt = analogRead(sensExt); // Lectura del sensor de temperatura exterior
  tempData.humGen = analogRead(sensHum);  // Lectura del sensor de humedad general

  // Enviar datos de sensor a través del UART
  mySerial.write((uint8_t *)&tempData, sizeof(tempData)); // Como funciona? Lo tengo anotado por ahi, pero no importa mucho, simplemente replica esto, aunque explico de forma simple

  /*
    la parte de (uint8_t *)&tempData convierte, el contenido en esa direccion de memoria(la estructura), a una especie de array de bytes, para que se envie mas rapido. el sizeof(tempData)
    simplemente indica el tamaño de lo que tiene que enviar.
  */

  // ESTO ES PARA LEER UN STRING ENVIADO DESDE C++, HAY QUE ADAPTARLO A UNA ESTRUCTURA
  if (mySerial.available())
  {
    String command = mySerial.readStringUntil('\n'); // Leer el comando recibido hasta el carácter de nueva línea

    // Procesar comando recibido
    if (command == "ACTUATOR_ON")
    {
      // Activar actuador
      digitalWrite(5, HIGH); // Encender el actuador
    }
    else if (command == "ACTUATOR_OFF")
    {
      // Desactivar actuador
      digitalWrite(5, LOW); // Apagar el actuador
    }
  }

  // Pequeña demora para no saturar el puerto serie
  delay(100); // Esperar 100 ms
}
```
