#include "pinControl.h" // inclusion de la cabecera

// El analog:: designa la clase con la que se trabajata

analog::analog(uint16_t analogpin)
{ // Esta funcion tomara el pin que se ingrese y guardarlo en el atributo

    this->analogpin = analogpin; // el this--> sirve para establecer la primera variable como el atributo del objeto
}

analog::~analog()
{
}

void analog::recep(uint16_t analogpin)
{ // defino el metodo en el cual trabajara nuestra funcion
    pinMode(analogpin, INPUT);
    float analogval = analogRead(analogpin);
    Serial.println(analogval);
}

digital::digital(uint8_t digitalpin)
{

    this->digitalpin = digitalpin;
}

digital::~digital()
{
}

void digital::emitir(uint8_t divitalpin, uint8_t digitalval)
{

    pinMode(digitalpin, OUTPUT);
    digitalWrite(digitalpin, digitalval);
}