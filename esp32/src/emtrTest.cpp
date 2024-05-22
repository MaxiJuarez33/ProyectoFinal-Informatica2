#include <Arduino.h>
#include "pinControl.h"

#define sensInt 15 // definicion de los pines
#define sensExt 2
#define sensHum 4
#define actLed 5

analog tempInt(sensInt); // se nombra al objeto que se utilizara en el class y se le ingresa el atributo
analog tempExt(sensExt);
analog humGen(sensHum);
digital ledOut(actLed);

int digitalState;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  tempInt.recep(); // funcion declarada previamente junto con su atributo
  tempExt.recep();
  humGen.recep();

  ledOut.emitir(digitalState);

  delay(2000);
}
