#include <Arduino.h>
#include "temp.h"

#define pintemp A0  // definicion de los pines
#define pintemp1 A1

temp my_temp(pintemp); // se nombra al objeto que se utilizara en el class y se le ingresa el atributo
temp my_temp1(pintemp1);

void setup() {

  Serial.begin(9600);
}

void loop() {

  my_temp.recep(pintemp); // funcion declarada previamente junt ocon su atributo
  my_temp1.recep(pintemp1);
  delay (2000);
}