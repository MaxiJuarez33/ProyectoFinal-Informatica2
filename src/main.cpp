#include <Arduino.h>
#include "temp.h"

#define pintemp A0
#define pintemp1 A1

temp my_temp(pintemp);
temp my_temp1(pintemp1);

void setup() {

  Serial.begin(9600);
}

void loop() {

  my_temp.recep(pintemp); 
  my_temp1.recep(pintemp1);
  delay (2000);
}