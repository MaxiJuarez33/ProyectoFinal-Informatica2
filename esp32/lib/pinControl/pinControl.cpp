#include "pinControl.h" // inclusion de la cabecera

// El analog:: designa la clase con la que se trabajata

analog::analog(uint8_t analogpin)
{ // Esta funcion tomara el pin que se ingrese y guardarlo en el atributo

    this->analogpin = analogpin; // el this--> sirve para establecer la primera variable como el atributo del objeto
}


void analog::recep()
{ // defino el metodo en el cual trabajara nuestra funcion
    pinMode(analogpin, INPUT);
    float analogval = analogRead(analogpin);
    Serial.println(analogval);
}

digital::digital(uint8_t digitalpin)
{

    this->digitalpin = digitalpin;
}


void digital::emitir(uint8_t digitalval)
{

    pinMode(digitalpin, OUTPUT);
    digitalWrite(digitalpin, digitalval);
}