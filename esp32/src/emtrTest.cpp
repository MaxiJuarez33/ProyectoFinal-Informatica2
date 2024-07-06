#include <Arduino.h>
#include "pinControl.h"

analog tempInt(15); // se nombra al objeto que se utilizara en el class y se le ingresa el atributo
analog tempExt(2);
analog humGen(4);
digital ledOut(2);

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
