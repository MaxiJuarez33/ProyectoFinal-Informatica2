#include <Arduino.h>
#include "emtr.h"

#define analogpin A0  // definicion de los pines
#define analogpin A1
#define digitalpin 1

analog my_pin(analogpin); // se nombra al objeto que se utilizara en el class y se le ingresa el atributo
analog my_pin1(analogpin);
digital my_pin2(digitalpin);

int digitalval;

void setup() {

  Serial.begin(9600);
}

void loop() {

  my_pin.recep(analogpin); // funcion declarada previamente junto con su atributo
  my_pin1.recep(analogpin);

  my_pin2.emitir(digitalpin,digitalval);

  delay (2000);
}

