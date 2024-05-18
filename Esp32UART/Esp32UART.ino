#include <HardwareSerial.h>

#define sensInt 15
#define sensExt 2
#define sensHum 4

// Definimos el objeto serial
HardwareSerial mySerial(0); // UART0

struct TempData {
  int tempInt;
  int tempExt;
  int humGen;
};

void setup() {
  // Configuramos la velocidad de la comunicación serie
  Serial.begin(115200); // Puerto serie estándar para depuración
  mySerial.begin(9600); // UART0 para comunicación con la PC

  // Inicializamos pines de sensores y actuadores
  pinMode(sensInt, INPUT); // Pin del sensor (ajusta según tu configuración)
  pinMode(sensExt, INPUT);
  pinMode(sensHum, INPUT);
  pinMode(5, OUTPUT); // Pin del actuador (ajusta según tu configuración)
}

void loop() {
  // Leer datos de sensores
  TempData tempData;
  tempData.tempInt = analogRead(sensInt); // Ejemplo de lectura de sensor
  tempData.tempExt = analogRead(sensExt);
  tempData.humGen = analogRead(sensHum);

  // Enviar datos de sensor a través del UART
  mySerial.write((uint8_t*)&tempData, sizeof(tempData));

  // Revisar si hay datos recibidos en el UART
  if (mySerial.available()) {
    String command = mySerial.readStringUntil('\n');

    // Procesar comando recibido
    if (command == "ACTUATOR_ON") {
      // Activar actuador
      digitalWrite(5, HIGH);
    } else if (command == "ACTUATOR_OFF") {
      // Desactivar actuador
      digitalWrite(5, LOW);
    }
  }

  // Pequeña demora para no saturar el puerto serie
  delay(100);
}
